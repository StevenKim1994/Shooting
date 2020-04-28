#pragma once

#include "iStd.h"

void loadJump();
void freeJump();
void drawJump(float dt);
void keyJump(iKeyState stat, iPoint point);

struct MapTile
{
	uint8 attr; // 0 이동 가능, 1 이동 불가
	uint8 imgIndex;
};
#define MapTileNumX 30
#define MapTileNumY 12

#define MapTileWidth 60
#define MapTileHeight 60

#define tile_color_canMove 1, 1, 1, 1
#define tile_color_canNotMove 1, 0, 1, 1
#define tile_color_end 1, 1, 0, 1

#define natureSpeed 100

struct MapHero // 나중에 클래스로 바꿀예정.....첨부터 확장하면 길어지니...
{
	iPoint position;
	iSize size;

	float speed;

	void move(iPoint movement);
};



#define MapCharWidth 30
#define MapCharHeight 30


#define MapCharSpeed 200

#define hero_color 0, 0, 1, 1
