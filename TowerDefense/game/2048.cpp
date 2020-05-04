#include "2048.h"

#include "Loading.h"
#include "Menu.h"

BLOCK* _block;
BLOCK** block;

int blockNumberMax;

void load2048()
{
	printf("load2048()\n");




	int i, num = T_NUM_X * T_NUM_Y;
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(T_SIZE_W, T_SIZE_H);
	iImage* imgNumber = new iImage();
	for (i = 0; i < 16; i++)
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

	_block = (BLOCK*)malloc(sizeof(BLOCK) * num);
	for (i = 0; i < num; i++)
		_block[i].img = imgNumber->copy();

	delete imgNumber;

	block = (BLOCK**)calloc(sizeof(BLOCK*), num);

	blockNumberMax = 1;
	for (i = 0; i < T_NUM_X; i++)
		addBlock(i, i, 1, true);

	loadEffectBlock();
}

void free2048()
{
	printf("freeIntro()\n");

	int i, num = T_NUM_X * T_NUM_Y;
	for (i = 0; i < num; i++)
		delete _block[i].img;
	free(_block);
	free(block);

	freeEffectBlock();
}

void draw2048(float dt)
{
	setRGBA(1, 0, 1, 1);
	fillRect(0, 0, T_SIZE_W * T_NUM_X, T_SIZE_H * T_NUM_Y);

	setRGBA(1, 1, 1, 1);
	int i, num = T_NUM_X * T_NUM_Y;
	for (i = 0; i < num; i++)
	{
		BLOCK* b = block[i];
		if (b)
		{
			if (b->createDt < _createDt)
			{
				float s = b->createDt / _createDt;
				float d = 360 * s;
				b->createDt += dt;

				Texture* tex = b->img->tex;
				iPoint p = iPointMake(T_SIZE_W * (i % T_NUM_X),
					T_SIZE_H * (i / T_NUM_X));
				p += iPointMake(T_SIZE_W / 2, T_SIZE_H / 2);
				drawImage(tex, p.x, p.y, 0, 0, tex->width, tex->height,
					VCENTER | HCENTER, s, s, 2, d, REVERSE_NONE);
			}
			else if (b->combineDt < _combineDt)
			{
				float s = 0.9f + 0.1f * _sin(360 * b->combineDt / _combineDt);
				b->combineDt += dt;

				Texture* tex = b->img->tex;
				iPoint p = iPointMake(T_SIZE_W * (i % T_NUM_X),
					T_SIZE_H * (i / T_NUM_X));
				p += iPointMake(T_SIZE_W / 2, T_SIZE_H / 2);
				drawImage(tex, p.x, p.y, 0, 0, tex->width, tex->height,
					VCENTER | HCENTER, s, s, 2, 0, REVERSE_NONE);
			}
			else
			{
				iPoint p = iPointMake(T_SIZE_W * (i % T_NUM_X),
					T_SIZE_H * (i / T_NUM_X));
				b->img->paint(dt, p);
			}
		}
	}

	if (drawEffectBlock(dt) == false)
	{

		uint32 key = getKeyDown();
		int move = -1;
		if (key & keyboard_left) move = 0;
		else if (key & keyboard_right) move = 1;
		else if (key & keyboard_up) move = 2;
		else if (key & keyboard_down) move = 3;
		if (move != -1)
			moveBlock(move);
	}

	drawPopGameOver(dt);
}

void key2048(iKeyState stat, iPoint point)
{
	if (keyPopGameOver(stat, point))
		return;

	switch (stat) {

	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
		break;

	case iKeyStateEnded:
		break;
	}
}

void addBlock(int x, int y, int num, bool create, bool combine)
{
	int index = T_NUM_X * y + x;
	BLOCK* b = block[index] = &_block[index];
	b->number = num;
#if 0
	int i;
	for (i = -1; num > 0; i++)
		num = num >> 1;
#else
	int i = -1;
	while (num)
	{
		num = num >> 1;
		i++;
	}
#endif
	b->img->setTexAtIndex(i);

	b->createDt = _createDt;
	b->combineDt = _combineDt;
	if (create)
		b->createDt = 0.0f;
	if (combine)
		b->combineDt = 0.0f;
}

