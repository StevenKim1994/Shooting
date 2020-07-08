#include "Menu.h"

#include "Loading.h"
#include "Proc.h"

#include "Sadari.h"
#include "Jump.h"
#include "2048.h"
#include "Td.h"



Texture* texBg;
Texture* texTitle;

Texture* texTable;
iRect rtTable;
iPoint offTable;

void loadMenu()
{
	texBg = createImage("assets/menu/menuBg.jpg");
	texTitle = createImage("assets/menu/menuTitle.png");
	createPopPress();
	createPopMenu();
	createPopSlot();
	createPopSettings();

	showPopPress(true);
	texTable = createImage("assets/menu/tableBg.jpg");
	rtTable = iRectMake(devSize.width / 3, devSize.height / 3, devSize.width / 3, devSize.height / 3);
	offTable = iPointMake(0, 0);
}

void freeMenu()
{
	freeImage(texBg);
	freeImage(texTitle);
	freePopPress();
	freePopMenu();
	freePopSlot();
	freePopSettings();

	freeImage(texTable);
}

void drawMenu(float dt)
{
	// texBg : 1400 x 933
	float r0 = devSize.width / texBg->width;
	float r1 = devSize.height / texBg->height;
	float r = max(r0, r1);
	/*drawImage(texBg, devSize.width / 2, devSize.height / 2,
		0, 0, texBg->width, texBg->height, VCENTER | HCENTER,
		r, r, 2, 0, REVERSE_NONE);

	drawImage(texTitle, devSize.width / 2, 50,
		0, 0, texTitle->width, texTitle->height, TOP | HCENTER,
		0.6f, 0.6f, 2, 0, REVERSE_NONE);
		*/
	//drawPopPress(dt);
	//drawPopMenu(dt);
	//drawPopSlot(dt);
	//drawPopSettings(dt);

#if 0
	setClip(rtTable.origin.x, rtTable.origin.y,
		rtTable.size.width, rtTable.size.height);
	drawImage(texTable, offTable.x, offTable.y, TOP | LEFT);
	setClip(0, 0, 0, 0);
#endif

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

bool touching = false;
iPoint prevPoint;
void moveTable(iPoint& off, iPoint mp, const iRect& rt, const iSize& size)
{
	off += mp;
	float minX = rt.origin.x + rt.size.width - size.width;
	float minY = rt.origin.y + rt.size.height - size.height;
	float maxX = rt.origin.x;
	float maxY = rt.origin.y;
	if (off.x < minX) off.x = minX;
	else if (off.x > maxX) off.x = maxX;
	if (off.y < minY) off.y = minY;
	else if (off.y > maxY) off.y = maxY;
}

void keyMenu(iKeyState stat, iPoint point)
{
#if 0
	if (stat == iKeyStateBegan)
	{
		prevPoint = point;
		touching = true;
	}
	else if (stat == iKeyStateMoved)
	{
		if (touching)
		{
			moveTable(offTable, point - prevPoint, rtTable, iSizeMake(texTable->width, texTable->height));
			prevPoint = point;
		}
	}
	else if (stat == iKeyStateEnded)
	{
		touching = false;
	}
	return;
#endif

	if (keyPopSettings(stat, point))
		return;

	if (keyPopSlot(stat, point))
		return;

	if (keyPopMenu(stat, point))
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
	showPopMenu(true);
}

void createPopPress()
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
		//showPopMenu(true);
	}
	return true;
}

// -----------------------------------
// popMenu
// -----------------------------------
iPopup* popMenu;
iImage** imgMenuBtn;

void createPopMenu()
{
	//const char* str[3] = { "게임시작", "옵션", "게임종료" };

	iPopup* pop = new iPopup(iPopupStyleZoom);

	imgMenuBtn = (iImage**)malloc(sizeof(iImage*) * 3);
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
		imgMenuBtn[i] = img;
	}

	pop->openPosition = iPointMake(devSize.width / 2, devSize.height / 2);
	pop->closePosition = iPointMake((devSize.width - 280) / 2,
		(devSize.height - 350) / 2);
	popMenu = pop;
}

void freePopMenu()
{
	delete popMenu;
	free(imgMenuBtn);
}

void showPopMenu(bool show)
{
	popMenu->show(show);
}

void drawPopMenu(float dt)
{
	for (int i = 0; i < 3; i++)
		imgMenuBtn[i]->setTexAtIndex(i == popMenu->selected);
	popMenu->paint(dt);
}

bool keyPopMenu(iKeyState stat, iPoint point)
{
	if (popMenu->bShow == false)
		return false;
	if (popMenu->stat != iPopupStatProc)
		return true;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popMenu->selected;
		if (i == -1) break;
		audioPlay(1);
		if (i == 0)
		{
			// start(slot)
			showPopSlot(true);
		}
		else if (i == 1)
		{
			// option
			showPopSettings(true);
		}
		else if (i == 2)
		{
			// gameover
		}
		break;

	case iKeyStateMoved:
		for (i = 0; i < 3; i++)
		{
			if (containPoint(point, imgMenuBtn[i]->touchRect(popMenu->closePosition)))
			{
				j = i;// audio
				break;
			}
		}
		if (popMenu->selected != j)
			audioPlay(0);
		popMenu->selected = j;
		break;

	case iKeyStateEnded:
		break;
	}

	return true;
}

