#pragma once

#include "MapEditor.h"

WndCtrlSystem* wcsMapEditor;

// Left - state
HWND hCbState;

// Left - offset
HWND* hEbOff;

// Left - image
HWND hBtnOpenImage;
void btnOpenImageUpdate(HWND hwnd);
HWND* hEbOpenImage;

void methodTileUpdate(float dt);

// List - collision
HWND hCbCollsition;

//Right world Map
HWND* hEbMapNum;
HWND* hBtnMapOpen;

HWND hChkTileOnOff;

HWND* hEbMapOffset;

void btnMapOpenUpdate(HWND hwmd);
void methodMapUpdate(float dt);

// tileset Info
HWND* hEbTileSize;
HWND hEbTileRotate;
HWND* hEbTileRGBA;
HWND* hEbTileImgSet;



// OpenGL View
WndGL* wgTile;
WndGL* wgMap;


bool chkTileOnOff = false;

void TileOnOff(HWND hwnd);

void loadMapEditor(HWND hwnd)
{
	int i;
	initWndCtrlSystem();
	wcsMapEditor = new WndCtrlSystem(hwnd);
	setWndCtrlSystem(wcsMapEditor);

	createWndStatic(130, 5, 125, 30, "타일뷰", NULL, NULL);
	createWndStatic(15, 500, 125, 80, "타일상태 지정:", NULL, NULL);
	const char* strState[3] = { "이동","이미지", "충돌" };
	hCbState = createWndComboBox(25, 520, 80, 160, strState, 3, NULL, NULL);
	createWndStatic(160, 500, 125, 80, "타일크기 지정:", NULL, NULL);
	hEbTileSize = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
	{
		createWndStatic(160, 530 + 30 * i, 20, 20, i == 0 ? "X:" : "Y:", NULL, NULL);
		hEbTileSize[i] = createWndEditBox(180, 525 + 30 * i, 75, 20, "15", WndEditBoxStyle_int, NULL, NULL);
	}

	createWndStatic(15, 420, 270, 70, "타일뷰 오프셋", NULL, NULL);
	hEbOff = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
	{
		createWndStatic(15 + 140 * i, 450, 20, 20, i == 0 ? "X:" : "Y:", NULL, NULL);
		hEbOff[i] = createWndEditBox(45 + 130 * i, 450, 60, 25, "0", WndEditBoxStyle_int, NULL, NULL);
	}

	createWndStatic(15, 600, 125, 80, "타일 회전:", NULL, NULL);
	createWndStatic(15, 630, 50, 30, "각도:", NULL, NULL);
	hEbTileRotate = createWndEditBox(70, 630, 50, 20, "0", WndEditBoxStyle_int, NULL, NULL);

	const char* strRGBA[4] = { "R:", "G:" , "B:" , "A:" };
	createWndStatic(150, 600, 125, 80, "타일RGBA", NULL, NULL);
	hEbTileRGBA = (HWND*)malloc(sizeof(HWND) * 4);
	for (i = 0; i < 4; i++)
	{
		createWndStatic(150 + 30 * i, 630, 30, 30, strRGBA[i], NULL, NULL);
		hEbTileRGBA[i] = createWndEditBox(150 + 30 * i, 660, 30, 30, "255", WndEditBoxStyle_int, NULL, NULL);
	}


	hBtnOpenImage = createWndButton(15, 5, 100, 25, "Open Image", NULL, btnOpenImageUpdate);
	createWndStatic(15, 700, 125, 150, "타일자르기", NULL, NULL);
	hEbOpenImage = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
		hEbOpenImage[i] = createWndEditBox(15, 720 + 30 * i, 35, 25, "1", WndEditBoxStyle_int, NULL, NULL);


	createWndStatic(150, 700, 150, 150, "선택할 타일이미지 크기", NULL, NULL);
	hEbTileImgSet = (HWND*)malloc(sizeof(HWND) * 2);

	const char* strImgSet[2] = { "가로:", "세로:" };
	for (int i = 0; i < 2; i++)
	{
		createWndStatic(160, 745 + 35 * i, 45, 45, strImgSet[i], NULL, NULL);
		hEbTileImgSet[i] = createWndEditBox(200, 745 + 35 * i, 35, 25, "15", WndEditBoxStyle_int, NULL, NULL);
	}


	const char* strCollition[3] = { "모두이동불가", "공중이동불가", "모두이동가능" };


	createWndStatic(800, 5, 125, 25, "맵 크기:", NULL, NULL);
	hEbMapNum = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
		hEbMapNum[i] = createWndEditBox(1000 + 100 * i, 5, 70, 25, "150", WndEditBoxStyle_int, NULL, NULL);
	createWndStatic(1080, 5, 10, 25, "x", NULL, NULL);
	createWndStatic(650, 5, 50, 25, "맵 뷰", NULL, NULL);

	hChkTileOnOff = createWndCheckBox(1200, 5, 135, 25, "맵뷰 타일On/Off", NULL, TileOnOff);

	const char* strMap[2] = { "Open Map", "Save Map" };
	hBtnMapOpen = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
		hBtnMapOpen[i] = createWndButton(400 + 105 * i, 5, 100, 25, strMap[i], NULL, btnMapOpenUpdate);

	const char* strOff[2] = { "가로", "세로" };
	hEbMapOffset = (HWND*)malloc(sizeof(HWND) * 2);
	createWndStatic(300, 700, 125, 300, "맵뷰 오프셋", NULL, NULL);
	for(i = 0; i<2; i++)
	{
		createWndStatic(300, 720 + 30 * i, 50, 30, strOff[i], NULL, NULL);
		hEbMapOffset[i] = createWndEditBox(350, 720 + 30 * i, 50, 30, "0", WndEditBoxStyle_int, NULL, NULL);
	}

	wgTile = createOpenGL(15, 40, 270, 350, methodTileUpdate, 270, 350); //TILE OPENGL
	wgMap = createOpenGL(300, 40, 1100, 650, methodMapUpdate, 1100, 650); //MAP OPENGL
}

