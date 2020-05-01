#pragma once

#include "iStd.h"

void loadTd();
void freeTd();
void drawTd(float dt);
void keyTd(iKeyState stat, iPoint point);

void createPotTdUI();
void freePopTdUI();
void showPopTdUI(bool show);
void drawPopTdUI(float dt);
bool keyPopTdUI(iKeyState stat, iPoint point);

// 타일 정보

#define td_tile_num_x 16
#define td_tile_num_y 8
#define td_tile_size_w 60
#define td_tile_size_h 60

#define td_can_move 1
#define td_can_install 2
#define td_canNot_move 3

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
	
	int monster[4]; //[0] : location출발 [1] : location도착 [2] : index [3] : level [4]: monster_nums
	float delayDt; // 몬스터가 생성되는 간격시간
	
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

// ui (생명, 웨이브, 점수) : 멈춤, 웨이브 빨리 지나가게 하는 버튼

// 줌인 줌아웃

// ui (설치, 파괴 -> 공격범위, 가격, 레벨)

// 게임종료 



