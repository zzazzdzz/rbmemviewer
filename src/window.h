#pragma once

void createWindow(HINSTANCE hInst, int nShow);
int messagePump();
void recalculateWindowSize();
void updateWindowSize();
void recalculateAndUpdateWindowSize();

extern HWND mainWindow;
extern WNDCLASSEX windowClass;

extern int W;
extern int H;