void freeMapEditor()
{
	delete wcsMapEditor;
}



LRESULT colorMapEditor(WPARAM wParam, LPARAM lParam)
{
	return wcsMapEditor->color(wParam, lParam);
}

void updateMapEditor(WPARAM wParam, LPARAM lParam)
{
	wcsMapEditor->update(wParam, lParam);
}

void drawMapEditor(float dt)
{
	//opengl
}

iRect TileRect = iRectMake(15, 40, 270, 350);
iRect MapRect = iRectMake(300, 40, 1100, 650);

iPoint SeletedPoint;
iRect Seleted;

iPoint DrawPoint;

Texture* seletedTex;

/*
RECT rt;
GetWindowRect(wcsMapEditor0>hwndParent, &rt);
RECT rtTile;
GetWindowRect(wgTile->gl->hWnd, &rtTile);
RECT rtMap;
GetWindowRect(wgMap->gl->hWnd, &rtMap);
int caption = GetSystemMetrics(SM_CYCAPTION);
int menu = GetSystemMetrics(SM_CYMENU);
int xframe = GetSystemMetrics(SM_CXFRAME);
int yfream = getSystemMetrics(SM_CYFRAME);

iRect tileRt = iRectMake(rtTile.left, rtTile.top, rtTile.right - rtTile.left, rtTile.bottom - rtTile.top);
tileRt.origin.x = titleRt.origin.x - rt.left - xfream * 2;
tileRt.origin.y = titleRt.origin.y - rt.top - caption - menu - yframe * 2;
tileRt.origin.x += xBorder; tileRt.size.width += xBorder * 2;
tileRt.origin.y += yBorder; tileRt.size.height += yBorder * 2;

iRect mapRt = iRectMake(rtMap.left, rtMap.top, rtMap.right - rtMap.left, rtMap.bottom - rtMap.top);
mapRt.origin.x = mapRt.origin.x - rt.left, xframe * 2;
mapRt.origin.y = mapRt.origin.y - rt.top - caption - menu - yframe * 2;
mapRt.origin.x += xBorder; mapRt.size.width += xBorder * 2;
mapRt.origin.y += yBorder; mapRt.size.height += yBorder *2;

///////////
   RECT rt;
   GetWindowRect(wcsMapEditor->hwndParent, &rt);
   RECT rtTile;// (5, 105, 200, 200) (13(+8), 156(+51), 200, 200)
   GetWindowRect(wgTile->gl->hWnd, &rtTile);
   RECT rtMap;// (300, 40, 300, 300) (308(+8), 91(+51), 300, 300)
   GetWindowRect(wgMap->gl->hWnd, &rtMap);
   int caption = GetSystemMetrics(SM_CYCAPTION);// 23
   int menu = GetSystemMetrics(SM_CYMENU);// 20
   int xframe = GetSystemMetrics(SM_CXFRAME);// 4
   int yframe = GetSystemMetrics(SM_CYFRAME);// 4
   int xBorder = GetSystemMetrics(SM_CXBORDER);// 1
   int yBorder = GetSystemMetrics(SM_CYBORDER);// 1

   iRect tileRt = iRectMake(rtTile.left, rtTile.top, rtTile.right - rtTile.left, rtTile.bottom - rtTile.top);
   tileRt.origin.x = tileRt.origin.x - rt.left - xframe * 2;
   tileRt.origin.y = tileRt.origin.y - rt.top - caption - menu - yframe * 2;
   tileRt.origin.x += xBorder; tileRt.size.width += xBorder * 2;// GetClientRect
   tileRt.origin.y += yBorder;   tileRt.size.height += yBorder * 2;
   iRect mapRt = iRectMake(rtMap.left, rtMap.top, rtMap.right - rtMap.left, rtMap.bottom - rtMap.top);
   mapRt.origin.x = mapRt.origin.x - rt.left - xframe * 2;
   mapRt.origin.y = mapRt.origin.y - rt.top - caption - menu - yframe * 2;
   mapRt.origin.x += xBorder; mapRt.size.width += xBorder * 2;// GetClientRect
   mapRt.origin.y += yBorder; mapRt.size.height += yBorder * 2;

   //printf("rt(%d, %d, %d, %d)tile(%d, %d, %d, %d)map(%d, %d, %d, %d) (%d, %d, %d, %d) point(%f, %f)\n",
   //   rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top,
   //   rtTile.left, rtTile.top, rtTile.right - rtTile.left, rtTile.bottom - rtTile.top,
   //   rtMap.left, rtMap.top, rtMap.right - rtMap.left, rtMap.bottom - rtMap.top,
   //   caption, menu, xframe, yframe,
   //   point.x, point.y);

   if (containPoint(point, tileRt))
   {
	  point -= tileRt.origin;
	  printf("tile(%f, %f)\n", point.x, point.y);
   }
   else if (containPoint(point, mapRt))
   {
	  point -= mapRt.origin;
	  printf("map(%f, %f)\n", point.x, point.y);
   }
///////////
*/

