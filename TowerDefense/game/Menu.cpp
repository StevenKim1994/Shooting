#include "Menu.h"

#include "Loading.h"
#include "Proc.h"

MenuStat menuStat;

Tile* tile;
int tileNum;
int* selectedTile;
iPoint positionCurse;

Book book[playerNum] = {
	{"OpenGL 4",	49000},
	{"프로그래밍",	45000},
	{"알고리즘",		32400},
	{"코딩인터뷰",	40500},
};

bool* result;

Solution* solution;
int goKind;
int goIndex;
iPoint goPosition;

void loadMenu()
{
	tileNum = playerNum * heightNum;
	tile = (Tile*)malloc(sizeof(Tile) * tileNum);

	int wid = (devSize.width - 200) / (playerNum - 1);
	int hei = (devSize.height - 200) / heightNum;
	for (int i = 0; i < tileNum; i++)
	{
		Tile* t = &tile[i];
		t->index = -1;
		iPoint p = iPointMake(100 + wid * (i % playerNum),
							  114 + hei * (i / playerNum));
		t->position = p;
		t->touch = iRectMake(p.x-20, p.y-10, 40, 20);
	}

	for (int j = 4; j < 6; j++)
	{
		for (int i = 0; i < playerNum-1; i++)
		{
			int m = playerNum * j + i;
			if (tile[m].index==-1 && random() % 100 < 50)
			{
				int n = playerNum * (4 + random() % 2) + 1 + i;
				if (tile[n].index == -1)
				{
					tile[m].index = n;
					tile[n].index = m;
				}
			}
		}
	}

	result = (bool*)calloc(sizeof(bool), playerNum);
	result[random() % playerNum] = true;

	menuStat = MenuStatReady;
	selectedTile = (int*)malloc(sizeof(int) * 2);
	selectedTile[0] = -1;
	selectedTile[1] = -1;

	solution = (Solution*)malloc(sizeof(Solution) * playerNum);
}
void freeMenu()
{		 
	free(tile);
	free(result);

	free(selectedTile);
	free(solution);
}

void drawMenu(float dt)
{
	setRGBA(0, 0, 0, 1);
	fillRect(0, 0, devSize.width, devSize.height);

	int i, j, x, y;
	int wid = (devSize.width - 200)/(playerNum-1);
	int hei = (devSize.height - 200)/heightNum;

	setRGBA(1, 1, 1, 1);
	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	for (i = 0; i < playerNum; i++)
	{
		x = 100 + wid * i;
		drawLine(	x, 100,
					x, devSize.height - 100);

		drawString(x, 20, TOP | HCENTER, book[i].name);
		drawString(x, 70, TOP | HCENTER, "%d", book[i].price);

		drawString(x, devSize.height - 80, TOP | HCENTER,
			result[i] ? "당첨" : "꽝");
#if modeDebug
		setRGBA(0, 0, 1, 1);
		setStringSize(20);
		for (j = 0; j < heightNum; j++)
		{
			Tile* t = &tile[playerNum * j + i];
			drawRect(t->touch);
			drawString(t->position.x, t->position.y,
				VCENTER | HCENTER, "%d", t->index);
		}
		setRGBA(1, 1, 1, 1);
#endif
	}

	// 연결된 타일
	for (i = 0; i < tileNum; i++)
	{
		Tile* t0 = &tile[i];
		if (t0->index > -1)
		{
			Tile* t1 = &tile[t0->index];
			setRGBA(1, 1, 0, 1);
			drawLine(t0->position, t1->position);
			setRGBA(1, 1, 1, 1);
		}
	}
	// 드래그 중일때
	if (selectedTile[0] != -1)
	{
		setRGBA(1, 0, 1, 1);
		drawLine(tile[selectedTile[0]].position, positionCurse);
		if (selectedTile[1] != -1)
		{
			setRGBA(1, 0, 0, 1);
			drawLine(tile[selectedTile[0]].position,
					 tile[selectedTile[1]].position);
		}
		setRGBA(1, 1, 1, 1);
	}

	if (menuStat == MenuStatReady)
	{
		//setRGBA(0.5, 0.5, 0.5, 0.5);
		setRGBA(0.5, 0.5, 0.5, 1.0);
		fillRect(0, 210, devSize.width, 60);
	}
	else if (menuStat == MenuStatGo)
	{
		setRGBA(1, 0, 1, 1);

		Solution* s = &solution[goKind];

		setRGBA(0, 1, 0, 1);
		if (goIndex < s->num-1)
		{
			// animation
			int i;
			for (i = 0; i < goIndex; i++)
			{
				iPoint sp = tile[s->index[i]].position;
				iPoint ep = tile[s->index[1 + i]].position;
				drawLine(sp, ep);
			}
			i = goIndex;
			iPoint sp = tile[s->index[i]].position;
			iPoint ep = tile[s->index[1 + i]].position;

			iPoint v = iPointVector(ep - sp);
			v *= 100;
			//goPosition += v * dt;
			if (goPosition.x < ep.x) {
				goPosition.x += v.x * dt;
				if (goPosition.x > ep.x)
					goPosition.x = ep.x;
			} else if (goPosition.x > ep.x) {
				goPosition.x += v.x * dt;
				if (goPosition.x < ep.x)
					goPosition.x = ep.x;
			}
			if (goPosition.y < ep.y) {
				goPosition.y += v.y * dt;
				if (goPosition.y > ep.y)
					goPosition.y = ep.y;
			} else if (goPosition.y > ep.y) {
				goPosition.y += v.y * dt;
				if (goPosition.y < ep.y)
					goPosition.y = ep.y;
			}
			if (goPosition == ep)
				goIndex++;
			drawLine(sp, goPosition);
		}
		else
		{
			int i, j = s->num - 1;
			for (i = 0; i < j; i++)
			{
				iPoint sp = tile[s->index[i]].position;
				iPoint ep = tile[s->index[1 + i]].position;
				drawLine(sp, ep);
			}
		}
	}
}

