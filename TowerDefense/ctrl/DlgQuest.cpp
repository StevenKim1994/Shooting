#include "DlgQuest.h"
#include "ScriptData.h"

HWND hDlgQuest;
WndCtrlSystem* wcsQuest;

void dragDlgQuest(const char* path)
{
	//chcek .nom 인지..

	if (strcmp(&path[strlen(path) - 4], QUEST_EXTENSION))
		return;

	loadDataQuest(path);
}


void loadDlgQuest(HWND hwnd, HINSTANCE hinstance)
{
	hDlgQuest = NULL; // dlgNomProc

	wcsQuest = new WndCtrlSystem(hwnd, hinstance);
	wcsQuest->dragAcceptFiles(dragDlgQuest);
	setWndCtrlSystem(wcsQuest);

	// add ctrl...
}

void freeDlgQuest()
{
	delete wcsQuest;

	DestroyWindow(hDlgQuest);
}

void showDlgQuest(bool show)
{
	if (show == false)
		ShowWindow(hDlgQuest, SW_HIDE);

	else
	{
		ShowWindow(hDlgQuest, SW_RESTORE);
		SetFocus(hDlgQuest);
	}
}

void dragDlgQuest(WPARAM wParam, LPARAM lParam)
{
	
}

void updateDlgQuest(WPARAM wParam, LPARAM lParam)
{
	wcsQuest->update(wParam, lParam);
}

void drawDlgQuest(float dt)
{

}

LRESULT coloDlgQuest(WPARAM wParam, LPARAM lParam)
{
	return wcsQuest->color(wParam, lParam);
}

void keyDlgQuest(iKeyState stat, iPoint point)
{
}

LRESULT dlgQuestProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result;
	switch (msg)
	{

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
		result = wcsQuest->color(wParam, lParam);
		if (result)
			return result;
		break;
	case WM_DROPFILES:

		wcsQuest->dropFiles(wParam, lParam);
		break;

	case WM_COMMAND:
		wcsQuest->update(wParam, lParam);
		break;

	case WM_CLOSE:
		showDlgQuest(false);

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void loadDataQuest(const char* path)
{
}

void saveDataQuest(const char* path)
{
}
