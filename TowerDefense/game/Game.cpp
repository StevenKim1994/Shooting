#include "Game.h"

#include "Loading.h"
#include "Intro.h"
#include "Menu.h"
#include "Proc.h"
#include "Ending.h"
#include "Test.h"
#include "Jump.h";
void loadGame()
{


	AudioInfo ai[4] = { { "assets/snd/0.wav", false, 0.6f },
		{"assets/snd/1.wav", false, 0.6f },
		{"assets/snd/2.wav", true, 1.0f },
		{"assets/snd/3.wav", true, 1.0f },
	};

	loadAudio(ai , 4);
	audioPlay(2);


	loadJump();
	gameState = 101;
}

void freeGame()
{
	switch (gameState) {
	case gs_intro:	freeIntro(); break;
	case gs_menu:	freeMenu(); break;
	case gs_proc:	freeProc(); break;
	case gs_ending:	freeEnding(); break;
	case 100:		freeTest(); break;
	case 101:		freeJump(); break;
	}

//	freeShortestPath();
}

void drawGame(float dt)
{
//	drawShortestPath(dt);
//	return;

	//setClip(0, 50, devSize.width / 2, 300);
	switch (gameState) {
	case gs_intro:	drawIntro(dt); break;
	case gs_menu:	drawMenu(dt); break;
	case gs_proc:	drawProc(dt); break;
	case gs_ending:	drawEnding(dt); break;
	case 100:		drawTest(dt); break;
	case 101:		drawJump(dt); break;
	}

	//setClip(0, 0, 0, 0);
	drawLoading(dt);


	//static Texture* tex = createImage("assets/ex.png");
	//drawImageLikeCircle(tex, devSize.width / 2, devSize.height / 2, 1.0);
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
	case 100:		keyTest(stat, point); break;
	case 101:		keyJump(stat, point); break;
	}
}
