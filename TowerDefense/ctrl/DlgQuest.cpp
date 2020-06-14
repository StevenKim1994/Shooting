#include "DlgQuest.h"
#include "ScriptData.h"

HWND hDlgQuest;
WndCtrlSystem* wcsQuest;

HWND* hBtnQuestOpen;
void btnQuestOpenUpdate(WPARAM wParam, LPARAM lParam);

HWND hEbQuestName;
void ebQuestNameUpdate(WPARAM wParam, LPARAM lParam);

HWND hEbQuestContent;
void ebQuestContentUpdate(WPARAM wParam, LPARAM lParam);

HWND hBtnQuestSubmit;
void btnQuestSubmitUpdate(WPARAM wParam, LPARAM lParam);

HWND hLbQuestList;
void lbQuestListUpdate(WPARAM wParam, LPARAM lParam);

extern HWND hCbSayQuest;// Script.cpp

void dragDlgQuest(const char* path)
{
	if (strcmp(&path[strlen(path) - 4], QUEST_EXTENSION))
		return;

	loadDataQuest(path);
}

void loadDlgQuest()
{
	int i;

	hDlgQuest = createWndDlg(0, 0, 400, 400, "Quest", dlgQuestProc);

	wcsQuest = new WndCtrlSystem(hDlgQuest);
	wcsQuest->dragAcceptFiles(dragDlgQuest);
	setWndCtrlSystem(wcsQuest);

	// add ctrl.....
	const char* strBtn[2] = { "Open", "Save" };
	hBtnQuestOpen = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
		hBtnQuestOpen[i] = createWndButton(5 + 65 * i, 5, 60, 30, strBtn[i], NULL, btnQuestOpenUpdate);

	// name + submit
	hEbQuestName = createWndEditBox(5, 40, 80, 30, "Title", NULL, ebQuestNameUpdate);
	hEbQuestContent = createWndEditBox(5, 75, 80, 30, "Content", NULL, ebQuestContentUpdate);
	hBtnQuestSubmit = createWndButton(90, 40, 60, 30, "Submit", NULL, btnQuestSubmitUpdate);

	// listBox 
	const char* strQuestList[1] = { "End of Quest" };
	hLbQuestList = createWndListBox(5, 130, 110, 200, strQuestList, 1, NULL, lbQuestListUpdate);

	// open Image
	// photo
}

