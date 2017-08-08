#pragma once

#ifndef STILL_RUNNING
    #define STILL_RUNNING 259
#endif

void refreshRAM();
bool testProcess(int pid);
void findCompatibleProcess();
void readRAMFromCompatibleProcess();
void processConnectionTick();

extern uint8_t RAM[0x2000];
extern int curRAMOffset;
extern int connectedPid;

enum connstatus_t {
    STATUS_NO_PROCESS = 0,
    STATUS_SEARCHING = 1,
    STATUS_ACTIVE = 2
};
extern connstatus_t connectionStatus;

extern const char *errMsgNoProcess;
extern const char *errMsgSearching;