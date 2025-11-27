/*
 * Breaker IED v1 - Vulnerable Version
 * Basic GOOSE subscription without sequence validation
 * Features: GOOSE Subscriber + XCBR model + Operation logging
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
#define COMM_LOSS_TIMEOUT_MS 5000
#define MAX_OPERATION_LOG 100

// IEC 61850-7-4 XCBR (Circuit Breaker) Data Model - Phase 2
typedef struct {
    // XCBR.Pos (Position) - DPC (Double Point Control)
    int32_t pos_stVal;          // 0=intermediate, 1=off, 2=on, 3=bad
    uint8_t pos_q;              // Quality
    uint64_t pos_t;             // Timestamp
    
    // XCBR.OpCnt (Operation Counter) - INS (Integer Status)
    int32_t opCnt_stVal;        // Number of operations
    uint8_t opCnt_q;            // Quality
    uint64_t opCnt_t;           // Timestamp
    
    // XCBR.CBOpCap (Circuit Breaker Operating Capability) - ENS (Enumerated Status)
    int32_t cbOpCap_stVal;      // 0=not-available, 1=available
    uint8_t cbOpCap_q;          // Quality
    uint64_t cbOpCap_t;         // Timestamp
    
    // XCBR.BlkOpn (Block Opening) - SPS (Single Point Status)
    bool blkOpn_stVal;          // Block trip operations
    uint8_t blkOpn_q;           // Quality
    uint64_t blkOpn_t;          // Timestamp
    
    // XCBR.BlkCls (Block Closing) - SPS
    bool blkCls_stVal;          // Block close operations
    uint8_t blkCls_q;           // Quality
    uint64_t blkCls_t;          // Timestamp
} XCBRData;

typedef struct {
    uint64_t timestamp;
    int32_t position;
    uint32_t stNum;
    uint32_t sqNum;
    bool ptrcTrip;
    bool ptrcOp;
    bool cswiClose;
    char reason[64];
} OperationLog;

typedef struct {
    XCBRData xcbr;
    uint64_t lastGooseReceived;
    bool commLost;
    bool testMode;
    OperationLog logs[MAX_OPERATION_LOG];
    int logIndex;
    uint32_t lastStNum;
    uint32_t lastSqNum;
} BreakerState;

static int running = 1;
static BreakerState breaker = {
    .xcbr = {
        .pos_stVal = 2, .pos_q = 0xC0, .pos_t = 0,          // Position: on (closed)
        .opCnt_stVal = 0, .opCnt_q = 0xC0, .opCnt_t = 0,    // Operation count
        .cbOpCap_stVal = 1, .cbOpCap_q = 0xC0, .cbOpCap_t = 0, // Available
        .blkOpn_stVal = false, .blkOpn_q = 0xC0, .blkOpn_t = 0, // Not blocked
        .blkCls_stVal = false, .blkCls_q = 0xC0, .blkCls_t = 0  // Not blocked
    },
    .lastGooseReceived = 0,
    .commLost = false,
    .testMode = false,
    .logIndex = 0,
    .lastStNum = 0,
    .lastSqNum = 0
};
static Semaphore breakerLock;

static void sigint_handler(int signalId)
{
    running = 0;
}

uint64_t getTimestampMs()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)(tv.tv_sec) * 1000 + (uint64_t)(tv.tv_usec) / 1000;
}

const char* getPositionString(int32_t pos)
{
    switch(pos) {
        case 0: return "INTERMEDIATE";
        case 1: return "OFF/OPEN";
        case 2: return "ON/CLOSED";
        case 3: return "BAD";
        default: return "UNKNOWN";
    }
}

void updateWebStatus()
{
    FILE *f = fopen("/tmp/breaker_status.json", "w");
    if (f) {
        fprintf(f, "{\"pos\":%d,\"opCnt\":%d,\"stNum\":%u,\"sqNum\":%u,\"commStatus\":\"%s\"}",
                breaker.xcbr.pos_stVal,
                breaker.xcbr.opCnt_stVal,
                breaker.lastStNum,
                breaker.lastSqNum,
                breaker.commLost ? "LOST" : "OK");
        fclose(f);
    }
}

void logOperation(int32_t newPos, uint32_t stNum, uint32_t sqNum, 
                  bool ptrcTrip, bool ptrcOp, bool cswiClose, const char* reason)
{
    OperationLog* log = &breaker.logs[breaker.logIndex];
    log->timestamp = getTimestampMs();
    log->position = newPos;
    log->stNum = stNum;
    log->sqNum = sqNum;
    log->ptrcTrip = ptrcTrip;
    log->ptrcOp = ptrcOp;
    log->cswiClose = cswiClose;
    snprintf(log->reason, sizeof(log->reason), "%s", reason);
    
    breaker.logIndex = (breaker.logIndex + 1) % MAX_OPERATION_LOG;
}

void operateBreaker(int32_t newPosition, const char* reason,
                    uint32_t stNum, uint32_t sqNum, bool ptrcTrip, bool ptrcOp, bool cswiClose)
{
    if (breaker.xcbr.pos_stVal == newPosition)
        return;

    printf("\n========================================\n");
    printf("XCBR BREAKER OPERATION (Phase 2)\n");
    printf("Time:     %lu ms\n", getTimestampMs());
    printf("From:     %s\n", getPositionString(breaker.xcbr.pos_stVal));
    printf("To:       %s\n", getPositionString(newPosition));
    printf("Reason:   %s\n", reason);
    printf("PTRC.Tr:  %s\n", ptrcTrip ? "TRUE" : "FALSE");
    printf("PTRC.Op:  %s\n", ptrcOp ? "TRUE" : "FALSE");
    printf("CSWI.Cls: %s\n", cswiClose ? "TRUE" : "FALSE");
    printf("stNum:    %u\n", stNum);
    printf("sqNum:    %u\n", sqNum);
    printf("========================================\n\n");

    // Set intermediate position during operation
    breaker.xcbr.pos_stVal = 0; // intermediate
    uint64_t now = getTimestampMs();
    breaker.xcbr.pos_t = now;
    
    Thread_sleep(BREAKER_OPERATE_TIME_MS);
    
    // Set final position
    breaker.xcbr.pos_stVal = newPosition;
    breaker.xcbr.opCnt_stVal++;
    breaker.xcbr.opCnt_t = getTimestampMs();
    breaker.xcbr.pos_t = breaker.xcbr.opCnt_t;
    
    logOperation(newPosition, stNum, sqNum, ptrcTrip, ptrcOp, cswiClose, reason);
    updateWebStatus();
}

static void gooseListener(GooseSubscriber subscriber, void* parameter)
{
    Semaphore_wait(breakerLock);
    breaker.lastGooseReceived = getTimestampMs();
    breaker.commLost = false;
    Semaphore_post(breakerLock);

    if (!GooseSubscriber_isValid(subscriber)) {
        printf("WARNING: Invalid GOOSE message (parse error: %d)\n", 
               GooseSubscriber_getParseError(subscriber));
        return;
    }

    if (GooseSubscriber_isTest(subscriber)) {
        printf("INFO: Test message received (ignored)\n");
        return;
    }

    MmsValue* values = GooseSubscriber_getDataSetValues(subscriber);
    if (!values || MmsValue_getType(values) != MMS_ARRAY)
        return;

    if (MmsValue_getArraySize(values) < 3) {
        printf("ERROR: Invalid dataset size (expected >= 3, got %d)\n", 
               MmsValue_getArraySize(values));
        return;
    }

    // Parse Phase 2 PTRC/CSWI dataset from MMS server
    bool ptrcTrip = MmsValue_getBoolean(MmsValue_getElement(values, 0));    // PTRC.Tr.stVal
    bool ptrcOp = MmsValue_getBoolean(MmsValue_getElement(values, 1));      // PTRC.Op.stVal
    bool cswiClose = MmsValue_getBoolean(MmsValue_getElement(values, 2));   // CSWI.OpCls.stVal
    // Index 3 is supervision bit - ignored by breaker logic

    uint32_t stNum = GooseSubscriber_getStNum(subscriber);
    uint32_t sqNum = GooseSubscriber_getSqNum(subscriber);

    printf("📡 GOOSE Received: PTRC.Tr=%s, PTRC.Op=%s, CSWI.Cls=%s (stNum:%u)\n",
           ptrcTrip ? "T" : "F", ptrcOp ? "T" : "F", cswiClose ? "T" : "F", stNum);

    Semaphore_wait(breakerLock);
    
    // Update sequence numbers (vulnerable - no validation)
    breaker.lastStNum = stNum;
    breaker.lastSqNum = sqNum;

    // IEC 61850: Block operations if test mode active
    if (breaker.testMode) {
        printf("INFO: Test mode active - command blocked\n");
        Semaphore_post(breakerLock);
        return;
    }

    // Check XCBR operating capability
    if (breaker.xcbr.cbOpCap_stVal != 1) {
        printf("WARNING: XCBR not available for operation\n");
        Semaphore_post(breakerLock);
        return;
    }

    // Process PTRC trip command (both Tr and Op must be true)
    if (ptrcTrip && ptrcOp && !breaker.xcbr.blkOpn_stVal && breaker.xcbr.pos_stVal == 2) {
        operateBreaker(1, "PTRC Trip Command", stNum, sqNum, ptrcTrip, ptrcOp, cswiClose);
    }
    // Process CSWI close command
    else if (cswiClose && !breaker.xcbr.blkCls_stVal && breaker.xcbr.pos_stVal == 1) {
        operateBreaker(2, "CSWI Close Command", stNum, sqNum, ptrcTrip, ptrcOp, cswiClose);
    }
    
    // Always update web status to reflect current sequence numbers
    updateWebStatus();

    Semaphore_post(breakerLock);
}

void* monitorThread(void* arg)
{
    while (running) {
        Thread_sleep(1000);
        
        Semaphore_wait(breakerLock);
        uint64_t now = getTimestampMs();
        
        if (!breaker.commLost && (now - breaker.lastGooseReceived) > COMM_LOSS_TIMEOUT_MS) {
            breaker.commLost = true;
            printf("\n*** ALARM: GOOSE Communication Lost ***\n\n");
        }
        
        // Update web status every second for live display
        updateWebStatus();
        
        Semaphore_post(breakerLock);
    }
    return NULL;
}

int main(int argc, char** argv)
{
    char* interface = (argc > 1) ? argv[1] : "eth0";

    printf("===========================================\n");
    printf("  Breaker IED v1 - Vulnerable Version    \n");
    printf("  Enhanced GOOSE + XCBR Data Model       \n");
    printf("  SCL-Ready Architecture                  \n");
    printf("===========================================\n");
    printf("Interface: %s\n", interface);
    printf("Data Model: BREAKER_IED/SWGR/XCBR1\n");
    printf("Initial Position: ON/CLOSED (2)\n\n");

    signal(SIGINT, sigint_handler);
    breakerLock = Semaphore_create(1);

    GooseReceiver receiver = GooseReceiver_create();
    GooseReceiver_setInterfaceId(receiver, interface);

    // Subscribe to Phase 2 Control IED GOOSE
    GooseSubscriber subscriber = GooseSubscriber_create("CTRL_IED/PROT/LLN0$GO$gcbControl", NULL);

    uint8_t dstMac[6] = {0x01, 0x0c, 0xcd, 0x01, 0x00, 0x01};
    GooseSubscriber_setDstMac(subscriber, dstMac);
    GooseSubscriber_setAppId(subscriber, 1000);
    GooseSubscriber_setListener(subscriber, gooseListener, NULL);

    GooseReceiver_addSubscriber(receiver, subscriber);
    GooseReceiver_start(receiver);

    if (!GooseReceiver_isRunning(receiver)) {
        printf("ERROR: Failed to start GOOSE receiver\n");
        printf("Check: 1) Root privileges 2) Interface exists\n");
        return -1;
    }

    Thread monThread = Thread_create(monitorThread, NULL, false);
    Thread_start(monThread);

    // Write initial status for web interface
    updateWebStatus();

    printf("✅ Breaker IED v1 Ready (Vulnerable)\n");
    printf("📡 GOOSE Subscriber: Listening for PTRC/CSWI commands\n");

    while (running) {
        Thread_sleep(1000);
    }

    running = 0;
    Thread_destroy(monThread);
    GooseReceiver_stop(receiver);
    GooseReceiver_destroy(receiver);
    Semaphore_destroy(breakerLock);

    printf("\n✅ Breaker IED v1 stopped\n");
    return 0;
}