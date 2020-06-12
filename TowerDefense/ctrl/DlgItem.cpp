#include "DlgItem.h"
#include "ScriptData.h"

HWND hDlgItem;
WndCtrlSystem* wcsItem;

HWND* hBtnItemOpen;
void btnItemOpenUpdate(WPARAM wParam, LPARAM lParam);

HWND hEbItemName;
void ebItemNameUpdate(WPARAM wParam, LPARAM lParam);

HWND hBtnItemSubmit;
void btnItemSubmitUpdate(WPARAM wParam, LPARAM lParam);

HWND hLbItemList;
void lbItemListUpdate(WPARAM wParam, LPARAM lParam);

extern HWND hCbSayRewardItem;// Script.cpp

void dragDlgItem(const char* path)
{
	if (strcmp(&path[strlen(path) - 4], ITEM_EXTENSION))
		return;

	loadDataItem(path);
}

void loadDlgItem()
{
	int i;

	hDlgItem = createWndDlg(0, 0, 400, 400, "Item", dlgItemProc);

	wcsItem = new WndCtrlSystem(hDlgItem);
	wcsItem->dragAcceptFiles(dragDlgItem);
	setWndCtrlSystem(wcsItem);

	// add ctrl.....
	const char* strBtn[2] = { "Open", "Save" };
	hBtnItemOpen = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
		hBtnItemOpen[i] = createWndButton(5 + 65 * i, 5, 60, 30, strBtn[i], NULL, btnItemOpenUpdate);

	// name + submit
	hEbItemName = createWndEditBox(5, 40, 80, 30, "Name", NULL, ebItemNameUpdate);
	hBtnItemSubmit = createWndButton(90, 40, 60, 30, "Submit", NULL, btnItemSubmitUpdate);

	// listBox 
	const char* strItemList[1] = { "End of Item" };
	hLbItemList = createWndListBox(5, 130, 110, 200, strItemList, 1, NULL, lbItemListUpdate);

	// open Image
	// photo
}

void freeDlgItem()
{
	delete wcsItem;

	DestroyWindow(hDlgItem);
	free(hBtnItemOpen);
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
	wcsItem->dropFiles(wParam, lParam);
}

LRESULT colorDlgItem(WPARAM wParam, LPARAM lParam)
{
	return wcsItem->color(wParam, lParam);
}

void updateDlgItem(WPARAM wParam, LPARAM lParam)
{
	wcsItem->update(wParam, lParam);
}

void drawDlgItem(float dt)
{

}

void keyDlgItem(iKeyState stat, iPoint point)
{

}

LRESULT CALLBACK dlgItemProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result;

	switch (msg) {

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORDLG:
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
		//DestroyWindow(hDlgItem);
		// unshow
		showDlgItem(false);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefDlgProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void loadDataItem(const char* path)
{
	//printf("path = %s\n", path);
	FILE* pf = fopen(path, "rb");
	loadDataItem(pf);
	fclose(pf);
}
void loadDataItem(FILE* pf)
{
	int i, num;
	num = countWndListBox(hLbItemList) - 1;
	for (i = 0; i < num; i++)
	{
		removeWndListBox(hLbItemList, 0);
		removeWndComboBox(hCbSayRewardItem, 0);
	}

	arrayItem->removeAll();

	fread(&num, sizeof(int), 1, pf);
	for (i = 0; i < num; i++)
	{
		Item* itm = (Item*)malloc(sizeof(Item));

		int len;
		fread(&len, sizeof(int), 1, pf);
		itm->name = (char*)calloc(sizeof(char), 1 + len);
		fread(itm->name, sizeof(char), len, pf);

		fread(&len, sizeof(int), 1, pf);
		itm->path = (char*)calloc(sizeof(char), 1 + len);
		fread(itm->path, sizeof(char), len, pf);

		addWndListBox(hLbItemList, i, itm->name);
		addWndComboBox(hCbSayRewardItem, i, itm->name);
		arrayItem->addObject(itm);
	}
}

void saveDataItem(const char* path)
{
	//printf("path = %s\n", path);
	FILE* pf = fopen(path, "wb");
	saveDataItem(pf);
	fclose(pf);
}
void saveDataItem(FILE* pf)
{
	int i, num = arrayItem->count;
	fwrite(&num, sizeof(int), 1, pf);

	for (i = 0; i < num; i++)
	{
		Item* itm = (Item*)arrayItem->objectAtIndex(i);
		int len = strlen(itm->name);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(itm->name, sizeof(char), len, pf);
		len = strlen(itm->path);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(itm->path, sizeof(char), len, pf);
	}
}

void btnItemOpenUpdate(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = (HWND)lParam;

	int i;
	for (i = 0; i < 2; i++)
	{
		if (hBtnItemOpen[i] == hwnd)
			break;
	}

	if (i == 0)
	{
		// open
		const char* path = openFileDlg(true, TEXT("Item Files(*.itm)\0*.itm\0"));
		if (path)
			loadDataItem(path);
	}
	else
	{
		// save
		const char* path = openFileDlg(false, TEXT("Item Files(*.itm)\0*.itm\0"));
		if (path)
		{
			char _path[256];
			if (strcmp(&path[strlen(path) - 4], ".itm") == 0)
				strcpy(_path, path);
			else
				sprintf(_path, "%s.itm", path);
			saveDataItem(_path);
		}
	}
	//SetFocus(hDlgItem);
	//SetWindowPos(hDlgItem, HWND_TOPMOST, x, y, width, height, SWP_HIDEWINDOW);
}

void ebItemNameUpdate(WPARAM wParam, LPARAM lParam)
{
	// 금지어, 숫자 차단

	char* s = getWndText(hEbItemName);
	printf("%s\n", s);
	free(s);
}

void btnItemSubmitUpdate(WPARAM wParam, LPARAM lParam)
{
	char* str = getWndText(hEbItemName);

	int index = indexWndListBox(hLbItemList);
	int count = countWndListBox(hLbItemList);
	if (index < count - 1)
	{
		removeWndListBox(hLbItemList, index);
		addWndListBox(hLbItemList, index, str);
		setWndListBox(hLbItemList, index);

		removeWndComboBox(hCbSayRewardItem, index);
		addWndComboBox(hCbSayRewardItem, index, str);
		setWndComboBox(hCbSayRewardItem, index);

		Item* itm = (Item*)arrayItem->objectAtIndex(index);
		//free(str);
		free(itm->name);
		itm->name = str;
	}
	else
	{
		addWndListBox(hLbItemList, index, str);
		setWndListBox(hLbItemList, index + 1);

		addWndComboBox(hCbSayRewardItem, index, str);
		setWndComboBox(hCbSayRewardItem, index + 1);

		//free(str);
		free(inputItem->name);
		inputItem->name = str;
		arrayItem->addObject(inputItem);
		inputItem = newItem();
	}
}

void lbItemListUpdate(WPARAM wParam, LPARAM lParam)
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

	int index = indexWndListBox(hLbItemList);
	int count = countWndListBox(hLbItemList);
	if (index < count - 1)
	{
		char* str = getWndListBox(hLbItemList, index);
		setWndText(hEbItemName, str);
		free(str);
	}
	else
		setWndText(hEbItemName, "Name");
}
