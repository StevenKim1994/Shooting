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
	bool combine;
};

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

void addBlock(int x, int y, int num);
void combineBlock(BLOCK* b);
void copyBlock(BLOCK* to, BLOCK* from);
void moveBlock(int direction);