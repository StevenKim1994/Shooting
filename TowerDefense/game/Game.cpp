#include "Game.h"

#include "Loading.h"
#include "Intro.h"
#include "Menu.h"
#include "Proc.h"
#include "Ending.h"
void loadGame()
{
/*
	int* a = (int*)malloc(sizeof(int) * 4);
	int* b = (int*)malloc(sizeof(int) * 16);
	
	for (int i = 0; i < 16; i++)
	{
		b[i] = 15-i;
		printf("%d ", b[i]);

	}
	printf("\n");

	int p = 2, q = 1, w = 2, h = 2;

	

	drawRect(0.3f, 0.3f, 0.2f, 0.2f, 0);

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			a[w * j + i] = b[4 * (q + j) + p + i];//memcpy(&a[w*j+i], &b[4 *  (q+j) + p+i], sizeof(int)*2);
	

	for (int i = 0; i < 4; i++)
		printf("%d ", a[i]);
	

	free(a);
	free(b);

	

	*/
#if 1
	loadEnding();
	gameState = gs_ending;
#elif 0
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

