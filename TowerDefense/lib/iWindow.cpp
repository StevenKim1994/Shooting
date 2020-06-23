#include "iWindow.h"

#include "iStd.h"

WndCtrlSystem::WndCtrlSystem(HWND hwnd)
{
	hwndParent = hwnd;

	
	hinstance = (HINSTANCE)GetWindowLong(hwnd,GWLP_HINSTANCE); 
	wc = (WndCtrl*)malloc(sizeof(WndCtrl) * 1024);
	wcNum = 0;
	methodDrag = NULL;
}

WndCtrlSystem::~WndCtrlSystem()
{
	for (int i = 0; i < wcNum; i++)
		DestroyWindow(wc[i].hwnd);

	free(wc);
}


void WndCtrlSystem::dragAcceptFiles(WndDrag drag)
{
	DragAcceptFiles(hwndParent, true);
	methodDrag = drag;
}

void WndCtrlSystem::dropFiles(WPARAM wParam, LPARAM lParam)
{
	HDROP hDrop = (HDROP)wParam;
	HWND hwnd = (HWND)lParam;
	wchar_t wstrPath[1024];
	DragQueryFile(hDrop, 0, wstrPath, 1024);

	char* strPath = utf16_to_utf8(wstrPath);
	if (methodDrag)
		methodDrag(strPath);
	free(strPath);
}

void WndCtrlSystem::add(HWND hwnd, WndStyle style, WndCtrlColor color,  WndCtrlUpdate update)
{
	WndCtrl* c = &wc[wcNum];
	c->hwnd = hwnd;
	c->style = style;
	c->color = color;
	c->update = update;

	wcNum++;
}

LRESULT WndCtrlSystem::color(WPARAM wParam, LPARAM lParam)
{
	//HDC hdc = (HDC)wParam;
	HWND hwnd = (HWND)lParam;
	
	for (int i = 0; i < wcNum; i++)
	{
		WndCtrl* c = &wc[i];
		if (c->hwnd == hwnd)
		{
			if(c->color)
				return c->color((HDC)wParam, hwnd);

			return (LRESULT)0;
		}
	}

	return (LRESULT)0;
}

void WndCtrlSystem::update(WPARAM wParam, LPARAM lParam)
{
	int id = LOWORD(wParam);

	WndCtrl* c = &wc[id];

	int event = HIWORD(wParam);

	switch (c->style)
	{
	case WndStyle_static: // none
		break;
	case WndStyle_button:
	case WndStyle_radio:
		if(c->update)
			c->update(c->hwnd); // (HWND)lParm == c->hwnd
		break;
	case WndStyle_checkBox:
	{
		bool checked = getCheckBox(c->hwnd);
		setCheckBox(c->hwnd, !checked);

		if (c->update)
			c->update(c->hwnd);
		break;
	}

	case WndStyle_comboBox:

		if (event == CBN_SELCHANGE)
		{
			if (c->update)
				c->update(c->hwnd);
		}
		break;

	case WndStyle_listBox:
		switch (HIWORD(wParam))
		{
		case LBN_SELCHANGE:
			if (c->update)
				c->update(c->hwnd);
			break;
		case LBN_DBLCLK:
			// to do...
			break;
		}
		break;

	case WndStyle_editBox:
		switch (event)
		{
		case EN_MAXTEXT:
			// alert : 몇자 제한 입니다. 

			break;

		case EN_UPDATE:
			break;
		case EN_KILLFOCUS:
		case EN_CHANGE:
			if (c->update)
				c->update(c->hwnd);
			break;
		}
		break;

	case WndStyle_dialog:
		break;
	case WndStyle_opengl:
		break;
	}
	c->hwnd;
	c->style;


}

void initWndCtrlSystem()
{
	InitCommonControls();

}

WndCtrlSystem* wcs; // 현재 사용하고 있는 컨트롤시스템


extern HWND* tileOpenGLScroll;
extern HWND* MapOpenGLScroll;
int tileOpenGL = 0;
int MapOpenGL = 0;


