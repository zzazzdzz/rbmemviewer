#pragma once
#include <stdint.h>

bool verifyMemoryFragment(uint8_t *mem, size_t offset, size_t size);
size_t searchMemoryFragment(uint8_t *mem, size_t size);
size_t findGameRAMInProcessMemory(int pid);
bool testRAM(int pid);
void globalRAMStart(int pid);
bool globalRAMCopy();

extern size_t curRAMOffsetVA;
extern HANDLE hCurProc;
