#pragma once

#include "iStd.h"

// ------------------------------
// Unit
// ------------------------------
void loadTdUnit();
void freeTdUnit();
void drawTdUnit(float dt);

void addTdMonster(int index, int lv, int locationS, int locationE, float delay);
void addTdTower(int index, int location);

// ------------------------------
// Effect
// ------------------------------
void loadTdEffect();
void freeTdEffect();
void drawTdEffect(float dt);

void addTdBB(iPoint p, iPoint v, float att);
void addTdExp(int index, iPoint p);

#define _dmgDt 0.2f

// ------------------------------
// Monster
// ------------------------------
struct Monster
{
	Texture* tex;
	float hp, _hp;
	float speed, _speed;
	float slowDt, _slowDt;
	float dmgDt;
	float delay;

	iPoint p, tp;
	int path[64], pathNum, pathIndex;

	bool paint(float dt);
	void move(float dt);
	void setDmg(float att);
	void setSlow(float att, float slowDt, float slowRate);
};

// ------------------------------
// Tower
// ------------------------------
struct Tower
{
	Texture* tex;
	bool alive;
	float att;
	int range;
	float rate, _rate;
	float focusRate;
	iPoint p;
	float degree;
	Monster* dst;

	bool paint(float dt);
	void fire(Monster* m);
};

// -----------------------------
// BB
// -----------------------------
struct BB
{
	Texture* tex;

	iPoint p, v;
	
	float speed;

	bool alive;
	float att;
	bool bSlow;

	float slowDt;

	bool paint(float dt);

	

};