#pragma once

#include "iStd.h"

void loadMenu();
void freeMenu();
void drawMenu(float dt);
void keyMenu(iKeyState stat, iPoint point);

enum MenuStat {
	MenuStatReady = 0,
	MenuStatGo,
	MenuStatResult
};

#define playerNum 4
#define heightNum 10

struct Tile {
	char index;
	iRect touch;
	iPoint position;
};

#define modeDebug 1

struct Book {
	const char* name;
	int price;
};

struct Solution {
	char index[100];
	int num;
};

void checkSolution();

// -------------------------------------
// popMenu
// -------------------------------------
void createPopMenu();
void freePopMenu();
void showPopMenu(bool show);
void drawPopMenu(float dt);
bool keyPopMenu(iKeyState stat, iPoint point);

// -------------------------------------
// popHowto
// -------------------------------------
void createPopHowto();
void freePopHowto();
void showPopHowto(bool show);
void drawPopHowto(float dt);
bool keyPopHowto(iKeyState stat, iPoint point);

// -------------------------------------
// popResult
// -------------------------------------
void createPopResult();
void freePopResult();
void showPopResult(bool show);
void drawPopResult(float dt);
bool keyPopResult(iKeyState stat, iPoint point);

#if 0
OpenGL 4 Shading Language Cookbook, 3 / E
http ://www.yes24.com/Product/Goods/66859151
정가	49, 000원
판매가	49, 000원

프로그래밍 대회에서 배우는 알고리즘 문제 해결 전략 세트
http ://www.yes24.com/Product/Goods/8006522?Acode=101
정가 	50, 000원
판매가	45, 000원(10 % 할인)

알고리즘 트레이닝 : 자료 구조, 알고리즘 문제 해결 핵심 노하우
http ://www.yes24.com/Product/Goods/42415865
정가 	36, 000원
판매가	32, 400원(10 % 할인)

코딩인터뷰 완전분석 187가지 프로그래밍 문제와 해법[개정판]
http ://www.yes24.com/Product/Goods/44305533?Acode=101
	정가 	45, 000원
	판매가	40, 500원(10 % 할인)
#endif
