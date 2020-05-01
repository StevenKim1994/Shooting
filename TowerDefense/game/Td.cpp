#include "Td.h"

#include "Loading.h"
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
// Unit
// ------------------------------
#define _dmgDt 0.2f

struct Monster
{
	bool alive;
	Texture* tex;
	float hp, _hp;
	float speed, _speed;
	float slowDt, _slowDt;
	float dmgDt;
	float delay;

	iPoint p, tp;
	int path[64], pathNum, pathIndex;

	bool paint(float dt)
	{
		if (delay < 0.0f)
		{
			delay += dt;
			return false;
		}
		move(dt);

		float r = 1.0f, g = 1.0f, b = 1.0f;
		if (dmgDt)
		{
			float c = dmgDt / _dmgDt;
			g -= c;
			b -= c;

			dmgDt -= dt;
			if (dmgDt < 0.0f)
				dmgDt = 0.0f;
		}

		if (slowDt)
		{
			float c = slowDt / _slowDt;
			r -= c;
			g -= c;

			slowDt -= dt;
			if (slowDt < 0.0f)
				slowDt = 0.0f;
		}

		if (g < 0.0f)
			g = 0.0f;
		setRGBA(r, g, b, 1);

		drawImage(tex, p.x, p.y, VCENTER | HCENTER);

		setRGBA(1, 1, 1, 1);

		if (hp == 0.0f)
			return true;
		else if (p == tp && pathIndex == pathNum)
		{
			// life--;// #td
			// if( life==0 ) gameover(fail);
			return true;
		}

		return false;
	}

	void move(float dt)
	{
		if (p != tp)
		{
			iPoint v = tp - p;
			v /= iPointLength(v);

			if (p.x < tp.x)
			{
				p.x += v.x * speed * dt;
				if (p.x > tp.x) p.x = tp.x;
			}
			else if (p.x > tp.x)
			{
				p.x += v.x * speed * dt;
				if (p.x < tp.x) p.x = tp.x;
			}
			if (p.y < tp.y)
			{
				p.y += v.y * speed * dt;
				if (p.y > tp.y) p.y = tp.y;
			}
			else if (p.y > tp.y)
			{
				p.y += v.y * speed * dt;
				if (p.y < tp.y) p.y = tp.y;
			}
		}
		else
		{
			if (pathIndex < pathNum)
			{
				int index = path[pathIndex];
				pathIndex++;
				tp = centerOfTileIndex(index);
			}
		}
	}

	void setDmg(float att)
	{
		hp -= att;
		if (hp < 0.0f)
			hp = 0.0f;
		dmgDt = _dmgDt;
	}

	void setSlow(float att, float slowDt, float slowRate)
	{
		setDmg(att);

		if (this->slowDt < slowDt)
		{
			this->slowDt = slowDt;
			_slowDt = slowDt;
		}
		float s = _speed * slowRate;
		if (speed > s)
			speed = s;
	}
};

Monster** _monster;
Monster** monster;
int monsterNum;

struct Tower
{
	bool alive;
	float att;
	int range;
	float rate, _rate;
	float focusRate;
	iPoint p;
	float degree;
	Monster* dst;

	bool paint(float dt)
	{
		if (dst == NULL)
		{
			float min = 0xffffff;
			Monster* d = NULL;
			for (int i = 0; i < monsterNum; i++)
			{
				Monster* m = monster[i];
				float distance = iPointLength(p - m->p);
				if (distance < min)
				{
					min = distance;
					d = m;
				}
			}
			if (min < range)
				dst = d;
		}
		else
		{
			if (iPointLength(p - dst->p) > range)
			{
				dst = NULL;
			}
			else
			{
				//iPointRotate; 적방향을 가리키도록 함.
				// degree += focusRate * dt;// #td
				if (1)
				{
					if (rate < _rate)
						rate += dt;
					else
					{
						rate -= _rate;
						fire(dst);
					}

				}
			}
		}

		return false;
	}

	void fire(Monster* m)
	{
		iPoint v = m->p - p;
		v /= iPointLength(v);
		addTdBB(p, v, att);
	}
};

Tower** _tower;
Tower** tower;
int towerNum;

iSort* tdSort;