// -----------------------------------
// popSlot
// -----------------------------------
iPopup* popSlot;
iImage** imgSlotBtn;

void drawPopSlotBefore(iPopup* me, float dt);

void createPopSlot()
{
	// pop
	iPopup* pop = new iPopup(iPopupStyleMove);
	popSlot = pop;

	// bg - add
	iImage* imgBg = new iImage();
	iGraphics* g = iGraphics::instance();
	{
		iSize size = iSizeMake(256, 256);
		g->init(size);

		setRGBA(1, 1, 0, 0.9f);
		g->fillRect(0, 0, size.width, size.height, 10);
		setRGBA(1, 1, 1, 1);

		Texture* tex = g->getTexture();
		imgBg->addObject(tex);
		freeImage(tex);
	}
	pop->addObject(imgBg);

	// btn - add(slot x3 & x)
	iPoint posBtn[5] = {
		{(256 - 201) / 2, 70 + 80 * 0},
		{(256 - 201) / 2, 70 + 80 * 1},
		{(256 - 201) / 2, 70 + 80 * 2},
		{(256 - 201) / 2, 70 + 80 * 3},
		{256 - 32, 0}
	};
	const char* strSlot[4] = { "사다리","점프", "2048", "타워디펜스" };
	imgSlotBtn = (iImage**)malloc(sizeof(iImage*) * 5);
	for (int i = 0; i < 5; i++)
	{
		iImage* imgBtn = new iImage();
		if (i < 4)
		{
			// slot
			for (int j = 0; j < 2; j++)
			{
				iSize size = iSizeMake(201, 77);
				g->init(size);

				igImage* ig = g->createIgImage("assets/menu/slotBtn%d.png", j);
				g->drawImage(ig, 0, 0, TOP | LEFT);
				g->freeIgImage(ig);

				setStringSize(30);
				setStringRGBA(1, 1, 1, 1);
				setStringBorder(0);
				g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER,
					strSlot[i]);

				Texture* tex = g->getTexture();
				imgBtn->addObject(tex);
				freeImage(tex);
			}
		}
		else// if (i == 4)
		{
			// x
			for (int j = 0; j < 2; j++)
			{
				Texture* tex = createImage("assets/menu/btnX%d.png", j);
				imgBtn->addObject(tex);
				freeImage(tex);
			}
		}
		imgBtn->position = posBtn[i];
		pop->addObject(imgBtn);
		imgSlotBtn[i] = imgBtn;
	}

	// pop setting(open/close)
	pop->openPosition = iPointMake((devSize.width - imgBg->tex->width) / 2,
		-imgBg->tex->height);
	pop->closePosition = iPointMake((devSize.width - imgBg->tex->width) / 2,
		(devSize.height - imgBg->tex->height) / 2);
	pop->methodDrawBefore = drawPopSlotBefore;
}

void freePopSlot()
{
	delete popSlot;
	free(imgSlotBtn);
}

void showPopSlot(bool show)
{
	popSlot->show(show);
}

void drawPopSlotBefore(iPopup* me, float dt)
{
	for (int i = 0; i < 4; i++)
	{
		imgSlotBtn[i]->setTexAtIndex(i == popSlot->selected);
	}
}

void drawPopSlot(float dt)
{
	popSlot->paint(dt);
}

bool keyPopSlot(iKeyState stat, iPoint point)
{
	if (popSlot->bShow == false)
		return false;
	if (popSlot->stat != iPopupStatProc)
		return true;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popSlot->selected;
		if (i == -1) break;
		if (i < 4)
		{
			printf("selected = %d\n", i);
			METHOD_LOADING m[4] = { loadSadari, loadJump, load2048 , loadTd};
			setLoading(gs_sadari + i, freeMenu, m[i]);
		}
		else// if (i == 3)
		{
			showPopSlot(false);
		}
		break;

	case iKeyStateMoved:
		for (i = 0; i < 5; i++)
		{
			if (containPoint(point, imgSlotBtn[i]->touchRect(popSlot->closePosition)))
			{
				j = i;
				break;
			}
		}
		popSlot->selected = j;
		break;

	case iKeyStateEnded:
		break;
	}

	return true;
}

// -----------------------------------
// popSettings
// -----------------------------------
iPopup* popSettings;
iImage** imgSettingsBtn;
float bgmPop, sfxPop;

