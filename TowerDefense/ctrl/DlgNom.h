#pragma once

#include "iStd.h"

void loadDlgNom(HWND hwnd, HINSTANCE hinstance);
void freeDlgNom();
void dragDlgNom(WPARAM wParam, LPARAM lParam);
void showDlgNom(bool show);
void updateDlgNom(WPARAM wParam, LPARAM lParam);
void drawDlgNom(float dt);

void keyDlgNom(iKeyState stat, iPoint point);

LRESULT CALLBACK dlgNomProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define NOM_EXTENSION ".nom"

void loadDataNom(const char* path);
void saveDataNom(const char* path);