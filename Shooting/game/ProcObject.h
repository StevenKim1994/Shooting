#pragma once

#include "iStd.h"

// -----------------------------------------------------
// Object
// -----------------------------------------------------
class Object
{
public:
	Object();
	virtual ~Object();

	virtual void paint(float dt) = 0;
	virtual iRect rect() = 0;
public:
	bool alive;
	iPoint position;
};

// -----------------------------------------------------
// Plane
// -----------------------------------------------------
class Plane : public Object// x 1
{
public:
	Plane();
	virtual ~Plane();

	bool fire(iPoint tp);
	virtual void paint(float dt);
	virtual iRect rect();

public:
	float rate, _rate;
};

extern Plane* me;

// -----------------------------------------------------
// Enemy
// -----------------------------------------------------
class Enemy;
typedef void (*METHOD_ENEMY)(Enemy* e);

class Enemy : public Object// x 10
{
public:
	Enemy();
	virtual ~Enemy();

	void move(iPoint targetPosition, METHOD_ENEMY arrive);
	void move(iPoint* path, int pathNum, METHOD_ENEMY arrive);
	virtual void paint(float dt);
	virtual iRect rect();

public:
	iPoint targetPosition, vector;
	METHOD_ENEMY method;
	iPoint path[20];
	int pathNum, pathIndex;
};

extern Enemy** _enemy;
extern Enemy** enemy;
extern int enemyNum;
#define _enemyNum 20

void loadEnemy();
void freeEnemy();
Enemy* addEnemy(iPoint p);

#define enemyMovement 200.0f

// -----------------------------------------------------
// Ball
// -----------------------------------------------------
class Ball : public Object// x3, x 20
{
public:
	Ball(int index);
	virtual ~Ball();

	void fire(iPoint startPosition, iPoint targetPosition);
	virtual void paint(float dt);
	virtual iRect rect();

public:
	bool alive;
	Object* owner;
	int index;
	float speed;
	iPoint vector;
};

extern Ball*** _ball;// 비활성화 된 총알(alive == false)
extern Ball** ball;// 활성화 된 총알(alive == true)
extern int ballNum;

void loadBall();
void freeBall();
bool addBall(Object* owner, int index, iPoint sp, iPoint tp);


void loadObject();
void freeObject();
void drawObject(float dt);

void updateAI(float dt);
