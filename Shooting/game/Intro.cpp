#include "Intro.h"

#include "Loading.h"
#include "Menu.h"

iImage** imgs;
int selectedIntro;

void loadIntro()
{
	printf("loadIntro()\n");

	//printf("%f\n", getDistanceLine1(iPointMake(2.5, 3.5), iPointMake(0, 0), iPointMake(1.5, 1.5)));

	

	
	const char* strMenu[3] = {
		"게임시작",
		"옵션",
		"게임종료"
	};
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(256, 64);
	setStringSize(20);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(1);
	setStringBorderRGBA(0, 0, 0, 1);

	igImage* ig = g->createIgImage("assets/btn.png");

	imgs = (iImage**)malloc(sizeof(iImage*) * 3);
	for (int i = 0; i < 3; i++)
	{
		iImage* img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			g->init(size);
			//if (j == 0) setRGBA(0, 0, 1, 1);
			//else setRGBA(0, 1, 0, 1);
			//g->fillRect(0, 0, size.width, size.height, 10);
			g->drawImage(ig, 0, -64 * j, TOP | LEFT);

			g->drawString(size.width / 2, size.height / 2,
				VCENTER | HCENTER, strMenu[i]);

			Texture* tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake((devSize.width - size.width) / 2,
			(devSize.height - size.height) / 2 - 90 + 70 * i);

		imgs[i] = img;
	}

	g->freeIgImage(ig);

	selectedIntro = -1;

	createPopOption();
}

void freeIntro()
{
	printf("freeIntro()\n");
	for (int i = 0; i < 3; i++)
		delete imgs[i];
	free(imgs);

	freePopOption();
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

	drawPopOption(dt);
}

void keyIntro(iKeyState stat, iPoint point)
{
	if (keyPopOption(stat, point))
		return;

	switch (stat) {

	case iKeyStateBegan:
		//setLoading(gs_menu, freeIntro, loadMenu);
		if (selectedIntro == 1)
		{
			showPopOption(true);
		}
		break;

	case iKeyStateMoved:
		selectedIntro = -1;
		for (int i = 0; i < 3; i++)
		{
			if (containPoint(point, imgs[i]->touchRect()))
			{
				selectedIntro = i;
				break;
			}
		}
		break;

	case iKeyStateEnded:
		break;
	}
}

// -------------------------------
// popOption
// -------------------------------
iPopup* popOption;
iImage** imgOptionBtn;

void createPopOption()
{
	//popOption = new iPopup(iPopupStyleMove);
	popOption = new iPopup(iPopupStyleAlpha);

	iGraphics* g = iGraphics::instance();

	// background
	iSize size = iSizeMake(512, 256);
	{
		g->init(size);

		setRGBA(0, 0, 0, 1);
		g->fillRect(0, 0, size.width, size.height, 20);

		Texture* tex = g->getTexture();
		iImage* img = new iImage();
		img->addObject(tex);
		freeImage(tex);

		popOption->addObject(img);
	}

	// button
	const char* strOption[3] = {
		"BGM", "SFX", "CLOSE"
	};

	imgOptionBtn = (iImage**)malloc(sizeof(iImage*) * 3);

	setStringSize(20);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);

	iSize sizeBtn = iSizeMake(480, 50);
	for (int i = 0; i < 3; i++)
	{
		iImage* img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			g->init(sizeBtn);
			if (j == 0) setRGBA(1, 0, 0, 1);
			else setRGBA(0, 1, 0, 1);
			g->fillRect(0, 0, sizeBtn.width, sizeBtn.height, 10);

			g->drawString(sizeBtn.width / 2, sizeBtn.height / 2, VCENTER | HCENTER, strOption[i]);

			Texture* tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake((size.width - sizeBtn.width) / 2, 50 + 60 * i);
		popOption->addObject(img);
		imgOptionBtn[i] = img;
	}

	popOption->openPosition = iPointMake((devSize.width - size.width) / 2,
		-size.height);
	popOption->closePosition = iPointMake((devSize.width - size.width) / 2,
		(devSize.height - size.height) / 2);
	popOption->methodOpen = NULL;
	popOption->methodClose = NULL;
}

void freePopOption()
{
	delete popOption;
	free(imgOptionBtn);
}

void showPopOption(bool show)
{
	popOption->show(show);
}

void drawPopOption(float dt)
{
	for (int i = 0; i < 3; i++)
		imgOptionBtn[i]->setTexAtIndex(i == popOption->selected);

	popOption->paint(dt);
}

bool keyPopOption(iKeyState stat, iPoint point)
{
	if (popOption->bShow == false)
		return false;
	if (popOption->stat != iPopupStatProc)
		return true;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		if (popOption->selected == 2)
			showPopOption(false);
		break;

	case iKeyStateMoved:
		for (i = 0; i < 3; i++)
		{
			if (containPoint(point, imgOptionBtn[i]->touchRect(popOption->closePosition)))
			{
				j = i;
				break;
			}
		}
		if (j != -1)
		{
			// if( popOption->selected!=j )  audio play;
			popOption->selected = j;
		}
		break;

	case iKeyStateEnded:
		break;
	}

	return true;
}


