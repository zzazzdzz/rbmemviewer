#pragma once

void initializeGraphics();
void redrawGraphics();

extern HBITMAP bTileset;
extern HDC hTileset;

extern HBITMAP bBackBuffer;
extern HDC hBackBuffer;

extern HFONT hFontNormal;
extern HFONT hFontBold;

extern const int ROW_BYTEVIEW_WIDTH;
extern const int ROW_TILEVIEW_WIDTH;
extern const int ROW_ADDRVIEW_WIDTH;
extern const int ROW_TEXT_OFFSET;
extern const int ROW_HEIGHT;
extern const int TILEVIEW_WH;

extern int BYTES_PER_ROW;
extern int NUMBER_OF_ROWS;
extern const int BYTES_PER_ROW_MAX;
extern const int NUMBER_OF_ROWS_MAX;
extern const int BYTES_PER_ROW_MIN;
extern const int NUMBER_OF_ROWS_MIN;

