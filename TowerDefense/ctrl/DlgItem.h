#pragma once

#include "iStd.h"

void loadDlgItem();
void freeDlgItem();
void showDlgItem(bool show);
void drawDlgItem(float dt);
void keyDlgItem(iKeyState stat, iPoint point);

LRESULT CALLBACK dlgItemProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define ITEM_EXTENSION ".itm"
void loadDataItem(FILE* pf);
void loadDataItem(const char* path);
void saveDataItem(FILE* pf);
void saveDataItem(const char* path);

