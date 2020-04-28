#pragma once

#include "iStd.h"

void loadJump();
void freeJump();
void drawJump(float dt);
void keyJump(iKeyState stat, iPoint point);

struct MapTile
{
	uint8 attr;// 0 이동 가능, 1 이동 불가
	uint8 imgIndex;
};

#define MapTileNumX 30
#define MapTileNumY 12
#define MapTileWidth 60
#define MapTileHeight 60

#define tile_color_canMove		1, 1, 1, 1
#define tile_color_canNotMove	1, 0, 1, 1
#define tile_color_end			1, 1, 0, 1

#define natureSpeed 100
#define powGravity 200
#define powJump 500

struct MapHero
{
	iPoint position;
	iSize size;
	float speed;

	iPoint jumpment;
	int jumpNum, _jumpNum;

	void move(iPoint movement);

	void jump();
	void applyJump(iPoint& movement, float dt);
};

#define MapCharWidth 30
#define MapCharHeight 30
#define MapCharSpeed 200

#define hero_color 0, 0, 1, 1