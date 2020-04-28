#include "Jump.h"

uint8 tileAttr[MapTileNumX * MapTileNumY] = {
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2,
	2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2,
	2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2,
	2, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 2,
	2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2,
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
	2, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 2,
	2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2,
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
};

MapTile* mt;
iPoint offMt;

MapHero* mh;


void loadJump()
{
	int i, num = MapTileNumX * MapTileNumY;
	mt = (MapTile*)malloc(sizeof(MapTile) * num);
	for (i = 0; i < num; i++)
	{
		MapTile* t = &mt[i];
		t->attr = tileAttr[i];
		//t->imgIndex;
	}

	offMt = iPointZero;

	mh = (MapHero*)malloc(sizeof(MapHero));
	mh->position = iPointMake(MapTileWidth / 2, MapTileHeight / 2);
	mh->size = iSizeMake(MapCharWidth, MapCharHeight);
	mh->speed = MapCharSpeed;
}

void freeJump()
{
	free(mt);
	free(mh);
}

void drawJump(float dt)
{
	int i, num = MapTileNumX * MapTileNumY;
	for (i = 0; i < num; i++)
	{
		MapTile* t = &mt[i];
		switch (t->attr) {
		case 0: setRGBA(tile_color_canMove); break;
		case 1: setRGBA(tile_color_canNotMove); break;
		case 2: setRGBA(tile_color_end); break;
		}
		float x = offMt.x + MapTileWidth * (i % MapTileNumX);
		float y = offMt.y + MapTileHeight * (i / MapTileNumX);
		fillRect(x, y, MapTileWidth, MapTileHeight);
		setRGBA(0, 0, 0, 1);
		drawRect(x + 2, y + 2, MapTileWidth - 4, MapTileHeight - 4);
	}

	setRGBA(hero_color);
	fillRect(mh->position.x + offMt.x - mh->size.width / 2,
		mh->position.y + offMt.y - mh->size.height / 2,
		mh->size.width, mh->size.height);

	uint32 keyStat = getKeyStat();
	iPoint v = iPointZero;
	if (keyStat & keyboard_left) v.x = -1;
	else if (keyStat & keyboard_right) v.x = 1;
	if (keyStat & keyboard_up) v.y = -1;
	else if (keyStat & keyboard_down) v.y = 1;
	if (v != iPointZero)
	{
		v /= iPointLength(v);
		mh->move(v * (mh->speed * dt));

		iPoint vp = offMt + mh->position;
#if 0// 실시간 + 항상 센터
		if (vp.x != devSize.width / 2)
		{
			offMt.x = devSize.width / 2 - mh->position.x;
			if (offMt.x > 0)
				offMt.x = 0;
			else if (offMt.x < devSize.width - MapTileWidth * MapTileNumX)
				offMt.x = devSize.width - MapTileWidth * MapTileNumX;
		}
		if (vp.y != devSize.height / 2)
		{
			offMt.y = devSize.height / 2 - mh->position.y;
			if (offMt.y > 0)
				offMt.y = 0;
			else if (offMt.y < devSize.height - MapTileHeight * MapTileNumY)
				offMt.y = devSize.height - MapTileHeight * MapTileNumY;
		}
#else// 실시간 + 1/3 벗어난 경우
		if (vp.x < devSize.width * 0.333333f)
		{
			// 왼쪽으로 넘어갔을 경우
			offMt.x += (devSize.width * 0.333333f - vp.x);
			if (offMt.x > 0)
				offMt.x = 0;
		}
		else if (vp.x > devSize.width * 0.666667f)
		{
			// 오른쪽으로 넘어갔을 경우
			offMt.x += (devSize.width * 0.666667f - vp.x);
			if (offMt.x < devSize.width - MapTileWidth * MapTileNumX)
				offMt.x = devSize.width - MapTileWidth * MapTileNumX;
		}
		if (vp.y < devSize.height * 0.333333f)
		{
			// 위로 넘어갔을 경우
			offMt.y += (devSize.height * 0.333333f - vp.y);
			if (offMt.y > 0)
				offMt.y = 0;
		}
		else if (vp.y > devSize.height * 0.666667f)
		{
			// 아래로 넘어갔을 경우
			offMt.y += (devSize.height * 0.666667f - vp.y);
			if (offMt.y < devSize.height - MapTileHeight * MapTileNumY)
				offMt.y = devSize.height - MapTileHeight * MapTileNumY;
		}
#endif
	}
	else// if(v == iPointZero) // 입력이 없을떄! 다시 카메라 제자리로!
	{
		iPoint vp = offMt + mh->position;
		
		if (vp.x != devSize.width / 2)
		{
			if (vp.x < devSize.width / 2)
			{
				offMt.x += natureSpeed * dt;
				vp.x = offMt.x + mh->position.x;
				if (vp.x > devSize.width / 2)
					offMt.x = devSize.width / 2 - mh->position.x;
			}
			

			else //if (vp.x > devSize.width / 2)
			{
				offMt.x -= natureSpeed * dt;
				vp.x = offMt.x + mh->position.x;
				if (vp.x < devSize.width / 2)
					offMt.x = devSize.width / 2 - mh->position.x;

			}
			if (offMt.x > 0)
				offMt.x = 0;
			else if (offMt.x < devSize.width - MapTileHeight * MapTileNumX)
				offMt.x = devSize.width - MapTileHeight * MapTileNumX;
		}
		if (vp.y != devSize.height / 2)
		{
			if (vp.y < devSize.height / 2)
			{
				offMt.y += natureSpeed * dt;
				vp.y = offMt.y + mh->position.y;
				if (vp.y > devSize.height / 2)
					offMt.y = devSize.height / 2 - mh->position.y;

			}
			else //if (vp.y > devSize.height / 2)
			{
				offMt.y -= natureSpeed * dt;
				vp.y = offMt.y + mh->position.y;
				if (vp.y < devSize.height / 2)
					offMt.y = devSize.height / 2 - mh->position.y;
			}
		

			if (offMt.y > 0)
				offMt.y = 0;
			else if (offMt.y < devSize.height - MapTileHeight * MapTileNumY)
				offMt.y = devSize.height - MapTileHeight * MapTileNumY;
			
		}
		
	}

	// scroll type
	// 1. 항상 주인공 센터
	// 2. 액정의 범위 벗어나면 스크롤

	// 카메라
	// 실시간
	// 따라가는
}

