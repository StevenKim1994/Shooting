#pragma once

#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(lib, "comctl32")

enum WndStyle
{
    WndStyle_NULL = -1,
    WndStyle_static = 0,
    WndStyle_button,
    WndStyle_checkBox,
    WndStyle_radio,
    WndStyle_comboBox,
    WndStyle_listBox,
    WndStyle_editBox,
    WndStyle_dialog,
    WndStyle_opengl,
};

typedef void (*WndDrag)(const char* path);
typedef void (*WndCtrlUpdate)(WPARAM wParam, LPARAM lParam);
typedef LRESULT (*WndCtrlColor)(WPARAM wParam, LPARAM lParam); // wparam : hdc , lparam : color;

struct WndCtrl
{
    HWND hwnd;
    WndStyle style;
    WndCtrlColor color;
    WndCtrlUpdate update;
};

class WndCtrlSystem
{
public:
    WndCtrlSystem(HWND hwnd);
    virtual ~WndCtrlSystem();

    void dragAcceptFiles(WndDrag drag);
    void dropFiles(WPARAM wParam, LPARAM lParam);

    void add(HWND hwnd, WndStyle style, WndCtrlColor color, WndCtrlUpdate update);

    LRESULT color(WPARAM wParam, LPARAM lParam); // wParam hdc / lParam hwnd
    void update(WPARAM wParam, LPARAM lParam);
    
public:
    HWND hwndParent; // 부모윈도우
    HINSTANCE hinstance;
    WndDrag methodDrag;
    WndCtrl* wc; // 자식윈도우들의 배열
    int wcNum; // 자식윈도우 개수

};

void initWndCtrlSystem();
void setWndCtrlSystem(WndCtrlSystem* _wcs);

//
// Static
//
HWND createWndStatic(int x, int y, int width, int height,
    const char* str, WndCtrlColor color, WndCtrlUpdate update);
//
// Button
//
HWND createWndButton(int x, int y, int width, int height,
    const char* str, WndCtrlColor color, WndCtrlUpdate update);

HWND createWndCheckBox(int x, int y, int width, int height,
    const char* str, WndCtrlColor color, WndCtrlUpdate update);
bool getCheckBox(HWND hwnd);
void setCheckBox(HWND hwnd, bool on);

HWND createWndRadio(int x, int y, int width, int height,
    const char* str, WndCtrlColor color, WndCtrlUpdate update);
bool getWndRadio(HWND hwnd);
void setWndRadio(HWND hwnd, bool on);

//
// Group
//
HWND createWndGroup(int x, int y, int width, int height,
    const char* str, WndCtrlColor color, WndCtrlUpdate update);

//
// ComboBox
//
HWND createWndComboBox(int x, int y, int width, int height,
    const char** line, int lineNum, WndCtrlColor color, WndCtrlUpdate update);
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
    const char** line, int lineNum, WndCtrlColor color, WndCtrlUpdate update);
void addWndListBox(HWND hwnd, int index, const char* str);
void addWndListBox(HWND hwnd, const char** line, int lineNum);
void removeWndListBox(HWND hwnd, int index);
int countWndListBox(HWND hwnd);
int indexWndListBox(HWND hwnd);
char* getWndListBox(HWND hwnd, int index);
void setWndListBox(HWND hwnd, int index);

void setWndEditBosLength(int maxLength);
HWND createWndEditBox(int x, int y, int width, int height,
    const char* str, WndCtrlColor color, WndCtrlUpdate update);

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

