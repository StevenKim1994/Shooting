#include "DlgItem.h"
#include "ScriptData.h"

HWND hDlgItem;
WndCtrlSystem* wcsItem;

void dragDlgItem(const char* path)
{
	//chcek .nom 인지..

	if (strcmp(&path[strlen(path) - 4], ITEM_EXTENSION))
		return;

	loadDataItem(path);
}
void loadDlg(HWND hwnd, HINSTANCE hinstance)
{
	hDlgItem = NULL; // dlgNomProc

	wcsItem = new WndCtrlSystem(hwnd, hinstance);
	wcsItem->dragAcceptFiles(dragDlgItem);
	setWndCtrlSystem(wcsItem);

	// add ctrl...
}

void freeDlgItem()
{
	delete wcsItem;

	DestroyWindow(hDlgItem);
}

void showDlgItem(bool show)
{
	if (show == false)
		ShowWindow(hDlgItem, SW_HIDE);

	else
	{
		ShowWindow(hDlgItem, SW_RESTORE);
		SetFocus(hDlgItem);
	}
}

void dragDlgItem(WPARAM wParam, LPARAM lParam)
{

}

void updateDlgItem(WPARAM wParam, LPARAM lParam)
{
	wcsItem->update(wParam, lParam);
}

void drawDlgItem(float dt)
{

}

LRESULT coloDlgItem(WPARAM wParam, LPARAM lParam)
{
	return wcsItem->color(wParam, lParam);
}

void keyDlgItem(iKeyState stat, iPoint point)
{
}

LRESULT dlgItemProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result;
	switch(msg)
	{
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
		result = wcsItem->color(wParam, lParam);
		if (result)
			return result;
		break;
	case WM_DROPFILES:

		wcsItem->dropFiles(wParam, lParam);
		break;

	case WM_COMMAND:
		wcsItem->update(wParam, lParam);
		break;

	case WM_CLOSE:
		showDlgItem(false);

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void loadDataItem(const char* path)
{
}

void saveDataItem(const char* path)
{
}
