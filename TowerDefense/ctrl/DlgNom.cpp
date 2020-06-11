#include "DlgNom.h"
#include "ScriptData.h"


HWND hDlgNom;
WndCtrlSystem* wcsNom;

// Window

HWND* hBtnNomOpen;
void btnNomOpenUpdate(WPARAM wParam, LPARAM lParam);

HWND hEbNomName;
void ebNomNameUpdate(WPARAM wParam, LPARAM lParam);

HWND hBtnNomSubmit;
void btnNomSubmitUpdate(WPARAM wParam, LPARAM lParam);

HWND hLbNomList;
void lbNomListUpdate(WPARAM wParam, LPARAM lParam);


// ~Window


void dragDlgNom(const char* path)
{	
	
	if (strcmp(&path[strlen(path) - 4], NOM_EXTENSION))
		return;

	loadDataNom(path);
}
void loadDlgNom()
{
	int i;

	hDlgNom = createWndDlg(0,0,400,400, "Nom", dlgNomProc); // dlg create

	wcsNom = new WndCtrlSystem(hDlgNom);
	wcsNom->dragAcceptFiles(dragDlgNom);
	setWndCtrlSystem(wcsNom);

	// add ctrl...

	const char* strBtn[2] = { "OPEN", "SAVE" }; // 프로젝트 여는 버튼
	hBtnNomOpen = (HWND*)malloc(sizeof(HWND) * 2);

	for (int i = 0; i < 2; i++)
		hBtnNomOpen[i] = createWndButton(5+65*i, 5, 60, 30, strBtn[i], NULL, btnNomOpenUpdate);

	hEbNomName = createWndEditBox(5, 40, 80, 30, "Name", NULL, ebNomNameUpdate);

	hBtnNomSubmit = createWndButton(90, 40, 60, 30, "Submit", NULL, btnNomSubmitUpdate); 

	const char* strNomList[1] = { "End of Nom" };
	hLbNomList = createWndListBox(5, 130, 110, 200, strNomList, 1, NULL, lbNomListUpdate);

	// listBox 

	// openImgBtn;
	// Img

}

void freeDlgNom()
{
	delete wcsNom;

	DestroyWindow(hDlgNom);

	free(hBtnNomOpen);
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
		return DefDlgProc(hwnd, msg, wParam, lParam);
	}
}

void loadDataNom(const char* path)
{
	int i;
	int num = countWndListBox(hLbNomList) - 1;

	for (i = 0; i < num; i++)
		removeWndListBox(hLbNomList, i);

	arrayNom->removeAll();

	FILE* pf = fopen(path, "rb");

	//int i;
	fread(&num, sizeof(int), 1, pf);

	for (int i = 0; i < num; i++)
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
		arrayNom->addObject(nom);
	}

	fclose(pf);
}

void saveDataNom(const char* path)
{
	FILE* pf = fopen(path, "wb");
	int i;
	int num = arrayNom->count;
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


	fclose(pf);
}

void btnNomOpenUpdate(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = (HWND)lParam;

	int i;

	for (i = 0; i < 2; i++)
	{
		if (hwnd == hBtnNomOpen[i])
		{
			break;
		}
	}

	if (i == 0) // OPEN
	{
		const char* path = openFileDlg(true, TEXT("Nom Files(*.nom)\0*.nom\0"));
		if (path)
			loadDataNom(path);
	}
	else // SAVE
	{
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
}

void ebNomNameUpdate(WPARAM wParam, LPARAM lParam)
{
	// 금지어, 숫자 차단
}

void btnNomSubmitUpdate(WPARAM wParam, LPARAM lParam)
{
	// 에디트박스에 잇는 내용 리스트박스로 push!

	char* str = getWndText(hEbNomName);

	int index = indexWndListBox(hLbNomList);
	int count = countWndListBox(hLbNomList);

	if (index < count - 1)
		removeWndListBox(hLbNomList, index);

	addWndListBox(hLbNomList, index, str);

	if (index < count - 1)
		setWndListBox(hLbNomList, index);

	else
		setWndListBox(hLbNomList, index + 1);

	free(inputNom->name);
	inputNom->name = str;

	if (index < count - 1)
		arrayNom->addObject(index, inputNom);

	else
		arrayNom->addObject(inputNom);

	inputNom = newNom();
}

void lbNomListUpdate(WPARAM wParam, LPARAM lParam)
{
	int event = HIWORD(wParam); 
	
	switch (event)
	{
	case LBN_DBLCLK		 :
	case LBN_SELCHANGE   :
	case LBN_SELCANCEL   : 
	case LBN_SETFOCUS    :
	case LBN_KILLFOCUS   :
		break;
	
	default:
		break;
	}

	int index = indexWndListBox(hLbNomList);
	int count = countWndListBox(hLbNomList);

	if (index < count - 1)
	{
		char* str = getWndListBox(hLbNomList, index);
		setWndText(hLbNomList, str);
		free(str);
	}
	else
		setWndText(hLbNomList, "Name");

}
