#include "Test.h"

#include "Loading.h"
#include "Menu.h"

Texture* texBg;
Texture* texTitle;

void loadTest()
{
	texBg = createImage("assets/menu/menuBg.jpg");
	texTitle = createImage("assets/menu/menuTitle.png");
	createPopPrss();
	createPopStart();

	showPopPress(true);
}

void freeTest()
{
	freeImage(texBg);
	freeImage(texTitle);
	freePopPress();
	freePopStart();
}

void drawTest(float dt)
{
	// texBg : 1400 x 933
	float r0 = devSize.width / texBg->width;
	float r1 = devSize.height / texBg->height;
	float r = max(r0, r1);
	drawImage(texBg, devSize.width / 2, devSize.height / 2,
		0, 0, texBg->width, texBg->height, VCENTER | HCENTER,
		r, r, 2, 0, REVERSE_NONE);

	drawImage(texTitle, devSize.width/2, 50,
		0, 0, texTitle->width, texTitle->height, TOP | HCENTER,
		0.6f, 0.6f, 2, 0, REVERSE_NONE);

	drawPopPress(dt);
	drawPopStart(dt);



#if 0
	static float delta = 0.0f;
	delta += dt;
	if (delta > 1.0)
	{
		delta = 0.0f;
		setLoading(gs_menu, freeTest, loadMenu);
	}
#endif
}

void keyTest(iKeyState stat, iPoint point)
{
	if (keyPopStart(stat, point))
		return;

	if (keyPopPress(stat, point))
		return;
}

// -----------------------------------
// popPress
// -----------------------------------
iPopup* popPress;

void closePopPress(iPopup* me)
{
	showPopStart(true);
}

void createPopPrss()
{
	iPopup* pop = new iPopup(iPopupStyleAlpha);

	iImage* img = new iImage();
	Texture* tex = createImage("assets/menu/pressAnyKey.png");
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	iPoint p = iPointMake((devSize.width - tex->width) / 2,
		devSize.height * 0.67f - tex->height / 2);
	pop->openPosition = p;
	pop->closePosition = p;
	pop->methodClose = closePopPress;
	popPress = pop;
}

void freePopPress()
{
	delete popPress;
}

void showPopPress(bool show)
{
	popPress->show(show);
}

void drawPopPress(float dt)
{
	popPress->paint(dt);
}

bool keyPopPress(iKeyState stat, iPoint point)
{
	if (popPress->bShow == false)
		return false;
	if (popPress->stat != iPopupStatProc)
		return true;

	if (stat == iKeyStateBegan)
	{
		showPopPress(false);
		//showPopStart(true);
	}
	return true;
}

// -----------------------------------
// popStart
// -----------------------------------
iPopup* popStart;
iImage** imgStartBtn;

void createPopStart()
{
	//const char* str[3] = { "게임시작", "옵션", "게임종료" };

	iPopup* pop = new iPopup(iPopupStyleZoom);

	imgStartBtn = (iImage**)malloc(sizeof(iImage*) * 3);
	for (int i = 0; i < 3; i++)
	{
		iImage* img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			Texture* tex = createImage("assets/menu/menuBtn%d%d.png", i, j);
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake(0, 120 * i);
		pop->addObject(img);
		imgStartBtn[i] = img;
	}

	pop->openPosition = iPointMake(devSize.width / 2, devSize.height / 2);
	pop->closePosition = iPointMake((devSize.width - 280) / 2,
									(devSize.height - 350) / 2);
	popStart = pop;
}

void freePopStart()
{
	delete popStart;
	free(imgStartBtn);
}

void showPopStart(bool show)
{
	popStart->show(show);
}

void drawPopStart(float dt)
{
	for (int i = 0; i < 3; i++)
		imgStartBtn[i]->setTexAtIndex(i == popStart->selected);
	popStart->paint(dt);
}

bool keyPopStart(iKeyState stat, iPoint point)
{
	if (popStart->bShow == false)
		return false;
	if (popStart->stat != iPopupStatProc)
		return true;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popStart->selected;
		if (i == 0)
		{
			// start(slot)
		}
		else if (i == 1)
		{
			// option
		}
		else if (i == 2)
		{
			// gameover
		}
		break;

	case iKeyStateMoved:
		for (i = 0; i < 3; i++)
		{
			if (containPoint(point, imgStartBtn[i]->touchRect(popStart->closePosition)))
			{
				j = i;
				break;
			}
		}
		popStart->selected = j;
		break;

	case iKeyStateEnded:
		break;
	}

	return true;
}

