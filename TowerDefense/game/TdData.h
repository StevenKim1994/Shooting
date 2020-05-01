#pragma once

#include "iStd.h"

// 타일 정보
#define td_tile_num_x 16
#define td_tile_num_y 8
#define td_tile_size_w 60
#define td_tile_size_h 60
static iPoint centerOfTileIndex(int index)
{
	return iPointMake(td_tile_size_w * (index % td_tile_num_x) + td_tile_size_w / 2,
		td_tile_size_h * (index / td_tile_num_x) + td_tile_size_h / 2);
}

#define td_can_move 1
#define td_can_install 2
#define td_canNot_move 99
#define td_location_start 11
#define td_location_end	 12

#define td_rgba_can_move	1, 1, 1, 1
#define td_rgba_can_install 1, 0, 1, 1
#define td_rgba_canNot_move 0, 0, 0, 1
#define td_rgba_start 0, 1, 0, 1
#define td_rgba_end 0, 1, 0, 1

extern int tileTd[td_tile_num_x * td_tile_num_y];

// 웨이브 정보
struct MonsterInfo
{
	float hp;
	float speed;
};
extern MonsterInfo monsterInfo[3];

struct WaveInfo
{
	// [0] : location출발, [1] : location도착 [2] : index, [3] : lv, [4] : num
	int monster[5];
	float delayDt;
};

struct TowerInfo
{
	int price;
	float att;
	int range;
	float rate;
	float focusRate;
};
extern TowerInfo towerInfo[2];


struct StageInfo
{
	int life;
	int wave;
	WaveInfo waveInfo[30];
};

extern StageInfo stageInfo[2];



// field(길찾기, 몹)

// ui(생명, 웨이브, 점수) : 멈춤, 웨이브서두르는것

// ui(설치, 파괴 => 공격범위, 가격, 레벨)

// 게임종료