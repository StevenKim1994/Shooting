#include "ProcObject.h"

// -----------------------------------------------------
// Object
// -----------------------------------------------------
Object::Object()
{
	alive = false;
	position = iPointMake(0, 0);
}

Object::~Object()
{

}

// -----------------------------------------------------
// Plane
// -----------------------------------------------------
Plane* me;

Plane::Plane() : Object()
{
	position = iPointMake(0, 0);
	_rate = 0.15f;
	rate = 0.0f;
}

Plane::~Plane()
{
	// to do
}

bool Plane::fire(iPoint tp)
{
	if (rate < _rate)
		return false;

	if (addBall(this, random() % 3, position, tp))
	{
		rate -= _rate;
		return true;
	}

	return false;
}

void Plane::paint(float dt)
{
	setRGBA(0, 0, 1, 1);
	fillRect(rect(), 16);

	rate += dt;
	if (rate > _rate)
		rate = _rate;
}

iRect Plane::rect()
{
	return iRectMake(position.x - 16, position.y - 16, 32, 32);
}

// -----------------------------------------------------
// Enemy
// -----------------------------------------------------
Enemy::Enemy() : Object()
{
	alive = false;
	position =
	targetPosition =
	vector = iPointMake(0, 0);
	method = NULL;
	pathNum = pathIndex = 0;
}

Enemy::~Enemy()
{

}

void Enemy::move(iPoint targetPosition, METHOD_ENEMY arrive)
{
	move(&targetPosition, 1, arrive);
}

void Enemy::move(iPoint* path, int pathNum, METHOD_ENEMY arrive)
{
	for (int i = 0; i < pathNum; i++)
		this->path[i] = path[i];
	this->pathNum = pathNum;
	pathIndex = 0;
	method = arrive;

	targetPosition = path[0];
	vector = iPointVector(targetPosition - position);
}

void Enemy::paint(float dt)
{
	setRGBA(1, 0, 1, 1);
	fillRect(rect(), 2);

	if (position != targetPosition)
	{
		//position += vector * 200 * dt;
		if (position.x < targetPosition.x)
		{
			position.x += vector.x * enemyMovement * dt;
			if (position.x > targetPosition.x)
				position.x = targetPosition.x;
		}
		else if (position.x > targetPosition.x)
		{
			position.x += vector.x * enemyMovement * dt;
			if (position.x < targetPosition.x)
				position.x = targetPosition.x;
		}

		if (position.y < targetPosition.y)
		{
			position.y += vector.y * enemyMovement * dt;
			if (position.y > targetPosition.y)
				position.y = targetPosition.y;
		}
		else if (position.y > targetPosition.y)
		{
			position.y += vector.y * enemyMovement * dt;
			if (position.y < targetPosition.y)
				position.y = targetPosition.y;
		}

		if (position == targetPosition)
		{
			pathIndex++;
			if (pathIndex < pathNum)
			{
				targetPosition = path[pathIndex];
				vector = iPointVector(targetPosition - position);
			}
			else
			{
				if (method)
					method(this);
			}
		}
	}
}

iRect Enemy::rect()
{
	return iRectMake(position.x - 16, position.y - 16, 32, 32);
}

Enemy** _enemy;// pool memory : 실제 사용가능한(할당한) 적 모두
Enemy** enemy;// _enemy에서 활성화(화면에 나타난) 것
int enemyNum;// enemy 개수

void loadEnemy()
{
	_enemy = new Enemy * [20];
	for (int i = 0; i < _enemyNum; i++)
		_enemy[i] = new Enemy();

	enemy = new Enemy * [20];
	enemyNum = 0;
}

void freeEnemy()
{
	for (int i = 0; i < _enemyNum; i++)
		delete _enemy[i];
	delete _enemy;

	delete enemy;
}

Enemy* addEnemy(iPoint p)
{
	for (int i = 0; i < _enemyNum; i++)
	{
		Enemy* e = _enemy[i];
		if (e->alive == false)
		{
			e->alive = true;
			e->position = p;
			e->targetPosition = p;
			e->pathIndex = e->pathNum;

			enemy[enemyNum] = e;
			enemyNum++;

			return e;
		}
	}
	return NULL;
}

// -----------------------------------------------------
// Ball
// -----------------------------------------------------
Ball::Ball(int index) : Object()
{
	this->index = index;
	alive = false;
	owner = NULL;
	float s[3] = { 100, 200, 300 };
	speed = s[index];
	vector = iPointMake(1, 0);// 크기1인 단위 벡터
}

Ball::~Ball()
{

}

// (100, 100), (150, 250)
void Ball::fire(iPoint startPosition, iPoint targetPosition)
{
	alive = true;

	position = startPosition;
	// 크기1 + 방향
	vector = iPointVector(targetPosition - startPosition);
}

