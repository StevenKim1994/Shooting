#pragma once

#include "iStd.h"

void loadDlgQuest();
void freeDlgQuest();
void showDlgQuest(bool show);
void dragDlgQuest(WPARAM wParam, LPARAM lParam);
void updateDlgQuest(WPARAM wParam, LPARAM lParam);
void drawDlgQuest(float dt);
LRESULT coloDlgQuest(WPARAM wParam, LPARAM lParam);
void keyDlgQuest(iKeyState stat, iPoint point);

LRESULT CALLBACK dlgQuestProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


#define QUEST_EXTENSION ".qst"


void loadDataQuest(const char* path);
void saveDataQuest(const char* path);