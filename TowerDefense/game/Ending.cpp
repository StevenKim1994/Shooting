#include "Ending.h"

#include "Loading.h"

Texture* texDot;
iRect txt[4][3];

Texture** strTex;
Texture* answerTex;

iPoint offset;
int tileW, tileH;
int tileNumX, tileNumY;
int selectedTile;

iStrTex** stNumber;
Texture* methodStNumber(const char* str);
iStrTex* stSelect;

iImage** imgNumber;


Texture* methodStNumber(const char* str)
{
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(tileW, tileH);
	g->init(size);

	setRGBA(1, 1, 1, 1);
	g->drawRect(2, 2, tileW - 4, tileH - 4, 10);
	setStringSize(20);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, str);

	Texture* tex = g->getTexture();
	return tex;
}


struct Dot
{
	float life , _life;
	iPoint p, v;// position,단위 vector 

	float speed;

	bool paint(float dt)
	{
		life += dt;
		if (life < _life)
		{
			p += v * speed;
			setRGBA(1, 1, 1, 1.0f - life / _life);
			drawImage(texDot, p.x, p.y, 0, 0, texDot->width, texDot->height,
				VCENTER | HCENTER, 1.0f, 1.0f, 2, 0, REVERSE_NONE);
			return false;
		}
		return true;
	}
	//float alpha, alphaS, alphaE;
};

Dot* _dot;
Dot** dot;
int dotNum;
#define _dotNum 1000

float dotCreateDt;
#define _dotCreateDt	0.1f

Dot* addDot()
{
	for (int i = 0; i < _dotNum; i++)
	{
		Dot* d = &_dot[i];
		if (d->life >= d->_life)
		{
			return d;
		}
	}
	return NULL;
}



void updateDot(float dt)
{
	dotCreateDt += dt;
	if (dotCreateDt < _dotCreateDt)
		return;
	dotCreateDt -= _dotCreateDt;

	Dot* d = addDot();
	if (d == NULL)
		return;
	d->life = 0.0f;
	d->_life = (10 + random() % 50) / 50.0f;
	d->p = iPointMake(devSize.width / 2 + (-20 + random() % 40) / 10.f,
		devSize.height / 2 + (-20 + random() % 40) / 10.f);
	d->v = iPointRotate(iPointMake(1, 0), iPointMake(0, 0), 60 + random() % 60);
	d->speed = (20 + random() % 40) / 10.f;

	dot[dotNum] = d;
	dotNum++;
}

void loadEnding()
{
#if 0 // 내가 한것
	_dot = (Dot*)calloc(sizeof(Dot), _dotNum);
	dot = (Dot**)malloc(sizeof(Dot*) * _dotNum);
	dotNum = 0;
	dotCreateDt = 0.0f;

	uint8* rgba = (uint8*)calloc(sizeof(uint8), 64 * 64 * 4);
	float half = 32.0f;
	for (int j = 0; j < 64; j++)
	{
		for (int i = 0; i < 64; i++)
		{
			float d = sqrtf((i - half) * (i - half) + (j - half) * (j - half));
			if (d < half)
			{
				uint8* buf = &rgba[64 * 4 * j + 4 * i];
				memset(buf, 0xFF, 3);
				buf[3] = 0xFF * (half - d) / half;
			}
		}
	}
	texDot = createImageWithRGBA(rgba, 64, 64);


#else // 강사님이 한것

	offset = iPointMake(10, 10);
	tileW = 100;
	tileH = 100;
	tileNumX = 3;
	tileNumY = 4;
	selectedTile = -1;

	int num = tileNumX * tileNumY;
	stNumber = (iStrTex**)malloc(sizeof(iStrTex*) * num);
	imgNumber = (iImage**)malloc(sizeof(iImage*) * num);
	for (int i = 0; i < num; i++)
	{
		iStrTex* st = new iStrTex(methodStNumber);

		st->setString("%d", i);

		stNumber[i] = st;

		iImage* img = new iImage();
		img->addObject(st->tex);
		img->position = iPointMake(tileW * (i % tileNumX),
			tileH * (i / tileNumX));
		imgNumber[i] = img;
	}

	setStringSize(20);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	stSelect = new iStrTex(NULL);
	stSelect->setString("%d", selectedTile);
#endif
	
}

void freeEnding()
{
#if 0 // 원래 파티클
	freeImage(texDot);


#else // 강사님이 한것
	int num = tileNumX * tileNumY;
	for (int i = 0; i < num; i++)
	{
		delete stNumber[i];
		delete imgNumber[i];
	}
	free(stNumber);
	free(imgNumber);
	delete stSelect;
#endif
	
}

void drawEnding(float dt)
{
#if 0 // 내가 한것


	//iRect txt[4][3];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
		{
			drawRect(110.f * j, 110.f * i, 30.0f, 30.0f, 0);
			txt[i][j] = iRectMake(110.f * j, 110.f * i, 30.f, 30.f);
}

	
#elif 0 // 원래 파티클
	//drawImage(texDot, devSize.width / 2, devSize.height / 2,
	//	VCENTER | HCENTER);
	updateDot(dt);
	for (int i = 0; i < dotNum; i++)
	{
		if (dot[i]->paint(dt))
		{
			dotNum--;
			dot[i] = dot[dotNum];
			i--;
}
	}

#else //강사님이 한것
	int num = tileNumX * tileNumY;
	for (int i = 0; i < num; i++)
	{
		//iPoint p = iPointMake(	offset.x + tileW * (i % tileNumX),
		//						offset.y + tileH * (i / tileNumX));
		//stNumber[i]->paint(p.x + tileW / 2, p.y + tileH / 2, VCENTER | HCENTER);
		imgNumber[i]->paint(dt, offset);
	}
	stSelect->paint(offset.x + tileW * 3 + 20,
		offset.y, TOP | LEFT, "%d", selectedTile);

	

#endif
}

void keyEnding(iKeyState stat, iPoint point)
{
	//if (stat == iKeyStateBegan)
	//	setLoading(gs_menu, freeProc, loadMenu);
	//
	// myButtonChecker
#if 0  //내가 한것
	if(stat == iKeyStateEnded)
	{
		

		for(int i = 0; i<4; i++)
		{
			for(int j = 0; j<3; j++)
			{
				if (containPoint(point, txt[i][j]))
					printf("%d\n", i * 3 + j);

				else
					printf("-1\n");
				
			}
		}
	}
#else // 강사님이 한것

	if (stat == iKeyStateBegan)
	{
		//setLoading(gs_menu, freeProc, loadMenu);
	}
	else if (stat == iKeyStateMoved)
	{
		int i, j = -1, num = tileNumX * tileNumY;
		for (int i = 0; i < num; i++)
		{
			//iRect rt = iRectMake(offset.x + tileW * (i % tileNumX),
			//					offset.y + tileH * (i / tileNumX),
			//					tileW, tileH);
			//if (containPoint(point, rt))
			if (containPoint(point, imgNumber[i]->touchRect(offset)))
			{
				j = i;
				break;
			}
		}
		if (selectedTile != j)
			;// audioplay
		selectedTile = j;
	}
	else if (stat == iKeyStateEnded)
	{

	}

#endif
}
