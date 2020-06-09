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

void openScriptData(const char* path)
{
	int i, len;

	FILE* pf = fopen(path, "rb");

	int num;
	fread(&num, sizeof(int), 1, pf);
	arrayNom->removeAll();
	for (i = 0; i < num; i++)
	{
		Nom* n = (Nom*)malloc(sizeof(Nom));
		fread(&len, sizeof(int), 1, pf);
		n->name = (char*)calloc(sizeof(char), 1 + len);
		fread(n->name, len, 1, pf);
		fread(&len, sizeof(int), 1, pf);
		n->path = (char*)calloc(sizeof(char), 1 + len);
		fread(n->path, len, 1, pf);

		arrayNom->addObject(n);
	}

	fread(&num, sizeof(int), 1, pf);
	arrayItem->removeAll();
	for (i = 0; i < num; i++)
	{
		Item* item = (Item*)malloc(sizeof(Item));
		fread(&len, sizeof(int), 1, pf);
		item->name = (char*)calloc(sizeof(char), 1 + len);
		fread(item->name, len, 1, pf);
		fread(&len, sizeof(int), 1, pf);
		item->path = (char*)calloc(sizeof(char), 1 + len);
		fread(item->path, len, 1, pf);

		arrayItem->addObject(item);
	}

	fread(&num, sizeof(int), 1, pf);
	arrayQuest->removeAll();
	for (i = 0; i < num; i++)
	{
		Quest* q = (Quest*)malloc(sizeof(Quest));
		fread(&len, sizeof(int), 1, pf);
		fread(q->title, len, 1, pf);
		fread(&len, sizeof(int), 1, pf);
		fread(q->content, len, 1, pf);

		arrayQuest->addObject(q);
	}

	fread(&num, sizeof(int), 1, pf);
	arraySay->removeAll();
	for (i = 0; i < num; i++)
	{
		Say* s = (Say*)malloc(sizeof(Say));

		fread(&s->nomIndex, sizeof(int), 1, pf);

		fread(&len, sizeof(int), 1, pf);
		s->comment = (char*)calloc(sizeof(char), 1 + len);
		fread(s->comment, len, 1, pf);

		fread(&s->itemIndex, sizeof(int), 1, pf);
		fread(&s->gold, sizeof(int), 1, pf);
		fread(&s->exp, sizeof(int), 1, pf);
		fread(&s->questIndex, sizeof(int), 1, pf);

		arraySay->addObject(s);
	}

	fclose(pf);
}

void saveScriptData(const char* path)
{
	int i, len;

	FILE* pf = fopen(path, "wb");

	int num = arrayNom->count;
	fwrite(&num, sizeof(int), 1, pf);
	for (i = 0; i < num; i++)
	{
		Nom* n = (Nom*)arrayNom->objectAtIndex(i);
		len = strlen(n->name);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(n->name, len, 1, pf);
		len = strlen(n->path);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(n->path, len, 1, pf);
	}

	num = arrayItem->count;
	fwrite(&num, sizeof(int), 1, pf);
	for (i = 0; i < num; i++)
	{
		Item* itemp = (Item*)arrayItem->objectAtIndex(i);
		len = strlen(itemp->name);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(itemp->name, len, 1, pf);
		len = strlen(itemp->path);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(itemp->path, len, 1, pf);
	}

	num = arrayQuest->count;
	fwrite(&num, sizeof(int), 1, pf);
	for (i = 0; i < num; i++)
	{
		Quest* q = (Quest*)arrayQuest->objectAtIndex(i);
		len = strlen(q->title);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(q->title, len, 1, pf);
		len = strlen(q->content);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(q->content, len, 1, pf);
	}

	num = arraySay->count;
	fwrite(&num, sizeof(int), 1, pf);
	for (i = 0; i < num; i++)
	{
		Say* s = (Say*)arraySay->objectAtIndex(i);

		fwrite(&s->nomIndex, sizeof(int), 1, pf);

		len = strlen(s->comment);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(s->comment, len, 1, pf);

		fwrite(&s->itemIndex, sizeof(int), 1, pf);
		fwrite(&s->gold, sizeof(int), 1, pf);
		fwrite(&s->exp, sizeof(int), 1, pf);
		fwrite(&s->questIndex, sizeof(int), 1, pf);
	}

	fclose(pf);
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

