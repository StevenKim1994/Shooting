#pragma once

#include "iStd.h"

#include "../resource.h"

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
*/