void WndCtrlSystem::scroll(WPARAM wParam, LPARAM lParam)
{
	// lParam : HWND
	// wParam : Data

	int state = LOWORD(wParam);
	HWND hwnd = (HWND)lParam;

	WndCtrl* c = NULL;
	for (int i = 0; i < wcs->wcNum; i++)
	{
		c = &wcs->wc[i];
		if (wcs->wc[i].hwnd == hwnd)
		{
			break;

		}
	}    

	switch (c->style)
	{
	case WndStyle_scrollbar:
		if (state == SB_THUMBTRACK || state == SB_THUMBPOSITION)
		{
			SetScrollPos(hwnd, SB_CTL, HIWORD(wParam), TRUE);
		}
		else if (state == SB_LINELEFT)
		{
			SetScrollPos(hwnd, SB_CTL, GetScrollPos(hwnd, SB_CTL) - 10, TRUE);
		}
		else if (state == SB_LINERIGHT)
		{
			SetScrollPos(hwnd, SB_CTL, GetScrollPos(hwnd, SB_CTL) + 10, TRUE);
		}
		else if (state == SB_PAGELEFT)
		{
			SetScrollPos(hwnd, SB_CTL, GetScrollPos(hwnd, SB_CTL) - 1, TRUE);
		}
		else if (state == SB_PAGERIGHT)
		{
			SetScrollPos(hwnd, SB_CTL, GetScrollPos(hwnd, SB_CTL) + 1, TRUE);
		}

		if (c->update)
			c->update(c->hwnd);

		break;
	case WndStyle_static: // none
	case WndStyle_button:
	case WndStyle_radio:
	case WndStyle_checkBox:
	case WndStyle_comboBox:
	case WndStyle_listBox:
	case WndStyle_editBox:
	case WndStyle_dialog:
	case WndStyle_opengl:
	case WndStyle_trackbar:
		if (state == TB_ENDTRACK || state == TB_THUMBTRACK)
		{
			if (c->update)
				c->update(c->hwnd);
		}
		break;
	}

}

void setWndCtrlSystem(WndCtrlSystem* _wcs)
{
	wcs = _wcs;
}

HWND createWndStatic(int x, int y, int width, int height,
	const char* str, WndCtrlColor color , WndCtrlUpdate update)
{
	wchar_t* ws = utf8_to_utf16(str);
	HWND hwnd = CreateWindow(WC_STATIC, ws,
		WS_CHILD | WS_VISIBLE | ES_CENTER, x, y, width, height,
		(HWND)wcs->hwndParent, (HMENU)wcs->wcNum, (HINSTANCE)wcs->hinstance, NULL);
	wcs->add(hwnd, WndStyle_static, color, update);

	free(ws);

	return hwnd;
}

HWND createWndButton(int x, int y, int width, int height,
	const char* str, WndCtrlColor color, WndCtrlUpdate update)
{
	wchar_t* ws = utf8_to_utf16(str);
	//HWND hwnd = CreateWindow(
	HWND hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
		WC_BUTTON, ws,
		WS_TABSTOP |
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, width, height,
		(HWND)wcs->hwndParent, (HMENU)wcs->wcNum, (HINSTANCE)wcs->hinstance, NULL);
	wcs->add(hwnd, WndStyle_button, color, update);
	free(ws);

	return hwnd;
}

HWND createWndCheckBox(int x, int y, int width, int height,
	const char* str, WndCtrlColor color, WndCtrlUpdate update)
{
	wchar_t* ws = utf8_to_utf16(str);
	HWND hwnd = CreateWindow(WC_BUTTON, ws,
		WS_TABSTOP |
		WS_CHILD | WS_VISIBLE | BS_CHECKBOX, x, y, width, height,
		(HWND)wcs->hwndParent, (HMENU)wcs->wcNum, (HINSTANCE)wcs->hinstance, NULL);
	wcs->add(hwnd, WndStyle_checkBox, color, update);
	free(ws);

	return hwnd;
}

bool getCheckBox(HWND hwnd)
{
	return SendMessage(hwnd, (UINT)BM_GETCHECK, (WPARAM)0, (LPARAM)0);
}

void setCheckBox(HWND hwnd, bool on)
{
	SendMessage(hwnd, BM_SETCHECK, on ? BST_CHECKED : BST_UNCHECKED, 0);
}

