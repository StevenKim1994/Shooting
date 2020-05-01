#pragma once

#include "iStd.h"

void loadTd();
void freeTd();
void drawTd(float dt);
void keyTd(iKeyState stat, iPoint point);

// ------------------------------
// Bg
// ------------------------------
void loadTdBg();
void freeTdBg();
void drawTdBg(float dt);

// ------------------------------
// popTdUI
// ------------------------------
void createPopTdUI();
void freePopTdUI();
void showPopTdUI(bool show);
void drawPopTdUI(float dt);
bool keyPopTdUI(iKeyState stat, iPoint point);

// ------------------------------
// ProcData
// ------------------------------
struct ProcData
{
	void takeTime(float dt);
};

// field(길찾기, 몹)

// ui(생명, 웨이브, 점수) : 멈춤, 웨이브서두르는것

// ui(설치, 파괴 => 공격범위, 가격, 레벨)

// 게임종료