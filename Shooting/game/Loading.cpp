#include "Loading.h"

int gameState, _gameState;
METHOD_LOADING methodLoadingFree;
METHOD_LOADING methodLoadingLoad;

#define _loadingDt 1000.0f
float loadingDt = _loadingDt;

void setLoading(int newGameState, METHOD_LOADING free, METHOD_LOADING load)
{
	_gameState = newGameState;
	methodLoadingFree = free;
	methodLoadingLoad = load;

	loadingDt = 0.0f;
}

void drawLoading(float dt)
{
	if (loadingDt == _loadingDt)
		return;

	float alpha;

	if (loadingDt < 0.5f)
	{
		loadingDt += dt;
		if (loadingDt > 0.5f)
			loadingDt = 0.5f;
		alpha = loadingDt / 0.5f;
	}
	else if (loadingDt == 0.5f)
	{
		methodLoadingFree();
		methodLoadingLoad();
		gameState = _gameState;
		loadingDt += 0.000001f;
		alpha = 1.0f;
	}
	else
	{
		loadingDt += dt;
		alpha = 1.0f - (loadingDt - 0.5f) / 0.5f;
		if (loadingDt > 1.0f)
			loadingDt = _loadingDt;
	}

	setRGBA(0, 0, 0, alpha);
	fillRect(0, 0, devSize.width, devSize.height);
}

bool keyLoading(iKeyState stat, iPoint point)
{
	if( loadingDt < _loadingDt )
		return true;
	return false;
}
