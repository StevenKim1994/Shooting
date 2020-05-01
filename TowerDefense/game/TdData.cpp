#include "TdData.h"

#define AA td_location_start
#define BB td_location_end
#define XX 99
int tileTd[td_tile_num_x * td_tile_num_y] =
{
	01, 01, 01, 01, 01, 01, 01, 01, 01, XX, XX, XX, XX, XX, XX, XX,
	AA, 01, 01, 01, 01, 01, 01, 01, 01, XX, XX, XX, XX, XX, XX, XX,
	01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, BB,
	01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, XX,
	AA, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, XX, 01,
	01, 01, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, 01, BB,// 
	AA, 01, 01, XX, XX, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01,
	01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01,
};

StageInfo stageInfo[2] =
{
	// stage 0
	{
		5,// life
		3,// wave
		{	// location(s/e), monIndex, monLv, monNum
			{ {16 * 1, 16 * 2, 0, 3}, 1.5f },// waveInfo 0
			{ {16 * 4, 16 * 2, 1, 4}, 1.0f },// waveInfo 1
			{ {16 * 6, 16 * 5, 0, 5}, 0.5f },// waveInfo 2
		}
	},
	// stage 1
	{

	},
};

MonsterInfo monsterInfo[3] =
{
	// monster0
	{ 20, 100 },
	// monster1
	{ 30, 50},
	// monster1
	{ 100, 30},
};

TowerInfo towerInfo[2] =
{
	// 가격/공격력/공범/공속/포커스
	{ 0, 3.0f, 300, 0.5, 100 },// tower0 - 공격
	{ 0, 0.1f, 500, 0.5, 100 },// tower1 - 이속
};