#include "Game.h"

#include "Loading.h"
#include "Intro.h"
#include "Menu.h"
#include "Proc.h"
#include "Ending.h"
void loadGame()
{
#if 1
	loadEnding();
	gameState = gs_ending;
#else
	loadMenu();
	gameState = gs_menu;
#endif
}

void freeGame()
{
	switch (gameState) {
	case gs_intro:	freeIntro(); break;
	case gs_menu:	freeMenu(); break;
	case gs_proc:	freeProc(); break;
	case gs_ending:	freeEnding(); break;
	}
}

void drawGame(float dt)
{
	switch (gameState) {
	case gs_intro:	drawIntro(dt); break;
	case gs_menu:	drawMenu(dt); break;
	case gs_proc:	drawProc(dt); break;
	case gs_ending: drawEnding(dt); break;
	}

	drawLoading(dt);
}

void keyGame(iKeyState stat, iPoint point)
{
	if (keyLoading(stat, point))
		return;

	switch (gameState) {
	case gs_intro:	keyIntro(stat, point); break;
	case gs_menu:	keyMenu(stat, point); break;
	case gs_proc:	keyProc(stat, point); break;
	case gs_ending: keyEnding(stat, point); break;
	}
}
