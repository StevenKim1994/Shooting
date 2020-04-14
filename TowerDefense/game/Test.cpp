#include "Test.h"


float dtTest;
iPopup* popTest;
iImage** imgTestBtn;
iImage* imgTestChar;

void cbCharAnimation(iImage* me);

void loadTest()
{
	iPopup* pop = new iPopup(iPopupStyleMove);


	
	// btn
	imgTestBtn = (iImage**)malloc(sizeof(iImage*) * 1);

	iImage* img = new iImage();

#if 0 
	Texture** texs = createDivideImage(1, 2, "assets/start.png");
	

	for (int i = 0; i < 2; i++)
	{
		img->addObject(texs[i]);
		freeImage(texs[i]);
	}

	free(texs);
#else // 1280 x 720(360) // 사이즈 작게하는법

	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(128, 36);

	igImage* ig = g->createIgImage("assets/start.png");
	
	for(int i = 0; i < 2 ; i++)
	{
		g->init(size);
		g->drawImage(ig, 0, -36 * i, 0.1, 0.1, TOP | LEFT);

		Texture* tex = g->getTexture();
		img->addObject(tex);
		freeImage(tex);
	}
	g->freeIgImage(ig);
	
#endif


	img->position = iPointMake(devSize.width/2, devSize.height/2);
	pop->addObject(img);
	imgTestBtn[0] = img;

	// character
	img = new iImage();
	for (int j = 0; j < 8; j++)
	{
		Texture* tex = createImage("assets/char/tile0%d.png", j + 22);
		img->addObject(tex);
		freeImage(tex);
	}
	imgTestChar = img;
	
	img->_aniDt = 0.1f;
	img->repeatNum = 2;
	imgTestChar = img;
	pop->addObject(img);


	pop->openPosition = iPointMake(devSize.width, 0);
	pop->closePosition = iPointMake(0, 0);
	popTest = pop;


	popTest->show(true);
}

void cbCharAnimation(iImage* me)
{
	printf("end animation\n");
}

void freeTest()
{
	delete popTest;
	free(imgTestBtn);
}

void drawTest(float dt)
{
	for (int i = 0; i < 1; i++)
		imgTestBtn[i]->setTexAtIndex(i == popTest->selected);

	popTest->paint(dt);

}

void keyTest(iKeyState stat, iPoint point)
{
	int i , j = -1;

	switch(stat)
	{
	case iKeyStateBegan:
		i = popTest->selected;
		if( i == -1)
		{
			imgTestChar->startAnimation(cbCharAnimation);
		}
		if(i == 0)
		{
			printf("start\n");
		}
		break;
	case iKeyStateMoved:
		for(int i =0; i<1; i++)
		{
			if(containPoint(point, imgTestBtn[i]->touchRect(popTest->closePosition)))
			{
				j = i;
				break;
			}
		}
		popTest->selected = j;
		break;
		
	case iKeyStateEnded:
		break;
	}
}