HWND createWndRadio(int x, int y, int width, int height,
	const char* str, WndCtrlColor color, WndCtrlUpdate update)
{
	wchar_t* ws = utf8_to_utf16(str);
	HWND hwnd = CreateWindow(WC_BUTTON, ws,
		WS_TABSTOP |
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, x, y, width, height,
		(HWND)wcs->hwndParent, (HMENU)wcs->wcNum, (HINSTANCE)wcs->hinstance, NULL);
	wcs->add(hwnd, WndStyle_radio, color, update);
	free(ws);

	return hwnd;
}

bool getWndRadio(HWND hwnd)
{
	return SendMessage(hwnd, (UINT)BM_GETCHECK, (WPARAM)0, (LPARAM)0);
}

void setWndRadio(HWND hwnd, bool on)
{
	SendMessage(hwnd, BM_SETCHECK, on ? BST_CHECKED : BST_UNCHECKED, 0);
}

HWND createWndGroup(int x, int y, int width, int height,
	const char* str, WndCtrlColor color, WndCtrlUpdate update)
{
	wchar_t* ws = utf8_to_utf16(str);
	HWND hwnd = CreateWindow(WC_BUTTON, ws,
		WS_TABSTOP |
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX, x, y, width, height,
		(HWND)wcs->hwndParent, (HMENU)wcs->wcNum, (HINSTANCE)wcs->hinstance, NULL);
	wcs->add(hwnd, WndStyle_static, color, update);
	free(ws);

	return hwnd;
}

HWND createWndComboBox(int x, int y, int width, int height,
	const char** line, int lineNum, WndCtrlColor color, WndCtrlUpdate update)
{
	HWND hwnd = CreateWindow(WC_COMBOBOX, NULL,
		WS_TABSTOP |
		WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN | CBS_HASSTRINGS, x, y, width, height,
		(HWND)wcs->hwndParent, (HMENU)wcs->wcNum, (HINSTANCE)wcs->hinstance, NULL);
	wcs->add(hwnd, WndStyle_comboBox, color, update);
	addWndComboBox(hwnd, line, lineNum);
	setWndComboBox(hwnd, 0);
	return hwnd;
}

void addWndComboBox(HWND hwnd, int index, const char* str)
{
	wchar_t* s = utf8_to_utf16(str);
	SendMessage(hwnd, CB_INSERTSTRING, (WPARAM)index, (LPARAM)s);
	free(s);
}

void addWndComboBox(HWND hwnd, const char** line, int lineNum)
{
	for (int i = 0; i < lineNum; i++)
		addWndComboBox(hwnd, i, line[i]);
}

void removeWndComboBox(HWND hwnd, int index)
{
	SendMessage(hwnd, CB_DELETESTRING, (WPARAM)index, (LPARAM)0);
}

int countWndComboBox(HWND hwnd)
{
	return SendMessage(hwnd, CB_GETCOUNT, (WPARAM)0, (LPARAM)0);
}

char* getWndComboBox(HWND hwnd, int index)
{
	wchar_t wstr[128];
	SendMessage(hwnd, CB_GETLBTEXT, (WPARAM)index, (LPARAM)wstr);
	return utf16_to_utf8(wstr);
}

