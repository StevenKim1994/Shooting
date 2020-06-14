#pragma once

#include "iStd.h"

void loadDlgQuest();
void freeDlgQuest();
void showDlgQuest(bool show);
void drawDlgQuest(float dt);
void keyDlgQuest(iKeyState stat, iPoint point);

LRESULT CALLBACK dlgQuestProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define QUEST_EXTENSION ".qst"
void loadDataQuest(FILE* pf);
void loadDataQuest(const char* path);
void saveDataQuest(FILE* pf);
void saveDataQuest(const char* path);

void setStringLimit(const char* str, int num, char* output);
