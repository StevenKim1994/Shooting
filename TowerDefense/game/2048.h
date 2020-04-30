#pragma once

#include "iStd.h"

void load2048();
void free2048();
void draw2048(float dt);
void key2048(iKeyState stat, iPoint point);


struct BLOCK
{
	int number;
	iImage* img; // 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096;
	BLOCK* com;// 더해지는 블록대상의 주소 bool combine; 
	float createDt; // 생성되는 시간
	float combineDt;
};
#define _createDt 0.1f

#define _effectBlockDt 0.2f
#define _combineDt 0.1f

/*
원래는 4 * 4 판 
n * n 



처음에는 2,4 or 2 ,2 두개의 숫자가 랜덤위치에 생성됨.
Rule /left, right, up, down/ 
1. 방향키를 누르면 한쪽에 모여있던 같은 숫자끼리 합쳐진다. 한번만 , 8 , 8 , 16  있으면 32가 바로되는게 아니라 16, 16이 되는거임.
2. 반대 편에 최고 숫자보다 작은 숫자가 랜덤하게 하나 생성됨. 만약 이전에 아래방향키를 눌럿을때 위쪽에 숫자가 하나 생김(현재 최고 숫자보다 아래숫자).
3. 1 1 1 이 있을때 오른쪽 방향키를 눌렀을땐 왼쪽부터 합쳐짐 2 1 이 됨.
4. 종료의 조건 : 움직일 수 없을때 (합쳐질게 없을때 && 더이상 생길수 없을때)
5. 클리어의 조건 : 한 숫자를 2048로 만들때
*/

#define T_NUM_X 4 // 가로크기
#define T_NUM_Y 4 // 세로크기

#define T_SIZE_W 60
#define T_SIZE_H 60

void addBlock(int x, int y, int num, bool ani = false, bool combine = false);

void copyBlock(BLOCK* to, BLOCK* from);
void moveBlock(int direction);

iPoint positionOfIndex(int x, int y);

enum BlockStat {
	BlockStatDestroy = 0,
	BlockStatIncrease,
	BlcokStatKeep,
};
struct EffectBlock
{
	Texture* tex;
	iPoint sp, ep;

	int number; 
	BlockStat bs; 
	BLOCK* b;
};

extern int directionCreate;
#define _effectBlockDt 0.2f
		
void loadEffectBlock();
void freeEffectBlock();

bool drawEffectBlock(float dt); // 움직이는것들을 보여줄뿐인 함수
void addEffectBlock(BLOCK* b, iPoint cp, BlockStat stat); // stat : 0 소멸, 1 증가 2 그대로
void changeEffectBlock(BLOCK* b, iPoint ep);

// -----------------------------------------------------------
void createPopGameOver();
void freePopGameOver();
void showPopGameOver(bool show);
void drawPopGameOver(float dt);
bool keyPopGameOver(iKeyState stat, iPoint point);