void copyBlock(BLOCK* b, BLOCK* a)
{
	b->number = a->number;
	int i, num = b->number;
	for (i = -1; num > 0; i++)
		num = num >> 1;
	b->img->setTexAtIndex(i);
	b->com = NULL;
}

iPoint positionOfIndex(int x, int y)
{
	return iPointMake(T_SIZE_W * x, T_SIZE_H * y);

}

void moveBlock(int direction)
{
	int i, num = T_NUM_X * T_NUM_Y;
	for (i = 0; i < num; i++)
	{
		if (block[i])
			block[i]->com = NULL;
	}

	if (direction == 0)// left
	{
		for (int j = 0; j < T_NUM_Y; j++)
		{
			// 합치기
			for (int i = T_NUM_X - 1; i > 0; i--)
			{
				BLOCK* b = block[T_NUM_X * j + i];
				if (b == NULL) continue;
				if (b->com) continue;

				for (int k = i - 1; k > -1; k--)
				{
					BLOCK* a = block[T_NUM_X * j + k];
					if (a)
					{
						if (b->number == a->number)
						{
#if 0
							combineBlock(a);
							block[T_NUM_X * j + i] = NULL;// b==NULL
#else// #ani
							a->com = b;
							block[T_NUM_X * j + i] = NULL;// b = NULL
							addEffectBlock(a, positionOfIndex(k, j), BlockStatIncrease);
							addEffectBlock(a->com, positionOfIndex(i, j), BlockStatDestroy);
#endif
						}
						break;
					}
				}
			}
			// 땡기기
			for (int i = 0; i < T_NUM_X - 1; i++)
			{
				BLOCK* b = block[T_NUM_X * j + i];
				if (b == NULL)
				{
					bool exist = false;
					for (int k = i + 1; k < T_NUM_X; k++)
					{
						BLOCK* a = block[T_NUM_X * j + k];
						if (a)
						{
#if 0
							//block[T_NUM_X * j + i] = a;// b = a;
							copyBlock(block[T_NUM_X * j + i] = &_block[T_NUM_X * j + i], a);
							block[T_NUM_X * j + k] = NULL;// a = NULL;
#else// #ani
							if (a->com)
							{
								changeEffectBlock(a, positionOfIndex(i, j));
								changeEffectBlock(a->com, positionOfIndex(i, j));
								block[T_NUM_X * j + k] = NULL;// a = NULL;
							}
							else
							{
								addEffectBlock(a, positionOfIndex(k, j), BlcokStatKeep);
								changeEffectBlock(a, positionOfIndex(i, j));
								block[T_NUM_X * j + k] = NULL;
							}
#endif
							exist = true;
							break;
						}
					}
					if (exist == false)
						break;
				}
#if 1// #ani
				else if (b->com)
					changeEffectBlock(b->com, positionOfIndex(i, j));
#endif
			}
		}
	}
	else if (direction == 1)// right
	{
		for (int j = 0; j < T_NUM_Y; j++)
		{
			// 합치기
			for (int i = 0; i < T_NUM_X - 1; i++)
			{
				BLOCK* b = block[T_NUM_X * j + i];
				if (b == NULL) continue;
				if (b->com) continue;

				for (int k = i + 1; k < T_NUM_X; k++)
				{
					BLOCK* a = block[T_NUM_X * j + k];
					if (a)
					{
						if (b->number == a->number)
						{
#if 0
							combineBlock(a);
							block[T_NUM_X * j + i] = NULL;// b==NULL
#else// #ani
							a->com = b;
							block[T_NUM_X * j + i] = NULL;// b==NULL
							addEffectBlock(a, positionOfIndex(k, j), BlockStatIncrease);
							addEffectBlock(a->com, positionOfIndex(i, j), BlockStatDestroy);
#endif
						}
						break;
					}
				}
			}
			// 땡기기
			for (int i = T_NUM_X - 1; i > -1; i--)
			{
				BLOCK* b = block[T_NUM_X * j + i];
				if (b == NULL)
				{
					bool exist = false;
					for (int k = i - 1; k > -1; k--)
					{
						BLOCK* a = block[T_NUM_X * j + k];
						if (a)
						{
#if 0
							//block[T_NUM_X * j + i] = a;// b = a;
							copyBlock(block[T_NUM_X * j + i] = &_block[T_NUM_X * j + i], a);
							block[T_NUM_X * j + k] = NULL;// a = NULL;
#else// #ani
							if (a->com)
							{
								changeEffectBlock(a, positionOfIndex(i, j));
								changeEffectBlock(a->com, positionOfIndex(i, j));
								block[T_NUM_X * j + k] = NULL;// a = NULL
							}
							else
							{
								//copyBlock(block[T_NUM_X * j + i] = &_block[T_NUM_X * j + i], a);
								addEffectBlock(a, positionOfIndex(k, j), BlcokStatKeep);
								changeEffectBlock(a, positionOfIndex(i, j));
								block[T_NUM_X * j + k] = NULL;
							}
#endif
							exist = true;
							break;
						}
					}
					if (exist == false)
						break;
				}
#if 1// #ani
				else if (b->com)
					changeEffectBlock(b->com, positionOfIndex(i, j));
#endif
			}
		}
	}
	else if (direction == 2)// up
	{
		for (int i = 0; i < T_NUM_X; i++)
		{
			// 합치기
			for (int j = T_NUM_Y - 1; j > -1; j--)
			{
				BLOCK* b = block[T_NUM_X * j + i];
				if (b == NULL) continue;
				if (b->com) continue;

				for (int k = j - 1; k > -1; k--)
				{
					BLOCK* a = block[T_NUM_X * k + i];
					if (a)
					{
						if (b->number == a->number)
						{
#if 0
							combineBlock(a);
							block[T_NUM_X * j + i] = NULL;// b==NULL
#else// #ani
							a->com = b;
							block[T_NUM_X * j + i] = NULL;// b = NULL
							addEffectBlock(a, positionOfIndex(i, k), BlockStatIncrease);
							addEffectBlock(a->com, positionOfIndex(i, j), BlockStatDestroy);
#endif
						}
						break;
					}
				}
			}
			// 땡기기
			for (int j = 0; j < T_NUM_Y; j++)
			{
				BLOCK* b = block[T_NUM_X * j + i];
				if (b == NULL)
				{
					bool exist = false;
					for (int k = j + 1; k < T_NUM_Y; k++)
					{
						BLOCK* a = block[T_NUM_X * k + i];
						if (a)
						{
#if 0
							//block[T_NUM_X * j + i] = a;// b = a;
							copyBlock(block[T_NUM_X * j + i] = &_block[T_NUM_X * j + i], a);
							block[T_NUM_X * k + i] = NULL;// a = NULL;
#else// #ani
							if (a->com)
							{
								changeEffectBlock(a, positionOfIndex(i, j));
								changeEffectBlock(a->com, positionOfIndex(i, j));
								block[T_NUM_X * k + i] = NULL;// a = NULL;
							}
							else
							{
								addEffectBlock(a, positionOfIndex(i, k), BlcokStatKeep);
								changeEffectBlock(a, positionOfIndex(i, j));
								block[T_NUM_X * k + i] = NULL;
							}
#endif
							exist = true;
							break;
						}
					}
					if (exist == false)
						break;
				}
#if 1// #ani
				else if (b->com)
					changeEffectBlock(b->com, positionOfIndex(i, j));
#endif
			}
		}
	}
	else if (direction == 3)// down
	{
		for (int i = 0; i < T_NUM_X; i++)
		{
			// 합치기
			for (int j = 0; j < T_NUM_Y - 1; j++)
			{
				BLOCK* b = block[T_NUM_X * j + i];
				if (b == NULL) continue;
				if (b->com) continue;

				for (int k = j + 1; k < T_NUM_Y; k++)
				{
					BLOCK* a = block[T_NUM_X * k + i];
					if (a)
					{
						if (b->number == a->number)
						{
#if 0
							combineBlock(a);
							block[T_NUM_X * j + i] = NULL;// b==NULL
#else// #ani
							a->com = b;
							block[T_NUM_X * j + i] = NULL;// b = NULL
							addEffectBlock(a, positionOfIndex(i, k), BlockStatIncrease);
							addEffectBlock(a->com, positionOfIndex(i, j), BlockStatDestroy);
#endif
						}
						break;
					}
				}
			}
			// 땡기기
			for (int j = T_NUM_Y - 1; j > -1; j--)
			{
				BLOCK* b = block[T_NUM_X * j + i];
				if (b == NULL)
				{
					bool exist = false;
					for (int k = j - 1; k > -1; k--)
					{
						BLOCK* a = block[T_NUM_X * k + i];
						if (a)
						{
#if 0
							//block[T_NUM_X * j + i] = a;// b = a;
							copyBlock(block[T_NUM_X * j + i] = &_block[T_NUM_X * j + i], a);
							block[T_NUM_X * k + i] = NULL;// a = NULL;
#else// #ani
							if (a->com)
							{
								changeEffectBlock(a, positionOfIndex(i, j));
								changeEffectBlock(a->com, positionOfIndex(i, j));
								block[T_NUM_X * k + i] = NULL;// a = NULL;
							}
							else
							{
								addEffectBlock(a, positionOfIndex(i, k), BlcokStatKeep);
								changeEffectBlock(a, positionOfIndex(i, j));
								block[T_NUM_X * k + i] = NULL;
							}
#endif
							exist = true;
							break;
						}
					}
					if (exist == false)
						break;
				}
#if 1// #ani
				else if (b->com)
					changeEffectBlock(b->com, positionOfIndex(i, j));
#endif
			}
		}
	}

	directionCreate = direction;
}

