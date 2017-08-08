#pragma once

extern char hexNumbers2[3 * 0x100];
extern char hexNumbers4[5 * 0x10000];

extern char versionStringBuf[256];

void initializeHexTables();
const char* toHex2(int v);
const char* toHex4(int v);
const char* exeVersionString(const char *filename);
