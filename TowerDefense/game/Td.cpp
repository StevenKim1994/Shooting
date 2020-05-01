#include "Td.h"

void loadTd()
{
	printf("loadTd()\n");
	createPotTdUI();
}

void freeTd()
{
}

void drawTd(float dt)
{
	setRGBA(1, 0, 0, 1);
	fillRect(0, 0, devSize.width, devSize.height);

	drawPopTdUI(dt);
}

void keyTd(iKeyState stat, iPoint point)
{
	if (keyPopTdUI(stat, point))
		return true;

	switch (stat)
	{
	iKeyStateBegan:
		break;

	iKeyStateMoved:
		break;

	iKeyStateEnded:
		break;
	}
	
}

void createPotTdUI()
{
}

void freePopTdUI()
{
}

void showPopTdUI(bool show)
{
}

void drawPopTdUI(float dt)
{
}

bool keyPopTdUI(iKeyState stat, iPoint point)
{
	return false;
}

int tileTd[td_tile_num_x * td_tile_num_y] =
{
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,//
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,//
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

StageInfo stageInfo[2] =
{
	// stage 0
	{
		5, // life
		3, // wave
		{ // locationState, locationEnd, monIndex, monLv, monNum
			{ {16*1,16*2,0, 3}, 0.5f},// waveInfo 0 : 첫번째 위치
			{ {16*4,16*2,1 ,4}, 0.3f},// waveInfo 1
			{ {16*6,16*5,0 , 5}, 0.1f}, //waveInfo 2
		}
	},

	// stage 1
	{

	},

};

MonsterInfo monsterInfo[3] =
{
	// monster 0
	{20, 100}, // hp , speed
	// monster 1
	{30, 50},
	// monster 2
	{100, 30}, 
};

TowerInfo towerInfo[2] =
{
	// tower0 - 공격
	{0, 3.0f,300, 0.5f, 100},
	// topwer1 - 몬스터의 이동속도를 느리게
	{0, 0.1f, 500, 0.5f, 100},
	// 가격 / 공격력 / 공격범위 / 공격속도/ 포커스속도
};