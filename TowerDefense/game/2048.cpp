#include "2048.h"

#include "Loading.h"
#include "Menu.h"

BLOCK* _block;
BLOCK** block;

int blockNumberMax;

float blockAniDt;

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
	{
		_block[i].img = imgNumber->copy();
		_block[i].p = iPointZero;
	}

	delete imgNumber;

	block = (BLOCK**)calloc(sizeof(BLOCK*), num);

	blockNumberMax = 1;
	for (i = 0; i < T_NUM_X; i++)
		addBlock(i, i, 1);

	blockAniDt = 0.0f;
}

void free2048()
{
	printf("freeIntro()\n");

	int i, num = T_NUM_X * T_NUM_Y;
	for (i = 0; i < num; i++)
		delete _block[i].img;
	free(_block);
	free(block);
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
			iPoint p = iPointMake(T_SIZE_W * (i % T_NUM_X),
				T_SIZE_H * (i / T_NUM_X));
#if 1// #ani
			p += b->p * (blockAniDt / block_move_dt);
#endif
			b->img->paint(dt, p);
		}
	}

	if (blockAniDt)
	{
		blockAniDt -= dt;
		if (blockAniDt <= 0.0f)
		{
			blockAniDt = 0.0f;
			for (i = 0; i < num; i++)
			{
				if (block[i])
					block[i]->p = iPointZero;
			}
		}
		return;
	}
	uint32 key = getKeyDown();
	int move = -1;
	if (key & keyboard_left) move = 0;
	else if (key & keyboard_right) move = 1;
	else if (key & keyboard_up) move = 2;
	else if (key & keyboard_down) move = 3;
	if (move != -1)
		moveBlock(move);
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
}

void combineBlock(BLOCK* b)
{
	b->number *= 2;
	int i, num = b->number;
	for (i = -1; num > 0; i++)
		num = num >> 1;
	b->img->setTexAtIndex(i);
	b->combine = true;
}

void copyBlock(BLOCK* b, BLOCK* a)
{
	b->number = a->number;
	int i, num = b->number;
	for (i = -1; num > 0; i++)
		num = num >> 1;
	b->img->setTexAtIndex(i);
	b->combine = a->combine;
}

void moveBlock(int direction)
{
	int i, num = T_NUM_X * T_NUM_Y;
	for (i = 0; i < num; i++)
	{
		if (block[i])
			block[i]->combine = false;
	}

	int add = -1;
	if (direction == 0)// left
	{
		for (int j = 0; j < T_NUM_Y; j++)
		{
			// 합치기
			for (int i = T_NUM_X - 1; i > 0; i--)
			{
				BLOCK* b = block[T_NUM_X * j + i];
				if (b == NULL) continue;
				if (b->combine) continue;

				for (int k = i - 1; k > -1; k--)
				{
					BLOCK* a = block[T_NUM_X * j + k];
					if (a)
					{
						if (b->number == a->number)
						{
							combineBlock(a);
							block[T_NUM_X * j + i] = NULL;// b==NULL
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
							//block[T_NUM_X * j + i] = a;// b = a;
							copyBlock(block[T_NUM_X * j + i] = &_block[T_NUM_X * j + i], a);
#if 1// #ani
							block[T_NUM_X * j + i]->p = iPointMake(T_SIZE_W * (i - k), 0);
							blockAniDt = block_move_dt;
#endif
							block[T_NUM_X * j + k] = NULL;// a = NULL;
							exist = true;
							break;
						}
					}
					if (exist == false)
						break;
				}
			}
		}
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
	else if (direction == 1)// right
	{
		for (int j = 0; j < T_NUM_Y; j++)
		{
			// 합치기
			for (int i = 0; i < T_NUM_X - 1; i++)
			{
				BLOCK* b = block[T_NUM_X * j + i];
				if (b == NULL) continue;
				if (b->combine) continue;

				for (int k = i + 1; k < T_NUM_X; k++)
				{
					BLOCK* a = block[T_NUM_X * j + k];
					if (a)
					{
						if (b->number == a->number)
						{
							combineBlock(a);
							block[T_NUM_X * j + i] = NULL;// b==NULL
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
							//block[T_NUM_X * j + i] = a;// b = a;
							copyBlock(block[T_NUM_X * j + i] = &_block[T_NUM_X * j + i], a);
							block[T_NUM_X * j + k] = NULL;// a = NULL;
							exist = true;
							break;
						}
					}
					if (exist == false)
						break;
				}
			}
		}
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
	else if (direction == 2)// up
	{
		for (int i = 0; i < T_NUM_X; i++)
		{
			// 합치기
			for (int j = T_NUM_Y - 1; j > -1; j--)
			{
				BLOCK* b = block[T_NUM_X * j + i];
				if (b == NULL) continue;
				if (b->combine) continue;

				for (int k = j - 1; k > -1; k--)
				{
					BLOCK* a = block[T_NUM_X * k + i];
					if (a)
					{
						if (b->number == a->number)
						{
							combineBlock(a);
							block[T_NUM_X * j + i] = NULL;// b==NULL
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
							//block[T_NUM_X * j + i] = a;// b = a;
							copyBlock(block[T_NUM_X * j + i] = &_block[T_NUM_X * j + i], a);
							block[T_NUM_X * k + i] = NULL;// a = NULL;
							exist = true;
							break;
						}
					}
					if (exist == false)
						break;
				}
			}
		}
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
	else if (direction == 3)// down
	{
		for (int i = 0; i < T_NUM_X; i++)
		{
			// 합치기
			for (int j = 0; j < T_NUM_Y - 1; j++)
			{
				BLOCK* b = block[T_NUM_X * j + i];
				if (b == NULL) continue;
				if (b->combine) continue;

				for (int k = j + 1; k < T_NUM_Y; k++)
				{
					BLOCK* a = block[T_NUM_X * k + i];
					if (a)
					{
						if (b->number == a->number)
						{
							combineBlock(a);
							block[T_NUM_X * j + i] = NULL;// b==NULL
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
							//block[T_NUM_X * j + i] = a;// b = a;
							copyBlock(block[T_NUM_X * j + i] = &_block[T_NUM_X * j + i], a);
							block[T_NUM_X * k + i] = NULL;// a = NULL;
							exist = true;
							break;
						}
					}
					if (exist == false)
						break;
				}
			}
		}
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
		addBlock(add % T_NUM_X, add / T_NUM_X, i);

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

