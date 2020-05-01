#include "Td.h"

#include "Loading.h"
#include "TdData.h"
#include "TdObject.h"
#include "Menu.h"

void loadTd()
{
	printf("loadTd()\n");
	loadTdBg();
	loadTdUnit();
	loadTdEffect();
	createPopTdUI();
}

void freeTd()
{
	printf("loadTd()\n");
	freeTdBg();
	freeTdUnit();
	freeTdEffect();
	freePopTdUI();
}

void drawTd(float dt)
{
	drawTdBg(dt);

	drawTdUnit(dt);
	drawTdEffect(dt);
	drawPopTdUI(dt);
}

void keyTd(iKeyState stat, iPoint point)
{
	if (keyPopTdUI(stat, point))
		return;

	switch (stat) {

	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
		break;

	case iKeyStateEnded:
		break;
	}
}

// ------------------------------
// Bg
// ------------------------------
void loadTdBg() {}
void freeTdBg() {}
void drawTdBg(float dt)
{
	int i, num = td_tile_num_x * td_tile_num_y;
	for (i = 0; i < num; i++)
	{
		int x = td_tile_size_w * (i % td_tile_num_x);
		int y = td_tile_size_h * (i / td_tile_num_x);
		int index = tileTd[i];
		if (index == td_can_move) setRGBA(td_rgba_can_move);
		else if (index == td_can_install) setRGBA(td_rgba_can_install);
		else if (index == td_canNot_move) setRGBA(td_rgba_canNot_move);
		else if (index == td_location_start) setRGBA(td_rgba_start);
		else if (index == td_location_end) setRGBA(td_rgba_end);
		fillRect(x, y, td_tile_size_w, td_tile_size_h);
	}
}

// ------------------------------
// popTdUI
// ------------------------------
void createPopTdUI()
{

}

void freePopTdUI()
{

}

void showPopTdUI(bool show)
{

}

void drawPopTdUI(float dt)
{

}

bool keyPopTdUI(iKeyState stat, iPoint point)
{
	return false;
}