void Ball::paint(float dt)
{
	float _c[3][4] = {
		{1, 0, 0, 1},
		{0, 1, 0, 1},
		{0, 0, 1, 1},
	};
	float* c = _c[index];
	setRGBA(c[0], c[1], c[2], c[3]);
	fillRect(rect(), 8);

	position += vector * (speed * dt);
	if (position.x < 0.0f ||
		position.x > devSize.width ||
		position.y < 0.0f ||
		position.y > devSize.height)
	{
		alive = false;// 화면에 벗어낫다.
	}
}

iRect Ball::rect()
{
	return iRectMake(position.x - 8, position.y - 8, 16, 16);
}

Ball*** _ball;// 비활성화 된 총알(alive == false)
Ball** ball;// 활성화 된 총알(alive == true)
int ballNum;

void loadBall()
{
	_ball = new Ball * *[3];
	for (int i = 0; i < 3; i++)
	{
		_ball[i] = new Ball * [20];
		for (int j = 0; j < 20; j++)
			_ball[i][j] = new Ball(i);
	}

	ball = new Ball * [20];
	ballNum = 0;
}

void freeBall()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 20; j++)
			delete _ball[i][j];
		delete _ball[i];
	}
	delete _ball;

	delete ball;
}

bool addBall(Object* owner, int index, iPoint sp, iPoint tp)
{
	int fireIndex = random() % 3;
	for (int i = 0; i < 20; i++)
	{
		Ball* b = _ball[fireIndex][i];
		if (b->alive == false)
		{
			b->alive = true;
			b->owner = owner;
			b->fire(sp, tp);

			ball[ballNum] = b;
			ballNum++;

			printf("fire %d(%f, %f)\n", fireIndex, tp.x, tp.y);
			return true;
		}
	}
	return false;
}

Object** obj;
int objNum;
iSort* sort;
void loadObject()
{
	me = new Plane();
	me->position = iPointMake(devSize.width / 2,
		devSize.height * 0.67);
	loadEnemy();
	loadBall();

	obj = new Object*[200];
	objNum = 0;

	sort = new iSort();
}

void freeObject()
{
	delete me;
	freeEnemy();
	freeBall();

	delete obj;

	delete sort;
}

void drawObject(float dt)
{
	// background
	setRGBA(0.5f, 0.5f, 0.5f, 1.0f);
	fillRect(0, 0, devSize.width, devSize.height);

	int i, j;

	sort->init();

	// hero
	obj[0] = me;
	objNum = 1;
	sort->add(me->position.y);
	// enemy
	for (i = 0; i < enemyNum; i++)
	{
		if (enemy[i]->alive == false)
		{
			enemyNum--;
			for (j = i; j < enemyNum; j++)
				enemy[j] = enemy[1 + j];
			i--;
			continue;
		}
		obj[objNum] = enemy[i];
		objNum++;
		sort->add(enemy[i]->position.y);
	}
	// ball
	for (i = 0; i < ballNum; i++)
	{
		if (ball[i]->owner == me)// 내가 쏜거
		{
			for (j = 0; j < enemyNum; j++)
			{
				if (containRect(ball[i]->rect(), enemy[j]->rect()))
				{
					// enemy hp--
					// hp==0, alive = false;
					enemy[j]->alive = false;
					ball[i]->alive = false;
					// 피격 이펙트, 효과음, 점수 올라감
					break;
				}
			}
		}
		else// 적이 손거
		{
			if (containRect(ball[i]->rect(), me->rect()))
			{
				// me hp--
				// hp==0, alive = false
				me->alive = false;
				ball[i]->alive = false;
				// 피격 이펙트, 효과음, 점수 올라감

				// life--,
				// if life==0, gameover      other appear
			}
		}

		if (ball[i]->alive == false)
		{
			ballNum--;
			for (j = i; j < ballNum; j++)
				ball[j] = ball[1 + j];
			i--;
			continue;
		}

		obj[objNum] = ball[i];
		objNum++;
		sort->add(ball[i]->position.y);
	}

	// sorting
	sort->update();
	// paint
	for (i = 0; i < sort->sdNum; i++)
		obj[ sort->get(i) ]->paint(dt);

	// keyboard ctrl.(move)
	iPoint v = iPointMake(0, 0);
	if (getKeyStat(keyboard_left))		v.x = -1.0f;
	else if (getKeyStat(keyboard_right))v.x = 1.0f;
	if (getKeyStat(keyboard_up))		v.y = -1.0f;
	else if (getKeyStat(keyboard_down))	v.y = 1.0f;
	if (v != iPointMake(0, 0))
	{
		v = iPointVector(v);
		me->position += v * 30;
	}
	// keyboard ctrl.(fire)
	if (getKeyStat(keyboard_space))
		me->fire(me->position + iPointMake(0, -1));

	updateAI(dt);
}

#define _appearEnemyDt 2.5f
float appearEnemyDt = _appearEnemyDt;

void updateAI(float dt)
{
	appearEnemyDt += dt;
	if (appearEnemyDt > _appearEnemyDt)
	{
		appearEnemyDt -= _appearEnemyDt;
		iPoint p = iPointMake(100 + random() % ((int)devSize.width - 200), -100);
		Enemy* e = addEnemy(p);
		e->move(p + iPointMake(0, devSize.height + 200), NULL);
	}
}
