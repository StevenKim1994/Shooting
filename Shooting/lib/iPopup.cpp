#include "iPopup.h"

#include "iStd.h"

iPopup::iPopup(iPopupStyle style)
{
	arrayImg = new iArray(freeImg);

	this->style = style;
	stat = iPopupStatClose;
	bShow = false;
	_showDt = default_showDt;
	showDt = 0.0f;

	openPosition = iPointMake(0, 0);
	closePosition = iPointMake(0, 0);
	methodOpen = NULL;
	methodClose = NULL;
	methodDrawBefore = NULL;
	methodDrawAfter = NULL;

	selected = -1;
}

iPopup::~iPopup()
{
	delete arrayImg;
}

void iPopup::freeImg(void* data)
{
	iImage* img = (iImage*)data;
	delete img;
}

void iPopup::addObject(void* img)
{
	arrayImg->addObject(img);
}

void iPopup::show(bool show)
{
	if (show)
	{
		bShow = true;
		if (stat != iPopupStatClose)
			return;
		stat = iPopupStatOpen;
	}
	else
	{
		//bShow = false;
		if (stat != iPopupStatProc)
			return;
		stat = iPopupStatClose;
	}
	showDt = 0.0f;
}

void iPopup::paint(float dt)
{
	if (bShow == false)
		return;

	float r, g, b, a;
	getRGBA(r, g, b, a);

	if (style == iPopupStyleNone)
	{
		iPoint off = closePosition;

		setRGBA(1, 1, 1, 1.0f);
		if (methodDrawBefore)
			methodDrawBefore(this, off, iPointMake(1, 1), 1.0f);

		int i, num = arrayImg->count;
		for (i = 0; i < num; i++)
		{
			iImage* img = (iImage*)arrayImg->objectAtIndex(i);
			img->paint(dt, off);
		}

		if (methodDrawAfter)
			methodDrawAfter(this, off, iPointMake(1, 1), 1.0f);
	}
	else if (style == iPopupStyleAlpha)
	{
		float alpha;
		iPoint off;
		if (stat == iPopupStatOpen)
		{
			alpha = linear(showDt / _showDt, 0.0f, 1.0f);
			off = linear(showDt / _showDt, openPosition, closePosition);
			showDt += dt;
			if (showDt >= _showDt)
			{
				stat = iPopupStatProc;
				if (methodOpen)
					methodOpen(this);
			}
		}
		else if (stat == iPopupStatProc)
		{
			alpha = 1.0f;
			off = closePosition;
		}
		else// if (stat == iPopupStatClose)
		{
			alpha = linear(showDt / _showDt, 1.0f, 0.0f);
			off = linear(showDt / _showDt, closePosition, openPosition);
			showDt += dt;
			if (showDt >= _showDt)
			{
				bShow = false;
				if (methodClose)
					methodClose(this);
			}
		}

		setRGBA(1, 1, 1, alpha);
		if (methodDrawBefore)
			methodDrawBefore(this, off, iPointMake(1, 1), alpha);

		int i, num = arrayImg->count;
		for (i = 0; i < num; i++)
		{
			iImage* img = (iImage*)arrayImg->objectAtIndex(i);
			img->paint(dt, off);
		}

		if (methodDrawAfter)
			methodDrawAfter(this, off, iPointMake(1, 1), alpha);
	}
	else if (style == iPopupStyleMove)
	{
		iPoint off;
		if (stat == iPopupStatOpen)
		{
			off = easeIn(showDt / _showDt, openPosition, closePosition);
			showDt += dt;
			if (showDt >= _showDt)
			{
				stat = iPopupStatProc;
				if (methodOpen)
					methodOpen(this);
			}
		}
		else if (stat == iPopupStatProc)
		{
			off = closePosition;
		}
		else// if (stat == iPopupStatClose)
		{
			off = easeOut(showDt / _showDt, closePosition, openPosition);
			showDt += dt;
			if (showDt >= _showDt)
			{
				bShow = false;
				if (methodClose)
					methodClose(this);
			}
		}

		setRGBA(1, 1, 1, 1.0f);
		if (methodDrawBefore)
			methodDrawBefore(this, off, iPointMake(1, 1), 1.0f);

		int i, num = arrayImg->count;
		for (i = 0; i < num; i++)
		{
			iImage* img = (iImage*)arrayImg->objectAtIndex(i);
			img->paint(dt, off);
		}

		if (methodDrawAfter)
			methodDrawAfter(this, off, iPointMake(1, 1), 1.0f);
	}

	setRGBA(r, g, b, a);
}
