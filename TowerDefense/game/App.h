#pragma once

#include "iStd.h"

#include "../resource.h"

extern iPoint cursor;
void loadCursor();
void freeCursor();
void drawCursor(float dt);
bool updateCursor(bool inClient);

extern bool isFullscreen;
void goFullscreen();

extern int win_border_width, win_border_height;
void enforceResolution(int edge, RECT& rect, int win_border_width, int win_border_height);

#define DEV_MIN_RATE 1/3.
/*
	1. Encoding - utf-8
	1) .editorconfig 파일 생성

	root = true

	[*]
	charset = utf-8

	2) c/c++ 명령줄
	/utf-8

	2. 미리컴파일된 헤더 파일 사용안함

	3. 디버그 모드 로그확인
	#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")

	4. 매크로
	_CRT_SECURE_NO_WARNINGS : strcpy_s _s 함수 못 쓰게 하는거 막음
	_USE_MATH_DEFINES : M_PI
	GDIPVER=0x0110 : gdi+상위버전 사용

	/////////////////////////////////////////////////
	Debugging
	1. logo (콘솔창, 파일, 서버)
	2. ide (제공)
	3. 과부하

	f5 : 디버그 > 디버깅 시작
	f9 : 디버그 > 중단점 설정/해제 break point
	f10 : 디버그 > 프로시저 단위 실행 step over
	f11 : 디버그 > 한 단계씩 코드 실행 step into

	/////////////////////////////////////////////////
	imporve performance
	1. call by value or reference
	2. loop
	3. new malloc ( class or struct )

	4. image - frequency / size
	5. storage - 숫자 / 지역변수
	6. file io - struct padding
	7. sound - 
*/

