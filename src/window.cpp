#include <windows.h>
#include "window.h"
#include "drawing.h"
#include "wndproc.h"

const char *windowClassName = "wPokeViewer";
const int wndStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

int W;
int H;

HWND mainWindow;
WNDCLASSEX windowClass;

void createWindow(HINSTANCE hInst, int nShow){
    memset(&windowClass, 0, sizeof(windowClass));
    windowClass.lpszClassName = windowClassName;
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = wndProc;
    windowClass.hInstance = hInst;
    windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    RegisterClassEx(&windowClass);
    recalculateWindowSize();
    RECT wndRect = {0, 0, W, H};
    AdjustWindowRect(&wndRect, wndStyle, FALSE);
    mainWindow = CreateWindow(windowClassName, 
                              "RBMemViewer", wndStyle, 100, 100, 
                              wndRect.right - wndRect.left, 
                              wndRect.bottom - wndRect.top,
                              NULL, NULL, hInst, NULL);
    ShowWindow(mainWindow, nShow);
    UpdateWindow(mainWindow);
    SetTimer(mainWindow, 10, 1000 / 30, NULL);
}

int messagePump(){
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

void recalculateWindowSize(){
    W = BYTES_PER_ROW * ROW_BYTEVIEW_WIDTH +    // byteview
        BYTES_PER_ROW * ROW_TILEVIEW_WIDTH +    // tileview
        BYTES_PER_ROW + BYTES_PER_ROW + 2 +     // borders
        ROW_ADDRVIEW_WIDTH;                     // addrbar
    H = NUMBER_OF_ROWS * ROW_HEIGHT +           // viewsize
        NUMBER_OF_ROWS - 1;                     // borders
}

void updateWindowSize(){
    RECT wndRect = {0, 0, W, H};
    AdjustWindowRect(&wndRect, wndStyle, FALSE);
    SetWindowPos(mainWindow, NULL, 0, 0, 
                 wndRect.right - wndRect.left, 
                 wndRect.bottom - wndRect.top,
                 SWP_NOMOVE | SWP_NOZORDER);
}

void recalculateAndUpdateWindowSize(){
    recalculateWindowSize();
    updateWindowSize();
}
