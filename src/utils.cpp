#include <windows.h>
#include <cstdio>
#include <cstdint>
#include "utils.h"

char hexNumbers2[3 * 0x100];
char hexNumbers4[5 * 0x10000];

char versionStringBuf[256];

void initializeHexTables(){
    const char *charset = "0123456789ABCDEF";
    for (int i=0; i<=0xff; i++){
        hexNumbers2[i*3] = charset[(i >> 4) & 0xf];
        hexNumbers2[i*3 + 1] = charset[i & 0xf];
        hexNumbers2[i*3 + 2] = '\0';
    }
    for (int i=0; i<=0xffff; i++){
        hexNumbers4[i*5] = charset[(i >> 12) & 0xf];
        hexNumbers4[i*5 + 1] = charset[(i >> 8) & 0xf];
        hexNumbers4[i*5 + 2] = charset[(i >> 4) & 0xf];
        hexNumbers4[i*5 + 3] = charset[i & 0xf];
        hexNumbers4[i*5 + 4] = '\0';
    }
}

const char* toHex2(int v){
    v &= 0xff;
    return &hexNumbers2[3 * v];
}

const char* toHex4(int v){
    v &= 0xffff;
    return &hexNumbers4[5 * v];
}

const char* exeVersionString(const char *filename){
    size_t len = GetFileVersionInfoSize(filename, 0);
    if (len == 0) return NULL;
    char *buf = new char[len];
    char *ret = NULL;
    if (GetFileVersionInfo(filename, 0, len, buf)){
        int *codepage;
        if (VerQueryValue(buf, "\\VarFileInfo\\Translation", (LPVOID*)&codepage, &len)){
            char fmt[128];
            sprintf(fmt, "\\StringFileInfo\\%04x%04x\\FileDescription",
                    (*codepage) & 0xffff, (*codepage) >> 16);
            char *strPtr;
            if (VerQueryValue(buf, fmt, (LPVOID*)&strPtr, &len)){
                sprintf(versionStringBuf, "%.250s", strPtr);
				ret = versionStringBuf;
            }
        }
    }
    delete [] buf;
    return ret;
}