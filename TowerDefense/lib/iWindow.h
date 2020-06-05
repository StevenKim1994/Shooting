#pragma once

#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(lib, "Comctl32.lib")

void initWndCtrlSystem();

void testCtrlSystem(HWND hwnd, HINSTANCE hinstance);

HWND createWndStatic(int x, int y, int width, int height, const char* str);

HWND createWndButton(int x, int y, int width, int height, const char* str);

void testcheckButton(WPARAM wParam, LPARAM lParam);



typedef void (*methodChooseColor) (int r, int g, int b);

void showChooseColor();
