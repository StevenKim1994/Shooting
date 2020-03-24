#pragma once

#include "iStd.h"

void loadIntro();
void freeIntro();
void drawIntro(float dt);
void keyIntro(iKeyState stat, iPoint point);

// -------------------------------
// popOption
// -------------------------------
void createPopOption();
void freePopOption();
void showPopOption(bool show);
void drawPopOption(float dt);
bool keyPopOption(iKeyState stat, iPoint point);

