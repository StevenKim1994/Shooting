#include "DlgNom.h"
#include "ScriptData.h"


HWND hDlgNom;
WndCtrlSystem* wcsNom;

void dragDlgNom(const char* path)
{
	//chcek .nom 인지..
	
	if (strcmp(&path[strlen(path) - 4], NOM_EXTENSION))
		return;

	loadDataNom(path);
}
void loadDlgNom(HWND hwnd, HINSTANCE hinstance)
{
	hDlgNom = NULL; // dlgNomProc

	wcsNom = new WndCtrlSystem(hwnd, hinstance);
	wcsNom->dragAcceptFiles(dragDlgNom);
	setWndCtrlSystem(wcsNom);

	// add ctrl...
}

void freeDlgNom()
{
	delete wcsNom;

	DestroyWindow(hDlgNom);
}

void dragDlgNom(WPARAM wParam, LPARAM lParam)
{
	
}

void showDlgNom(bool show)
{
	if (show == false)
		ShowWindow(hDlgNom, SW_HIDE);

	else
	{
		ShowWindow(hDlgNom, SW_RESTORE);
		SetFocus(hDlgNom);
	}
}

void updateDlgNom(WPARAM wParam, LPARAM lParam)
{
	wcsNom->update(wParam, lParam);
}

void drawDlgNom(float dt)
{
	
}



void keyDlgNom(iKeyState stat, iPoint point)
{

}

LRESULT dlgNomProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result;

	switch (msg)
	{

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
		result = wcsNom->color(wParam, lParam);
		if (result)
			return result;
		break;
	case WM_DROPFILES:

		wcsNom->dropFiles(wParam, lParam);
		break;

	case WM_COMMAND:
		wcsNom->update(wParam, lParam);
		break;

	case WM_CLOSE:
		showDlgNom(false);

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void loadDataNom(const char* path)
{



}

void saveDataNom(const char* path)
{
}
