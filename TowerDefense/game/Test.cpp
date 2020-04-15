#include "Test.h"

void loadTest()
{
	createPopStart();
	createPopCalc();

	showPopStart(true);
}

void freeTest()
{
	freePopStart();
	freePopCalc();
}

void drawTest(float dt)
{
	drawPopStart(dt);
	drawPopCalc(dt);
}

void keyTest(iKeyState stat, iPoint point)
{
	if (keyPopCalc(stat, point) ||
		keyPopStart(stat, point))
		return;
}

// ------------------------------------
// popStart
// ------------------------------------
iPopup* popTest;
iImage** imgTestBtn;
iImage* imgTestChar;

void createPopStart()
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
#else// 1280 x 720(360)
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(128, 36);
	igImage* ig = g->createIgImage("assets/start.png");;
	for (int i = 0; i < 2; i++)
	{
		g->init(size);
		g->drawImage(ig, 0, -36 * i, 0.1, 0.1, TOP | LEFT);

		Texture* tex = g->getTexture();
		img->addObject(tex);
		freeImage(tex);
	}
	g->freeIgImage(ig);
#endif

	img->position = iPointMake(0, 0);
	pop->addObject(img);
	imgTestBtn[0] = img;

	// char
	img = new iImage();
	for (int j = 0; j < 8; j++)
	{
		Texture* tex = createImage("assets/char/tile0%d.png", 22 + j);
		img->addObject(tex);
		freeImage(tex);
	}
	img->_aniDt = 0.1f;
	img->_repeatNum = 2;
	pop->addObject(img);
	imgTestChar = img;

	pop->openPosition = iPointMake(devSize.width, 0);
	pop->closePosition = iPointMake(0, 0);
	popTest = pop;
}

void freePopStart()
{
	delete popTest;
	free(imgTestBtn);
}

void showPopStart(bool show)
{
	popTest->show(show);
}

void drawPopStart(float dt)
{
	for (int i = 0; i < 1; i++)
		imgTestBtn[i]->setTexAtIndex(i == popTest->selected);
	popTest->paint(dt);
}

void cbCharAnimation(iImage* me)
{
	printf("end animation\n");
}

bool keyPopStart(iKeyState stat, iPoint point)
{
	if (popTest->bShow == false)
		return false;
	if (popTest->stat != iPopupStatProc)
		return true;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popTest->selected;
		if (i == -1)
			imgTestChar->startAnimation(cbCharAnimation);
		else if (i == 0)
		{
			printf("start\n");
			showPopCalc(true);
		}
		break;

	case iKeyStateMoved:
		for (i = 0; i < 1; i++)
		{
			if (containPoint(point, imgTestBtn[i]->touchRect(popTest->closePosition)))
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

	return true;
}

// ------------------------------------
// popCalc
// ------------------------------------
iPopup* popCalc;
iStrTex** stEdit;// calc + result
Texture* methodStEdit(const char* str);
iImage** imgCalcBtn;// [0~9] + [+-*/] + [=] + clr + close

typedef void (*MethodCalc)(int n);
MethodCalc* methodCalc;// 0[0~9] + 1[+-*/] + 2[=] + clr

void inputNumber(int n);
void inputCalc(int n);
void inputResult(int n);
void inputClear(int n);
void inputClose(int n);

Texture* createBtnTexture(int index, bool selected);
void drawPopCalcAfter(iPopup* me, iPoint position, iPoint scale, float alpha);

void createPopCalc()
{
	int i, j;
	iPopup* pop = new iPopup(iPopupStyleMove);

	// 7, 8, 9  +    close
	// 4, 5, 6  -
	// 1, 2, 3  x
	// 0 / clr  = 
	iPoint posBtn[17] = {
		{64 * 0, 64 * 3},//0
		{64 * 0, 64 * 2}, {64 * 1, 64 * 2},	{64 * 2, 64 * 2},// 1, 2, 3
		{64 * 0, 64 * 1}, {64 * 1, 64 * 1},	{64 * 2, 64 * 1},// 4, 5, 6
		{64 * 0, 64 * 0}, {64 * 1, 64 * 0},	{64 * 2, 64 * 0},// 7, 8, 9
		{64 * 3, 64 * 0}, {64 * 3, 64 * 1}, {64 * 3, 64 * 2}, {64 * 3, 64 * 3},// +, -, x, /
		{64 * 4, 64 * 3}, {64 * 1, 64 * 3}, {64 * 4, 64 * 0},// =, clr, close
	};
	imgCalcBtn = (iImage**)malloc(sizeof(iImage*) * 17);
	for (i = 0; i < 17; i++)
	{
		iImage* img = new iImage();
		for (j = 0; j < 2; j++)
		{
			Texture* tex = createBtnTexture(i, j);
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = posBtn[i] + iPointMake(50, 60);
		imgCalcBtn[i] = img;
		pop->addObject(img);
	}

	pop->methodDrawAfter = drawPopCalcAfter;
	popCalc = pop;

	stEdit = (iStrTex**)malloc(sizeof(iStrTex*) * 2);
	for (i = 0; i < 2; i++)
		stEdit[i] = new iStrTex(methodStEdit);
	stEdit[0]->setString(" ");// { 0 }
	stEdit[1]->setString("0");

	methodCalc = (MethodCalc*)malloc(sizeof(MethodCalc) * 17);
	for (i = 0; i < 10; i++)
		methodCalc[i] = inputNumber;
	for (i = 0; i < 4; i++)
		methodCalc[10 + i] = inputCalc;
	methodCalc[14] = inputResult;
	methodCalc[15] = inputClear;
	methodCalc[16] = inputClose;
}

void freePopCalc()
{
	delete popCalc;
	free(imgCalcBtn);
	for (int i = 0; i < 2; i++)
		delete stEdit[i];
	free(stEdit);
}

void showPopCalc(bool show)
{
	popCalc->show(show);
	if (show)
	{
		// if it's opend, it will be initialized..
		extern char strNumber[128];
		extern int numIndex;

		strcpy(strNumber, "0");
		numIndex = 0;

		stEdit[0]->setString(" ");
		stEdit[1]->setString("0");
	}
}

void drawPopCalcAfter(iPopup* me, iPoint position, iPoint scale, float alpha)
{
	for (int i = 0; i < 17; i++)
		imgCalcBtn[i]->setTexAtIndex(i == popCalc->selected);

	for (int i = 0; i < 2; i++)
		stEdit[i]->paint(position.x, position.y + 30 * i, TOP | LEFT);
}

void drawPopCalc(float dt)
{
	popCalc->paint(dt);
}

bool keyPopCalc(iKeyState stat, iPoint point)
{
	if (popCalc->bShow == false)
		return false;
	if (popCalc->stat != iPopupStatProc)
		return true;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popCalc->selected;
		if (i != -1)
			methodCalc[i](i % 10);
		break;

	case iKeyStateMoved:
		for (i = 0; i < 17; i++)
		{
			if (containPoint(point, imgCalcBtn[i]->touchRect(popCalc->closePosition)))
			{
				j = i;
				break;
			}
		}
		popCalc->selected = j;
		break;

	case iKeyStateEnded:
		break;
	}

	return true;
}

Texture* methodStEdit(const char* str)
{
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(512, 30);
	g->init(size);

	setRGBA(0.5, 0.5, 0.5, 1);
	g->fillRect(0, 0, size.width, size.height);

	setStringSize(20);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	g->drawString(0, size.height / 2, VCENTER | LEFT, str);

	return g->getTexture();
}

Texture* createBtnTexture(int index, bool selected)
{
	iGraphics* g = iGraphics::instance();
	iSize size;
	if (index < 15) size = iSizeMake(64, 64);
	else size = iSizeMake(128, 64);
	g->init(size);

	if (selected == false)	setRGBA(1, 1, 1, 1);
	else					setRGBA(0.5, 0.5, 0.5, 1);
	g->fillRect(0, 0, size.width, size.height, 10);

	const char* s[17] = {
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
		"+", "-", "x", "/", "=",
		"clr", "close"
	};
	setStringSize(20);
	if (selected == false)	setStringRGBA(0, 0, 0, 1);
	else					setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, s[index]);

	return g->getTexture();
}

