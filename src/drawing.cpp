#include <windows.h>
#include <cstdint>
#include <cstdlib>
#include "drawing.h"
#include "window.h"
#include "utils.h"
#include "logic.h"
#include "memory.h"

const int ROW_BYTEVIEW_WIDTH = 24;
const int ROW_TILEVIEW_WIDTH = 16;
const int ROW_ADDRVIEW_WIDTH = 40;
const int ROW_TEXT_OFFSET = 6;
const int ROW_HEIGHT = 16;
const int TILEVIEW_WH = 16;

int BYTES_PER_ROW = 20;
int NUMBER_OF_ROWS = 18;
const int BYTES_PER_ROW_MAX = 30;
const int NUMBER_OF_ROWS_MAX = 24;
const int BYTES_PER_ROW_MIN = 4;
const int NUMBER_OF_ROWS_MIN = 2;

const HBRUSH hBackgroundGray = CreateSolidBrush(RGB(127, 127, 127));
const HBRUSH hBackgroundShaded = CreateSolidBrush(RGB(220, 220, 220));
const HBRUSH hBackgroundWhite = CreateSolidBrush(RGB(255, 255, 255));
const HPEN hForegroundBlack = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
const HPEN hForegroundGray = CreatePen(PS_SOLID, 1, RGB(127, 127, 127));

HBITMAP bTileset;
HDC hTileset;

HBITMAP bBackBuffer;
HDC hBackBuffer;

HFONT hFontNormal;
HFONT hFontBold;

void initializeGraphics(){
    bTileset = (HBITMAP)LoadImage(NULL, "tileset.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!bTileset){
        MessageBox(
            NULL,
            "Could not load tileset data.\nIs tileset.bmp in the application directory?",
            "Error",
            MB_OK | MB_ICONERROR
        );
        exit(1);
    }
    
    hTileset = CreateCompatibleDC(NULL);
    SelectObject(hTileset, bTileset);
    
    bBackBuffer = CreateBitmap(1400, 600, 1, 32, NULL);
    hBackBuffer = CreateCompatibleDC(NULL);
    SelectObject(hBackBuffer, bBackBuffer);
    
    LOGFONT lf;
    ZeroMemory(&lf, sizeof(lf));
    lf.lfHeight = 14;
    lf.lfWeight = FW_NORMAL;
    lf.lfOutPrecision = OUT_TT_PRECIS;
    lf.lfQuality = CLEARTYPE_QUALITY;
    strcpy(lf.lfFaceName, "Courier New");
    hFontNormal = CreateFontIndirect(&lf);
    lf.lfWeight = FW_BOLD;
    hFontBold = CreateFontIndirect(&lf);
}

void redrawGraphics(){
    if (connectionStatus != STATUS_ACTIVE){
        SelectObject(hBackBuffer, hForegroundGray);
        SelectObject(hBackBuffer, hFontBold);
        SetTextAlign(hBackBuffer, TA_CENTER | TA_TOP | TA_NOUPDATECP);
        Rectangle(hBackBuffer, -1, -1, W+1, H+1);
        const char *errMsg = "?";
        if (connectionStatus == STATUS_NO_PROCESS){
            errMsg = errMsgNoProcess;
        }else if (connectionStatus == STATUS_SEARCHING){
            errMsg = errMsgSearching;
        }
        TextOut(hBackBuffer, W/2, H/2 - ROW_HEIGHT, errMsg, strlen(errMsg));
    }else{
        SetTextAlign(hBackBuffer, TA_LEFT | TA_TOP | TA_NOUPDATECP);
        for (int i = 0; i < NUMBER_OF_ROWS; i++){
            int yy = -1 + (ROW_HEIGHT+1) * i;
            SelectObject(hBackBuffer, hBackgroundWhite);
            for (int j = 0; j < BYTES_PER_ROW; j++){
                int xx = -1 + ROW_ADDRVIEW_WIDTH + j * (ROW_BYTEVIEW_WIDTH+1) + 1;
                Rectangle(hBackBuffer, xx, yy,
                          xx + ROW_BYTEVIEW_WIDTH + 2, yy + ROW_HEIGHT + 2);
                xx = W - j * ROW_TILEVIEW_WIDTH - j - ROW_TILEVIEW_WIDTH - 1;
                Rectangle(hBackBuffer, xx, yy,
                          xx + ROW_TILEVIEW_WIDTH + 2, yy + ROW_HEIGHT + 2);
            }
            SelectObject(hBackBuffer, hBackgroundShaded);
            Rectangle(hBackBuffer, -1, yy,
                      ROW_ADDRVIEW_WIDTH + 1, yy + ROW_HEIGHT + 2);
        }
        int sepX = ROW_ADDRVIEW_WIDTH + (ROW_BYTEVIEW_WIDTH+1) * BYTES_PER_ROW;
        Rectangle(hBackBuffer, sepX, -1, sepX+3, H+1);
        sepX = ROW_ADDRVIEW_WIDTH - 2;
        Rectangle(hBackBuffer, sepX, -1, sepX+3, H+1);
        SetBkMode(hBackBuffer, TRANSPARENT);
        SelectObject(hBackBuffer, hFontBold);
        for (int i = 0; i < NUMBER_OF_ROWS; i++){
            TextOut(hBackBuffer, 5, 1 + (ROW_HEIGHT+1) * i,
                    toHex4(0xc000 + curRAMOffset + i * BYTES_PER_ROW), 4);
        }
        SelectObject(hBackBuffer, hBackgroundWhite);
        SelectObject(hBackBuffer, hFontNormal);
        for (int i = 0; i < NUMBER_OF_ROWS; i++){
            int yy = (ROW_HEIGHT+1) * i;
            for (int j = 0; j < BYTES_PER_ROW; j++){
                int xx = ROW_ADDRVIEW_WIDTH + (ROW_BYTEVIEW_WIDTH+1) * j + ROW_TEXT_OFFSET;
                int curByte = RAM[curRAMOffset + i * BYTES_PER_ROW + j];
                TextOut(hBackBuffer, xx, yy, toHex2(curByte), 2);
                xx = W - (BYTES_PER_ROW-j) * (ROW_TILEVIEW_WIDTH+1) + 1;
                BitBlt(hBackBuffer, xx, yy, TILEVIEW_WH, TILEVIEW_WH, 
                       hTileset, (TILEVIEW_WH+1) * (curByte % 16),
                       (TILEVIEW_WH+1) * (curByte / 16), SRCCOPY);
            }
        }
    }
}