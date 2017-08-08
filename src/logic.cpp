#include <windows.h>
#include <psapi.h>
#include <cstdint>
#include "logic.h"
#include "utils.h"
#include "memory.h"

const char *errMsgNoProcess = "No supported emulator found. Please start a compatible emulator.";
const char *errMsgSearching = "Load up a R/B ROM and view your Trainer Card...";

const char *identVBA = "VisualBoyAdvance";
const char *identBGB = "bgb.exe";

connstatus_t connectionStatus = STATUS_NO_PROCESS;

uint8_t RAM[0x2000];
int curRAMOffset;
int connectedPid = -1;

void refreshRAM(){
    if (connectionStatus != STATUS_ACTIVE) return;
    if (!globalRAMCopy()){
        connectionStatus = STATUS_NO_PROCESS;
    }
}

bool testProcess(int pid){
    bool ok = false;
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                               FALSE, pid);
    if (hProc){
        char *path = new char[MAX_PATH];
        if (GetModuleFileNameEx(hProc, NULL, path, MAX_PATH)){
            const char *version = exeVersionString(path);
            if (version && memcmp(version, identVBA, strlen(identVBA)) == 0){
                ok = true;
            }
            if (strlen(path) > strlen(identBGB)){
                char *subpath = &path[strlen(path) - strlen(identBGB)];
                if (memcmp(subpath, identBGB, strlen(identBGB)) == 0){
                    ok = true;
                }
            }
        }
        delete [] path;
        CloseHandle(hProc);
    }
    return ok;
}

void findCompatibleProcess(){
    DWORD processes[4096];
    DWORD len;
    if (!EnumProcesses(processes, sizeof(processes), &len)) return;
    int count = len / sizeof(int);
    for (int i=0; i<count; i++){
        if (processes[i] != 0){
            if (testProcess(processes[i])){
                connectedPid = processes[i];
                hCurProc = NULL;
                connectionStatus = STATUS_SEARCHING;
            }
        }
    }
}

void readRAMFromCompatibleProcess(){
    if (hCurProc == NULL){
        globalRAMStart(connectedPid);
    }
    DWORD ec = 0;
    GetExitCodeProcess(hCurProc, &ec);
    if (ec != STILL_RUNNING){
        connectionStatus = STATUS_NO_PROCESS;
        return;
    }
    if (testRAM(connectedPid)){
        connectionStatus = STATUS_ACTIVE;
    }
}

void processConnectionTick(){
    if (connectionStatus == STATUS_NO_PROCESS){
        findCompatibleProcess();
    }
    if (connectionStatus == STATUS_SEARCHING){
        readRAMFromCompatibleProcess();
    }
}