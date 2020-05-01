#include "TdObject.h"

#include "TdData.h"

// ------------------------------
// Unit
// ------------------------------
Monster** _monster;// 3종류, 각 100개
Monster** monster;// 활성화
int monsterNum;

Tower** _tower;// 2종류, 각 100개
Tower** tower;// 활성화
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
		if (m->hp == 0.0f)
		{
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

BB** _bb;
BB** bb;
int bbNum;


void loadTdEffect()
{
	int i;
	_bb = (BB**)malloc(sizeof(BB) * 2);
	for (i = 0; i < 2; i++)
	{
		_bb[i] = (BB*)calloc(sizeof(BB) , 200);

	}

	bb = (BB**)malloc(sizeof(BB*) * 400);
	
	bbNum = 0;


}

void freeTdEffect()
{

	int i;

	for (i = 0; i < 2; i++)
		free(_bb[i]);
	free(_bb);
	free(bb);

}
void drawTdEffect(float dt)
{
	int i;

	for (i = 0; i < bbNum; i++)
	{
		if (bb[i]->paint(dt))
		{
			bbNum--;
			bb[i] = bb[bbNum];
			i--;
		}
	}
}

void addTdBB(int index, iPoint p, iPoint v, float att)
{
	for (int i = 0; i < 200; i++)
	{
		BB* b = &_bb[index][i];
		if (b->alive == false)
		{
			b->alive = true;
			b->p = p;
			b->v = v;
			b->att = att;

			b->bSlow = index;
			 		
			if (index == 1)
				b->slowDt = 1.0f;
			
			bb[bbNum] = b;
			bbNum++;
			return;
		}
	}
}
void addTdExp(int index, iPoint p)
{

}

// ------------------------------
// Monster
// ------------------------------
bool Monster::paint(float dt)
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

void Monster::move(float dt)
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

void Monster::setDmg(float att)
{
	hp -= att;
	if (hp < 0.0f)
		hp = 0.0f;
	dmgDt = _dmgDt;
}

void Monster::setSlow(float att, float slowDt, float slowRate)
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

// ------------------------------
// Tower
// ------------------------------
bool Tower::paint(float dt)
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

void Tower::fire(Monster* m)
{
	iPoint v = m->p - p;
	v /= iPointLength(v);
	addTdBB(index, p, v, att);
}

bool BB::paint(float dt)
{
	if (!containPoint(p, iRectMake(-50, -50, devSize.width + 100, devSize.height + 100)))
	{
		alive = false;
		return true;
	}
	float min = 0xfffff;
	Monster* mon = NULL;
	for (int i = 0; i < monsterNum; i++)
	{
		Monster* m = monster[i];
		iRect rt = iRectMake(m->p.x - m->tex->width/ 2, m->p.y - m->tex->height / 2, m->tex->width, m->tex->height );
		if (containPoint(p, rt))
		{
			float distance = iPointLength(m->p - p);

			if (distance < min)
			{
				min = distance;
				mon = m;
			}
		}
	}

	if (mon)
	{
		if (bSlow)
			mon->setSlow(att, slowDt, 0.5f);
		else
			mon->setDmg(att);
		
		
		//피격이펙트!
		alive = false;
		return true;
	}

	//draw
	drawImage(tex, p.x, p.y, VCENTER | HCENTER);

	return false;
}