void loadTdUnit()
{
	int i;

	_monster = (Monster**)malloc(sizeof(Monster*) * 3);
	for (i = 0; i < 3; i++)
	{
		_monster[i] = (Monster*)calloc(sizeof(Monster), 100);

	}
	monster = (Monster**)malloc(sizeof(Monster*) * 300);
	monsterNum = 0;

	_tower = (Tower**)malloc(sizeof(Tower*) * 2);
	for (i = 0; i < 2; i++)
	{
		_tower[i] = (Tower*)calloc(sizeof(Tower), 100);

	}
	tower = (Tower**)malloc(sizeof(Tower*) * 200);
	towerNum = 0;

	tdSort = new iSort();
}
void freeTdUnit()
{
	int i;

	for (i = 0; i < 3; i++)
		free(_monster[i]);
	free(_monster);
	free(monster);

	for (i = 0; i < 2; i++)
		free(_tower[i]);
	free(_tower);
	free(tower);

	delete tdSort;
}

void drawTdUnit(float dt)
{
	int i;

#if 0
	for (i = 0; i < monsterNum; i++)
	{
		if (monster[i]->paint(dt))
		{
			monsterNum--;
			monster[i] = monster[monsterNum];
			i--;
		}
	}

	for (i = 0; i < towerNum; i++)
	{
		if (tower[i]->paint(dt))
		{
			towerNum--;
			tower[i] = tower[towerNum];
			i--;
		}
	}
#endif

	tdSort->init();

	for (i = 0; i < monsterNum; i++)
	{
		if (monster[i]->hp == 0)
		{
			monsterNum--;
			monster[i] = monster[monsterNum - 1];
			i--;
			continue;
		}
		tdSort->add(monster[i]->p.y);
	}
	for (i = 0; i < towerNum; i++)
		tdSort->add(tower[i]->p.y);

	tdSort->update();

	for (i = 0; i < tdSort->sdNum; i++)
	{
		int index = tdSort->get(i);
		if (index < monsterNum) monster[index]->paint(dt);
		else tower[index - monsterNum]->paint(dt);
	}
}

void addTdMonster(int index, int lv, int locationS, int locationE, float delay)
{
	for (int i = 0; i < 100; i++)
	{
		Monster* m = &_monster[index][i];
		if (m->alive == false)
		{
			m->alive = true;
			m->p = m->tp = centerOfTileIndex(locationS);
			// path, pathNum #td
			m->delay = -delay;

			monster[monsterNum] = m;
			monsterNum++;
			return;
		}
	}
}
void addTdTower(int index, int location)
{
	for (int i = 0; i < 100; i++)
	{
		Tower* t = &_tower[index][i];
		if (t->alive == false)
		{
			t->alive = true;
			t->p = centerOfTileIndex(location);
			// #td initialize

			tower[towerNum] = t;
			towerNum++;
			return;
		}
	}
}

// ------------------------------
// Effect
// ------------------------------
void loadTdEffect()
{

}

void freeTdEffect()
{

}
void drawTdEffect(float dt)
{

}

void addTdBB(iPoint p, iPoint v, float att)
{

}
void addTdExp(int index, iPoint p)
{

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

#define AA td_location_start
#define BB td_location_end
#define XX 99
int tileTd[td_tile_num_x * td_tile_num_y] =
{
	01, 01, 01, 01, 01, 01, 01, 01, 01, XX, XX, XX, XX, XX, XX, XX,
	AA, 01, 01, 01, 01, 01, 01, 01, 01, XX, XX, XX, XX, XX, XX, XX,
	01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, BB,
	01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, XX,
	AA, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, XX, 01,
	01, 01, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, 01, BB,// 
	AA, 01, 01, XX, XX, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01,
	01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01,
};

StageInfo stageInfo[2] =
{
	// stage 0
	{
		5,// life
		3,// wave
		{	// location(s/e), monIndex, monLv, monNum
			{ {16 * 1, 16 * 2, 0, 3}, 1.5f },// waveInfo 0
			{ {16 * 4, 16 * 2, 1, 4}, 1.0f },// waveInfo 1
			{ {16 * 6, 16 * 5, 0, 5}, 0.5f },// waveInfo 2
		}
	},
	// stage 1
	{

	},
};

MonsterInfo monsterInfo[3] =
{
	// monster0
	{ 20, 100 },
	// monster1
	{ 30, 50},
	// monster1
	{ 100, 30},
};

TowerInfo towerInfo[2] =
{
	// 가격/공격력/공범/공속/포커스
	{ 0, 3.0f, 300, 0.5, 100 },// tower0 - 공격
	{ 0, 0.1f, 500, 0.5, 100 },// tower1 - 이속
};