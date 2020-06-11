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

HWND hEbQuestContent;
void ebQuestContentUpdate(WPARAM wPram, LPARAM lParam);

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
	hDlgQuest = createWndDlg(0, 0, 800, 400, "Quest", dlgQuestProc); // dlg create

	wcsQuest = new WndCtrlSystem(hDlgQuest);
	wcsQuest->dragAcceptFiles(dragDlgQuest);
	setWndCtrlSystem(wcsQuest);

	// add ctrl...

	const char* strBtn[2] = { "OPEN", "SAVE" }; // 프로젝트 여는 버튼
	hBtnQuestOpen = (HWND*)malloc(sizeof(HWND) * 2);

	for (int i = 0; i < 2; i++)
		hBtnQuestOpen[i] = createWndButton(5+65*i, 5, 60, 30, strBtn[i], NULL, btnQuestOpenUpdate);

	hEbQuestName = createWndEditBox(5, 40, 300, 30, "Title", NULL, ebQuestNameUpdate);

	hBtnQuestSubmit = createWndButton(5, 80, 60, 30, "Submit", NULL, btnQuestSubmitUpdate); 

	const char* strquestList[1] = { "End of Quest" };
	hLbQuestList = createWndListBox(5, 130, 700, 200, strquestList, 1, NULL, lbQuestListUpdate);

	hEbQuestContent = createWndEditBox(325, 40, 300, 30, "Content", NULL, ebQuestContentUpdate);
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
	FILE* pf = fopen(path, "rb");
	loadDataQuest(pf);
	fclose(pf);
}

void saveDataQuest(const char* path)
{
	FILE* pf = fopen(path, "wb");
	saveDataQuest(pf);

	fclose(pf);
}

void loadDataQuest(FILE* pf)
{
	int i;
	int num = countWndListBox(hLbQuestList) - 1;

	for (i = 0; i < num; i++)
		removeWndListBox(hLbQuestList, i);

	arrayQuest->removeAll();


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

	
}

void saveDataQuest(FILE* pf)
{
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

void setStringLimit(const char* str, int num, char* output)
{
	int i, len = strlen(str);
	int n;

	if (num > len)
		num = len;
	for (i = 0, n = 0; i < len;)
	{
		n++;
		if (iString::isUnicode(&str[i]))
			i += 3;
		else
			i++;

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
	// 에디트박스에 잇는 내용 리스트박스로 push!

	char* strTitle = getWndText(hEbQuestName);
	char* strContent = getWndText(hEbQuestContent);
	if (strcmp(strTitle, "Title") == 0 || strcmp(strContent, "Content")== 0)
	{
		free(strTitle);
		free(strContent);
		return;
	}

	char strComment[64];

	setStringLimit(strTitle, 3 , strComment);
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
		
		Quest* qst = (Quest*)arrayQuest->objectAtIndex(index);

		free(inputQuest->title);
		qst->title = strTitle;

		free(inputQuest->content);
		qst->content = strContent;

		arrayQuest->addObject(index, inputQuest);

	}
	else
	{
		addWndListBox(hLbQuestList, index, strComment);
		setWndListBox(hLbQuestList, index +1 );

		free(inputQuest->title);
		inputQuest->title = strTitle;

		free(inputQuest->content);
		inputQuest->content = strContent;

		arrayQuest->addObject(inputQuest);
		
	}

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

void ebQuestContentUpdate(WPARAM wPram, LPARAM lParam)
{
}
