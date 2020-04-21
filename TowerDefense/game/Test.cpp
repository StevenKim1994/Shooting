#include "Test.h"

#include "Loading.h"
#include "Menu.h"

Texture* texBg;
Texture* texTitle;

Texture* texTable;
iRect rtTable;
iPoint offTable;


void loadTest()
{
	texBg = createImage("assets/menu/menuBg.jpg");
	texTitle = createImage("assets/menu/menuTitle.png");
	createPopPrss();
	createPopStart();
	createPopSlot();
	createPopSettings();

	showPopPress(true);

	texTable = createImage("assets/menu/tableBg.jpg");
	rtTable = iRectMake(devSize.width / 3, devSize.height / 3, devSize.width / 3, devSize.height / 3);
	offTable = iPointMake(0, 0);
}

void freeTest()
{

	freeImage(texBg);
	freeImage(texTitle);
	freePopPress();
	freePopStart();
	freePopSlot();
	freePopSettings();

	freeImage(texTable);
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

	drawImage(texTitle, devSize.width / 2, 50,
		0, 0, texTitle->width, texTitle->height, TOP | HCENTER,
		0.6f, 0.6f, 2, 0, REVERSE_NONE);

	drawPopPress(dt);
	drawPopStart(dt);
	drawPopSlot(dt);
	drawPopSettings(dt);

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

void moveTable(iPoint& off, iPoint mp, const iRect& rt, const iSize& dstSize)
{
	off += mp;

	float minX = rt.origin.x + rt.size.width - dstSize.width;
	float maxX = rt.origin.x;
	float minY = rt.origin.y + rt.size.height - dstSize.height;
	float maxY = rt.origin.y;

	if (off.y < minY) off.y = minY;
	else if (off.y > maxY) off.y = maxY;
	if (off.x < minX) off.x = minX;
	else if (off.x > maxX) off.x = maxX;

}

void keyTest(iKeyState stat, iPoint point)
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
			showPopSlot(true);
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

// -----------------------------------
// popSlot
// -----------------------------------
iPopup* popSlot;
iImage** imgSlotBtn;

void drawPopSlotBefore(iPopup* me, float dt)
{
	for (int i = 0; i < 3; i++)
	{
		imgSlotBtn[i]->setTexAtIndex(i == popSlot->selected);
	}


}

void createPopSlot()
{
	//popup create
	iPopup* pop = new iPopup(iPopupStyleMove);
	popSlot = pop;


	// bg - add
	iImage* imgBg = new iImage();
	iGraphics* g = iGraphics::instance();



	//////CreateTexture;
	{

		iSize size = iSizeMake(256, 256);
		g->init(size);

		setRGBA(1, 1, 0, 0.95f); // yellow
		g->fillRect(0, 0, size.width, size.height);

		Texture* tex = g->getTexture();
		imgBg->addObject(tex);
		freeImage(tex);

	}//////CreateTexture;



	pop->addObject(imgBg);


	// btn - add ( slot x2 , xbtn )
	imgSlotBtn = (iImage**)malloc(sizeof(iImage) * 3);
	iPoint posBtn[3] = { {(256-150)/2, 70} , {(256-150)/2, 70+70} , {256-32, 0} , };
	
	for (int i = 0; i < 3; i++)
	{
		iImage* imgBtn = new iImage();
	
		if (i < 2)
		{ 
			for (int j = 0; j < 2; j++)
			{
				// create slot
				iSize size = iSizeMake(150, 64);
				g->init(size);

				if (j == 0) setRGBA(0, 0, 0, 1);
				else		setRGBA(0.5, 0.5, 0.5, 1);
				g->fillRect(0, 0, size.width, size.height, 10);

				setStringSize(30);
				setStringRGBA(1, 1, 1, 1);
				setStringBorder(0);
				g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "Slot %d", i + 1);




				Texture* tex = g->getTexture();
				imgBtn->addObject(tex);
				freeImage(tex);
			}
		}
		else if (i == 2)
		{
			for (int j = 0; j < 2; j++)
			{

				// create x
				iSize size = iSizeMake(32, 32);
				g->init(size);

				if (j == 0) setRGBA(0, 0, 0, 1);
				else		setRGBA(0.5, 0.5, 0.5, 1);
				g->fillRect(0, 0, size.width, size.height, 10);

				setStringSize(30);
				setStringRGBA(1, 1, 1, 1);
				setStringBorder(0);
				g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "X", i + 1);


				Texture* tex = g->getTexture();
				imgBtn->addObject(tex);
				freeImage(tex);
			}
		}

	

		imgBtn -> position = posBtn[i];
		pop->addObject(imgBtn);
		imgSlotBtn[i] = imgBtn;
	}



	// pop Settings( open position / close position )

	pop->openPosition = iPointMake((devSize.width - imgBg->tex->width) / 2, (devSize.height - imgBg->tex->height) / 2);
	pop->closePosition = iPointMake((devSize.width - imgBg->tex->width) / 2, (devSize.height - imgBg->tex->height) / 2);

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

