#include "iImage.h"

#include "iStd.h"

void freeImg(void* data)
{
	Texture* tex = (Texture*)data;
	if (tex->retainCount > 1)
	{
		tex->retainCount--;
		return;
	}
	freeImage(tex);
}

iImage::iImage()
{
	arrayImg = new iArray(freeImg);
	tex = NULL;
	position = iPointMake(0, 0);
	animation = false;
	aniDt = 0.0f;
	_aniDt = 0.08f;
	frame = 0;
	repeatNum = 0;// inf
	method = NULL;
}

iImage::~iImage()
{
	delete arrayImg;
}

void iImage::addObject(Texture* tex)
{
	arrayImg->addObject(tex);
	tex->retainCount++;
	if (this->tex == NULL)
		this->tex = tex;
}

void iImage::setTexAtIndex(int index)
{
	Texture* tex = (Texture*)arrayImg->objectAtIndex(index);
	if (tex)
	{
		this->tex = tex;
		frame = index;
	}
}

void iImage::paint(float dt, iPoint off)
{
	if (animation)
	{
		aniDt += dt;
		if (aniDt < _aniDt)
		{
			;
		}
		else
		{
			aniDt -= _aniDt;
			frame++;
			if (frame == arrayImg->count)
			{
				if( repeatNum==0 )
					frame = 0;
				else
				{
					if (method)
						method(this);
					if (lastFrame)
						frame = arrayImg->count - 1;
					else
						frame = 0;
					animation = false;
				}
				tex = (Texture*)arrayImg->objectAtIndex(frame);
			}
		}
	}

	iPoint p = position + off;
	drawImage(tex, p.x, p.y, TOP | LEFT);
}

void iImage::startAnimation(IMAGE_METHOD m)
{
	animation = true;
	method = m;
	aniDt = 0.0f;
	frame = 0;
}

iRect iImage::touchRect(iPoint p)
{
	p += position;
	iRect rt = iRectMake(p.x, p.y,
						tex->width, tex->height);
	return rt;
}
