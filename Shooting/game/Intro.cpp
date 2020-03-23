#include "Intro.h"

#include "Loading.h"
#include "Menu.h"

iImage** imgs;
int selectedIntro;

void loadIntro()
{
	printf("loadIntro()\n");

	const char* strMenu[3] = {
		"게임시작",
		"옵션",
		"게임종료"
	};
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(100, 40);
	setStringSize(20);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(1);
	setStringBorderRGBA(0, 0, 0, 1);

	imgs = (iImage**)malloc(sizeof(iImage*) * 3);
	for (int i = 0; i < 3; i++)
	{
		iImage* img = new iImage();
		for(int j=0; j<2; j++)
		{
			g->init(size);
			if (j == 0) setRGBA(0, 0, 1, 1);
			else setRGBA(0, 1, 0, 1);
			g->fillRect(0, 0, size.width, size.height, 10);

			g->drawString(size.width / 2, size.height / 2,
				VCENTER | HCENTER, strMenu[i]);

			Texture* tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake((devSize.width - size.width) / 2,
			(devSize.height - size.height) / 2 - 90 + 60 * i);

		imgs[i] = img;
	}

	selectedIntro = -1;
}

void freeIntro()
{
	printf("freeIntro()\n");
	for (int i = 0; i < 3; i++)
		delete imgs[i];
	free(imgs);
}

void drawIntro(float dt)
{
	setRGBA(1, 0, 0, 1);
	fillRect(0, 0, devSize.width, devSize.height);

	setRGBA(1, 1, 1, 1);
	for (int i = 0; i < 3; i++)
	{
		imgs[i]->setTexAtIndex(i == selectedIntro);
		imgs[i]->paint(dt, iPointMake(0, 0));
	}
	if (selectedIntro != -1)
		printf("selectedIntro = %d\n", selectedIntro);
}

void keyIntro(iKeyState stat, iPoint point)
{
	if (stat == iKeyStateBegan)
	{
		//setLoading(gs_menu, freeIntro, loadMenu);
		
	}
	else if (stat == iKeyStateMoved)
	{
		selectedIntro = -1;
		for (int i = 0; i < 3; i++)
		{
			if (containPoint(point, imgs[i]->touchRect()))
			{
				selectedIntro = i;
				break;
			}
		}
	}
}
