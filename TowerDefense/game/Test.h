#pragma once

#include "iStd.h"

void loadTest();
void freeTest();
void drawTest(float dt);
void keyTest(iKeyState stat, iPoint point);

// ------------------------------------
// popStart
// ------------------------------------
void createPopStart();
void freePopStart();
void showPopStart(bool show);
void drawPopStart(float dt);
bool keyPopStart(iKeyState stat, iPoint point);

void cbCharAnimation(iImage* me);

// ------------------------------------
// popCalc
// ------------------------------------
void createPopCalc();
void freePopCalc();
void showPopCalc(bool show);
void drawPopCalc(float dt);
bool keyPopCalc(iKeyState stat, iPoint point);

// ------------------------------------
// popExe
// ------------------------------------
void createPopExe();
void freePopExe();
void showPopExe(bool show);
void drawPopExe(float dt);
bool keyPopExe(iKeyState stat, iPoint point);
