#include "Script.h"
#include "ScriptData.h"
#include "DlgNom.h"


static HWND hWnd;
static HINSTANCE hInstance;

HWND hEbNom;
void ebNomUpdate(WPARAM wParam, LPARAM lParam);

HWND hBtnNomAdd;
void btnNomAddUpdate(WPARAM wParam, LPARAM lParam);
HWND hLbNom;
void lbNomUpdate(WPARAM wParam, LPARAM lParam);
HWND hEbItem;
void ebItemUpdate(WPARAM wParam, LPARAM lParam);
HWND hBtnItemAdd;
void btnItemAddUpdate(WPARAM wParam, LPARAM lParam);
HWND hLbItem;
void lbItemUpdate(WPARAM wParam, LPARAM lParam);

WndCtrlSystem* wcsScript;

void dragScript(const char* path)
{
	const char* ext = &path[strlen(path) - 4];

	if (strcmp(ext, SCRIPT_EXTENSION) == 0)
		loadDataScript(path);

	else if (strcmp(ext, PROJECT_EXTENSION) == 0)
		loadDataProject(path);
}


void loadScript(HWND hwnd, HINSTANCE hinstance)
{
	initWndCtrlSystem(); // 무조건해야댐 이미만들어진거 부를라면 무조건!!!

	// to do....

	loadScriptData();

	wcsScript = new WndCtrlSystem(hwnd, hinstance);
	wcsScript->dragAcceptFiles(dragScript);

	setWndCtrlSystem(wcsScript);
		hEbNom = createWndEditBox(5, 5, 100, 30, "name", NULL, ebNomUpdate);
	hBtnNomAdd = createWndButton(110, 5, 100, 30, "ADD", NULL, btnNomAddUpdate);
	const char* lineNom[1] = { "End of Nom" };
	hLbNom = createWndListBox(5, 40, 120, 100, lineNom, 1, NULL, lbNomUpdate);

	hEbItem = createWndEditBox(5, 150, 100, 30, "name" , NULL , ebItemUpdate);
	hBtnItemAdd = createWndButton(110, 150, 100, 30, "ADD", NULL, btnItemAddUpdate);
	const char* lineItem[1] = { "End of Item" };
	hLbItem = createWndListBox(5, 185, 120, 100, lineItem, 1, NULL, lbItemUpdate);
	

}

void freeScript()
{
	// to do.....
	freeScriptData();
	delete wcsScript;
}

void dragScript(WPARAM wParam, LPARAM lParam)
{
	wcsScript->dropFiles(wParam, lParam);
}



LRESULT colorScript(WPARAM wParam, LPARAM lParam)
{
	return wcsScript->color(wParam, lParam);
}


void updateScript(WPARAM wParam, LPARAM lParam)
{
	wcsScript->update(wParam, lParam);
	
	
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

void loadDataScript(const char* path)
{
}

void saveDataScript(const char* paht)
{
}

void loadDataProject(const char* path)
{
}

void saveDataProject(const char* path)
{
}

void ebNomUpdate(WPARAM wParam, LPARAM lParam)
{
}

void btnNomAddUpdate(WPARAM wParam, LPARAM lParam)
{
	char* str = getWndText(hEbNom);

	int index = indexWndListBox(hLbNom);
	int count = countWndListBox(hLbNom);

	if (index < count - 1)
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

void lbNomUpdate(WPARAM wParam, LPARAM lParam)
{
	int index = indexWndListBox(hLbNom);
	int count = countWndListBox(hLbNom);

	if (index < count - 1)
	{
		char* str = getWndListBox(hLbNom, index);
		setWndText(hEbNom, str);
		free(str);
	}
}

void ebItemUpdate(WPARAM wParam, LPARAM lParam)
{
}

void btnItemAddUpdate(WPARAM wParam, LPARAM lParam)
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

void lbItemUpdate(WPARAM wParam, LPARAM lParam)
{
	int index = indexWndListBox(hLbItem);
	int count = indexWndListBox(hLbItem);

	if (index < count - 1)
	{
		char* str = getWndListBox(hLbItem, index);
		setWndText(hLbItem, str);
		free(str);
	}
}
