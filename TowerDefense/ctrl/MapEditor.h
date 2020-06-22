#pragma once

#include "iStd.h"
#include "iWindow.h"

void loadMapEditor(HWND hwnd);
void freeMapEditor();

LRESULT colorMapEditor(WPARAM wParam, LPARAM lParam);
void updateMapEditor(WPARAM wParam, LPARAM lParam);
void drawMapEditor(float dt);
void keyMapEditor(iKeyState stat, iPoint point);
void dragMapEditor(WPARAM wParam, LPARAM lParam);
void scrollMapEditor(WPARAM wParam, LPARAM lParam);

