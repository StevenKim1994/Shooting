#include "2048.h"

#include "Loading.h"
#include "Menu.h"


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

	uint32 key = getKeyDown();
	int move = -1;
	if (key & keyboard_left) move = 0;
	else if (key & keyboard_right) move = 1;
	else if (key & keyboard_up) move = 2;
	else if (key & keyboard_down) move = 3;

	if(move!= -1)
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
	int index = T_NUM_X * y + x;//블록의 인덱스
	BLOCK* b = block[index] = &_block[index];

	b->number = num;

	int i;

	for (i = -1; num > 0; i++)
		num = num >> 1;

	b->img->setTexAtIndex(i);


}

void combineBlock(BLOCK* b)
{
	b->number = b->number << 1;
	int i, num = b->number;
	for (i= - 1; num > 0; i++)
		num = num >> 1;

	b->img->setTexAtIndex(i);
	b->combine = true;
}

void copyBlock(BLOCK* to, BLOCK* from)
{
	to->number = from->number;
	int i, num = to->number;
	for (i = -1; num > 0; i++)
		num = num >> 1;
	to->img->setTexAtIndex(i);
	to->combine = from->combine;
}

void moveBlock(int direction)
{
	int i, num = T_NUM_X * T_NUM_Y;
	for (i = 0; i < num; i++)
	{
		if (block[i])
			block[i]->combine = false;
	}
	int add;

	if (direction == 0) // left 
	{
		for (int j = 0; j < T_NUM_Y; j++)
		{
			for (int i = T_NUM_X - 1; i > 0; i--)
			{
				BLOCK* b = block[T_NUM_X * j + i];
				if (b == NULL) continue;

				if (b->combine) continue; // 이미 한번 합쳐졌으면 더이상 합치면 안됨!

				for (int k = i - 1; i > -1; i--)
				{
					BLOCK* a = block[T_NUM_X * j + k];
					if (a)
					{
						if (b->number == a->number)
						{
							combineBlock(a);
							block[T_NUM_X * j + i] = NULL; // 오브젝트 풀에서 가져오므로 걍 NULL 시키면된다.
						}

						break;
					}
				}
			}

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

							copyBlock(block[T_NUM_X * j + i] = &_block[T_NUM_X * j + i], a);
							
							block[T_NUM_X * j + k] = NULL; //a =NULL;

							if(block[T_NUM_X*j +i-1 ] != NULL)
								if (block[T_NUM_X * j + i]->number == block[T_NUM_X * j + i - 1]->number)
								{
									combineBlock(block[T_NUM_X * j + i - 1]);
									block[T_NUM_X * j + i] = NULL;
								}

							exist = true;
							break;
						}

					}
					if (exist == false)
						break;
				}

				
			}

			// 남은 자리땡겨밀어내기
		}

		for (int i = T_NUM_X - 1; i > -1; i--)
		{
			bool exist = false;

			for (int j = 0; j < T_NUM_Y; j++)
			{
				if (block[T_NUM_X * j + i] == NULL)
				{
					exist = true;
					add = i; // 가로로 몇번째인지...찾음
					break;
				}
			}

			if (exist)
				break;

		}

		while (1)
		{
			int n = random() % T_NUM_Y;
			if (block[T_NUM_X * n + add] == NULL) // 만약 세로에서 랜덤으로 선택해서 그 곳에 값이 비어있다면 거기로 지정함!
			{
				add = T_NUM_X * n + add;
				break;
			}
		}

		

	}
	else if (direction == 1) // right
	{
		
	}
	else if (direction == 2) // up
	{

	}
	else if (direction == 3) // down
	{

	}


	num = blockNumberMax;
	for (i = -1; num > 0; i++)
		num = num >> 1;

	i++;
	i = random() % i;
	if (i < 1)
		i = 1;

	addBlock(add % T_NUM_X, add / T_NUM_X, i);
	
}
