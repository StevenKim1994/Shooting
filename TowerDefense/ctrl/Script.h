#pragma once

#include "iStd.h"

void loadScript(HWND hwnd, HINSTANCE hinstance);
void freeScript();
void dragScript(WPARAM wParam, LPARAM lParam);
void updateScript(WPARAM wParam, LPARAM lParam);
void drawScript(float dt);
void keyScript(iKeyState stat, iPoint point);

LRESULT CALLBACK dlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


