#include "DlgItem.h"
#include "ScriptData.h"

HWND hDlgItem;
WndCtrlSystem* wcsItem;

// Window

HWND* hBtnItemOpen;
void btnItemOpenUpdate(WPARAM wParam, LPARAM lParam);

HWND hEbItemName;
void ebItemNameUpdate(WPARAM wParam, LPARAM lParam);

HWND hBtnItemSubmit;
void btnItemSubmitUpdate(WPARAM wParam, LPARAM lParam);

HWND hLbItemList;
void lbItemListUpdate(WPARAM wParam, LPARAM lParam);


// ~Window

void dragDlgItem(const char* path)
{
	//chcek .nom 인지..

	if (strcmp(&path[strlen(path) - 4], ITEM_EXTENSION))
		return;

	loadDataItem(path);
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

void loadDlgItem()
{
	int i;

	hDlgItem= createWndDlg(0, 0, 400, 400, "Item", dlgItemProc); // dlg create

	wcsItem = new WndCtrlSystem(hDlgItem);
	wcsItem->dragAcceptFiles(dragDlgItem);
	setWndCtrlSystem(wcsItem);

	// add ctrl...

	const char* strBtn[2] = { "OPEN", "SAVE" }; // 프로젝트 여는 버튼
	hBtnItemOpen = (HWND*)malloc(sizeof(HWND) * 2);

	for (int i = 0; i < 2; i++)
		hBtnItemOpen[i] = createWndButton(5 + 65 * i, 5, 60, 30, strBtn[i], NULL, btnItemOpenUpdate);

	hEbItemName = createWndEditBox(5, 40, 80, 30, "Name", NULL, ebItemNameUpdate);

	hBtnItemSubmit = createWndButton(90, 40, 60, 30, "Submit", NULL, btnItemSubmitUpdate);

	const char* strItemList[1] = { "End of Item" };
	hLbItemList = createWndListBox(5, 130, 110, 200, strItemList, 1, NULL, lbItemListUpdate);

	// listBox 

	// openImgBtn;
	// Img
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
		return DefDlgProc(hwnd, msg, wParam, lParam);
		
	}

	return 0;
}

void loadDataItem(const char* path)
{
	int i;
	int num = countWndListBox(hLbItemList) - 1;

	for (i = 0; i < num; i++)
		removeWndListBox(hLbItemList, i);

	arrayItem->removeAll();

	FILE* pf = fopen(path, "rb");

	//int i;
	fread(&num, sizeof(int), 1, pf);

	for (int i = 0; i < num; i++)
	{
		Item* item = (Item*)malloc(sizeof(Item));
		int len;

		fread(&len, sizeof(int), 1, pf);
		item->name = (char*)calloc(sizeof(char), 1 + len);
		fread(item->name, sizeof(char), len, pf);

		fread(&len, sizeof(int), 1, pf);
		item->path = (char*)calloc(sizeof(char), 1 + len);
		fread(item->path, sizeof(char), len, pf);

		addWndListBox(hLbItemList, i,item->name);
		arrayItem->addObject(item);
	}

	fclose(pf);
}

void saveDataItem(const char* path)
{
	FILE* pf = fopen(path, "wb");
	int i;
	int num = arrayItem->count;
	fwrite(&num, sizeof(int), 1, pf);

	for (i = 0; i < num; i++)
	{
		Item* item = (Item*)arrayItem->objectAtIndex(i);

		int len = strlen(item->name);

		fwrite(&len, sizeof(int), 1, pf);
		fwrite(item->name, sizeof(char), len, pf);
		len = strlen(item->path);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(item->path, sizeof(char), len, pf);

	}


	fclose(pf);
}

void loadDataItem(FILE* pf)
{
}

void saveDataItem(FILE* pf)
{
}

void btnItemOpenUpdate(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = (HWND)lParam;

	int i;

	for (i = 0; i < 2; i++)
	{
		if (hwnd == hBtnItemOpen[i])
		{
			break;
		}
	}

	if (i == 0) // OPEN
	{
		const char* path = openFileDlg(true, TEXT("Item Files(*.itm)\0*.itm\0"));
		if (path)
			loadDataItem(path);
	}
	else // SAVE
	{
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
	//SetFocus(hDlgNom);
}

void ebItemNameUpdate(WPARAM wParam, LPARAM lParam)
{
}

void btnItemSubmitUpdate(WPARAM wParam, LPARAM lParam)
{
	// 에디트박스에 잇는 내용 리스트박스로 push!

	char* str = getWndText(hEbItemName);

	int index = indexWndListBox(hLbItemList);
	int count = countWndListBox(hLbItemList);

	if (index < count - 1)
		removeWndListBox(hLbItemList, index);

	addWndListBox(hLbItemList, index, str);

	if (index < count - 1)
		setWndListBox(hLbItemList, index);

	else
		setWndListBox(hLbItemList, index + 1);

	free(inputItem->name);
	inputItem->name = str;

	if (index < count - 1)
		arrayItem->addObject(index, inputItem);

	else
		arrayItem->addObject(inputItem);

	inputItem = newItem();
}

void lbItemListUpdate(WPARAM wParam, LPARAM lParam)
{
	int event = HIWORD(wParam);

	switch (event)
	{
	case LBN_DBLCLK:
	case LBN_SELCHANGE:
	case LBN_SELCANCEL:
	case LBN_SETFOCUS:
	case LBN_KILLFOCUS:
		break;

	default:
		break;
	}

	int index = indexWndListBox(hLbItemList);
	int count = countWndListBox(hLbItemList);

	if (index < count - 1)
	{
		char* str = getWndListBox(hLbItemList, index);
		setWndText(hLbItemList, str);
		free(str);
	}
	else
		setWndText(hLbItemList, "Name");
}
