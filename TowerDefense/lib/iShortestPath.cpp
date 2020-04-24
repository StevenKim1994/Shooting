#include "iShortestPath.h"

#include "iStd.h"

iShortestPath::iShortestPath()
{
	tiles = NULL;
	tileX = 0;
	tileY = 0;

	ti = NULL;
	tiNum = 0;

}

iShortestPath::~iShortestPath()
{
	if (ti)
		free(ti);
}

void iShortestPath::init(int* tiles, int tileX, int tileY)
{
	this->tiles = tiles;
	this->tileX = tileX;
	this->tileY = tileY;

	tileXY = tileX * tileY;

	if (tiNum < tileXY)
	{
		if(ti)
			free(ti);

		ti = (TileInfo*)malloc(sizeof(TileInfo) * tileXY);
		tiNum = tileXY;
	}

	for (int i = 0; i < tileXY; i++)
	{
		TileInfo* t = &ti[i];
		t->value = sp_inf;
		t->visit = false;
		t->pathNum = 0; 
	}

}

void iShortestPath::dijkstra(int start, int end, int* path, int& pathNum)
{
	TileInfo* t = &ti[start];
	t->value = 0;



	while (1)
	{

		// 방문하지 않은 최소값을 가진 타일을 찾는다.
		int curr = -1;
		int min = sp_inf;
		int next;

		for (int i = 0; i < tileXY; i++)
		{
			if (ti[i].value < min && ti[i].visit == false)
			{
				curr = i;
				min = ti[i].value;
			}
		}

		if (curr == -1) // 모두 방문되었다면 loop 탈출
			break;

		TileInfo* tiCurr = &ti[curr];
		TileInfo* tiNext;


		tiCurr->visit = true;
		//tiCurr->path[tiCurr->pathNum] = curr;
		//tiCurr->pathNum++;

		int x = curr % tileX;
		int y = curr / tileX;

		for (int i = 0; i < 4; i++)
		{
			if (i == 0)
			{
				if (curr % tileX == 0) continue;

				next = curr - 1;

			}
			else if (i == 1)
			{
				if (curr % tileX == tileX - 1) continue;

				next = curr + 1;
			}

			else if (i == 2)
			{
				if (curr / tileX == 0) continue;

				next = curr - tileX;
			}

			else if (i == 3)
			{
				if (curr / tileX == tileY - 1) continue;

				next = curr + tileX;
			}

		}
		tiNext = &ti[next];
		int value = tiCurr->value + tiles[next];
		if (value < tiNext->value)
		{
			tiNext->value = value;
			tiNext->pathNum = tiCurr->pathNum;
			memcpy(tiNext->path, tiCurr->path, sizeof(int) * tiCurr->pathNum);
			tiNext->path[tiCurr->pathNum] = next;
			tiNext->pathNum++;
		}


	}
	
}



int tiles[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 9, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 9, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 9, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 9, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 9, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

void drawShortestPath(float dt)
{

	iPoint off = iPointZero;
	for (int i = 0; i < 70; i++)
	{
		switch(tiles[i])
		{
		case 1: setRGBA(1, 1, 1, 1); break;
		case 9: setRGBA(1, 0, 0, 1); break;
		}
		fillRect(off.x + tile_size_w * (i % tile_num_w), off.y + tile_size_h * (i / tile_num_w), tile_size_w, tile_size_h, 5);
		setRGBA(1, 0, 1, 1);
		drawRect(off.x + tile_size_w * (i % tile_num_w)+2, off.y + tile_size_h * (i / tile_num_w)+2, tile_size_w-4, tile_size_h-4, 5);
	}


	setRGBA(1, 1, 1, 1);

	static iShortestPath* sp = NULL;

	if (sp == NULL)
	{
		sp = new iShortestPath();

		sp->init(tiles, 10, 7);
		int path[70];
		int pathNum;
		sp->dijkstra(21, 12, path, pathNum);

		for (int i = 0; i < pathNum; i++)
		{
			printf("%d, %d\n", i, path[i]);
		}
	}
}
