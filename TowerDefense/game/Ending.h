#pragma once

#include "iStd.h"

void loadEnding();
void freeEnding();
void drawEnding(float dt);
void keyEnding(iKeyState stat, iPoint point);

// ----------------------------------------
// popEndingMenu
// ----------------------------------------
void createPopEndingMenu();
void freePopEndingMenu();
void showPopEndingMenu(bool show);
void drawPopEndingMenu(float dt);
bool keyPopEndingMenu(iKeyState stat, iPoint point);

// ----------------------------------------
// popNumber
// ----------------------------------------
void createPopNumber();
void freePopNumber();
void showPopNumber(bool show);
void drawPopNumber(float dt);
bool keyPopNumber(iKeyState stat, iPoint point);