void keyMapEditor(iKeyState stat, iPoint point)
{
	//opengl

	if (stat == iKeyStateBegan)
	{
		if (containPoint(point, TileRect)) // Tile창 안에 마우스 포인터가 있을때
		{
			point.x -= TileRect.origin.x;
			point.y -= TileRect.origin.y;

			printf("point (%f , %f)\n", point.x, point.y);
			printf("TileSelected\n");
			setRGBA(1, 1, 1, 1);
			
			SeletedPoint = iPointMake(point.x, point.y);
			Seleted = iRectMake(point.x, point.y, getWndInt(hEbTileImgSet[0]), getWndInt(hEbTileImgSet[1]));

			int potW = nextPOT(Seleted.size.width);
			int potH = nextPOT(Seleted.size.height);
			uint8* rgba = (uint8*)calloc(sizeof(uint8), potW* potH *4);
			seletedTex = createImageWithRGBA(rgba, potW, potH);
			
		}
		else if (containPoint(point, MapRect)) // Map 창 안에 마우스 포인터가 있을떄
		{
			point.x -= MapRect.origin.x;
			point.y -= MapRect.origin.y;

			DrawPoint = iPointMake(point.x, point.y);

			printf("point (%f , %f)\n", point.x, point.y);
			printf("MapSelected\n");
			setRGBA(1, 1, 1, 1);
		}
		
	}

}

