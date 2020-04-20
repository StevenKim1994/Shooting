#include "Game.h"

#include "Loading.h"
#include "Intro.h"
#include "Menu.h"
#include "Proc.h"
#include "Ending.h"
#include "Test.h"

void loadGame()
{
#if 0
	loadIntro();
	gameState = gs_intro;
#elif 0
	loadMenu();
	gameState = gs_menu;
#elif 0
	loadEnding();
	gameState = gs_ending;
#else
	loadTest();
	gameState = 100;
#endif
}

void freeGame()
{
	switch (gameState) {
	case gs_intro:	freeIntro(); break;
	case gs_menu:	freeMenu(); break;
	case gs_proc:	freeProc(); break;
	case gs_ending:	freeEnding(); break;
	case 100:		freeTest(); break;
	}
}

void drawGame(float dt)
{
	
	switch (gameState) {
	case gs_intro:	drawIntro(dt); break;
	case gs_menu:	drawMenu(dt); break;
	case gs_proc:	drawProc(dt); break;
	case gs_ending:	drawEnding(dt); break;
	case 100:		drawTest(dt); break;
	}

	drawLoading(dt);

	//static Texture* tex = createImage("assets/ex.png");
	//drawImageLikeCircle(tex, devSize.width / 2, devSize.height / 2, 1.0);
}

void keyGame(iKeyState stat, iPoint point)
{
	if (keyLoading(stat, point))
		return;

	switch (gameState) {
	case gs_intro:	keyIntro(stat, point); break;
	case gs_menu:	keyMenu(stat, point); break;
	case gs_proc:	keyProc(stat, point); break;
	case gs_ending:	keyEnding(stat, point); break;
	case 100:		keyTest(stat, point); break;
	}
}
