#pragma once

#include "iType.h"

class iPopup;
typedef void (*IPOPUP_METHOD)(iPopup* me);

class iPopup
{
public:
	iPopup(	IPOPUP_METHOD methodOpen = 0,
			IPOPUP_METHOD methodClose = 0);
	virtual ~iPopup();

	static void freeImg(void* data);

	void show(bool show);
	void paint(float dt);

public:
	iArray* imgList;

	iPoint openPosition, closePosition;
	IPOPUP_METHOD methodOpen, methodClose;
};