int indexWndComboBox(HWND hwnd)
{
int index = SendMessage(hwnd, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
return index;
}

void setWndComboBox(HWND hwnd, int index)
{
	SendMessage(hwnd, CB_SETCURSEL, (WPARAM)index, (LPARAM)0);
}

HWND createWndTrackBar(int x, int y, int width, int height,
	const char* str, WndCtrlColor color, WndCtrlUpdate update, bool displayTicks)
{
	int lineNum;
	char** line = iString::getStringLine(str, lineNum);
	int MIN = atoi(line[0]);
	int MAX = atoi(line[1]);
	int POS = atoi(line[2]);

	bool vertical = (height > width);

	HWND hwnd = CreateWindow(TRACKBAR_CLASS, NULL,
		WS_TABSTOP |
		WS_CHILD | WS_VISIBLE |
		(displayTicks ? TBS_AUTOTICKS : TBS_NOTICKS) |
		(vertical ? TBS_VERT : TBS_HORZ),
		x, y, width, height,
		(HWND)wcs->hwndParent, (HMENU)wcs->wcNum, (HINSTANCE)wcs->hinstance, NULL);
	setWndTrackBarRange(hwnd, MIN, MAX);
	SendMessage(hwnd, TBM_SETPAGESIZE, (WPARAM)0, (LPARAM)MAX / 10.0f);
	SendMessage(hwnd, TBM_SETTICFREQ, (WPARAM)MAX / 10.0f, (LPARAM)0);
	//SendMessage(hwnd, TBM_SETPAGESIZE, 0, 0);
	//SendMessage(hwnd, TBM_SETTICFREQ, 0, 0);
	setWndTrackBarPos(hwnd, POS);
	wcs->add(hwnd, WndStyle_trackbar, color, update);

	iString::freeStringLine(line, lineNum);
	return hwnd;
}

HWND* addWndTrackBarBuddy(HWND hwnd, const char* str)
{
	RECT rt;
	GetClientRect(hwnd, &rt);
	bool vertical = (rt.bottom - rt.top > rt.right - rt.left);

	int lineNum;
	char** line = iString::getStringLine(str, lineNum);

	int anc[4] = { SS_RIGHT, SS_LEFT, SS_CENTER, SS_CENTER };

	HWND* h = (HWND*)malloc(sizeof(HWND) * 2);
	for (int i = 0; i < 2; i++)
	{
		wchar_t* ws = utf8_to_utf16(line[i]);
		h[i] = CreateWindow(WC_STATIC, ws, WS_CHILD | WS_VISIBLE | anc[2 * vertical + i],
			0, 0, 50, 20, wcs->hwndParent, (HMENU)wcs->wcNum, wcs->hinstance, NULL);
		wcs->add(hwnd, WndStyle_static, NULL, NULL);
		free(ws);
		SendMessage(hwnd, TBM_SETBUDDY, (WPARAM)(i == 0 ? TRUE : FALSE), (LPARAM)h[i]);
	}

	iString::freeStringLine(line, lineNum);

	return h;
}

void setWndTrackBarRange(HWND hwnd, int MIN, int MAX)
{
	SendMessage(hwnd, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(MIN, MAX));
}

void setWndTrackBarPos(HWND hwnd, int POS)
{
	SendMessage(hwnd, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)POS);
}

int getWndTrackBarPos(HWND hwnd)
{
	return SendMessage(hwnd, TBM_GETPOS, 0, 0);
}

HWND createWndListBox(int x, int y, int width, int height,
	const char** line, int lineNum, WndCtrlColor color, WndCtrlUpdate update)
{
	HWND hwnd = CreateWindow(WC_LISTBOX, NULL,
		WS_TABSTOP |
		WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY |
		WS_HSCROLL | WS_VSCROLL, x, y, width, height,
		(HWND)wcs->hwndParent, (HMENU)wcs->wcNum, (HINSTANCE)wcs->hinstance, NULL);
	wcs->add(hwnd, WndStyle_listBox, color, update);
	addWndListBox(hwnd, line, lineNum);
	setWndListBox(hwnd, 0);
	return hwnd;
}

void addWndListBox(HWND hwnd, int index, const char* str)
{
	wchar_t* s = utf8_to_utf16(str);
	SendMessage(hwnd, LB_INSERTSTRING, (WPARAM)index, (LPARAM)s);
	free(s);
}

