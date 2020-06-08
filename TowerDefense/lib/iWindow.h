#pragma once

#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(lib, "Comctl32.lib")

void initWndCtrlSystem();

void testCtrlSystem(HWND hwnd, HINSTANCE hinstance);

HWND createWndStatic(int x, int y, int width, int height, const char* str);
HWND createWndButton(int x, int y, int width, int height, const char* str);
HWND createWndCheckBox(int x, int y, int width, int height, const char* str);

void testcheckButton(WPARAM wParam, LPARAM lParam);

bool getCheckBox(HWND hwnd);
void setCheckBox(HWND hwnd, bool on);



typedef void (*methodChooseColor) (int r, int g, int b);
void showChooseColor(methodChooseColor);

//wchar_t
const char* openFileDlg(bool open, LPCWSTR filter); // true : load, false : save

HANDLE startOnlyRun(const wchar_t* className);
void endOnlyRun(HANDLE handle);


class iCriticalSection
{
private:
	iCriticalSection();

public :

	static iCriticalSection* instance();
	virtual ~iCriticalSection();

	void start();
	void end();

private:
	CRITICAL_SECTION cs;

};