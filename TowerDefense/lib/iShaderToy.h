#pragma once

#include "iType.h"
#include "iOpenGL.h"

struct STChannel
{
	int buf = -1;// bufX(-1) bufA(0), bufB(1), bufC(2), bufD(3)
	const char* strPath = NULL;// NULL, path
	TextureWrap wrap = CLAMP;
	TextureFilter filter = LINEAR;
	bool vFlip = false;
};

struct STBuf
{
	
	const char* strVertPath = NULL;
	const char* strFragPath = NULL;
	STChannel channel[4];
};

struct STInput
{
	const char* strCommonPath;
	STBuf sb[5];
};

class iShaderToy
{
public:
	iShaderToy(STInput* si);
	virtual ~iShaderToy();

	void paint(float dt);
	void key(iKeyState stat, iPoint point);

public:
	Texture** texBuf;// 4 for using fbo...
	GLuint* programID;// 5
	Texture*** texBufChannel;// 5 x 4
	GLenum*** settingBufChannel;// 5 x 4 x 2

	float takeTime;
	int frame;
	float mouse[4];
	int keyboard[5];
};
