#include "Ending.h"

#include "Loading.h"

void loadEnding()
{
	createPopEndingMenu();
	createPopNumber();

	showPopEndingMenu(true);
}

void freeEnding()
{
	freePopEndingMenu();
	freePopNumber();
}

void drawEnding(float dt)
{
	drawPopEndingMenu(dt);
	drawPopNumber(dt);
}

void keyEnding(iKeyState stat, iPoint point)
{
	if (keyPopNumber(stat, point))
		return;

	if (keyPopEndingMenu(stat, point))
		return;
}

// ----------------------------------------
// popEndingMenu
// ----------------------------------------
iPopup* popEndingMenu;
iImage** imgEndingMenuBtn;

void createPopEndingMenu()
{
	iImage* img;
	Texture* tex;
	int i, j;

	iPopup* pop = new iPopup(iPopupStyleMove);

	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(128, 64);
	imgEndingMenuBtn = (iImage**)malloc(sizeof(iImage*) * 1);
	const char* strBtn[] = { "열려팝업", };
	iPoint posBtn[] = { {100, 100}, };
	for (i = 0; i < 1; i++)
	{
		img = new iImage();
		for (j = 0; j < 2; j++)
		{
			g->init(size);

			if (j == 0) setRGBA(1, 0, 0, 1);
			else setRGBA(0, 0, 1, 1);
			g->fillRect(0, 0, size.width, size.height, 10);
			setStringSize(20);
			setStringRGBA(1, 1, 1, 1);
			setStringBorder(0);
			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);

			tex = g->getTexture();
			img->addObject(tex);
		}
		img->position = posBtn[i];
		imgEndingMenuBtn[i] = img;
		pop->addObject(img);
	}

	pop->openPosition = iPointMake(devSize.width, 0);
	pop->closePosition = iPointMake(0, 0);
	popEndingMenu = pop;
}

void freePopEndingMenu()
{
	delete popEndingMenu;
	free(imgEndingMenuBtn);
}

void showPopEndingMenu(bool show)
{
	popEndingMenu->show(show);
}

void drawPopEndingMenu(float dt)
{
	for (int i = 0; i < 1; i++)
		imgEndingMenuBtn[i]->setTexAtIndex(i == popEndingMenu->selected);
	popEndingMenu->paint(dt);
}

bool keyPopEndingMenu(iKeyState stat, iPoint point)
{
	if (popEndingMenu->bShow == false)
		return false;
	if (popEndingMenu->stat != iPopupStatProc)
		return true;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popEndingMenu->selected;
		if (i == 0)
		{
			showPopNumber(true);
		}
		break;

	case iKeyStateMoved:
		for (i = 0; i < 1; i++)
		{
			if (containPoint(point, imgEndingMenuBtn[i]->touchRect(popEndingMenu->closePosition)))
			{
				j = i;
				break;
			}
		}
		popEndingMenu->selected = j;
		break;

	case iKeyStateEnded:
		break;
	}

	return true;
}

// ----------------------------------------
// popNumber
// ----------------------------------------
iPopup* popNumber;
iImage** imgNumberBtn;

int tileW, tileH;
int tileNumX, tileNumY;

iStrTex** stNumber;
Texture* methodStNumber(const char* str);
iImage** imgNumber;

iStrTex* stSelect;
Texture* methodStSelect(const char* str);

void drawPopNumber(iPopup* me, float dt);

void openPopNumber(iPopup* me)
{
	showPopEndingMenu(false);
}

void closePopNumber(iPopup* me)
{
	showPopEndingMenu(true);
}

void createPopNumber()
{
	iImage* img;
	Texture* tex;
	int i, j;

	iPopup* pop = new iPopup(iPopupStyleMove);
	iPoint offset = iPointMake(10, 10);
	pop->openPosition = iPointMake(-1000, offset.y);
	pop->closePosition = offset;
	pop->methodDrawAfter = drawPopNumber;
	pop->methodOpen = openPopNumber;
	pop->methodClose = closePopNumber;

	tileW = 100;
	tileH = 100;
	tileNumX = 3;
	tileNumY = 4;

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

		pop->addObject(img);
	}

	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(128, 64);
	imgNumberBtn = (iImage**)malloc(sizeof(iImage*) * 1);
	const char* strBtn[] = { "닫아라", };
	iPoint posBtn[] = { {500, 300}, };
	for (i = 0; i < 1; i++)
	{
		img = new iImage();
		for (j = 0; j < 2; j++)
		{
			g->init(size);

			if (j == 0) setRGBA(1, 0, 0, 1);
			else setRGBA(0, 0, 1, 1);
			g->fillRect(0, 0, size.width, size.height, 10);
			setStringSize(20);
			setStringRGBA(1, 1, 1, 1);
			setStringBorder(0);
			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);

			tex = g->getTexture();
			img->addObject(tex);
		}
		img->position = posBtn[i];
		imgNumberBtn[i] = img;
		pop->addObject(img);
	}

	popNumber = pop;

	stSelect = new iStrTex(methodStSelect);
	stSelect->setString("%d", popNumber->selected);
}

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
Texture* methodStSelect(const char* str)
{
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(128, 64);
	g->init(size);

	setRGBA(0, 0, 1, 1);
	g->fillRect(0, 0, size.width, size.height, 10);
	setStringSize(20);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, str);

	Texture* tex = g->getTexture();
	return tex;
}

void freePopNumber()
{
	int num = tileNumX * tileNumY;
	for (int i = 0; i < num; i++)
	{
		delete stNumber[i];
		//delete imgNumber[i];
	}
	free(stNumber);
	free(imgNumber);
	delete popNumber;
	free(imgNumberBtn);
	delete stSelect;
}

void showPopNumber(bool show)
{
	popNumber->show(show);
	if (show)
	{
		//
	}
	else
	{
		//
	}
}


void drawPopNumber(iPopup* me, float dt)
{
	for (int i = 0; i < 1; i++)
		imgNumberBtn[i]->setTexAtIndex(100+i == popNumber->selected);

	stSelect->paint(tileW * 3 + 20, 0, TOP | LEFT,
		"%d", me->selected);
}

void drawPopNumber(float dt)
{
	popNumber->paint(dt);
}

bool keyPopNumber(iKeyState stat, iPoint point)
{
	if (popNumber->bShow == false)
		return false;
	if (popNumber->stat != iPopupStatProc)
		return true;

	int i, j = -1, num = tileNumX * tileNumY;

	switch (stat) {

	case iKeyStateBegan:
		//setLoading(gs_menu, freeProc, loadMenu);
		if (popNumber->selected == 100)
		{
			showPopNumber(false);
		}
		break;

	case iKeyStateMoved:
		for (i = 0; i < num; i++)
		{
			if (containPoint(point, imgNumber[i]->touchRect(popNumber->closePosition)))
			{
				j = i;
				break;
			}
		}
		for (i = 0; i < 1; i++)
		{
			if (containPoint(point, imgNumberBtn[i]->touchRect(popNumber->closePosition)))
			{
				j = 100+i;
				break;
			}
		}

		if (popNumber->selected != j)
			;// audioplay
		popNumber->selected = j;
		break;

	case iKeyStateEnded:
		break;
	}

	return true;
}



