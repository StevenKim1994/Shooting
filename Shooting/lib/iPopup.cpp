#include "iPopup.h"

#include "iStd.h"

iPopup::iPopup(IPOPUP_METHOD methodOpen, IPOPUP_METHOD methodClose)
{
	imgList = new iArray(freeImg);
	this->methodOpen = methodOpen;
	this->methodClose = methodClose;
}

iPopup::~iPopup()
{
	delete imgList;
}

void iPopup::freeImg(void* data)
{
	iImage* img = (iImage*)data;
	delete img;
}

void iPopup::show(bool show)
{
}

void iPopup::paint(float dt)
{
	iPoint off = closePosition;

	int i, num = imgList->count;
	for (i = 0; i < num; i++)
	{
		iImage* img = (iImage*)imgList->objectAtIndex(i);
		img->paint(dt, off);
	}
}