void drawPopSlot(float dt)
{


	popSlot->paint(dt); 
}

bool keyPopSlot(iKeyState stat, iPoint point)
{
	if (popSlot->bShow == false)
		return false;

	if (popSlot->stat != iPopupStatProc)
		return true; // 활짝열리지 않으면 키 처리는 무시하지만 밑 계층에 잇는거 활성화 못하게함

	int i;// = popSlot->selected;

	switch (stat)
	{
	case iKeyStateBegan:
		i = popSlot->selected; // 이런식으로 구현하면 실행코드 경량화, 가독성,  코드 보안성 올라감... 
		if (i == -1)
			break;

		if (i < 2)
		{
			printf("selected %d \n ", i);
		}
		else// if(i == 2) // x 버튼일떄
			showPopSlot(false);
		break;

	case iKeyStateMoved:
		for (i = 0; i < 3; i++)
		{
			if (containPoint(point, imgSlotBtn[i]->touchRect(popSlot->closePosition))) // 인자로 closePosition을 넣는 이유는 이 버튼이 popup 안에 잇는 좌표이기 때문이다.
			{													// 이 팝업의 좌표를 고려한 위치가 나온다...
				popSlot->selected = i;
				break;
			}
		}

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

void drawPopSettingsBefore(iPopup* me, float dt)
{
	for (int i = 0; i < 4; i++)
		imgSettingsBtn[i]->setTexAtIndex(i == popSettings->selected);
}


void createPopSettings()
{
	//popup
	iPopup* pop = new iPopup(iPopupStyleMove);
	popSettings = pop;



	//bg (bgm line / sfx line)
	iImage* imgBg = new iImage();
	iGraphics* g = iGraphics::instance();
	////////////Texture
	{
		iSize size = iSizeMake(256, 256);
		g->init(size);


		setRGBA(0, 0, 1, 0.9f);
		g->fillRect(0, 0, size.width, size.height, 10);
		setStringSize(30);
		setStringRGBA(1, 1, 1, 1);
		setStringBorder(0);
		g->drawString(20, 60+70*0, VCENTER | LEFT, "BGM");
		setRGBA(1, 1, 1, 1);
		g->fillRect(100, 60+70*0 -1, 140, 2);
		g->drawString(20, 60+70*1, VCENTER | LEFT, "SFX");
		g->fillRect(100, 60 + 70 * 1 - 1, 140, 2);



		Texture* tex = g->getTexture();
		imgBg->addObject(tex);
		freeImage(tex);
	}
	////////////Texture


	pop->addObject(imgBg);




	//btn (bgm thumb / sfx thumb / credits / x )
	iImage* imgThumb = new iImage(); 
	for (int i = 0; i < 2; i++) // thumb create
	{

	}

	iImage* imgCredits = new iImage(); // credits create
	for (int i = 0; i < 2; i++)
	{

	}


	

	imgSettingsBtn = (iImage**)malloc(sizeof(iImage*) * 4);

	for (int i = 0; i < 4; i++)
	{
		iImage* imgBtn;
		if (i == 0)
			imgBtn = imgThumb; // thumb copy
		else if (i == 1)
			imgBtn = imgSettingsBtn[0]->copy();//thumb copy
		else if (i == 2)
			imgBtn = imgCredits; //credits copy
		else // if (i ==3)
			imgBtn = imgSlotBtn[2]->copy(); // X

		pop->addObject(imgBtn);
		imgSettingsBtn[i] = imgBtn;
	}




	//pop (openPosition, closePosition) Bottom - up!!! 
	pop->openPosition = iPointMake((devSize.width - imgBg->tex->width) / 2, devSize.height); 
	pop->closePosition = iPointMake((devSize.width - imgBg->tex->width) / 2, (devSize.height - imgBg->tex->height) / 2);

	pop->methodDrawBefore = drawPopSettingsBefore;
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

	int i;

	switch (stat)
	{
	case iKeyStateBegan:
		i = popSettings->selected;
		if (i == 0 || i == 1)
		{
			//thumb
		}
		else if (i == 2)
		{
			//credits
		}
		else //if( i==3)
		{
			showPopSettings(false);
		}
		break;

	case iKeyStateMoved:
		for (int i = 0; i < 4; i++)
		{
			if (containPoint(point, imgSettingsBtn[i]->touchRect(popSettings->closePosition)))
			{
				popSettings->selected = i;
				break;
			}
		}
		break;

	case iKeyStateEnded:
		break;
	}

	return false;
}



