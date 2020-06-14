#pragma once

#include "iStd.h"
#include "iWindow.h"

typedef struct _Nom
{
	char* name;
	char* path;
} Nom, Item;

struct Quest
{
	char* title;
	char* content;
};

struct Say
{
	int nomIndex;
	char* comment;
	int itemIndex;
	int gold;
	int exp;
	int questIndex;
};

extern iArray* arrayNom;
extern Nom* inputNom;
void freeNom(void* parm);
Nom* newNom();

extern iArray* arrayItem;
extern Item* inputItem;
void freeItem(void* parm);
Item* newItem();

extern iArray* arrayQuest;
extern Quest* inputQuest;
void freeQuest(void* parm);
Quest* newQuest();

extern iArray* arraySay;
extern Say* inputSay;
void freeSay(void* parm);
Say* newSay();

void loadScriptData();
void freeScriptData();

/*
1. 캐릭터목록
몇놈(int) + (이름길이int + 이름 + 이미지길이int + 이미지데이타)
2. 아이템 목록
총개(int) + (이름길이int + 이름 + 이미지길이int + 이미지데이타)
3. 퀘스트 목록
총개(int) + (이름길이int + 이름 + 본문의 길이int + 본문)
3. 대화목록
몇마디(int) + (누구index-int + 말의길이int + 말 + 보상 아이템index-int + 보상 골드-int + 보상 경험치-int + 퀘스트인덱스)
*/