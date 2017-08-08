#include <windows.h>
#include <cstdint>
#include "wndproc.h"
#include "drawing.h"
#include "logic.h"
#include "window.h"

int callCounter = 0;

LRESULT CALLBACK wndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam){
    if (nMsg == WM_PAINT){
        PAINTSTRUCT ps;
        HDC hGraphics = BeginPaint(hWnd, &ps);
        BitBlt(hGraphics, 0, 0, W, H, hBackBuffer, 0, 0, SRCCOPY);
        EndPaint(hWnd, &ps);
        return 0;
    }
    if (nMsg == WM_TIMER){
        if (connectionStatus != STATUS_ACTIVE){
            if (callCounter == 0) processConnectionTick();
        }
        callCounter++;
        callCounter %= 30;
        refreshRAM();
        redrawGraphics();
        RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
        return 0;
    }
    if (nMsg == WM_KEYDOWN){
        switch (wParam){
            case VK_LEFT:
                curRAMOffset -= 1;
                break;
            case VK_RIGHT:
                curRAMOffset += 1;
                break;
            case VK_UP:
                curRAMOffset -= BYTES_PER_ROW;
                break;
            case VK_DOWN:
                curRAMOffset += BYTES_PER_ROW;
                break;
            case VK_PRIOR:
                curRAMOffset -= BYTES_PER_ROW * NUMBER_OF_ROWS;
                break;
            case VK_NEXT:
                curRAMOffset += BYTES_PER_ROW * NUMBER_OF_ROWS;
                break;
            case 'D':
                BYTES_PER_ROW++;
                if (BYTES_PER_ROW > BYTES_PER_ROW_MAX){
                    BYTES_PER_ROW = BYTES_PER_ROW_MAX;
                }
                recalculateAndUpdateWindowSize();
                break;
            case 'A':
                BYTES_PER_ROW--;
                if (BYTES_PER_ROW < BYTES_PER_ROW_MIN){
                    BYTES_PER_ROW = BYTES_PER_ROW_MIN;
                }
                recalculateAndUpdateWindowSize();
                break;
            case 'S':
                NUMBER_OF_ROWS++;
                if (NUMBER_OF_ROWS > NUMBER_OF_ROWS_MAX){
                    NUMBER_OF_ROWS = NUMBER_OF_ROWS_MAX;
                }
                recalculateAndUpdateWindowSize();
                break;
            case 'W':
                NUMBER_OF_ROWS--;
                if (NUMBER_OF_ROWS < NUMBER_OF_ROWS_MIN){
                    NUMBER_OF_ROWS = NUMBER_OF_ROWS_MIN;
                }
                recalculateAndUpdateWindowSize();
                break;
        }
        if (curRAMOffset < 0) curRAMOffset = 0;
        if (curRAMOffset > 0x2000 - BYTES_PER_ROW * NUMBER_OF_ROWS){
            curRAMOffset = 0x2000 - BYTES_PER_ROW * NUMBER_OF_ROWS;
        }
    }
    if (nMsg == WM_CLOSE){
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, nMsg, wParam, lParam);
}