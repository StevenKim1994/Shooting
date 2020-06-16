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

void btnMapOpenUpdate(HWND hwmd);
void methodMapUpdate(float dt);

// tileset Info
HWND* hEbTileSize;
HWND hEbTileRotate;
HWND* hEbTileRGBA;

// OpenGL View
WndGL* wgTile;
WndGL* wgMap;

void loadMapEditor(HWND hwnd)
{
	int i;
	initWndCtrlSystem();

	createWndStatic(130, 5, 125, 30, "타일뷰", NULL, NULL);

	wcsMapEditor = new WndCtrlSystem(hwnd);
	setWndCtrlSystem(wcsMapEditor);

	createWndStatic(15, 500, 125, 80, "타일상태 지정:", NULL, NULL);

	const char* strState[3] = { "이동","이미지", "충돌" };
	hCbState = createWndComboBox(25, 520, 80, 160, strState, 3, NULL, NULL);
	
	createWndStatic(160, 500, 125, 80, "타일크기 지정:", NULL, NULL);
	hEbTileSize = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
	{
		createWndStatic(160, 530 + 30* i, 20, 20, i == 0 ? "X:" : "Y:", NULL, NULL);
		hEbTileSize[i] = createWndEditBox(180, 525 + 30 * i, 75,20,"0", WndEditBoxStyle_int, NULL, NULL);
	}

	createWndStatic(15, 420, 270, 70, "타일뷰 오프셋", NULL, NULL);
	hEbOff = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
	{
		createWndStatic(15 + 140* i,450, 20, 20, i == 0 ? "X:" : "Y:", NULL, NULL);
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
		hEbTileRGBA[i] = createWndEditBox(150 + 30 * i, 660, 30, 30,"255", WndEditBoxStyle_int, NULL, NULL);
	}

	// open, numX, numY

	hBtnOpenImage = createWndButton(15, 5, 100, 25, "Open Image", NULL, btnOpenImageUpdate);
	hEbOpenImage = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
		hEbOpenImage[i] = createWndEditBox(110 + 40 * i, 70, 35, 25, "5", WndEditBoxStyle_int, NULL, NULL);
	
	wgTile = createOpenGL(15, 40, 270, 350, methodTileUpdate, 270, 350);
	

	const char* strCollition[3] = { "모두이동불가", "공중이동불가", "모두이동가능" };
	//hCbCollsition = createWndComboBox(5, 400, 130, 120, strCollition, 3, NULL, NULL);

	//Right

	createWndStatic(900, 5, 125, 25 ,"맵 크기:", NULL, NULL);
	hEbMapNum = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
		hEbMapNum[i] = createWndEditBox(1100 + 100 * i, 5, 70, 25, "100", WndEditBoxStyle_int, NULL, NULL);
	createWndStatic(1180, 5, 10, 25, "x", NULL, NULL);

	createWndStatic(750, 5, 50, 25, "맵 뷰", NULL, NULL);

	const char* strMap[2] = { "Open Map", "Save Map" };
	hBtnMapOpen = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
		hBtnMapOpen[i] = createWndButton(400 + 105 * i, 5, 100, 25, strMap[i], NULL, btnMapOpenUpdate);

	wgMap = createOpenGL(300, 40, 1100, 650, methodMapUpdate, 1100, 650);
	

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

void keyMapEditor(iKeyState stat, iPoint point)
{
	//opengl

	

	printf("point (%f , %f)\n", point.x, point.y);

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
		//to d
		int len = strlen(path);

		strImagePath = (char*)calloc(sizeof(char), 1 + len);
		strcpy(strImagePath, path);

	}
}


void methodTileUpdate(float dt)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	
	

}