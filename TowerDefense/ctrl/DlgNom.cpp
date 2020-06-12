#include "DlgNom.h"
#include "ScriptData.h"

HWND hDlgNom;
WndCtrlSystem* wcsNom;

HWND* hBtnNomOpen;
void btnNomOpenUpdate(WPARAM wParam, LPARAM lParam);

HWND hEbNomName;
void ebNomNameUpdate(WPARAM wParam, LPARAM lParam);

HWND hBtnNomSubmit;
void btnNomSubmitUpdate(WPARAM wParam, LPARAM lParam);

HWND hLbNomList;
void lbNomListUpdate(WPARAM wParam, LPARAM lParam);

extern HWND hCbSayNom;// Script.cpp

void dragDlgNom(const char* path)
{
	if (strcmp(&path[strlen(path) - 4], NOM_EXTENSION))
		return;

	loadDataNom(path);
}

void loadDlgNom()
{
	int i;

	hDlgNom = createWndDlg(0, 0, 400, 400, "Nom", dlgNomProc);

	wcsNom = new WndCtrlSystem(hDlgNom);
	wcsNom->dragAcceptFiles(dragDlgNom);
	setWndCtrlSystem(wcsNom);

	// add ctrl.....
	const char* strBtn[2] = { "Open", "Save" };
	hBtnNomOpen = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
		hBtnNomOpen[i] = createWndButton(5 + 65 * i, 5, 60, 30, strBtn[i], NULL, btnNomOpenUpdate);

	// name + submit
	hEbNomName = createWndEditBox(5, 40, 80, 30, "Name", NULL, ebNomNameUpdate);
	hBtnNomSubmit = createWndButton(90, 40, 60, 30, "Submit", NULL, btnNomSubmitUpdate);

	// listBox 
	const char* strNomList[1] = { "End of Nom" };
	hLbNomList = createWndListBox(5, 130, 110, 200, strNomList, 1, NULL, lbNomListUpdate);

	// open Image
	// photo
}

void freeDlgNom()
{
	delete wcsNom;

	DestroyWindow(hDlgNom);
	free(hBtnNomOpen);
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

void drawDlgNom(float dt)
{

}

void keyDlgNom(iKeyState stat, iPoint point)
{

}

LRESULT CALLBACK dlgNomProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result;

	switch (msg) {

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORDLG:
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
		//DestroyWindow(hDlgNom);
		// unshow
		showDlgNom(false);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefDlgProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void loadDataNom(const char* path)
{
	//printf("path = %s\n", path);
	FILE* pf = fopen(path, "rb");
	loadDataNom(pf);
	fclose(pf);
}
void loadDataNom(FILE* pf)
{
	int i, num;
	num = countWndListBox(hLbNomList) - 1;
	for (i = 0; i < num; i++)
	{
		removeWndListBox(hLbNomList, 0);
		removeWndComboBox(hCbSayNom, 0);
	}

	arrayNom->removeAll();

	fread(&num, sizeof(int), 1, pf);
	for (i = 0; i < num; i++)
	{
		Nom* nom = (Nom*)malloc(sizeof(Nom));

		int len;
		fread(&len, sizeof(int), 1, pf);
		nom->name = (char*)calloc(sizeof(char), 1 + len);
		fread(nom->name, sizeof(char), len, pf);

		fread(&len, sizeof(int), 1, pf);
		nom->path = (char*)calloc(sizeof(char), 1 + len);
		fread(nom->path, sizeof(char), len, pf);

		addWndListBox(hLbNomList, i, nom->name);
		addWndComboBox(hCbSayNom, i, nom->name);
		arrayNom->addObject(nom);
	}
}

void saveDataNom(const char* path)
{
	//printf("path = %s\n", path);
	FILE* pf = fopen(path, "wb");
	saveDataNom(pf);
	fclose(pf);
}
void saveDataNom(FILE* pf)
{
	int i, num = arrayNom->count;
	fwrite(&num, sizeof(int), 1, pf);

	for (i = 0; i < num; i++)
	{
		Nom* nom = (Nom*)arrayNom->objectAtIndex(i);
		int len = strlen(nom->name);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(nom->name, sizeof(char), len, pf);
		len = strlen(nom->path);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(nom->path, sizeof(char), len, pf);
	}
}

void btnNomOpenUpdate(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = (HWND)lParam;

	int i;
	for (i = 0; i < 2; i++)
	{
		if (hBtnNomOpen[i] == hwnd)
			break;
	}

	if (i == 0)
	{
		// open
		const char* path = openFileDlg(true, TEXT("Nom Files(*.nom)\0*.nom\0"));
		if (path)
			loadDataNom(path);
	}
	else
	{
		// save
		const char* path = openFileDlg(false, TEXT("Nom Files(*.nom)\0*.nom\0"));
		if (path)
		{
			char _path[256];
			if (strcmp(&path[strlen(path) - 4], ".nom") == 0)
				strcpy(_path, path);
			else
				sprintf(_path, "%s.nom", path);
			saveDataNom(_path);
		}
	}
	//SetFocus(hDlgNom);
	//SetWindowPos(hDlg, HWND_TOPMOST, x, y, width, height, SWP_HIDEWINDOW);
}

void ebNomNameUpdate(WPARAM wParam, LPARAM lParam)
{
	// 금지어, 숫자 차단

	char* s = getWndText(hEbNomName);
	printf("%s\n", s);
	free(s);
}

void btnNomSubmitUpdate(WPARAM wParam, LPARAM lParam)
{
	char* str = getWndText(hEbNomName);

	int index = indexWndListBox(hLbNomList);
	int count = countWndListBox(hLbNomList);
	if (index < count - 1)
	{
		removeWndListBox(hLbNomList, index);
		addWndListBox(hLbNomList, index, str);
		setWndListBox(hLbNomList, index);

		removeWndComboBox(hCbSayNom, index);
		addWndComboBox(hCbSayNom, index, str);
		setWndComboBox(hCbSayNom, index);

		Nom* nom = (Nom*)arrayNom->objectAtIndex(index);
		//free(str);
		free(nom->name);
		nom->name = str;
	}
	else
	{
		addWndListBox(hLbNomList, index, str);
		setWndListBox(hLbNomList, index + 1);

		addWndComboBox(hCbSayNom, index, str);
		setWndComboBox(hCbSayNom, index + 1);

		//free(str);
		free(inputNom->name);
		inputNom->name = str;
		arrayNom->addObject(inputNom);
		inputNom = newNom();
	}
}

void lbNomListUpdate(WPARAM wParam, LPARAM lParam)
{
	int event = HIWORD(wParam);
	switch (event) {
	case LBN_SELCHANGE:
	case LBN_DBLCLK:
	case LBN_SELCANCEL:
	case LBN_SETFOCUS:
	case LBN_KILLFOCUS:
		break;
	}

	int index = indexWndListBox(hLbNomList);
	int count = countWndListBox(hLbNomList);
	if (index < count - 1)
	{
		char* str = getWndListBox(hLbNomList, index);
		setWndText(hEbNomName, str);
		free(str);
	}
	else
		setWndText(hEbNomName, "Name");
}
