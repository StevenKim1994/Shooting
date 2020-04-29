#include "2048.h"

#include "Loading.h"
#include "Menu.h"

struct BLOCK
{
	int number;
	iImage* img; // 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096;
};

BLOCK* _block; // 실제로 각위치마다 존재하는 블럭
BLOCK** block; // 현재 화면에 있는 모든 블럭의 주소 없을시 주소 NULL임

int blockNumberMax;

void load2048()
{
	iGraphics* g = iGraphics::instance();
	iImage* imgNumber = new iImage();
	iSize size = iSizeMake(T_SIZE_W, T_SIZE_H);
	for (int i = 0; i < 16; i++) // block 생성
	{
		g->init(size);
		setRGBA(1, 1, 1, 1);
		g->fillRect(0, 0, size.width, size.height, 10);

		setStringSize(20);
		setStringRGBA(0, 0, 0, 1);
		setStringBorder(0);
		g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "%d", 1 << i);

		Texture* tex = g->getTexture();

		imgNumber->addObject(tex);
		freeImage(tex);
	}

	_block = (BLOCK*)malloc(sizeof(BLOCK) * T_NUM_X * T_NUM_Y);
	for (int i = 0; i < T_NUM_X * T_NUM_Y; i++)
		_block[i].img = imgNumber->copy();
	

	delete imgNumber;

	block = (BLOCK**)calloc(sizeof(BLOCK*),  T_NUM_X * T_NUM_Y); // 전체 게임 판 Init;

	blockNumberMax = 1;
	for (int i = 0; i < T_NUM_X; i++)
		addBlock(i, i, 1);


	 
}

void free2048()
{
	for (int i = 0; i < T_NUM_X * T_NUM_Y; i++)
		delete _block[i].img;

		free(_block);
		free(block);

	
}

void draw2048(float dt)
{
	setRGBA(1, 0, 1, 1);
	fillRect(0, 0, T_SIZE_W*T_NUM_X, T_SIZE_H*T_NUM_Y);

	setRGBA(1, 1, 1, 1);

	int i, num = T_NUM_X * T_NUM_Y;
	for (i = 0; i < num; i++)
	{
		BLOCK* b = block[i];
		if (b)
		{
			iPoint p = iPointMake(T_SIZE_W * (i % T_NUM_X), T_SIZE_H * (i / T_NUM_X));
			b->img->paint(dt, p);
		}
	}
	
}

void key2048(iKeyState stat, iPoint point)
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

void addBlock(int x, int y, int num)
{
	int index = T_NUM_X * y + x;//블록의 인덱스
	BLOCK* b = block[index] = &_block[index];

	b->number = num;

	int i;

	for (i = -1; num > 0; i++)
		num = num >> 1;

	b->img->setTexAtIndex(i);


}
