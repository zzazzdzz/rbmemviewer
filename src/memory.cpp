#include <cstdint>
#include <windows.h>
#include "memory.h"
#include "logic.h"

const uint8_t signature[] = {0x79, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 
                             0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 
                             0x7A, 0x7A, 0x7A, 0x7B, 0x7C, 0x7F, 0x7F, 0x7F};

const size_t signature_length = 8 * 3;
const size_t signature_offset = 0x3a0;

HANDLE hCurProc = NULL;
size_t curRAMOffsetVA;

bool verifyMemoryFragment(uint8_t *mem, size_t offset, size_t size){
    if (size < 0x2000) return false;
    if (size - offset < 0x2000) return false;
    // [wOAMBuffer+0x9c] == 0xa0
    if (mem[offset + 0x39c] != 0xa0) return false;
    return true;
}

size_t searchMemoryFragment(uint8_t *mem, size_t size){
    if (size < signature_offset) return size;
    size_t len = size - signature_length;
    for (size_t i=0; i<len; i++){
        bool ok = true;
        for (size_t j=0; j<signature_length; j++){
            if (mem[i+j] != signature[j]){
                ok = false;
                break;
            }
        }
        if (ok){
            size_t result = i - signature_offset;
            if (verifyMemoryFragment(mem, result, size)) return i;
            else return size;
        }
    }
    return size;
}

size_t findGameRAMInProcessMemory(int pid){
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | 
                               PROCESS_VM_OPERATION | PROCESS_VM_READ, 
                               FALSE, pid);
    size_t offset = 0;
    MEMORY_BASIC_INFORMATION mbi;
    while (offset <= 0x7ffff000){
        if (!VirtualQueryEx(hProc, (void*)offset, &mbi, sizeof(mbi))){
            break;
        }
        if (mbi.Protect & PAGE_READWRITE){
            uint8_t *mirror = new uint8_t[mbi.RegionSize];
            ReadProcessMemory(hProc, mbi.BaseAddress, mirror, mbi.RegionSize, NULL);
            size_t offset_found = searchMemoryFragment(mirror, mbi.RegionSize);
            if (offset_found < mbi.RegionSize){
                size_t effective_offset = offset + offset_found - signature_offset;
                return effective_offset;
            }
            delete [] mirror;
        }
        offset += mbi.RegionSize;
    }
    return 0;
}

bool testRAM(int pid){
    size_t offset = findGameRAMInProcessMemory(pid);
    if (offset){
        curRAMOffsetVA = offset;
        return true;
    }
    return false;
}

void globalRAMStart(int pid){
    hCurProc = OpenProcess(PROCESS_QUERY_INFORMATION | 
                           PROCESS_VM_OPERATION | PROCESS_VM_READ, 
                           FALSE, pid);
}

bool globalRAMCopy(){
    SIZE_T bRead = 0;
    if (!ReadProcessMemory(hCurProc, (void*)curRAMOffsetVA, RAM, 0x2000, &bRead)){
        return false;
    }
    if (bRead != 0x2000){
        return false;
    }
    return true;
}