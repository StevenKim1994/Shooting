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

WndGL* wgTile;
WndGL* wgMap;

void loadMapEditor(HWND hwnd)
{
	int i;
	initWndCtrlSystem();

	wcsMapEditor = new WndCtrlSystem(hwnd);
	setWndCtrlSystem(wcsMapEditor);

	const char* strState[3] = { "이동","이미지", "충돌" };
	hCbState = createWndComboBox(5, 5, 80, 160, strState, 3, NULL, NULL);
	// to do...
	hEbOff = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
	{
		createWndStatic(5 + 80 * i, 35, 10, 25, i == 0 ? "X" : "Y", NULL, NULL);
		hEbOff[i] = createWndEditBox(20 + 80 * i, 35, 60, 25, "0", WndEditBoxStyle_int, NULL, NULL);
	}

	// open, numX, numY

	hBtnOpenImage = createWndButton(5, 70, 100, 25, "Open Image", NULL, btnOpenImageUpdate);
	hEbOpenImage = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
		hEbOpenImage[i] = createWndEditBox(110 + 40 * i, 70, 35, 25, "5", WndEditBoxStyle_int, NULL, NULL);
	
	wgTile = createOpenGL(5, 105, 200, 200, methodTileUpdate, 200, 200);
	

	const char* strCollition[3] = { "모두이동불가", "공중이동불가", "모두이동가능" };
	hCbCollsition = createWndComboBox(5, 310, 130, 120, strCollition, 3, NULL, NULL);

	//Right
	hEbMapNum = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
		hEbMapNum[i] = createWndEditBox(300 + 60 * i, 5, 35, 25, "100", WndEditBoxStyle_int, NULL, NULL);
	createWndStatic(340, 5, 10, 25, "x", NULL, NULL);

	const char* strMap[2] = { "Open Map", "Save Map" };
	hBtnMapOpen = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
		hBtnMapOpen[i] = createWndButton(400 + 105 * i, 5, 100, 25, strMap[i], NULL, btnMapOpenUpdate);

	wgMap = createOpenGL(300, 40, 300, 300, methodMapUpdate, 300, 300);
	

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
	wgTile->gl->hWnd;
	wgMap->gl->hWnd;
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
	glClearColor(0, 1, 0, 1);
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

	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}