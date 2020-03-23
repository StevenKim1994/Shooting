#pragma once

#include "iType.h"

class iImage;

typedef void (*IMAGE_METHOD)(iImage* me);

class iImage
{
public:
	iImage();
	virtual ~iImage();

	void addObject(Texture* tex);
	void setTexAtIndex(int index);

	void paint(float dt, iPoint off);
	void startAnimation(IMAGE_METHOD m=0);

	iRect touchRect(iPoint p=iPointMake(0,0));

public:
	iArray* arrayImg;
	Texture* tex;
	iPoint position;
	bool animation;
	float aniDt, _aniDt;
	int frame;
	bool lastFrame;
	int repeatNum;	// 0 : inf
	IMAGE_METHOD method;
};


