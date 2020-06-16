#include "Script.h"
#include "iWindow.h"
#include "ScriptData.h"
#include "DlgNom.h"
#include "DlgItem.h"
#include "DlgQuest.h"

WndCtrlSystem* wcsScript;
// 최상단
HWND* hBtnOpenDlg;
void btnOpenDlgUpdate(HWND hwnd);

// 대화목록
HWND hLbSayList;
void lbSayListUpdate(HWND hwnd);

// 좌하단 - 말하는 놈, 보상아이템
HWND hCbSayNom, hCbSayRewardItem;
void cbSayNomUpdate(HWND hwnd);
void cbSayRewardItemUpdate(HWND hwnd);

HWND hEbSayRewardGold, hEbSayRewardExp;
void ebSayRewardGoldUpdate(HWND hwnd);
void ebSayRewardExpUpdate(HWND hwnd);

HWND hCbSayQuest;
void cbSayQuestUpdate(HWND hwnd);

// 중우하단 - 멘트
HWND hEbSayComment;
void ebSayCommentUpdate(HWND hwnd);

HWND hBtnSaySubmit;
void btnSaySubmitUpdate(HWND hwnd);


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

	initWndCtrlSystem();

	// to do....
	loadScriptData();

	wcsScript = new WndCtrlSystem(hwnd);
	wcsScript->dragAcceptFiles(dragScript);
	setWndCtrlSystem(wcsScript);

	// 최상단
	const char* strOpenDlg[7] = { "DlgNom", "DlgItem", "DlgQuest", "Open(scp)", "Save(scp)" , "Open(pjt)", "Save(pjt)" };
	hBtnOpenDlg = (HWND*)malloc(sizeof(HWND) * 7);
	for (i = 0; i < 7; i++)
		hBtnOpenDlg[i] = createWndButton(5 + 65 * i, 5, 60, 30, strOpenDlg[i], NULL, btnOpenDlgUpdate);

	// 대화목록
	const char* strList[1] = { "End of Dlg" };
	hLbSayList = createWndListBox(5, 40, 500, 300, strList, 1, NULL, lbSayListUpdate);

	// 좌하단 - 말하는 놈, 보상아이템
	const char* strNom[1] = { "End of Nom" };
	hCbSayNom = createWndComboBox(5, 350, 90, 400, strNom, 1, NULL, cbSayNomUpdate);
	const char* strItem[1] = { "End of Item" };
	hCbSayRewardItem = createWndComboBox(5, 390, 90, 400, strItem, 1, NULL, cbSayRewardItemUpdate);
	createWndStatic(5, 420, 90, 25, "Gold", NULL, NULL);
	setWndEditBosLength(10);
	hEbSayRewardGold = createWndEditBox(5, 450, 90, 25, "0", WndEditBoxStyle_int, NULL, ebSayRewardGoldUpdate);
	createWndStatic(5, 480, 90, 25, "Exp", NULL, NULL);
	hEbSayRewardExp = createWndEditBox(5, 510, 90, 25, "0", WndEditBoxStyle_int, NULL, ebSayRewardExpUpdate);
	const char* strQuest[1] = { "End of Quest" };
	hCbSayQuest = createWndComboBox(5, 540, 90, 400, strQuest, 1, NULL, cbSayQuestUpdate);

	// 중우하단 - 멘트
	setWndEditBosLength(300);
	hEbSayComment = createWndEditBox(100, 350, 430, 250, "Comment", WndEditBoxStyle_all, NULL, ebSayRewardGoldUpdate);

	hBtnSaySubmit = createWndButton(100, 610, 430, 25, "Submit", NULL, btnSaySubmitUpdate);


	loadDlgNom();
	loadDlgItem();
	loadDlgQuest();
}

void freeScript()
{
	// to do.....
	freeScriptData();
	freeDlgNom();
	freeDlgItem();
	freeDlgQuest();

	delete wcsScript;
}

void dragScript(WPARAM wParam, LPARAM lParam)
{
	wcsScript->dropFiles(wParam, lParam);
}

LRESULT colorScrip(WPARAM wParam, LPARAM lParam)
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

LRESULT dlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		// unshow 

		// destory 시키거나, 
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);        //화면에 있는게 갱신
	}
	return 0;
}