void drawPopSettingsBefore(iPopup* me, float dt);
void createPopSettings()
{
	// pop
	iPopup* pop = new iPopup(iPopupStyleMove);
	popSettings = pop;

	// bg (bgm line / sfx line)
	iImage* imgBg = new iImage();
	iGraphics* g = iGraphics::instance();
	{
		iSize size = iSizeMake(256, 256);
		g->init(size);

		setRGBA(0, 0, 1, 0.9f);
		g->fillRect(0, 0, size.width, size.height, 10);

		setStringSize(30);
		setStringRGBA(1, 1, 1, 1);
		setStringBorder(0);
		setRGBA(1, 1, 1, 1);
		g->drawString(20, 60 + 70 * 0, VCENTER | LEFT, "BGM");
		g->fillRect(90, 60 + 70 * 0 - 1, 140, 2);
		g->drawString(20, 60 + 70 * 1, VCENTER | LEFT, "SFX");
		g->fillRect(90, 60 + 70 * 1 - 1, 140, 2);

		Texture* tex = g->getTexture();
		imgBg->addObject(tex);
		freeImage(tex);
	}
	pop->addObject(imgBg);

	// btn (bgm thumb / sfx thumb / credtis / x )
	iImage* imgThumb = new iImage();
	for (int i = 0; i < 2; i++)
	{
		Texture* tex = createImage("assets/menu/optionBtn0%d.png", i);
		imgThumb->addObject(tex);
		freeImage(tex);
	}

	iImage* imgCredits = new iImage();
	for (int i = 0; i < 2; i++)
	{
		Texture* tex = createImage("assets/menu/optionBtn1%d.png", i);
		imgCredits->addObject(tex);
		freeImage(tex);
	}

	iPoint posBtn[4] = { {60, 30}, {60, 30 + 70}, // thumb
						{(256 - 201) / 2, 170},// credits
						imgSlotBtn[4]->position + iPointMake(20, 0) };
	imgSettingsBtn = (iImage**)malloc(sizeof(iImage*) * 4);
	for (int i = 0; i < 4; i++)
	{
		iImage* imgBtn;
		if (i == 0)		imgBtn = imgThumb;					// thumb
		else if (i == 1)imgBtn = imgSettingsBtn[0]->copy();	// thumb
		else if (i == 2)imgBtn = imgCredits;				// credits
		else			imgBtn = imgSlotBtn[4]->copy();		// x
		imgBtn->position = posBtn[i];
		pop->addObject(imgBtn);
		imgSettingsBtn[i] = imgBtn;
	}

	// pop (open / close)
	pop->openPosition = iPointMake((devSize.width - imgBg->tex->width) / 2,
		devSize.height);
	pop->closePosition = iPointMake((devSize.width - imgBg->tex->width) / 2,
		(devSize.height - imgBg->tex->height) / 2);
	pop->methodDrawBefore = drawPopSettingsBefore;

	bgmPop = 1.0;
	sfxPop = 1.0f;
	imgSettingsBtn[0]->position.x = 200;
	imgSettingsBtn[1]->position.x = 200;
}

void freePopSettings()
{
	delete popSettings;
	free(imgSettingsBtn);
}

void showPopSettings(bool show)
{
	popSettings->show(show);
}

void drawPopSettingsBefore(iPopup* me, float dt)
{
	for (int i = 0; i < 4; i++)
		imgSettingsBtn[i]->setTexAtIndex(i == popSettings->selected);
}

void drawPopSettings(float dt)
{
	popSettings->paint(dt);
}

bool keyPopSettings(iKeyState stat, iPoint point)
{
	if (popSettings->bShow == false)
		return false;
	if (popSettings->stat != iPopupStatProc)
		return true;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popSettings->selected;
		if (i == -1) break;
		audioPlay(1);
		if (i == 0 || i == 1)
		{
			// thumb
			touching = true;
			prevPoint = point;
		}
		else if (i == 2)
		{
			// credits
		}
		else// if (i==3)
		{
			showPopSettings(false);
		}
		break;

	case iKeyStateMoved:
		if (touching)
		{
			i = popSettings->selected;
			if (i < 2)
			{
				// move 60 ~ 200
				iImage* img = imgSettingsBtn[i];
				img->position.x += (point - prevPoint).x;
				if (img->position.x < 60)
					img->position.x = 60;
				else if (img->position.x > 200)
					img->position.x = 200;
				prevPoint = point;
			}
		}
		else
		{
			for (i = 0; i < 4; i++)
			{
				if (containPoint(point, imgSettingsBtn[i]->touchRect(popSettings->closePosition)))
				{
					j = i;
					break;
				}
			}
			if (popSettings->selected != j)
				audioPlay(0);
			popSettings->selected = j;
		}
		break;

	case iKeyStateEnded:
		touching = false;
		i = popSettings->selected;
		if (i == 0)
		{
			float r = (imgSettingsBtn[i]->position.x - 60) / 140;
			printf("bgm %f\n", r);
			bgmPop = r;
			audioVolume(bgmPop, sfxPop, 2);
		}
		else if (i == 1)
		{
			float r = (imgSettingsBtn[i]->position.x - 60) / 140;
			printf("sfx %f\n", r);
			sfxPop = r;
			audioVolume(bgmPop, sfxPop, 2);
		}
		break;
	}

	return true;
}


