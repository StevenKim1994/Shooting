#pragma once

#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(lib, "Comctl32.lib")

void initWndCtrlSystem();

void testCtrlSystem(HWND hwnd, HINSTANCE hinstance);


//
// Button
//
HWND createWndStatic(int x, int y, int width, int height, const char* str);
HWND createWndButton(int x, int y, int width, int height, const char* str);
HWND createWndCheckBox(int x, int y, int width, int height, const char* str);

void testcheckButton(WPARAM wParam, LPARAM lParam);

bool getCheckBox(HWND hwnd);
void setCheckBox(HWND hwnd, bool on);

HWND createWndRadio(int x, int y, int width, int height, const char* str);
bool getWndRadio(HWND hwnd);
void setWndRadio(HWND hwnd, bool on);

//
// Group
//
HWND createWndGroup(int x, int y, int width, int height, const char* str);

//
//ComboBox
//
HWND createWndComboBox(int x, int y, int width, int height, const char** line, int lineNum);
void addWndComboBox(HWND hwnd, int index, const char* str);
void addWndComboBox(HWND hwnd, const char** line, int lineNum);
void removeWndComboBox(HWND hwnd, int index);
void setWndComboBox(HWND hwnd, int index);
int indexWndComboBox(HWND hwnd);
int countWndComboBox(HWND hwnd);
char* getWndComboBox(HWND hwnd, int index);

//
//ListBox
//
HWND createWndListBox(int x, int y, int width, int height, const char** line, int lineNum);
void addWndListBox(HWND hwnd, int index, const char* str);
void addWndListBox(HWND hwnd, const char** line, int lineNum);
void removeWndListBox(HWND hwnd, int index);
void setWndListBox(HWND hwnd, int index);
int indexWndListBox(HWND hwnd);
int countWndListBox(HWND hwnd);
char* getWndListBox(HWND hwnd, int index);



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