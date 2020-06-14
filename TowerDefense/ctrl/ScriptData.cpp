#include "ScriptData.h"

void loadScriptData()
{
	arrayNom = new iArray(freeNom);
	inputNom = newNom();

	arrayItem = new iArray(freeItem);
	inputItem = newItem();

	arrayQuest = new iArray(freeQuest);
	inputQuest = newQuest();

	arraySay = new iArray(freeSay);
	inputSay = newSay();
}

void freeScriptData()
{
	delete arrayNom;
	freeNom(inputNom);

	delete arrayItem;
	freeItem(inputItem);

	delete arrayQuest;
	freeQuest(inputQuest);

	delete arraySay;
	freeSay(inputSay);
}

iArray* arrayNom;
Nom* inputNom;
void freeNom(void* parm)
{
	Nom* n = (Nom*)parm;
	free(n->name);
	free(n->path);
	free(n);
}
Nom* newNom()
{
	Nom* n = (Nom*)malloc(sizeof(Nom));
	n->name = (char*)malloc(1);
	n->path = (char*)malloc(1);
	return n;
}

iArray* arrayItem;
Item* inputItem;
void freeItem(void* parm)
{
	Item* i = (Item*)parm;
	free(i->name);
	free(i->path);
	free(i);
}
Item* newItem()
{
	Item* i = (Item*)malloc(sizeof(Item));
	i->name = (char*)malloc(1);
	i->path = (char*)malloc(1);
	return i;
}

iArray* arrayQuest;
Quest* inputQuest;
void freeQuest(void* parm)
{
	Quest* q = (Quest*)parm;
	free(q->title);
	free(q->content);
	free(q);
}
Quest* newQuest()
{
	Quest* q = (Quest*)malloc(sizeof(Quest));
	q->title = (char*)malloc(1);
	q->content = (char*)malloc(1);
	return q;
}

iArray* arraySay;
Say* inputSay;
void freeSay(void* parm)
{
	Say* s = (Say*)parm;
	free(s->comment);
	free(s);
}
Say* newSay()
{
	Say* s = (Say*)malloc(sizeof(Say));
	s->comment = (char*)malloc(1);
	return s;
}

