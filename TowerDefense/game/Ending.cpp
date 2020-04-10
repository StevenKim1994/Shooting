#include "Ending.h"

#include "Loading.h"

Texture* texDot;

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
}

void freeEnding()
{
	freeImage(texDot);
}

void drawEnding(float dt)
{
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
}

void keyEnding(iKeyState stat, iPoint point)
{
	//if (stat == iKeyStateBegan)
	//	setLoading(gs_menu, freeProc, loadMenu);
}
