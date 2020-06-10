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

LRESULT coloDlgNom(WPARAM wParam, LPARAM lParam)
{
	return wcsNom->color(wParam, lParam);
}

void keyDlgNom(iKeyState stat, iPoint point)
{

}

LRESULT dlgNomProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		ShowWindow(hDlgNom, SW_HIDE);
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
