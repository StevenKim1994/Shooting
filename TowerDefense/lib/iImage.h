#pragma once

#include "iType.h"

class iImage;

typedef void (*IMAGE_METHOD)(iImage* me);

class iImage
{
public:
	iImage();
	virtual ~iImage();

	static void freeTex(void* data);

	void addObject(Texture* tex);
	void setTexAtIndex(int index);
	void replaceAtIndex(int index, Texture* tex);

	void paint(float dt, iPoint off);
	void startAnimation(IMAGE_METHOD m = 0);

	iRect touchRect(iPoint p = iPointMake(0, 0));

public:
	iArray* arrayTex;
	Texture* tex;
	iPoint position;

	bool selected;
	float selectedDt, _selectedDt;
	float selectedScale;
	// 0.0s => 0.2s
	// 1.0 => 0.8f : -0.2f
	// 1.0 => 1.2f : +0.2f

	bool animation;
	float aniDt, _aniDt;
	int frame;
	bool lastFrame;
	int repeatNum = 0;	// 0 : inf
	int _repeatNum = 0;
	IMAGE_METHOD method;
};

#define default_selectedDt 0.1f
#define default_selectedScale +0.2f


/*
 *#pragma once

#include "iType.h"

class iImage;

typedef void (*IMAGE_METHOD)(iImage* me);

class iImage
{
public:
	iImage();
	virtual ~iImage();

	static void freeTex(void* data);

	void addObject(Texture* tex);
	void setTexAtIndex(int index);

	void paint(float dt, iPoint off);
	void startAnimation(IMAGE_METHOD m=0);

	iRect touchRect(iPoint p=iPointMake(0,0));

public:
	iArray* arrayTex; // 텍스처의 목록
	Texture* tex; // 현재 텍스처
	iPoint position;

	bool selected; // 이미지가 선택되었다 확인 하는 변수
	float selectedDt, _selectedDt; // 선택되었을때 크기가 변경되는 사이시간
	float selectedScale; // 크기변경 스케일 비율 selectedDt 0.0 -> 1.0이 될때 Scale사이즈로... 

	
	bool animation;
	float aniDt, _aniDt;
	int frame;
	bool lastFrame;
	int repeatNum;	// 0 : inf
	IMAGE_METHOD method;
};

#define default_selectedDt 0.2f
#define default_selectedScale -0.2f

*/