void dragMapEditor(WPARAM wParam, LPARAM lParam)
{
	wcsMapEditor->dropFiles(wParam, lParam);
}

Texture** texs = NULL;

char* strImagePath = NULL;



void btnOpenImageUpdate(HWND hwnd)
{
	const char* path = openFileDlg(true, TEXT("Image Files(*png, *jpg)\0*.png;*.jpg\0"));

	if (path)
	{
		//to do
		int len = strlen(path);

		strImagePath = (char*)calloc(sizeof(char), 1 + len);
		strcpy(strImagePath, path);

	}
}


void methodTileUpdate(float dt)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setRGBA(0, 1, 0, 1);
	drawRect(SeletedPoint.x, SeletedPoint.y, Seleted.size.width, Seleted.size.height);

	if (strImagePath)
	{

		int numX = getWndInt(hEbOpenImage[0]);
		int numY = getWndInt(hEbOpenImage[1]);

		if (texs)
		{
			int i, j = numX * numY;

			for (int i = 0; i < j; i++)
				freeImage(texs[i]);
			free(texs);
		}

		texs = createDivideImage(numX, numY, strImagePath);

		free(strImagePath);
		strImagePath = NULL;
	}

	if (texs == NULL)
		return;

	setRGBA(1, 1, 1, 1);
	int numX = getWndInt(hEbOpenImage[0]);
	int numY = getWndInt(hEbOpenImage[1]);


	int w = texs[0]->width;
	int h = texs[0]->height;

	for (int j = 0; j < numY; j++)
	{
		for (int i = 0; i < numX; i++)
		{
			drawImage(texs[numX * j + i], w * i, h * j, TOP | LEFT);
		}
	}

	
}

void btnMapOpenUpdate(HWND hwmd)
{
	bool open = (hBtnMapOpen[0] == hwmd);
	const char* path = openFileDlg(open, TEXT("Mao Files(*.map)\0*.map\0"));

		if (path)
		{
			// to do
		}
}

void methodMapUpdate(float dt)
{
	//world map

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setRGBA(1, 1, 1, 1);

	if(seletedTex)
		drawImage(seletedTex, DrawPoint.x, DrawPoint.y, TOP|LEFT);

	int TileSizeX = getWndInt(hEbTileSize[0]);
	int TileSizeY = getWndInt(hEbTileSize[1]);

	int MapSizeX = getWndInt(hEbMapNum[0]);
	int MapSizeY = getWndInt(hEbMapNum[1]);


	if (chkTileOnOff == true)
	{

		setRGBA(1, 0, 0, 1);
		drawRect(0, 0, MapSizeX, MapSizeY);



		setRGBA(1, 1, 1, 1);
		if (TileSizeX != 0 && TileSizeY != 0)
		{
			int row = MapSizeY / TileSizeY;
			int col = MapSizeX / TileSizeX;

			for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < col; j++)
				{
					drawRect(iRectMake(TileSizeX * j, TileSizeY * i, TileSizeX, TileSizeY));

				}
			}

		}
	}
	
	

}

void TileOnOff(HWND hwnd)
{
	chkTileOnOff = getCheckBox(hwnd);
}
