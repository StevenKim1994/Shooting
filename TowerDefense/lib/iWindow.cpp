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
HWND hBtnCheck;

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

			else
				openFileDlg(i == 1 ? true : false, TEXT("PNG Files\0\*.png;*.PNG\0ALL Files\0*.*\0"));
			return;
		}
	}

	if (hwnd == hBtnCheck)
	{
		setCheckBox(hwnd, !getCheckBox(hwnd));
		return;
	}
}

bool getCheckBox(HWND hwnd)
{
	return SendMessage(hwnd, (UINT)BM_GETCHECK, (WPARAM)0, (LPARAM)0);
}

void setCheckBox(HWND hwnd, bool on)
{
	SendMessage(hwnd, BM_SETCHECK, on ? BST_CHECKED : BST_UNCHECKED, 0);
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


static char* strOpenPath = NULL;
const char* openFileDlg(bool open, LPCWSTR filter)
{
	wchar_t wstrPath[1024];

	OPENFILENAME ofn;
	memset(&ofn, 0x00, sizeof(OPENFILENAME));
	
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1; // 파일을 선택할수 있는 갯수 설정
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrFile = wstrPath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	bool success = false;

	if (open) success = GetOpenFileName(&ofn);
	else success = GetSaveFileName(&ofn);

	if (success)
	{
		wsprintf(wstrPath, TEXT("%s"), ofn.lpstrFile);
		if (strOpenPath)
			free(strOpenPath);
		strOpenPath = utf16_to_utf8(wstrPath);
		printf("strOpenPath = %s\n", strOpenPath);
		return strOpenPath;
	}
	return NULL;


}

HANDLE startOnlyRun(const wchar_t* className)
{
	HANDLE event = CreateEvent(NULL, TRUE, FALSE, className);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(event);
		return NULL;
	}
	return event;
}

void endOnlyRun(HANDLE handle)
{
	CloseHandle(handle);
}

void testCtrlSystem(HWND hwnd, HINSTANCE hinstance)
{
	hWnd = hwnd;
	hInstance = hinstance;

	DragAcceptFiles(hWnd, true);

	createWndStatic(10, 10, 200, 80, "TOOL");
	
	hBt = (HWND*)malloc(sizeof(HWND) * 3);
	const char* strBtn[3] = { "Color","Open","Save" };

	for(int i = 0; i <3; i++)
		hBt[i]=createWndButton(10+220*i, 120, 200, 80, strBtn[i]);

	hBtnCheck = createWndCheckBox(10, 220, 200, 80, "동의함");


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

HWND createWndCheckBox(int x, int y, int width, int height, const char* str)
{
	wchar_t* ws = utf8_to_utf16(str);

	HWND hwnd = CreateWindow(WC_BUTTON, ws, WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_CHECKBOX, x, y, width, height, (HWND)hWnd, (HMENU)ctrlNum, (HINSTANCE)hInstance, NULL);

	ctrlNum++;
	free(ws);

	return hwnd;

}

static iCriticalSection* instanceCS = NULL;

iCriticalSection::iCriticalSection()
{
	InitializeCriticalSection(&cs);
}

iCriticalSection* iCriticalSection::instance()
{
	if (instanceCS == NULL)
		instanceCS = new iCriticalSection();
	return instanceCS;
}

iCriticalSection::~iCriticalSection()
{
	DeleteCriticalSection(&cs);
	delete instanceCS;

	instanceCS = NULL;
}

void iCriticalSection::start()
{
	EnterCriticalSection(&cs);
}

void iCriticalSection::end()
{
	LeaveCriticalSection(&cs);
}
