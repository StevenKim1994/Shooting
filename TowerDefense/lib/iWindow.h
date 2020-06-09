#pragma once

#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(lib, "comctl32")

void initWndCtrlSystem();

//
// Static
//
HWND createWndStatic(int x, int y, int width, int height,
    const char* str);
//
// Button
//
HWND createWndButton(int x, int y, int width, int height,
    const char* str);

HWND createWndCheckBox(int x, int y, int width, int height,
    const char* str);
bool getCheckBox(HWND hwnd);
void setCheckBox(HWND hwnd, bool on);

HWND createWndRadio(int x, int y, int width, int height,
    const char* str);
bool getWndRadio(HWND hwnd);
void setWndRadio(HWND hwnd, bool on);

//
// Group
//
HWND createWndGroup(int x, int y, int width, int height,
    const char* str);

//
// ComboBox
//
HWND createWndComboBox(int x, int y, int width, int height,
    const char** line, int lineNum);
void addWndComboBox(HWND hwnd, int index, const char* str);
void addWndComboBox(HWND hwnd, const char** line, int lineNum);
void removeWndComboBox(HWND hwnd, int index);
int countWndComboBox(HWND hwnd);
int indexWndComboBox(HWND hwnd);
char* getWndComboBox(HWND hwnd, int index);
void setWndComboBox(HWND hwnd, int index);

//
// ListBox
//
HWND createWndListBox(int x, int y, int width, int height,
    const char** line, int lineNum);
void addWndListBox(HWND hwnd, int index, const char* str);
void addWndListBox(HWND hwnd, const char** line, int lineNum);
void removeWndListBox(HWND hwnd, int index);
int countWndListBox(HWND hwnd);
int indexWndListBox(HWND hwnd);
char* getWndListBox(HWND hwnd, int index);
void setWndListBox(HWND hwnd, int index);

void setWndEditBosLength(int maxLength);
HWND createWndEditBox(int x, int y, int width, int height,
    const char* str);

void enableWnd(HWND hwnd, bool enable);
void setWndText(HWND hwnd, const char* szFormat, ...);
char* getWndText(HWND hwnd);
int getWndInt(HWND hwnd);
float getWndFloat(HWND hwnd);

HWND createWndDlg(int x, int y, int width, int height,
    const char* strTitle, WNDPROC wndProc);

typedef void (*methodChooseColor)(int r, int g, int b);
void showChooseColor(methodChooseColor method);

//const char* openFileDlg(bool open, const char* filter);
const char* openFileDlg(bool open, LPCWSTR filter);

HANDLE startOnlyRun(const wchar_t* className);
void endOnlyRun(HANDLE handle);

class iCriticalSection
{
private:
    iCriticalSection();
public:
    static iCriticalSection* instance();
    virtual ~iCriticalSection();

    void start();
    void end();

private:
    CRITICAL_SECTION cs;
};

