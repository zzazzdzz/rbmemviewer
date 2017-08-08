#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <cstdlib>

#include "memory.h"
#include "window.h"
#include "wndproc.h"
#include "utils.h"
#include "drawing.h"

int WINAPI WinMain(HINSTANCE hThis, HINSTANCE hPrev, LPSTR lpszArgument, int nShow){
    UNREFERENCED_PARAMETER(hPrev);
    UNREFERENCED_PARAMETER(lpszArgument);
    
    initializeHexTables();
    initializeGraphics();
    createWindow(hThis, nShow);
    return messagePump();
}