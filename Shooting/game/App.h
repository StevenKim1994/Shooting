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
	1. log (printf, 파일, 서버 ....) 을 통해서
	2. IDE (제공기능 , 메모리 그래프... 해제 되지 않고.. 또다시 그부분에서 할당되는부분...)
	3. 과부화 

	디버그 - 중단점 ...

	무한루프 확인? 디버그모드에서 모두 중단 (컨트롤 알트 쉬프트) 마지막실행된구문 찾아줌

	중단점 조건 (만약 i가 5일때 멈추고 싶다같은) 도 있음.....

	함수가 어디서 불렸는지 확인하고싶다 => 호출 스택 이용

	이 블럭에서의 변수확인 = > 조사식 로컬
	전역 변수는 = > 조사식에서 직접넣어 확인하기
*/

