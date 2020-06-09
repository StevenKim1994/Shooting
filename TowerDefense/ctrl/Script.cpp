#include "Script.h"
#include "ScriptData.h"

static HWND hWnd;
static HINSTANCE hInstance;

HWND hEbNom;
HWND hBtnNomAdd;
HWND hLbNom;

HWND hEbItem;
HWND hBtnItemAdd;
HWND hLbItem;


void loadScript(HWND hwnd, HINSTANCE hinstance)
{
	initWndCtrlSystem(hwnd, hinstance);

	DragAcceptFiles(hWnd, true);

	// to do....
	loadScriptData();
	hEbNom = createWndEditBox(5, 5, 100, 30, "name");
	hBtnNomAdd = createWndButton(110, 5, 100, 30, "ADD");
	const char* lineNom[1] = { "End of Nom" };
	hLbNom = createWndListBox(5, 40, 120, 100, lineNom, 1);

	hEbItem = createWndEditBox(5, 150, 100, 30, "name");
	hBtnItemAdd = createWndButton(110, 150, 100, 30, "ADD");
	const char* lineItem[1] = { "End of Item" };
	hEbItem = createWndListBox(5, 185, 120, 100, lineItem, 1);


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
	HWND hwnd = (HWND)lParam;
	if (hBtnNomAdd == hwnd)
	{
		char* str = getWndText(hEbNom);
		
		int index = indexWndListBox(hLbNom);
		int count = countWndListBox(hLbNom);

		if (index < count-1)
			removeWndListBox(hLbNom, index);

		addWndListBox(hLbNom, index, str);

		if (index < count - 1)
			setWndListBox(hLbNom, index);

		else
			setWndListBox(hLbNom, index + 1);
		
		free(inputNom->name);
		inputNom->name = str;

		if (index < count - 1)
			arrayNom->addObject(index, inputNom);

		else
			arrayNom->addObject(inputNom);

		inputNom = newNom();	
	}
	else if (hLbNom == hwnd)
	{
		int index = indexWndListBox(hLbNom);
		int count = countWndListBox(hLbNom);

		if (index < count-1)
		{
			char* str = getWndListBox(hLbNom, index);
			setWndText(hEbNom, str);
			free(str);
		}
	}

	else if (hBtnItemAdd == hwnd)
	{
		char* str = getWndText(hEbItem);

		int index = indexWndListBox(hLbItem);
		int count = countWndListBox(hLbItem);

		if (index < count - 1)
			removeWndListBox(hLbItem, index);

		addWndListBox(hLbItem, index, str);

		if (index < count - 1)
			setWndListBox(hLbItem, index);

		else
			setWndListBox(hLbItem, index + 1);

		free(inputItem->name);
		inputItem->name = str;

		if (index < count - 1)
			arrayItem->addObject(index, inputItem);

		else
			arrayItem->addObject(inputItem);

		inputItem = newItem();
	}

	else if (hLbItem == hwnd)
	{
		int index = indexWndListBox(hLbItem);
		int count = countWndListBox(hLbItem);

		if (index < count - 1)
		{
			char* str = getWndListBox(hLbItem, index);
			setWndText(hEbItem, str);
			free(str);
		}
	}
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

