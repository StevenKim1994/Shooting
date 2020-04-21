#pragma once

#include "iStd.h"

void loadTest();
void freeTest();
void drawTest(float dt);
void keyTest(iKeyState stat, iPoint point);

// pressAnyKey

// Menu (게임시작 / 옵션 / 게임종료)

// Slot

// Option
// - Credits / Howto

// Gamover
// - Really

// -----------------------------------
// popPress
// -----------------------------------
void createPopPrss();
void freePopPress();
void showPopPress(bool show);
void drawPopPress(float dt);
bool keyPopPress(iKeyState stat, iPoint point);

// -----------------------------------
// popStart
// -----------------------------------
void createPopStart();
void freePopStart();
void showPopStart(bool show);
void drawPopStart(float dt);
bool keyPopStart(iKeyState stat, iPoint point);

// -----------------------------------
// popSlot
// -----------------------------------
void createPopSlot();
void freePopSlot();
void showPopSlot(bool show);
void drawPopSlot(float dt);
bool keyPopSlot(iKeyState stat, iPoint point);


// -----------------------------------
// popSettings
// -----------------------------------

void createPopSettings();
void freePopSettings();
void showPopSettings(bool show);
void drawPopSettings(float dt);
bool keyPopSettings(iKeyState stat, iPoint point);