# Makefile for Control IED and Breaker IED - Phase 2
# SCL/ICD Ready Architecture

LIB_DIR = ./libiec61850
LIB_INCL = -I$(LIB_DIR)/src/common/inc \
           -I$(LIB_DIR)/src/goose \
           -I$(LIB_DIR)/src/hal/inc \
           -I$(LIB_DIR)/hal/inc \
           -I$(LIB_DIR)/src/mms/inc \
           -I$(LIB_DIR)/src/mms/inc_private \
           -I$(LIB_DIR)/src/iec61850/inc \
           -I$(LIB_DIR)/src/logging \
           -I$(LIB_DIR)/src/r_session \
           -I$(LIB_DIR)/config

LIB_PATH = $(LIB_DIR)/build/libiec61850.a

CFLAGS = -g -Wall $(LIB_INCL) -DCONFIG_IEC61850_INCLUDE_GOOSE=1
LDFLAGS = $(LIB_PATH) -lpthread

# Phase 2 targets
all: control_ied breaker_ied_v1 breaker_ied_v2

control_ied: control_ied.c
	$(CC) $(CFLAGS) control_ied.c -o control_ied $(LDFLAGS)

breaker_ied_v1: breaker_ied_v1.c
	$(CC) $(CFLAGS) breaker_ied_v1.c -o breaker_ied_v1 $(LDFLAGS)

breaker_ied_v2: breaker_ied_v2.c
	$(CC) $(CFLAGS) breaker_ied_v2.c -o breaker_ied_v2 $(LDFLAGS)

clean:
	rm -f control_ied breaker_ied_v1 breaker_ied_v2

test-build:
	@echo "Testing Phase 2 build..."
	@make clean
	@make
	@echo "✅ Phase 2 build successful"

.PHONY: all clean test-build