EffectBlock* _effectBlock;
EffectBlock** effectBlock;
int effectBlockNum;

float effectBlockDt;

int directionCreate;

void loadEffectBlock()
{
	int num = T_NUM_X * T_NUM_Y;
	_effectBlock = (EffectBlock*)malloc(sizeof(EffectBlock) * num);
	effectBlock = (EffectBlock**)malloc(sizeof(EffectBlock*) * num);
	effectBlockNum = 0;

	effectBlockDt = 0.0f;
}

void freeEffectBlock()
{
	free(_effectBlock);
	free(effectBlock);
}

bool drawEffectBlock(float dt)
{
	if (effectBlockNum == 0)
		return false;

	int i, num;

	float r1 = effectBlockDt / _effectBlockDt;
	float r0 = 1.0 - r1;

	for (i = 0; i < effectBlockNum; i++)
	{
		EffectBlock* eb = effectBlock[i];
		Texture* tex = eb->tex;
		iPoint p = eb->sp * r0 + eb->ep * r1;
		drawImage(tex, p.x, p.y, TOP | LEFT);
	}

	effectBlockDt += dt;
	if (effectBlockDt > _effectBlockDt)
	{
		for (i = 0; i < effectBlockNum; i++)
		{
			EffectBlock* eb = effectBlock[i];
			if (eb->bs > BlockStatDestroy)
			{
				int x = eb->ep.x;	x /= T_SIZE_W;
				int y = eb->ep.y;	y /= T_SIZE_H;
				int num = eb->number;// keep
				if (eb->bs == BlockStatIncrease)// increase
					num = num << 1;
				addBlock(x, y, num, false, eb->bs == BlockStatIncrease);
			}
		}
		effectBlockNum = 0;
		effectBlockDt = 0.0f;

		int add = -1;
		if (directionCreate == 0)
		{
			// 오른쪽에서 빈공간이 있는지 체크
			for (int i = T_NUM_X - 1; i > -1; i--)
			{
				bool exist = false;
				for (int j = 0; j < T_NUM_Y; j++)
				{
					if (block[T_NUM_X * j + i] == NULL)
					{
						exist = true;
						add = i;
						break;
					}
				}
				if (exist)
					break;
			}
			// 그 빈공간에서 랜덤한 세로 찾아서, add
			while (1)
			{
				int n = random() % T_NUM_Y;
				if (block[T_NUM_X * n + add] == NULL)
				{
					add = T_NUM_X * n + add;
					break;
				}
			}
		}
		else if (directionCreate == 1)
		{
			// 왼쪽에서 빈공간이 있는지 체크
			for (int i = 0; i < T_NUM_X; i++)
			{
				bool exist = false;
				for (int j = 0; j < T_NUM_Y; j++)
				{
					if (block[T_NUM_X * j + i] == NULL)
					{
						exist = true;
						add = i;
						break;
					}
				}
				if (exist)
					break;
			}
			// 그 빈공간에서 랜덤한 세로 찾아서, add
			while (1)
			{
				int n = random() % T_NUM_Y;
				if (block[T_NUM_X * n + add] == NULL)
				{
					add = T_NUM_X * n + add;
					break;
				}
			}
		}
		else if (directionCreate == 2)
		{
			// 아래쪽에서 빈공간이 있는지 체크
			for (int j = T_NUM_Y - 1; j > -1; j--)
			{
				bool exist = false;
				for (int i = 0; i < T_NUM_X; i++)
				{
					if (block[T_NUM_X * j + i] == NULL)
					{
						exist = true;
						add = j;
						break;
					}
				}
				if (exist)
					break;
			}
			// 그 빈공간에서 랜덤한 세로 찾아서, add
			while (1)
			{
				int n = random() % T_NUM_X;
				if (block[T_NUM_X * add + n] == NULL)
				{
					add = T_NUM_X * add + n;
					break;
				}
			}
		}
		else// if (directionCreate == 3)
		{
			// 위쪽에서 빈공간이 있는지 체크
			for (int j = 0; j < T_NUM_Y; j--)
			{
				bool exist = false;
				for (int i = 0; i < T_NUM_X; i++)
				{
					if (block[T_NUM_X * j + i] == NULL)
					{
						exist = true;
						add = j;
						break;
					}
				}
				if (exist)
					break;
			}
			// 그 빈공간에서 랜덤한 세로 찾아서, add
			while (1)
			{
				int n = random() % T_NUM_X;
				if (block[T_NUM_X * add + n] == NULL)
				{
					add = T_NUM_X * add + n;
					break;
				}
			}
		}

		if (add != -1)
		{
			num = blockNumberMax;
			for (i = -1; num > 0; i++)
				num = num >> 1;
			i++;
			i = random() % i;
			if (i < 1)
				i = 1;
			addBlock(add % T_NUM_X, add / T_NUM_X, i, true);

			// full
			bool full = true;
			num = T_NUM_X * T_NUM_Y;
			for (i = 0; i < num; i++)
			{
				if (block[i] == NULL)
				{
					full = false;
					break;
				}
			}
			if (full)
			{
				// 모든 블럭이 Full인 상태 && 인접 블럭이 같은것이 없을때  => 게임 종료
				bool gameover = true;
				num = T_NUM_X * T_NUM_Y;
				for (i = 0; i < num; i++)
				{
					int n = block[i]->number;
					int x = i % T_NUM_X;
					int y = i / T_NUM_X;
					if ((x < T_NUM_X - 1 && block[T_NUM_X * y + x + 1]->number == n) ||
						(y < T_NUM_Y - 1 && block[T_NUM_X * (y + 1) + x]->number == n))
					{
						gameover = false;
						break;
					}
				}

				if (gameover)
				{
					printf("gameOver\n");
				}
			}
		}
	}

	return true;
}