void addWndListBox(HWND hwnd, const char** line, int lineNum)
{
	for (int i = 0; i < lineNum; i++)
		addWndListBox(hwnd, i, line[i]);
}
void removeWndListBox(HWND hwnd, int index)
{
	SendMessage(hwnd, LB_DELETESTRING, (WPARAM)index, (LPARAM)0);
}
int countWndListBox(HWND hwnd)
{
	return SendMessage(hwnd, LB_GETCOUNT, (WPARAM)0, (LPARAM)0);
}
int indexWndListBox(HWND hwnd)
{
	int index = SendMessage(hwnd, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	return index;
}
char* getWndListBox(HWND hwnd, int index)
{
	wchar_t wstr[128];
	SendMessage(hwnd, LB_GETTEXT, (WPARAM)index, (LPARAM)wstr);
	return utf16_to_utf8(wstr);
}
void setWndListBox(HWND hwnd, int index)
{
	SendMessage(hwnd, LB_SETCURSEL, (WPARAM)index, (LPARAM)0);
}
HWND createWndScrollBar(int x, int y, int width, int height,int style, WndCtrlColor color, WndCtrlUpdate update)
{
	HWND hwnd = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | style,x,y,width, height, (HWND)wcs->hwndParent, (HMENU)wcs->wcNum, (HINSTANCE)wcs->hinstance, NULL);
	
	wcs->add(hwnd, WndStyle_scrollbar, color, update);
	return hwnd;
}
LRESULT CALLBACK WndEditBoxProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, WndEditBoxStyle style);

static int ebMaxLength = 10;
void setWndEditBosLength(int maxLength)
{
	ebMaxLength = maxLength;
}
LRESULT CALLBACK WndEditBoxAllProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return WndEditBoxProc(hwnd, msg, wParam, lParam, WndEditBoxStyle_all);
}

LRESULT CALLBACK WndEditBoxIntProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return WndEditBoxProc(hwnd, msg, wParam, lParam, WndEditBoxStyle_int);
}

LRESULT CALLBACK WndEditBoxFloatProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return WndEditBoxProc(hwnd, msg, wParam, lParam, WndEditBoxStyle_float);
}

LRESULT CALLBACK WndEditBoxProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, WndEditBoxStyle style)
{
	WNDPROC wpOld = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch (msg) {

	case WM_NCDESTROY:
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)wpOld);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
		break;

	case WM_CHAR:

		if (style == WndEditBoxStyle_all)
		{
			// ok
		}
		else if (style == WndEditBoxStyle_int)
		{
			if (wParam == VK_RETURN || wParam == VK_BACK || (wParam == '-' || wParam >= '0' && wParam <= '9'))
			{
				// ok
			}
			else
				return 0; // key blocking
		}
		else if (style == WndEditBoxStyle_float)
		{
			if (wParam == '.')
			{
				wchar_t ws[1024];
				int length = GetWindowTextLength(hwnd) + 1;
				GetWindowText(hwnd, ws, length);
				for (int i = 0; i < length; i++)
				{
					if (ws[i] == '.') // 점이 두개가 존재할수없게함.
					{
						return 0;
					}
				}
			}
			else if (wParam == VK_RETURN || wParam == VK_BACK || (wParam == '-' || wParam >= '0' && wParam <= '9'))
			{
				// ok
			}
			else
				return 0; // key blocking
		}
		break;

	}// end of switch

	return CallWindowProc(wpOld, hwnd, msg, wParam, lParam);
}


HWND createWndEditBox(int x, int y, int width, int height,
	const char* str, WndEditBoxStyle style, WndCtrlColor color, WndCtrlUpdate update)
{
	wchar_t* s = utf8_to_utf16(str);
	HWND hwnd = CreateWindow(WC_EDIT, s,
		WS_TABSTOP |
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
		x, y, width, height,
		(HWND)wcs->hwndParent, (HMENU)wcs->wcNum, (HINSTANCE)wcs->hinstance, NULL);
	free(s);
	wcs->add(hwnd, WndStyle_editBox, color, update);
	SendMessage(hwnd, (UINT)EM_LIMITTEXT, (WPARAM)ebMaxLength, (LPARAM)0);
	SetWindowLongPtr(hwnd, GWLP_USERDATA, GetWindowLongPtr(hwnd, GWLP_WNDPROC));
	LRESULT(CALLBACK * m[3])(HWND, UINT, WPARAM, LPARAM) = { WndEditBoxAllProc , WndEditBoxIntProc, WndEditBoxFloatProc};
	SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)m[style]);

	return hwnd;
}

