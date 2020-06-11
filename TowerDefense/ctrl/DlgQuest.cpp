#include "DlgQuest.h"
#include "ScriptData.h"

HWND hDlgQuest;
WndCtrlSystem* wcsQuest;

// Window

HWND* hBtnQuestOpen;
void btnQuestOpenUpdate(WPARAM wParam, LPARAM lParam);

HWND hEbQuestName;
void ebQuestNameUpdate(WPARAM wParam, LPARAM lParam);

HWND hBtnQuestSubmit;
void btnQuestSubmitUpdate(WPARAM wParam, LPARAM lParam);

HWND hLbQuestList;
void lbQuestListUpdate(WPARAM wParam, LPARAM lParam);


// ~Window


void dragDlgQuest(const char* path)
{
	//chcek .quest 인지..

	if (strcmp(&path[strlen(path) - 4], QUEST_EXTENSION))
		return;

	loadDataQuest(path);
}


void loadDlgQuest()
{
	int i;
	hDlgQuest = createWndDlg(0, 0, 400, 400, "Quest", dlgQuestProc); // dlg create

	wcsQuest = new WndCtrlSystem(hDlgQuest);
	wcsQuest->dragAcceptFiles(dragDlgQuest);
	setWndCtrlSystem(wcsQuest);

	// add ctrl...

	const char* strBtn[2] = { "OPEN", "SAVE" }; // 프로젝트 여는 버튼
	hBtnQuestOpen = (HWND*)malloc(sizeof(HWND) * 2);

	for (int i = 0; i < 2; i++)
		hBtnQuestOpen[i] = createWndButton(5+65*i, 5, 60, 30, strBtn[i], NULL, btnQuestOpenUpdate);

	hEbQuestName = createWndEditBox(5, 40, 80, 30, "Name", NULL, ebQuestNameUpdate);

	hBtnQuestSubmit = createWndButton(90, 40, 60, 30, "Submit", NULL, btnQuestSubmitUpdate); 

	const char* strquestList[1] = { "End of Quest" };
	hLbQuestList = createWndListBox(5, 130, 110, 200, strquestList, 1, NULL, lbQuestListUpdate);

	// listBox 

	// openImgBtn;
	// Img
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
		return DefDlgProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void loadDataQuest(const char* path)
{
	int i;
	int num = countWndListBox(hLbQuestList) - 1;

	for (i = 0; i < num; i++)
		removeWndListBox(hLbQuestList, i);

	arrayQuest->removeAll();

	FILE* pf = fopen(path, "rb");

	//int i;
	fread(&num, sizeof(int), 1, pf);

	for (int i = 0; i < num; i++)
	{
		Quest* quest = (Quest*)malloc(sizeof(Quest));
		int len;

		fread(&len, sizeof(int), 1, pf);
		quest->title = (char*)calloc(sizeof(char), 1 + len);
		fread(quest->title, sizeof(char), len, pf);

		fread(&len, sizeof(int), 1, pf);
		quest->content = (char*)calloc(sizeof(char), 1 + len);
		fread(quest->content, sizeof(char), len, pf);

		addWndListBox(hLbQuestList, i, quest->title);
		arrayQuest->addObject(quest);
	}

	fclose(pf);
}

void saveDataQuest(const char* path)
{
	FILE* pf = fopen(path, "wb");
	int i;
	int num = arrayQuest->count;
	fwrite(&num, sizeof(int), 1, pf);

	for (i = 0; i < num; i++)
	{
		Quest* quest = (Quest*)arrayQuest->objectAtIndex(i);

		int len = strlen(quest->title);

		fwrite(&len, sizeof(int), 1, pf);
		fwrite(quest->title, sizeof(char), len, pf);
		len = strlen(quest->content);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(quest->content, sizeof(char), len, pf);

	}


	fclose(pf);
}

void btnQuestOpenUpdate(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = (HWND)lParam;

	int i;

	for (i = 0; i < 2; i++)
	{
		if (hwnd == hBtnQuestOpen[i])
		{
			break;
		}
	}

	if (i == 0) // OPEN
	{
		const char* path = openFileDlg(true, TEXT("Quest Files(*.qst)\0*.qst\0"));
		if (path)
			loadDataQuest(path);
	}
	else // SAVE
	{
		const char* path = openFileDlg(false, TEXT("Quest Files(*.qst)\0*.qst\0"));;
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
	//SetFocus(hDlgNom);
}

void ebQuestNameUpdate(WPARAM wParam, LPARAM lParam)
{
}

void btnQuestSubmitUpdate(WPARAM wParam, LPARAM lParam)
{
	// 에디트박스에 잇는 내용 리스트박스로 push!

	char* str = getWndText(hEbQuestName);

	int index = indexWndListBox(hLbQuestList);
	int count = countWndListBox(hLbQuestList);

	if (index < count - 1)
		removeWndListBox(hLbQuestList, index);

	addWndListBox(hLbQuestList, index, str);

	if (index < count - 1)
		setWndListBox(hLbQuestList, index);

	else
		setWndListBox(hLbQuestList, index + 1);

	free(inputQuest->title);
	inputQuest->title = str;

	if (index < count - 1)
		arrayQuest->addObject(index, inputQuest);

	else
		arrayQuest->addObject(inputQuest);

	inputQuest = newQuest();
}

void lbQuestListUpdate(WPARAM wParam, LPARAM lParam)
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

	int index = indexWndListBox(hLbQuestList);
	int count = countWndListBox(hLbQuestList);

	if (index < count - 1)
	{
		char* str = getWndListBox(hLbQuestList, index);
		setWndText(hLbQuestList, str);
		free(str);
	}
	else
		setWndText(hLbQuestList, "Name");
}
