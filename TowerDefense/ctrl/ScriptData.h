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

*/