void keyMenu(iKeyState stat, iPoint point)
{
	if (menuStat == MenuStatReady)
	{
		if (stat == iKeyStateBegan)
		{
			for (int i = 0; i < playerNum; i++)
			{
				iRect rt = tile[i].touch;
				rt.origin.x += rt.size.width/2;
				rt.origin.x -= 50;
				rt.size.width = 100;
				rt.origin.y = 0;
				rt.size.height = 100;
				if (containPoint(point, rt))
				{
					menuStat = MenuStatGo;
					goKind = i;
					goIndex = 0;
					goPosition = tile[i].position;
					return;
				}
			}

			for (int i = 0; i < playerNum; i++)
			{
				for (int j = 0; j < heightNum; j++)
				{
					// 가리는 타일을 선택 방지
					if (j == 4 || j == 5 || j==heightNum - 1) continue;

					int k = playerNum * j + i;
					if (containPoint(point, tile[k].touch))
					{
						selectedTile[0] = k;
						positionCurse = point;
						break;
					}
				}
			}
		}
		else if (stat == iKeyStateMoved)
		{
			if (selectedTile[0] == -1)
				return;

			positionCurse = point;
			selectedTile[1] = -1;
			for (int i = 0; i < playerNum; i++)
			{
				for (int j = 0; j < heightNum; j++)
				{
					// 가리는 타일을 선택 방지
					if (j == 4 || j == 5 || j == heightNum - 1) continue;

					int k = playerNum * j + i;
					if (containPoint(point, tile[k].touch))
					{
						// 같은 걸 방지
						if (selectedTile[0] != k &&
						// 옆으로 두 칸 가는것도 방지
							abs(selectedTile[0] % playerNum - k % playerNum) == 1)
						{
							selectedTile[1] = k;
						}
						break;
					}
				}
			}
		}
		else if (stat == iKeyStateEnded)
		{
			if (selectedTile[0] != -1 && selectedTile[1] != -1)
			{
				// 연결된놈끼리 취소
				for (int n = 0; n < 2; n++)
				{
					if (selectedTile[n] != -1)
					{
						tile[
							tile[selectedTile[n]].index
						].index = -1;
					}
				}
				tile[selectedTile[0]].index = selectedTile[1];
				tile[selectedTile[1]].index = selectedTile[0];
			}
			selectedTile[0] = -1;
			selectedTile[1] = -1;
#if modeDebug
			checkSolution();
			for (int i = 0; i < playerNum; i++)
			{
				printf("solution[%d]===================\n", i);
				Solution* s = &solution[i];
				for (int j = 0; j < s->num; j++)
					printf("%d %s", s->index[j], j<s->num-1 ? "=>" : "\n");
			}
#endif
		}
	}
	else if (menuStat == MenuStatGo)
	{
		if (stat == iKeyStateEnded)
		{
			if( goIndex==solution[goKind].num-1 )
				menuStat = MenuStatReady;
		}
	}
}

void checkSolution()
{
	for (int i = 0; i < playerNum; i++)
	{
		Solution* s = &solution[i];
		s->num = 0;

		s->index[s->num] = i;
		s->num++;

		bool gone = false;
		while (1)
		{
			// 갈곳이 지정되지 않은 경우(-1), 밑으로
			int n = tile[s->index[s->num - 1]].index;
			if (gone || n == -1)
			{
				gone = false;
				s->index[s->num] = s->index[s->num - 1] + playerNum;
				s->num++;
			}
			// 갈곳이 지정된 경우, 그곳으로
			else
			{
				gone = true;
				s->index[s->num] = n;
				s->num++;
			}

			if (s->index[s->num - 1] / playerNum == heightNum - 1)
				break;
		}
	}
}