LRESULT CALLBACK WndEditBoxProcMultiProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC wpOld = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch (msg) {

	case WM_NCDESTROY:
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)wpOld);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
		break;

	case WM_GETDLGCODE:
	{
		LRESULT ret = CallWindowProc(wpOld, hwnd, msg, wParam, lParam);
		ret &= ~(DLGC_HASSETSEL | DLGC_WANTTAB);

		if (lParam &&
			((LPMSG)lParam)->message == WM_KEYDOWN &&
			((LPMSG)lParam)->wParam == VK_TAB)
			ret &= ~DLGC_WANTMESSAGE;
		return ret;
	
		//break;
	} // end of WM_GETDLGCODE	
	}// end of switch

	return CallWindowProc(wpOld, hwnd, msg, wParam, lParam);
}

HWND createWndEditBoxMultiline(int x, int y, int width, int height, const char* str, WndCtrlColor color, WndCtrlUpdate update)
{
	
	wchar_t* s = utf8_to_utf16(str);
	HWND hwnd = CreateWindow(WC_EDIT, s,
		WS_TABSTOP |
		WS_CHILD | WS_VISIBLE | WS_BORDER |
		ES_MULTILINE | ES_WANTRETURN | ES_NOHIDESEL | ES_AUTOVSCROLL |ES_AUTOHSCROLL, // WANTRETURN 엔터먹히게함
		x, y, width, height,
		(HWND)wcs->hwndParent, (HMENU)wcs->wcNum, (HINSTANCE)wcs->hinstance, NULL);
	free(s);
	wcs->add(hwnd, WndStyle_editBox, color, update);
	SendMessage(hwnd, (UINT)EM_LIMITTEXT, (WPARAM)ebMaxLength, (LPARAM)0);
	SetWindowLongPtr(hwnd, GWLP_USERDATA, GetWindowLongPtr(hwnd, GWLP_WNDPROC));
	SetWindowLongPtr(hwnd, GWLP_WNDPROC,(LONG_PTR)WndEditBoxProcMultiProc);

	return hwnd;


}



void enableWnd(HWND hwnd, bool enable)
{
	EnableWindow(hwnd, enable);
}

void setWndText(HWND hwnd, const char* szFormat, ...)
{
	char szText[1024];
	va_list args;

	va_start(args, szFormat);
	_vsnprintf(szText, sizeof(szText), szFormat, args);
	va_end(args);

	wchar_t* ws = utf8_to_utf16(szText);
	SetWindowText(hwnd, ws);
	free(ws);
}

char* getWndText(HWND hwnd)
{
	wchar_t ws[1024];
	int length = GetWindowTextLength(hwnd) + 1;
	GetWindowText(hwnd, ws, length);
	return utf16_to_utf8(ws);
}

int getWndInt(HWND hwnd)
{
	char* str = getWndText(hwnd);
	int n = atoi(str);
	free(str);
	return n;
}

float getWndFloat(HWND hwnd)
{
	char* str = getWndText(hwnd);
	int n = atof(str);
	free(str);
	return n;
}

extern HINSTANCE hInstance; // 외부에 있는 하나의 유일한 인스턴스
extern HWND hWnd;

HWND createWndDlg(int x, int y, int width, int height,
	const char* strTitle, WNDPROC wndProc)
{
	wchar_t* ws = utf8_to_utf16(strTitle);
	LPCWSTR dlgClassName = ws;

	WNDCLASSEX wc;
	if (GetClassInfoEx(NULL, TEXT("#32770"), &wc))
	{
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpfnWndProc = wndProc;
		wc.lpszClassName = dlgClassName;
		RegisterClassEx(&wc);
	}


	//HWND hDlg = CreateWindow(WC_DIALOG,
	HWND hDlg = CreateWindowEx(WS_EX_DLGMODALFRAME, dlgClassName,//WC_DIALOG,
		ws, WS_SYSMENU | WS_CAPTION, x, y, width, height,
		(HWND)hWnd, (HMENU)0, hInstance, NULL);

	//SetWindowPos(hDlg, HWND_TOPMOST, x, y, width, height, SWP_HIDEWINDOW); 항상위로

	free(ws);

	return hDlg;
}

