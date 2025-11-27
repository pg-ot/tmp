/*
 * Breaker IED v2 - Secure Version with stNum/sqNum Validation
 */

#include "goose_receiver.h"
#include "goose_subscriber.h"
#include "hal_thread.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

#define BREAKER_OPERATE_TIME_MS 50

typedef struct {
    int32_t pos_stVal;
    int32_t opCnt_stVal;
    uint32_t lastStNum;
    uint32_t lastSqNum;
} BreakerState;

static int running = 1;
static BreakerState breaker = {.pos_stVal = 2, .opCnt_stVal = 0, .lastStNum = 0, .lastSqNum = 0};
static Semaphore lock;

static void sigint_handler(int signalId) { running = 0; }

uint64_t getTimestampMs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)(tv.tv_sec) * 1000 + (uint64_t)(tv.tv_usec) / 1000;
}

const char* getPositionString(int32_t pos) {
    switch(pos) {
        case 1: return "OFF/OPEN";
        case 2: return "ON/CLOSED";
        default: return "UNKNOWN";
    }
}

void writeStatus() {
    FILE *f = fopen("/tmp/breaker_status.json", "w");
    if (f) {
        fprintf(f, "{\"pos\":%d,\"opCnt\":%d,\"stNum\":%u,\"sqNum\":%u,\"commStatus\":\"OK\"}", 
                breaker.pos_stVal, breaker.opCnt_stVal, breaker.lastStNum, breaker.lastSqNum);
        fclose(f);
    }
}

void operateBreaker(int32_t newPos, uint32_t stNum, uint32_t sqNum) {
    if (breaker.pos_stVal == newPos) return;
    
    printf("\n========================================\n");
    printf("BREAKER OPERATION (Validated)\n");
    printf("From: %s -> To: %s\n", getPositionString(breaker.pos_stVal), getPositionString(newPos));
    printf("stNum: %u, sqNum: %u\n", stNum, sqNum);
    printf("========================================\n\n");
    
    breaker.pos_stVal = newPos;
    breaker.opCnt_stVal++;
    breaker.lastStNum = stNum;
    breaker.lastSqNum = sqNum;
    writeStatus();
}

static void gooseListener(GooseSubscriber subscriber, void* parameter) {
    if (!GooseSubscriber_isValid(subscriber)) return;
    
    MmsValue* values = GooseSubscriber_getDataSetValues(subscriber);
    if (!values || MmsValue_getArraySize(values) < 3) return;
    
    bool trip = MmsValue_getBoolean(MmsValue_getElement(values, 0));
    bool op = MmsValue_getBoolean(MmsValue_getElement(values, 1));
    bool close = MmsValue_getBoolean(MmsValue_getElement(values, 2));
    // Index 3 is supervision bit - ignored by breaker logic
    
    uint32_t stNum = GooseSubscriber_getStNum(subscriber);
    uint32_t sqNum = GooseSubscriber_getSqNum(subscriber);
    
    Semaphore_wait(lock);
    
    // IEC 61850-8-1 compliant validation
    bool valid = false;
    
    if (breaker.lastStNum == 0) {
        // First message - accept any stNum
        valid = true;
    } else if (stNum > breaker.lastStNum) {
        // stNum increased - NEW STATE or heartbeat increment, accept
        valid = true;
    } else if (stNum == breaker.lastStNum && sqNum > breaker.lastSqNum) {
        // Same state, sqNum increased - retransmission within same state, accept
        valid = true;
    } else {
        // stNum < lastStNum OR (stNum == lastStNum && sqNum <= lastSqNum) - REJECT
        valid = false;
    }
    
    if (!valid) {
        printf("⚠️  REJECTED: Invalid stNum=%u (last=%u) sqNum=%u (last=%u)\n", 
               stNum, breaker.lastStNum, sqNum, breaker.lastSqNum);
        Semaphore_post(lock);
        return;
    }
    
    printf("✅ VALIDATED: stNum=%u sqNum=%u\n", stNum, sqNum);
    
    if (trip && op && breaker.pos_stVal == 2) {
        operateBreaker(1, stNum, sqNum);
    } else if (close && breaker.pos_stVal == 1) {
        operateBreaker(2, stNum, sqNum);
    } else {
        // Update sequence numbers even if no operation
        breaker.lastStNum = stNum;
        breaker.lastSqNum = sqNum;
    }
    
    Semaphore_post(lock);
}

int main(int argc, char** argv) {
    char* interface = (argc > 1) ? argv[1] : "eth0";
    
    printf("===========================================\n");
    printf("  Breaker IED v2 - Secure Version         \n");
    printf("  stNum/sqNum Validation Enabled           \n");
    printf("===========================================\n");
    printf("Interface: %s\n", interface);
    printf("Initial Position: ON/CLOSED (2)\n\n");
    
    signal(SIGINT, sigint_handler);
    lock = Semaphore_create(1);
    
    GooseReceiver receiver = GooseReceiver_create();
    GooseReceiver_setInterfaceId(receiver, interface);
    
    GooseSubscriber subscriber = GooseSubscriber_create("CTRL_IED/PROT/LLN0$GO$gcbControl", NULL);
    uint8_t dstMac[6] = {0x01, 0x0c, 0xcd, 0x01, 0x00, 0x01};
    GooseSubscriber_setDstMac(subscriber, dstMac);
    GooseSubscriber_setAppId(subscriber, 1000);
    GooseSubscriber_setListener(subscriber, gooseListener, NULL);
    
    GooseReceiver_addSubscriber(receiver, subscriber);
    GooseReceiver_start(receiver);
    
    if (!GooseReceiver_isRunning(receiver)) {
        printf("ERROR: Failed to start GOOSE receiver\n");
        return -1;
    }
    
    writeStatus();
    printf("✅ Breaker IED v2 Ready (Secure Mode)\n");
    printf("Press Ctrl+C to quit.\n\n");
    
    while (running) {
        Thread_sleep(1000);
        writeStatus();
    }
    
    GooseReceiver_stop(receiver);
    GooseReceiver_destroy(receiver);
    Semaphore_destroy(lock);
    
    return 0;
}