void MapHero::move(iPoint mp)
{
	if (mp.x < 0)
	{
		// left
		int tlx = position.x - size.width / 2; tlx /= MapTileWidth;
		int tly = position.y - size.height / 2; tly /= MapTileHeight;
		int bly = position.y + size.height / 2; bly /= MapTileHeight;
		int min = 0;
		for (int x = tlx - 1; x > -1; x--)
		{
			bool col = false;
			for (int y = tly; y < bly + 1; y++)
			{
				if (mt[MapTileNumX * y + x].attr == 1)
				{
					col = true;
					min = MapTileWidth * (x + 1);
					break;
				}
			}
			if (col)
				break;
		}
		//printf("min = %d, curr = %f\n", min, position.x);
		position.x += mp.x;
		if (position.x < min + size.width / 2)
			position.x = min + size.width / 2;
	}
	else if (mp.x > 0)
	{
		// right
		int TRX = position.x + size.width / 2; TRX /= MapTileWidth;
		int TRY = position.y - size.height / 2; TRY /= MapTileHeight;
		int BRY = position.y + size.height / 2; BRY /= MapTileHeight;
		int min = MapTileWidth * MapTileNumX;
		for (int x = TRX + 1; x < MapTileNumX; x++)
		{
			bool col = false;
			for (int y = TRY; y < BRY + 1; y++)
			{
				if (mt[MapTileNumX * y + x].attr == 1)
				{
					col = true;
					min = MapTileWidth * x - 1;
					break;
				}
			}
			if (col)
				break;
		}
		//printf("min = %d, curr = %f\n", min, position.x);
		position.x += mp.x;
		if (position.x > min - size.width / 2)
			position.x = min - size.width / 2;
	}


	if (mp.y < 0)
	{
		// up
		int TLY = position.y - size.height / 2; TLY /= MapTileHeight;
		int TLX = position.x - size.width / 2; TLX /= MapTileWidth;
		int TRX = position.x + size.width / 2; TRX /= MapTileWidth;
		int min = 0;
		for (int y = TLY - 1; y > -1; y--)
		{
			bool col = false;
			for (int x = TLX; x < TRX + 1; x++)
			{
				if (mt[MapTileNumX * y + x].attr == 1)
				{
					col = true;
					min = MapTileHeight * (y + 1);
					break;
				}
			}
			if (col)
				break;
		}
		//printf("min = %d, curr = %f\n", min, position.x);
		position.y += mp.y;
		if (position.y < min + size.height / 2)
			position.y = min + size.height / 2;
	}
	else if (mp.y > 0)
	{
		// down
		int TLY = position.y + size.height / 2; TLY /= MapTileHeight;
		int TLX = position.x - size.width / 2; TLX /= MapTileWidth;
		int TRX = position.x + size.width / 2; TRX /= MapTileWidth;
		int min = MapTileHeight * MapTileNumY;
		for (int y = TLY + 1; y < MapTileNumY; y++)
		{
			bool col = false;
			for (int x = TLX; x < TRX + 1; x++)
			{
				if (mt[MapTileNumX * y + x].attr == 1)
				{
					col = true;
					min = MapTileHeight * y - 1;
					break;
				}
			}
			if (col)
				break;
		}
		//printf("min = %d, curr = %f\n", min, position.x);
		position.y += mp.y;
		if (position.y > min - size.height / 2)
			position.y = min - size.height / 2;
	}
}


void keyJump(iKeyState stat, iPoint point)
{
}