//char strNumber[128] = {'0', };
char strNumber[128] = "0";
void inputNumber(int n)
{
	// 0 ~ 9
	int m = atoi(strNumber);
	if (m == 0)
	{
		sprintf(strNumber, "%d", n);
	}
	else
	{
		char s[2];
		sprintf(s, "%d", n);
		strcat(strNumber, s);
	}

	stEdit[1]->setString(strNumber);
}

int numNumber[128];
int numIndex = 0;

int symbol[128];

void inputCalc(int n)
{
	// +, -, x, /, =
	numNumber[numIndex] = atoi(strNumber);
	symbol[numIndex] = n;
	numIndex++;

	char s[1024] = "";// nunNumber + symbol
	for (int i = 0; i < numIndex; i++)
	{
		char str[128];
		sprintf(str, "%d", numNumber[i]);
		strcat(s, str);

		const char* kiho[5] = { "+", "-", "x", "/", "=" };
		strcat(s, kiho[symbol[i]]);
	}
	stEdit[0]->setString(s);

	strcpy(strNumber, "0");
	stEdit[1]->setString("0");
}

void inputResult(int n)
{
	// =
	inputCalc(4);

	int index = numIndex;
	int num[128];
	int sym[128];
	for (int i = 0; i < index; i++)
	{
		num[i] = numNumber[i];
		sym[i] = symbol[i];
	}
	// x, /
	while (1)
	{
		bool exist = false;
		for (int i = 0; i < index - 1; i++)
		{
			if (sym[i] == 2 || sym[i] == 3)// +, -, x, /
			{
				exist = true;

				if (sym[i] == 2)
					num[i] *= num[1 + i];
				else// if (sym[i] == 3)
					num[i] /= num[1 + i];

				index--;
				for (int j = 1 + i; j < index; j++)
					num[j] = num[1 + j];
				for (int j = i; j < index; j++)
					sym[j] = sym[1 + j];
				break;
			}
		}
		if (exist == false)
			break;
	}
	// +, -
	while (1)
	{
		bool exist = false;
		for (int i = 0; i < index - 1; i++)
		{
			if (sym[i] == 0 || sym[i] == 1)// +, -, x, /
			{
				exist = true;

				if (sym[i] == 0)
					num[i] += num[1 + i];
				else// if (sym[i] == 1)
					num[i] -= num[1 + i];

				index--;
				for (int j = 1 + i; j < index; j++)
					num[j] = num[1 + j];
				for (int j = i; j < index; j++)
					sym[j] = sym[1 + j];
				break;
			}
		}
		if (exist == false)
			break;
	}

	// index = 1;
	char s[128];
	sprintf(s, "%d", num[0]);
	stEdit[1]->setString(s);
}

void inputClear(int n)
{
	int len = strlen(strNumber);
	if (len == 1)
	{
		strNumber[0] = '0';
		strNumber[1] = 0;
	}
	else
	{
		strNumber[strlen(strNumber) - 1] = 0;
	}
	stEdit[1]->setString(strNumber);
}

void inputClose(int n)
{
	showPopCalc(false);
}
