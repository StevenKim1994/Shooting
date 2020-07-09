#pragma once

#include "iType.h"
#include "iOpenGL.h"

struct STChannel // 외부에서 입력하는 것
{
	int buf = -1; // img이거나 비어잇을시 : -1 , 그다음꺼부터 0시작함 
	const char* strPath = NULL; 
	TextureWrap wrap = CLAMP;
	TextureFilter filter = LINEAR;
	bool vFlip = false; // 상하반전
};

struct STInput
{
	const char* strVertPath = NULL;
	const char* strFragPath = NULL;
	STChannel channel[4];
};

class iShaderToy
{
public:
	iShaderToy(STInput* si);
	virtual ~iShaderToy();

	void paint(float dt);
	void key(iKeyState stat, iPoint point);

public:
	Texture** texBuf; // 4 for using FBO
	GLuint* programID; 
	Texture*** texBufChannel; // 5 * 4 

	float takeTime;
	int frame;

	float mouse[4];
	

};
