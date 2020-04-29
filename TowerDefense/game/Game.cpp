#include "Game.h"

#include "Loading.h"
#include "Intro.h"
#include "Menu.h"
#include "Proc.h"
#include "Ending.h"

#include "Sadari.h"
#include "Jump.h"
#include "2048.h"

void loadGame()
{
#if 0
	loadIntro();
	gameState = gs_intro;
#else
	loadMenu();
	gameState = gs_menu;
#endif

	AudioInfo ai[4] = {
		{ "assets/snd/0.wav", false, 0.6f },
		{ "assets/snd/1.wav", false, 0.6f },
		{ "assets/snd/2.wav", true, 1.0f },
		{ "assets/snd/3.wav", true, 1.0f },
	};
	loadAudio(ai, 4);

	audioPlay(2);

	//initShortestPath();
}

void freeGame()
{
	switch (gameState) {
	case gs_intro:	freeIntro(); break;
	case gs_menu:	freeMenu(); break;
	case gs_proc:	freeProc(); break;
	case gs_ending:	freeEnding(); break;
	case gs_sadari: freeSadari(); break;
	case gs_jump:	freeJump(); break;
	case gs_2048:	free2048(); break;
	}
	freeAudio();

	//freeShortestPath();
}

void drawGame(float dt)
{
	//drawShortestPath(dt);
	//return;

	switch (gameState) {
	case gs_intro:	drawIntro(dt); break;
	case gs_menu:	drawMenu(dt); break;
	case gs_proc:	drawProc(dt); break;
	case gs_ending:	drawEnding(dt); break;
	case gs_sadari:	drawSadari(dt); break;
	case gs_jump:	drawJump(dt); break;
	case gs_2048:	draw2048(dt); break;
	}

	drawLoading(dt);
}

void keyGame(iKeyState stat, iPoint point)
{
	//keyShortestPath(stat, point);
	//return;

	if (keyLoading(stat, point))
		return;

	switch (gameState) {
	case gs_intro:	keyIntro(stat, point); break;
	case gs_menu:	keyMenu(stat, point); break;
	case gs_proc:	keyProc(stat, point); break;
	case gs_ending:	keyEnding(stat, point); break;
	case gs_sadari:	keySadari(stat, point); break;
	case gs_jump:	keyJump(stat, point); break;
	case gs_2048:	key2048(stat, point); break;
	}
}