void showChooseColor(methodChooseColor method)
{
	CHOOSECOLOR cc;
	memset(&cc, 0x00, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hWnd;
	COLORREF color[16];
	cc.lpCustColors = color;
	cc.Flags = CC_RGBINIT | CC_FULLOPEN;
	if (!ChooseColor(&cc))
		return;

	int r = (cc.rgbResult & 0x0000ff);
	int g = (cc.rgbResult & 0x00ff00) >> 8;
	int b = (cc.rgbResult & 0xff0000) >> 16;
	printf("r = %d, g = %d, b = %d\n", r, g, b);
	if (method)
		method(r, g, b);
}

//const char* openFileDlg(bool open, const char* filter)
static char* strOpenPath = NULL;
const char* openFileDlg(bool open, LPCWSTR filter)
{
	wchar_t wstrPath[1024];

	OPENFILENAME ofn;
	memset(&ofn, 0x00, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
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

void endOnlyRun(HANDLE event)
{
	CloseHandle(event);
}



static iCriticalSection* instanceCS = NULL;
iCriticalSection* iCriticalSection::instance()
{
	if (instanceCS == NULL)
		instanceCS = new iCriticalSection();
	return instanceCS;
}

iCriticalSection::iCriticalSection()
{
	InitializeCriticalSection(&cs);
}

iCriticalSection::~iCriticalSection()
{
	DeleteCriticalSection(&cs);
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


// for Games
iOpenGL::iOpenGL(int x, int y, int width, int height, int bits, const char* name, bool fullscreen)
{
	DWORD dwExStyle;
	DWORD dwStyle;
	if (fullscreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}
	RECT rect;
	rect.left = x;
	rect.right = x + width;
	rect.top = y;
	rect.bottom = y + height;
	AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);

	wchar_t* szWindowClass = utf8_to_utf16(name);
	HWND hwnd = CreateWindowEx(dwExStyle,
		szWindowClass,
		szWindowClass,
		dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		x, y,
		width, height,
		(HWND)NULL, (HMENU)NULL, hInstance, NULL);
	free(szWindowClass);
	HDC hdc = GetDC(hwnd);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0x00, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = bits;
	pfd.cDepthBits = bits;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pixelFormat, &pfd);

	HGLRC hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	glewExperimental = TRUE;
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		dispose();
		return;
	}

#if 0
	int attr[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0,
	};
	if (wglewIsSupported("WGL_ARB_create_context"))
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hrc);

		hrc = wglCreateContextAttribsARB(hdc, NULL, attr);

		wglMakeCurrent(hdc, hrc);
	}
#endif

	hWnd = hwnd;
	hDC = hdc;
	hRC = hrc;
}


// for Ctrl
iOpenGL::iOpenGL(int x, int y, int width, int height, int bits, bool visible)
{
	DWORD dwStyle = WS_CHILD | WS_BORDER;
	if (visible)
		dwStyle |= WS_VISIBLE;

	HWND hwnd = CreateWindow(WC_STATIC, NULL,
		dwStyle,
		x, y, width, height, wcs->hwndParent, (HMENU)wcs->wcNum, wcs->hinstance, NULL);
	HDC hdc = GetDC(hwnd);
	wcs->add(hwnd, WndStyle_opengl, NULL, NULL);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0x00, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = bits;
	pfd.cDepthBits = bits;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pixelFormat, &pfd);

	int attr[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 1,
		WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0,
	};
	static HGLRC _hrc = NULL;
	if (_hrc)
	{
		HGLRC hrc = wglCreateContextAttribsARB(hdc, _hrc, attr);
		wglShareLists(_hrc, hrc);

		hWnd = hwnd;
		hDC = hdc;
		hRC = hrc;

		wglMakeCurrent(hdc, hrc);
		return;
	}

	HGLRC hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	glewExperimental = TRUE;
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		dispose();
		return;
	}

#if 1
	if (wglewIsSupported("WGL_ARB_create_context"))
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hrc);

		hrc = wglCreateContextAttribsARB(hdc, NULL, attr);
		_hrc = hrc;

		wglMakeCurrent(hdc, hrc);
	}
#endif

	hWnd = hwnd;
	hDC = hdc;
	hRC = hrc;
}

iOpenGL::~iOpenGL()
{
	dispose();
}

