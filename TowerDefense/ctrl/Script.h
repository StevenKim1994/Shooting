#pragma once

#include "iStd.h"

void loadScript(HWND hwnd, HINSTANCE hinstance);
void freeScript();
void dragScript(WPARAM wParam, LPARAM lParam);
void updateScript(WPARAM wParam, LPARAM lParam);
void drawScript(float dt);
LRESULT colorScript(WPARAM wParam, LPARAM lParam);
void keyScript(iKeyState stat, iPoint point);

LRESULT CALLBACK dlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define SCRIPT_EXTENSION ".scp"

void loadDataScript(const char* path);
void saveDataScript(const char* paht);

#define PROJECT_EXTENSION ".pjt"
void loadDataProject(const char* path);
void saveDataProject(const char* path);