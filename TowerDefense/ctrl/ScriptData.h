#pragma once

#include "iStd.h"
#include "iWindow.h"

struct Nom
{
	char* name;
	char* path;
};

struct Item
{
	char* name;
	char* path;
};

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
void openScriptData(const char* path);
void saveScriptData(const char* path);