void iOpenGL::setMakeCurrent()
{
	wglMakeCurrent(hDC, hRC);
}

void iOpenGL::swapBuffer()
{
	SwapBuffers(hDC);
	wglMakeCurrent(NULL, NULL);
}

void iOpenGL::dispose()
{
	if (hRC)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRC);
		hRC = NULL;
	}

	if (hWnd && hDC)
	{
		ReleaseDC(hWnd, hDC);
		hDC = NULL;
	}

	if (hWnd)
	{
		DestroyWindow(hWnd);
		hWnd = NULL;
	}

}

WndCtrlSystem* wcsOpenGL;
iArray* arrayGL;

void freeGL(void* parm)
{	
	WndGL* wg = (WndGL*)parm;

	delete wg->gl;

	if(wg->vao!= 1004)
		glDeleteVertexArrays(1, &wg->vao);
	free(wg);
}


void setupOpenGL(bool setup)
{
	if (setup == false)
	{
		delete wcsOpenGL;
		if (arrayGL)
			delete arrayGL;
		return;
	}
	wcsOpenGL = new WndCtrlSystem(hWnd);
	setWndCtrlSystem(wcsOpenGL);

	WndGL* wg = (WndGL*)malloc(sizeof(WndGL));
	wg->gl = new iOpenGL(0, 0, 480, 320, 32, false);
	initOpenGL();
	wg->vao = 1004;///////////////////////

	arrayGL = new iArray(freeGL);
	arrayGL->addObject(wg);
}

WndGL* createOpenGL(int x, int y, int width, int height, MethodWndGLUpdate m, int devWidth, int devHeight)
{
	WndGL* wg = (WndGL*)malloc(sizeof(WndGL));
	wg->gl = new iOpenGL(x, y, width, height, 32);
	wg->gl->setMakeCurrent();

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_ALWAYS);
	glClearDepth(1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	wglMakeCurrent(NULL, NULL);

	wg->method = m;
	wg->devSize = iSizeMake(devWidth, devHeight);

	arrayGL->addObject(wg);

	return wg;
}

void updateOpenGL(float dt)
{
	for (int i = 1; i < arrayGL->count; i++)
	{
		WndGL* wg = (WndGL*)arrayGL->objectAtIndex(i);
		wg->gl->setMakeCurrent();
#if 0
		RECT rt;
		GetClientRect(wg->gl->hWnd, &rt);
		devSize = wg->devSize;
		reshapeOpenGL(rt.right - rt.left, rt.bottom - rt.top);
#else
		devSize = wg->devSize;

		glViewport(0, 0, devSize.width, devSize.height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, devSize.width, devSize.height, 0, -1000, 1000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
#endif
		wg->method(dt);

		wg->gl->swapBuffer();
	}
}

bool checkOpenGLPosition(HWND hwndParent, HWND hwndOpengl, iPoint& point)
{
	RECT rt;
	GetWindowRect(hwndParent, &rt);
	RECT rtOpengl;
	GetWindowRect(hwndOpengl, &rtOpengl);
	int caption = GetSystemMetrics(SM_CYCAPTION);// 23
	int menu = GetSystemMetrics(SM_CYMENU);// 20
	int xframe = GetSystemMetrics(SM_CXFRAME);// 4
	int yframe = GetSystemMetrics(SM_CYFRAME);// 4
	int xBorder = GetSystemMetrics(SM_CXBORDER);// 1
	int yBorder = GetSystemMetrics(SM_CYBORDER);// 1
	iRect openglRt = iRectMake(rtOpengl.left, rtOpengl.top, rtOpengl.right - rtOpengl.left, rtOpengl.bottom - rtOpengl.top);
	openglRt.origin.x = openglRt.origin.x - rt.left - xframe * 2;
	openglRt.origin.y = openglRt.origin.y - rt.top - caption - menu - yframe * 2;
	openglRt.origin.x += xBorder; openglRt.size.width += xBorder * 2;// GetClientRect
	openglRt.origin.y += yBorder; openglRt.size.height += yBorder * 2;
	if (containPoint(point, openglRt))
	{
		point -= openglRt.origin;
		return true;
	}
	return false;
}