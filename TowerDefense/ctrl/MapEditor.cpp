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

iPoint tileOpenGLOff; // 타일 OpenGL 오프셋값
iPoint MapOpenGLOff; // 맵 OpenGL 오프셋값

HWND* tileOpenGLScroll;
HWND* MapOpenGLScroll;

void scrollTileOpenGL(HWND hwnd);
void scrollMapOpenGL(HWND hwnd);

bool checkSelect = false;
Texture* SelectedTexture = NULL;



struct TOTAL
{
	Texture* tex; // 전체이미지
	uint8* rgba; // 전체 RGBA값
	bool* gone; // 비교햇는지...
	iRect rt; // 선택된 영역
	iImage* img; // 선택된 영역의 이미지의 리스트들... 일종의 sprite의 모음...
};

TOTAL* total;



bool chkTileOnOff = false;

void TileOnOff(HWND hwnd);

void loadMapEditor(HWND hwnd)
{
	tileOpenGLOff = iPointZero;
	MapOpenGLOff = iPointZero;

	total = (TOTAL*)malloc(sizeof(TOTAL));
	const char* path = "assets/atlas0.png";
	total->tex = NULL;
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


	const char* strMapSize[2] = { "1920" ,"1080" };
	createWndStatic(800, 5, 125, 25, "맵 크기:", NULL, NULL);
	hEbMapNum = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
		hEbMapNum[i] = createWndEditBox(1000 + 100 * i, 5, 70, 25, strMapSize[i], WndEditBoxStyle_int, NULL, NULL);
	createWndStatic(1080, 5, 10, 25, "x", NULL, NULL);
	createWndStatic(650, 5, 50, 25, "맵 뷰", NULL, NULL);

	hChkTileOnOff = createWndCheckBox(1200, 5, 135, 25, "맵뷰 타일On/Off", NULL, TileOnOff);

	const char* strMap[2] = { "Open Map", "Save Map" };
	hBtnMapOpen = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
		hBtnMapOpen[i] = createWndButton(400 + 105 * i, 5, 100, 25, strMap[i], NULL, btnMapOpenUpdate);

	const char* strOff[2] = { "가로", "세로" };
	hEbMapOffset = (HWND*)malloc(sizeof(HWND) * 2);
	createWndStatic(300, 730, 125, 300, "맵뷰 오프셋", NULL, NULL);
	for(i = 0; i<2; i++)
	{
		createWndStatic(300, 750 + 30 * i, 50, 30, strOff[i], NULL, NULL);
		hEbMapOffset[i] = createWndEditBox(350, 750 + 30 * i, 50, 30, "0", WndEditBoxStyle_int, NULL, NULL);
	}

	

	tileOpenGLScroll = (HWND*)malloc(sizeof(HWND) * 2);

	wgTile = createOpenGL(15, 40, 270, 350, methodTileUpdate, 270, 350); //TILE OPENGL
	tileOpenGLScroll[0] = createWndScrollBar(15, 390, 270, 20, SBS_HORZ, NULL, scrollTileOpenGL); // wgTile 가로 스크롤바
	tileOpenGLScroll[1] = createWndScrollBar(285, 40, 20, 350, SBS_VERT, NULL, scrollTileOpenGL); // wgTile 세로 스크롤바

	MapOpenGLScroll = (HWND*)malloc(sizeof(HWND) * 2);

	wgMap = createOpenGL(330, 40, 1100, 650, methodMapUpdate, 1100, 650); //MAP OPENGL
	MapOpenGLScroll[0] = createWndScrollBar(330, 690, 1100, 20, SBS_HORZ, NULL, scrollMapOpenGL); // wgMap 가로스크롤바
	MapOpenGLScroll[1] = createWndScrollBar(1430, 40, 20, 650, SBS_VERT, NULL, scrollMapOpenGL); // wgMap 세로스크롤바

}

