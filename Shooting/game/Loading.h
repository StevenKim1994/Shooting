#pragma once

#include "iStd.h"

#define gs_intro	0
#define gs_menu		1
#define gs_proc		2
#define gs_none		0xFF
extern int gameState;

//setLoading(gs_proc, freeMenu, loadProc);

typedef void (*METHOD_LOADING)();
void setLoading(int newGameState, METHOD_LOADING free, METHOD_LOADING load);

void drawLoading(float dt);
bool keyLoading(iKeyState stat, iPoint point);

