#include "Script.h"
#include "ScriptData.h"
#include "DlgNom.h"
#include "DlgQuest.h"
#include "DlgItem.h"
static HWND hWnd;
static HINSTANCE hInstance;

HWND hEbItem;
void ebItemUpdate(WPARAM wParam, LPARAM lParam);
HWND hBtnItemAdd;
void btnItemAddUpdate(WPARAM wParam, LPARAM lParam);
HWND hLbItem;
void lbItemUpdate(WPARAM wParam, LPARAM lParam);

WndCtrlSystem* wcsScript;

HWND* hBtnOpenDlg;
void btnOpenDlgUpdate(WPARAM wParam, LPARAM lParam);

HWND hLbSayList;
void lbSayListUpdate(WPARAM wParam, LPARAM lParam);

HWND hCbSayNom;
void cbSayNomUpdate(WPARAM wParam, LPARAM lParam);

HWND hEbSayComment;
void ebSayComentUpdate(WPARAM wParam, LPARAM lParam);

HWND hCbSayRewardItem;
void cbSayRewardItemUpdate(WPARAM wParam, LPARAM lParam);

HWND hEbSayRewardGold;
void ebSayRewardGoldUpdate(WPARAM wParam, LPARAM lParam);

HWND hEbSayRewardExp;
void ebSayRewardExpUpdate(WPARAM wParam, LPARAM lParam);

HWND hCbSayQuest;
void cbSayQuestUpdate(WPARAM wPram, LPARAM lParam);

void dragScript(const char* path)
{
	const char* ext = &path[strlen(path) - 4];

	if (strcmp(ext, SCRIPT_EXTENSION) == 0)
		loadDataProject(path);

	else if (strcmp(ext, PROJECT_EXTENSION) == 0)
		loadDataProject(path);
}


void loadScript(HWND hwnd)
{
	int i;

	initWndCtrlSystem(); // 무조건해야댐 이미만들어진거 부를라면 무조건!!!

	// to do....
	loadScriptData();

	wcsScript = new WndCtrlSystem(hwnd);
	wcsScript->dragAcceptFiles(dragScript);
	setWndCtrlSystem(wcsScript);

	const char* strOpenDlg[3] = { "DlgNom", "DlgItem", "DlgQuest" };
	hBtnOpenDlg = (HWND*)malloc(sizeof(HWND) * 3);
	for (i = 0; i < 3; i++)
		hBtnOpenDlg[i] = createWndButton(5 + 65 * i, 5, 60, 30, strOpenDlg[i], NULL, btnOpenDlgUpdate);





	loadDlgNom();
	loadDlgItem();
	loadDlgQuest();



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
	FILE* pf = fopen(path, "wb");
	loadDataNom(pf);
	loadDataItem(pf);
	loadDataQuest(pf);
	loadDataSay(pf);

	fclose(pf);
}

void saveDataProject(const char* path)
{
	FILE* pf = fopen(path, "wb");

	saveDataNom(pf);
	saveDataItem(pf);
	saveDataQuest(pf);
	saveDataSay(pf);
	fclose(pf);
}

void loadDataSay(FILE* pf)
{
}

void loadDataSay(const char* path)
{
}

void saveDataSay(FILE* pf)
{
}

void saveDataSay(const char* path)
{
}

void ebNomUpdate(WPARAM wParam, LPARAM lParam)
{
}

void btnNomAddUpdate(WPARAM wParam, LPARAM lParam)
{
	
}

void lbNomUpdate(WPARAM wParam, LPARAM lParam)
{
	
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

void btnOpenDlgUpdate(WPARAM wParam, LPARAM lParam)
{
	void (*show[3])(bool) = { showDlgNom, showDlgItem, showDlgQuest };
	
	HWND hwnd = (HWND)lParam;
	int i;
	for (i = 0; i < 3; i++)
	{
		if (hBtnOpenDlg[i] == hwnd)
		{
			show[i](true);
			return;
		}
	}
	


}
