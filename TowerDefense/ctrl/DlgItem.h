#pragma once

#include "iStd.h"



void loadDlg(HWND hwnd, HINSTANCE hinstance);
void freeDlgItem();
void showDlgItem(bool show);
void dragDlgItem(WPARAM wParam, LPARAM lParam);
void updateDlgItem(WPARAM wParam, LPARAM lParam);
void drawDlgItem(float dt);
LRESULT coloDlgItem(WPARAM wParam, LPARAM lParam);
void keyDlgItem(iKeyState stat, iPoint point);

LRESULT CALLBACK dlgItemProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


#define ITEM_EXTENSION ".itm"


void loadDataItem(const char* path);
void saveDataItem(const char* path);