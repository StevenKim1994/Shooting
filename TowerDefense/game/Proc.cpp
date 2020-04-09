#include "Proc.h"

#include "Loading.h"
#include "Menu.h"
#include "ProcObject.h"

void loadProc()
{
	loadObject();
}

void freeProc()
{
	freeObject();
}

void drawProc(float dt)
{
	drawObject(dt);
}
void keyProc(iKeyState stat, iPoint point)
{
	if (stat == iKeyStateBegan)
		setLoading(gs_menu, freeProc, loadMenu);
}
