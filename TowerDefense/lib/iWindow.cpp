#pragma once

#include "iWindow.h"

#include "iStd.h"


static HWND hWnd;
static HINSTANCE hInstance;
int ctrlNum;

void initWndCtrlSystem()
{
	InitCommonControls(); 
	ctrlNum = 0;

}

HWND* hBt;
void testcheckButton(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = (HWND)lParam;
	for (int i = 0; i < 3; i++)
	{
		if (hwnd == hBt[i])
		{
			printf("selected Btn = %d\n", i);
			if (i == 0)
			{
				showChooseColor(NULL);
			}
			return;
		}
	}
}

void showChooseColor(methodChooseColor method)
{
	CHOOSECOLOR cc;// 색선택하는 이미 만들어져있는 윈도우
	memset(&cc, 0x00, sizeof(CHOOSECOLOR));
	
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hWnd;

	COLORREF color[16];
	cc.lpCustColors = color;
	cc.Flags = CC_RGBINIT | CC_FULLOPEN;

	if (!ChooseColor(&cc)) // 창이 열려있으면 어떠한 값도 넘겨주지 읺게 위함
		return;

	int r = (cc.rgbResult & 0x0000ff);
	int g = (cc.rgbResult & 0x00ff00) >> 8;
	int b = (cc.rgbResult & 0xff0000) >> 16;

	printf("r = %d, g = %d, b = %d\n", r, g, b);
	
	if(method) // 색이 선택되고 끝나면 method에 해당하는 함수의 인자로 rgb를 대입하고 그 함수를 호출한다.
		method(r, g, b);
}

void testCtrlSystem(HWND hwnd, HINSTANCE hinstance)
{
	hWnd = hwnd;
	hInstance = hinstance;
	createWndStatic(10, 10, 200, 80, "TOOL");
	
	hBt = (HWND*)malloc(sizeof(HWND) * 3);
	const char* strBtn[3] = { "Color","Open","Save" };

	for(int i = 0; i <3; i++)
		hBt[i]=createWndButton(10+220*i, 120, 200, 80, strBtn[i]);

}

HWND createWndStatic(int x, int y, int width, int height, const char* str)
{
	wchar_t* ws = utf8_to_utf16(str);

	HWND hwnd = CreateWindow(WC_STATIC, ws, WS_CHILD | WS_VISIBLE | ES_CENTER , x, y, width, height, (HWND)hWnd, (HMENU)ctrlNum, (HINSTANCE)hInstance, NULL);
	
	ctrlNum++;
	free(ws);

	return hwnd;

}

HWND createWndButton(int x, int y, int width, int height, const char* str)
{
	wchar_t* ws = utf8_to_utf16(str);

	HWND hwnd = CreateWindow(WC_BUTTON, ws, WS_TABSTOP |WS_CHILD | WS_VISIBLE | BS_PUSHBOX, x, y, width, height, (HWND)hWnd, (HMENU)ctrlNum, (HINSTANCE)hInstance, NULL);

	ctrlNum++;
	free(ws);

	return hwnd;

}