void freeMapEditor()
{
	delete wcsMapEditor;
	free(tileOpenGLScroll);
	free(MapOpenGLScroll);
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

iPoint SeletedPoint = iPointZero;
iRect Seleted;

iPoint DrawPoint;

Texture* seletedTex;


int _left, _right, _up, _down; // 각 최대 마지막 값

int goneSIZE = 0;



void keyMapEditor(iKeyState stat, iPoint point)
{
	
	
	if (stat == iKeyStateBegan)
	{
		

		
		if (containPoint(point, TileRect)) // Tile창 안에 마우스 포인터가 있을때
		{
			if (GetKeyState(VK_CONTROL)<0) // control키를 누른상태이면서 화면을 움직인다면 이때 Tile 출력하는 OpenGL offset 변경함
			{
				printf("Move TileOpenGL \n");
			}
			
			for (int i = 0; i < goneSIZE; i++)
				total->gone[i] = false;

			point.x -= TileRect.origin.x;
			point.y -= TileRect.origin.y;
		

			SeletedPoint = iPointMake(point.x , point.y );
			Seleted = iRectMake(point.x , point.y , getWndInt(hEbTileImgSet[0]), getWndInt(hEbTileImgSet[1]));
			_left = point.x - tileOpenGLOff.x;
			_up = point.y -tileOpenGLOff.y ;
			_right = point.x;
			_down = point.y;
			
		}
		else if (containPoint(point, MapRect)) // Map 창 안에 마우스 포인터가 있을떄
		{
			if (GetKeyState(VK_CONTROL)<0 ) // control키를 누른상태이면서 화면을 움직인다면 이때 Map 출력하는 OpenGL offset 변경함
			{
				printf("Map TileOpenGL \n");
			}
			point.x -= MapRect.origin.x;
			point.y -= MapRect.origin.y;

			DrawPoint = iPointMake(point.x - MapOpenGLOff.x, point.y - MapOpenGLOff.y);
			DrawPoint = iPointMake(point.x - MapOpenGLOff.x, point.y - MapOpenGLOff.y);
			
			setRGBA(1, 1, 1, 1);
			

		}
		
	}

}

void dragMapEditor(WPARAM wParam, LPARAM lParam)
{
	wcsMapEditor->dropFiles(wParam, lParam);
}

void scrollMapEditor(WPARAM wParam, LPARAM lParam)
{
	wcsMapEditor->scroll(wParam, lParam);
}

Texture** texs = NULL;
Texture* Imgtex = NULL;

char* strImagePath = NULL;
const char* ws;
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


void findRect(int x, int y)
{


	int i, j, k;
	int num;
	int potW = total->tex->potWidth;
	int potH = total->tex->potHeight;
	int* mustGoIndex = (int*)malloc(sizeof(int) *  potH * potW);

	mustGoIndex[0] = potW * y + x;

	int mustGoNum = 1;

	while (mustGoNum)
	{
		num = mustGoNum;

		for (k = 0; k < num; k++)
		{
			i = mustGoIndex[k] % potW;
			j = mustGoIndex[k] / potW;
			if (total->gone[potW * j + i] || total->rgba[potW * 4 * j + 4 * i + 3] == 0)
				continue;

			total->gone[potW * j + i] = true;
			if (i < _left)
				_left = i;
			if (i > _right)
				_right = i;
			if (j < _up)
				_up = j;
			if (j > _down)
				_down = j;


			if (i > 0) //왼쪾으로 갈수 있음
			{
				mustGoIndex[mustGoNum] = mustGoIndex[k] - 1;
				mustGoNum++;
			}
			if (i < potW - 1) //오른쪾으로 갈수 있음
			{
				mustGoIndex[mustGoNum] = mustGoIndex[k] + 1;
				mustGoNum++;
			}

			if (j > 0) //왼쪾으로 갈수 있음
			{
				mustGoIndex[mustGoNum] = mustGoIndex[k] - potW;
				mustGoNum++;
			}
			if (j < potH - 1) //오른쪾으로 갈수 있음
			{
				mustGoIndex[mustGoNum] = mustGoIndex[k] + potW;
				mustGoNum++;
			}
		}
		mustGoNum -= num;
		for (i = 0; i < mustGoNum; i++)
			mustGoIndex[i] = mustGoIndex[num + i];
	}
	//가야될 목록
	free(mustGoIndex);

}


void dragTotal(iRect drag)
{
	total->rt;
}



void methodTileUpdate(float dt)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setRGBA(0, 1, 0, 1);


	if (strImagePath)
	{
		setRGBA(1, 1, 1, 1);
		total->tex = createImage(strImagePath);
		wchar_t* ws = utf8_to_utf16(strImagePath);
		free(strImagePath);
		strImagePath = NULL;

		Bitmap* bmp = new Bitmap(ws);
		free(ws);
		int width;
		int height;

		total->rgba = bmp2rgba(bmp, width, height);
		delete bmp;
		total->gone = (bool*)calloc(sizeof(bool), nextPOT(width) * nextPOT(height));
		goneSIZE = nextPOT(width) * nextPOT(height);
		
		SetScrollRange(tileOpenGLScroll[0], SB_CTL, 0, max(0, total->tex->width - TileRect.size.width), TRUE); // 타일이미지스크롤바 설정
		SetScrollRange(tileOpenGLScroll[1], SB_CTL, 0, max(0, total->tex->height - TileRect.size.height), TRUE);  // 타일이미지 스크롤바 설정
	}


	if (total->tex)
	{
		iRect rt;
		setRGBA(1, 1, 1, 1);
		drawImage(total->tex, 0 - tileOpenGLOff.x, 0 - tileOpenGLOff.y, TOP | LEFT);
		setRGBA(1, 0, 0, 1);
		//drawRect(0, 0, total->tex->width, total->tex->height);

		

		if (GetKeyState(VK_CONTROL) < 0)
		{
			rt = iRectMake(SeletedPoint.x , SeletedPoint.y, getWndInt(hEbTileSize[0]), getWndInt(hEbTileSize[1]));
		}

		else
		{
			findRect(SeletedPoint.x  , SeletedPoint.y );
			rt = iRectMake(_left  , _up  ,_right- _left,_down-_up);
		}

		setRGBA(0,1,0,1);
		drawRect(rt);
		
	
		setRGBA(1, 1, 1, 1);
		
		int potW = nextPOT(_right-_left);
		int potH = nextPOT(_down-_up);
		uint8* rgba = (uint8*)calloc(sizeof(uint8), potW * potH * 4);


		for (int j = _up; j < _down; j++)
			memcpy(&rgba[potW * 4 * (j-_up)], &total->rgba[(int)total->tex->potWidth * 4 * j + 4 * _left], 4 * (_right - _left));

		if (SelectedTexture)
			freeImage(SelectedTexture);

		SelectedTexture = createImageWithRGBA(rgba, rt.size.width, rt.size.height);

		free(rgba);
		checkSelect = true;

		
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

	//glClearColor(0, 0, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	setRGBA(1, 1, 1, 1);
	
	int TileSizeX = getWndInt(hEbTileSize[0]);
	int TileSizeY = getWndInt(hEbTileSize[1]);

	int MapSizeX = getWndInt(hEbMapNum[0]);
	int MapSizeY = getWndInt(hEbMapNum[1]);


	SetScrollRange(MapOpenGLScroll[0], SB_CTL, 0, max(0, MapSizeX - MapRect.size.width), TRUE); // 타일이미지스크롤바 설정
	SetScrollRange(MapOpenGLScroll[1], SB_CTL, 0, max(0, MapSizeY - MapRect.size.height), TRUE);  // 타일이미지 스크롤바 설정

	setRGBA(1, 0, 0, 1);
	//drawRect(0, 0, MapSizeX, MapSizeY);
	


	if (chkTileOnOff == true)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

	if(checkSelect)
	{
		setRGBA(1, 1, 1, 1);
		
		drawImage(SelectedTexture, DrawPoint.x - 20 - MapOpenGLOff.x, DrawPoint.y - MapOpenGLOff.y, TOP | LEFT);
		
		checkSelect = false;

	}
}



void scrollTileOpenGL(HWND hwnd)
{
	
	printf("tile move!\n");
	_left = 0;
	_right = 0;
	_up = 0;
	_down = 0;

	if (hwnd == tileOpenGLScroll[0])
	{
		tileOpenGLOff.x = GetScrollPos(hwnd, SB_CTL);
	
	}
	else if (hwnd == tileOpenGLScroll[1])
	{
		tileOpenGLOff.y = GetScrollPos(hwnd, SB_CTL);
	}

	printf("tile off : %f %f \n", tileOpenGLOff.x, tileOpenGLOff.y);
}

void scrollMapOpenGL(HWND hwnd)
{
	printf("map move!\n");
	_left = 0;
	_right = 0;
	_up = 0;
	_down = 0;

	if (hwnd == MapOpenGLScroll[0])
	{
		MapOpenGLOff.x = GetScrollPos(hwnd, SB_CTL);
	}
	else if (hwnd == MapOpenGLScroll[1])
	{
		MapOpenGLOff.y = GetScrollPos(hwnd, SB_CTL);
	}
	
	printf("tile off : %f %f \n", MapOpenGLOff.x, MapOpenGLOff.y);
}



void TileOnOff(HWND hwnd)
{
	chkTileOnOff = getCheckBox(hwnd);
}
