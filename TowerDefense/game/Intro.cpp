#include "Intro.h"

#include "Loading.h"
#include "Menu.h"

void loadIntro()
{
	printf("loadIntro()\n");
}

void freeIntro()
{
	printf("freeIntro()\n");
}

void drawIntro(float dt)
{
}

void keyIntro(iKeyState stat, iPoint point)
{
	switch (stat) {

	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
		break;

	case iKeyStateEnded:
		break;
	}
}