void addEffectBlock(BLOCK* b, iPoint cp, BlockStat bs)
{
	EffectBlock* eb = &_effectBlock[effectBlockNum];
	effectBlock[effectBlockNum] = eb;
	effectBlockNum++;

	eb->tex = b->img->tex;
	eb->sp = eb->ep = cp;
	eb->number = b->number;
	eb->bs = bs;
	eb->b = b;
}

void changeEffectBlock(BLOCK* b, iPoint ep)
{
	for (int i = 0; i < effectBlockNum; i++)
	{
		EffectBlock* eb = effectBlock[i];
		if (eb->b == b)
		{
			eb->ep = ep;
			return;
		}
	}
}
iPopup* popGameOver;
iImage** imgGameOverBtn;

void createPopGameOver()
{
  
}

void freePopGameOver()
{
}

void showPopGameOver(bool show)
{
	popGameOver->show(show);
}

void drawPopGameOver(float dt)
{
	popGameOver->paint(dt);
}

bool keyPopGameOver(iKeyState stat, iPoint point)
{


	if (popGameOver->bShow == false)
		return false;

	if (popGameOver->stat != iPopupStatProc)
		return true;

	int i, j = -1;

	switch (stat)
	{
	case iKeyStateBegan:
		i = popGameOver->selected;
		if (i == 0)
		{
			setLoading(gs_menu, free2048, loadMenu);
		}
		break;
	case iKeyStateMoved:
		for (int i = 0; i < 1; i++)
		{
			if (containPoint(point, imgGameOverBtn[i]->touchRect(popGameOver->closePosition)))
			{
				j = i;
				break;
			}
			if (popGameOver->selected != j)
				audioPlay(0);

		}
		break;
	case iKeyStateEnded:
		break;

	}

	return true;
}

