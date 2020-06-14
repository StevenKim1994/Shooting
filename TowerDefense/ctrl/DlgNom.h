#pragma once

#include "iStd.h"

void loadDlgNom();
void freeDlgNom();
void showDlgNom(bool show);
void drawDlgNom(float dt);
void keyDlgNom(iKeyState stat, iPoint point);

LRESULT CALLBACK dlgNomProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define NOM_EXTENSION ".nom"
void loadDataNom(FILE* pf);
void loadDataNom(const char* path);
void saveDataNom(FILE* pf);
void saveDataNom(const char* path);