void loadDataProject(const char* path)
{
	FILE* pf = fopen(path, "rb");
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

void loadDataSay(const char* path)
{
	FILE* pf = fopen(path, "rb");
	loadDataSay(pf);
	fclose(pf);
}
void loadDataSay(FILE* pf)
{
	int i, len, num;
	int listBoxnum = countWndListBox(hLbSayList) - 1;
	for (i = 0; i < listBoxnum; i++)
	{
		removeWndListBox(hLbSayList, 0);
	}
	arraySay->removeAll();

	bool ready = true;
	fread(&num, sizeof(int), 1, pf);
	for (i = 0; i < num; i++)
	{
		Say* s = (Say*)malloc(sizeof(Say));

		fread(&s->nomIndex, sizeof(int), 1, pf);

		fread(&len, sizeof(int), 1, pf);
		s->comment = (char*)calloc(sizeof(char*), 1 + len);
		fread(s->comment, sizeof(char), len, pf);

		fread(&s->itemIndex, sizeof(int), 1, pf);
		fread(&s->gold, sizeof(int), 1, pf);
		fread(&s->exp, sizeof(int), 1, pf);
		fread(&s->questIndex, sizeof(int), 1, pf);

		if (s->nomIndex < arrayNom->count &&
			s->questIndex < arrayQuest->count &&
			s->itemIndex < arrayItem->count)
		{
			setWndComboBox(hCbSayNom, s->nomIndex);
			setWndComboBox(hCbSayQuest, s->questIndex);
			setWndComboBox(hCbSayRewardItem, s->questIndex);
			setWndText(hEbSayComment, s->comment);
			setWndText(hEbSayRewardGold, "%d", s->gold);
			setWndText(hEbSayRewardExp, "%d", s->exp);
			setWndListBox(hLbSayList, i);
			btnSaySubmitUpdate(NULL);
			free(s->comment);
			free(s);
		}
		else
		{
			ready = false;
			arraySay->addObject(s);
		}

		if (ready == false)
		{
			MessageBox(NULL, TEXT(".nom, .qst, .itm의 값이 전부 로드되지 않습니다 "), TEXT("ERROR"), MB_OK);
			return;
		}
	}
}

void saveDataSay(const char* path)
{
	FILE* pf = fopen(path, "wb");
	saveDataSay(pf);
	fclose(pf);
}
void saveDataSay(FILE* pf)
{
	int i, num = arraySay->count;
	fwrite(&num, sizeof(int), 1, pf);

	for (i = 0; i < num; i++)
	{
		Say* say = (Say*)arraySay->objectAtIndex(i);
		fwrite(&say->nomIndex, sizeof(int), 1, pf);

		int len = strlen(say->comment);
		fwrite(&len, sizeof(int), 1, pf);
		//fwrite(say->comment, sizeof(char) * len, 1, pf);
		fwrite(say->comment, len, 1, pf);

		fwrite(&say->itemIndex, sizeof(int), 1, pf);
		fwrite(&say->gold, sizeof(int), 1, pf);
		fwrite(&say->exp, sizeof(int), 1, pf);
		fwrite(&say->questIndex, sizeof(int), 1, pf);
	}
}

void btnOpenDlgUpdate(HWND hwnd)
{
	void (*show[3])(bool) = { showDlgNom, showDlgItem, showDlgQuest, };


	for (int i = 0; i < 3; i++)
	{
		if (hBtnOpenDlg[i] == hwnd)
		{
			show[i](true);
			return;
		}
	}

	void (*fio[4])(const char*) = { loadDataSay, saveDataSay, loadDataProject, saveDataProject };

	for (int i = 0; i < 4; i++)
	{
		if (hBtnOpenDlg[3 + i] == hwnd)
		{
			const char* path = openFileDlg(i % 2 == 0, i < 2 ?
				TEXT("SCP Files(*.scp)\0*.scp\0") :
				TEXT("Pjt Files(*.pjt)\0*.pjt\0"));
			if (path)
			{
				const char* ext = (i < 2 ? ".scp" : ".pjt");

				char _path[256];
				if (strcmp(&path[strlen(path) - 4], ext) == 0)
					strcpy(_path, path);
				else
					sprintf(_path, "%s%s", path, ext);

				fio[i](_path);
			}
			return;
		}
	}
}

void lbSayListUpdate(HWND hwnd)
{
	int index = indexWndListBox(hLbSayList);
	int count = countWndListBox(hLbSayList);
	if (index < count - 1)
	{
		Say* say = (Say*)arraySay->objectAtIndex(index);
		setWndComboBox(hCbSayNom, say->nomIndex);
		setWndText(hEbSayComment, say->comment);
		setWndComboBox(hCbSayRewardItem, say->itemIndex);
		setWndText(hEbSayRewardGold, "%d", say->gold);
		setWndText(hEbSayRewardExp, "%d", say->exp);
		setWndComboBox(hCbSayQuest, say->questIndex);
	}
}

// 좌하단 - 말하는 놈, 보상아이템
void cbSayNomUpdate(HWND hwnd)
{

}

void cbSayRewardItemUpdate(HWND hwnd)
{

}

void ebSayRewardGoldUpdate(HWND hwnd)
{

}
void ebSayRewardExpUpdate(HWND hwnd)
{

}

void cbSayQuestUpdate(HWND hwnd)
{

}

// 중우하단 - 멘트
void ebSayCommentUpdate(HWND hwnd)
{

}

void btnSaySubmitUpdate(HWND hwnd)
{
	int nomIndex = indexWndComboBox(hCbSayNom);
	const char* strNom = "X";
	if (nomIndex < arrayNom->count)
	{
		//char* strNom = getWndComboBox(hCbSayNom, nomIndex);
		Nom* nom = (Nom*)arrayNom->objectAtIndex(nomIndex);
		strNom = nom->name;
	}

	char* comment = getWndText(hEbSayComment);

	int itemIndex = indexWndComboBox(hCbSayRewardItem);
	const char* strItem = "X";
	if (itemIndex < arrayItem->count)
	{
		Item* itm = (Item*)arrayItem->objectAtIndex(itemIndex);
		strItem = itm->name;
	}

	int rewardGold = getWndInt(hEbSayRewardGold);
	char strGold[16]; sprintf(strGold, "%d", rewardGold);

	int rewardExp = getWndInt(hEbSayRewardExp);
	char strExp[16]; sprintf(strExp, "%d", rewardExp);

	int questIndex = indexWndComboBox(hCbSayQuest);
	const char* strQuest = "X";
	if (questIndex < arrayQuest->count)
	{
		Quest* qst = (Quest*)arrayQuest->objectAtIndex(questIndex);
		strQuest = qst->title;
	}

	char strComment[64];
	setStringLimit(strNom, 3, strComment);
	strcat(strComment, " : ");
	int len = strlen(strComment);
	setStringLimit(comment, 3, &strComment[len]);
	strcat(strComment, " : ");
	len = strlen(strComment);
	setStringLimit(strItem, 3, &strComment[len]);
	strcat(strComment, " : ");
	len = strlen(strComment);
	setStringLimit(strGold, 3, &strComment[len]);
	strcat(strComment, " : ");
	len = strlen(strComment);
	setStringLimit(strExp, 3, &strComment[len]);
	strcat(strComment, " : ");
	len = strlen(strComment);
	setStringLimit(strQuest, 3, &strComment[len]);

	int index = indexWndListBox(hLbSayList);
	int count = countWndListBox(hLbSayList);

	if (index < count - 1)
	{
		removeWndListBox(hLbSayList, index);
		addWndListBox(hLbSayList, index, strComment);
		setWndListBox(hLbSayList, index);

		Say* s = (Say*)arraySay->objectAtIndex(index);
		s->nomIndex = nomIndex;
		free(s->comment);
		s->comment = comment;
		s->itemIndex = itemIndex;
		s->gold = rewardGold;
		s->exp = rewardExp;
		s->questIndex = questIndex;
	}
	else
	{
		addWndListBox(hLbSayList, index, strComment);
		setWndListBox(hLbSayList, index);

		Say* s = inputSay;
		s->nomIndex = nomIndex;
		free(s->comment);
		s->comment = comment;
		s->itemIndex = itemIndex;
		s->gold = rewardGold;
		s->exp = rewardExp;
		s->questIndex = questIndex;
		arraySay->addObject(s);
		inputSay = newSay();
	}
}
