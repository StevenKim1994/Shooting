#include "Script.h"
#include "ScriptData.h"

static HWND hWnd;
static HINSTANCE hInstance;

void loadScript(HWND hwnd, HINSTANCE hinstance)
{
	initWndCtrlSystem();

	hWnd = hwnd;
	hInstance = hinstance;

	DragAcceptFiles(hWnd, true);

	// to do....
	loadScriptData();
}

void freeScript()
{
	// to do.....
	freeScriptData();
}

void dragScript(WPARAM wParam, LPARAM lParam)
{
	HDROP hDrop = (HDROP)wParam;
	HWND hwnd = (HWND)lParam;
	wchar_t wstrPath[1024];
	DragQueryFile(hDrop, 0, wstrPath, 1024);

	char* strPath = utf16_to_utf8(wstrPath);
	printf("strPath = [%s]\n", strPath);
	free(strPath);
}

void updateScript(WPARAM wParam, LPARAM lParam)
{

}

void drawScript(float dt)
{

}

void keyScript(iKeyState stat, iPoint point)
{

}

LRESULT CALLBACK dlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {

	case WM_CLOSE:
		// unshow
		// destroy
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