void freeDlgQuest()
{
	delete wcsQuest;

	DestroyWindow(hDlgQuest);
	free(hBtnQuestOpen);
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

void drawDlgQuest(float dt)
{

}

void keyDlgQuest(iKeyState stat, iPoint point)
{

}

LRESULT CALLBACK dlgQuestProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result;

	switch (msg) {

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORDLG:
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
		//DestroyWindow(hDlgQuest);
		// unshow
		showDlgQuest(false);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefDlgProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void loadDataQuest(const char* path)
{
	//printf("path = %s\n", path);
	FILE* pf = fopen(path, "rb");
	loadDataQuest(pf);
	fclose(pf);
}
void loadDataQuest(FILE* pf)
{
	int i, num;
	num = countWndListBox(hLbQuestList) - 1;
	for (i = 0; i < num; i++)
	{
		removeWndListBox(hLbQuestList, 0);
		removeWndComboBox(hCbSayQuest, 0);
	}

	arrayQuest->removeAll();

	fread(&num, sizeof(int), 1, pf);
	for (i = 0; i < num; i++)
	{
		Quest* qst = (Quest*)malloc(sizeof(Quest));

		int len;
		fread(&len, sizeof(int), 1, pf);
		qst->title = (char*)calloc(sizeof(char), 1 + len);
		fread(qst->title, sizeof(char), len, pf);

		fread(&len, sizeof(int), 1, pf);
		qst->content = (char*)calloc(sizeof(char), 1 + len);
		fread(qst->content, sizeof(char), len, pf);

		addWndListBox(hLbQuestList, i, qst->title);
		addWndComboBox(hCbSayQuest, i, qst->title);
		arrayQuest->addObject(qst);
	}
}

void saveDataQuest(const char* path)
{
	//printf("path = %s\n", path);
	FILE* pf = fopen(path, "wb");
	saveDataQuest(pf);
	fclose(pf);
}
void saveDataQuest(FILE* pf)
{
	int i, num = arrayQuest->count;
	fwrite(&num, sizeof(int), 1, pf);

	for (i = 0; i < num; i++)
	{
		Quest* qst = (Quest*)arrayQuest->objectAtIndex(i);
		int len = strlen(qst->title);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(qst->title, sizeof(char), len, pf);
		len = strlen(qst->content);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(qst->content, sizeof(char), len, pf);
	}
}

void btnQuestOpenUpdate(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = (HWND)lParam;

	int i;
	for (i = 0; i < 2; i++)
	{
		if (hBtnQuestOpen[i] == hwnd)
			break;
	}

	if (i == 0)
	{
		// open
		const char* path = openFileDlg(true, TEXT("Quest Files(*.qst)\0*.qst\0"));
		if (path)
			loadDataQuest(path);
	}
	else
	{
		// save
		const char* path = openFileDlg(false, TEXT("Quest Files(*.qst)\0*.qst\0"));
		if (path)
		{
			char _path[256];
			if (strcmp(&path[strlen(path) - 4], ".qst") == 0)
				strcpy(_path, path);
			else
				sprintf(_path, "%s.qst", path);
			saveDataQuest(_path);
		}
	}
	//SetFocus(hDlgQuest);
	//SetWindowPos(hDlgQuest, HWND_TOPMOST, x, y, width, height, SWP_HIDEWINDOW);
}

void ebQuestNameUpdate(WPARAM wParam, LPARAM lParam)
{
	// 금지어, 숫자 차단

	char* s = getWndText(hEbQuestName);
	printf("%s\n", s);
	free(s);
}

void ebQuestContentUpdate(WPARAM wParam, LPARAM lParam)
{
	//////////////////
}

void setStringLimit(const char* str, int num, char* output)
{
	int i, len = strlen(str), n;
#if 0
	if (num > len)
		num = len;
#else
	int _num = 0;
	for (i = 0; i < len; )
	{
		if (iString::isUnicode(&str[i]))
			i += 3;
		else
			i++;
		_num++;
	}
	if (num > _num)
		num = _num;
#endif
	for (i = 0, n = 0; i < len; )
	{
		if (iString::isUnicode(&str[i]))
			i += 3;
		else
			i++;

		n++;
		if (n == num)
		{
			memcpy(output, str, i);
			output[i] = 0;
			return;
		}
	}
}

void btnQuestSubmitUpdate(WPARAM wParam, LPARAM lParam)
{
	char* strTitle = getWndText(hEbQuestName);
	char* strContent = getWndText(hEbQuestContent);
	if (strcmp(strTitle, "Title") == 0 ||
		strcmp(strContent, "Content") == 0)
	{
		free(strTitle);
		free(strContent);
		return;
	}

	char strComment[64];
	setStringLimit(strTitle, 3, strComment);
	strcat(strComment, " : ");
	int len = strlen(strComment);
	setStringLimit(strContent, 3, &strComment[len]);

	int index = indexWndListBox(hLbQuestList);
	int count = countWndListBox(hLbQuestList);
	if (index < count - 1)
	{
		removeWndListBox(hLbQuestList, index);
		addWndListBox(hLbQuestList, index, strComment);
		setWndListBox(hLbQuestList, index);

		removeWndComboBox(hCbSayQuest, index);
		addWndComboBox(hCbSayQuest, index, strComment);
		setWndComboBox(hCbSayQuest, index);

		Quest* qst = (Quest*)arrayQuest->objectAtIndex(index);
		//free(strTitle);
		free(qst->title);
		qst->title = strTitle;
		//free(strContent);
		free(qst->content);
		qst->content = strContent;
	}
	else
	{
		addWndListBox(hLbQuestList, index, strComment);
		setWndListBox(hLbQuestList, index + 1);

		addWndComboBox(hCbSayQuest, index, strComment);
		setWndComboBox(hCbSayQuest, index + 1);

		//free(strTitle);
		free(inputQuest->title);
		inputQuest->title = strTitle;
		//free(strContent);
		free(inputQuest->content);
		inputQuest->content = strContent;

		arrayQuest->addObject(inputQuest);
		inputQuest = newQuest();
	}
}

void lbQuestListUpdate(WPARAM wParam, LPARAM lParam)
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

	int index = indexWndListBox(hLbQuestList);
	int count = countWndListBox(hLbQuestList);
	if (index < count - 1)
	{
		//char* str = getWndListBox(hLbQuestList, index);
		//setWndText(hEbQuestName, str);
		//free(str);
		Quest* qst = (Quest*)arrayQuest->objectAtIndex(index);
		setWndText(hEbQuestName, qst->title);
		setWndText(hEbQuestContent, qst->content);
	}
	else
	{
		setWndText(hEbQuestName, "Title");
		setWndText(hEbQuestContent, "Content");
	}
}

