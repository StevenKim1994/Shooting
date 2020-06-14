#pragma once

#include "iStd.h"

void loadScript(HWND hwnd);
void freeScript();
void dragScript(WPARAM wParam, LPARAM lParam);
LRESULT colorScrip(WPARAM wParam, LPARAM lParam);
void updateScript(WPARAM wParam, LPARAM lParam);
void drawScript(float dt);
void keyScript(iKeyState stat, iPoint point);

LRESULT CALLBACK dlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define PROJECT_EXTENSION ".pjt"
void loadDataProject(const char* path);
void saveDataProject(const char* path);

#define SCRIPT_EXTENSION ".scp"
void loadDataSay(FILE* pf);
void loadDataSay(const char* path);
void saveDataSay(FILE* pf);
void saveDataSay(const char* path);
