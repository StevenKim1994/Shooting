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

	Debugging
	1. logo (콘솔창, 파일, 서버)
	2. ide (제공)
	3. 과부하

	f5 : 디버그 > 디버깅 시작
	f9 : 디버그 > 중단점 설정/해제 break point
	f10 : 디버그 > 프로시저 단위 실행 step over
	f11 : 디버그 > 한 단계씩 코드 실행 step into

	imporve performance
	1. call by value or call by reference
	2. loop

	int n = 10000;

	for(int i =0, j = n-1; i < j; i++) // n-1 이 매번 반복되지 않는다.
	{
		TODO...
	}

	3. new malloc ( class or struct )
	4. image - frequency / size 
	5. storage - 숫자 / 지역변수 (중복 연산할수 있는 함수 내에 함수 인자를 한번만 연산해놓기...) , 또한 전역변수를 지역변수보다 접근속도가 차이가 난다... 참조하는거 미리 바로가기 변수(지역변수) 만들기...
	6. file io - struct padding
	7. sound - 60/1초 차이로 재생되는 경우 없애기




	
*/

