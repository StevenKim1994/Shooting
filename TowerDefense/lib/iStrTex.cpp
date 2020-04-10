#include "iStrTex.h"
#include "iStd.h"

iStrTex::iStrTex(method_istrtex m)
{
	method = m;
	
	strLength = 128;
	str = (char*)calloc(sizeof(char), 128);

	tex = NULL;
}

iStrTex::~iStrTex()
{
	free(str);

	if (tex)
		freeImage(tex);
}

void iStrTex::setString(const char* szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);

	char szText[1024];
	_vsnprintf(szText, sizeof(szText), szFormat, args);
	va_end(args);

	//tex update
	if (strcmp(str, szText) == 0)
		return;

	int len = strLength;
	
	while (len < strlen(szText) + 1)
		len *= 2;

	if(strLength < len)
	{
		strLength = len;
		free(str);
		str = (char*)malloc(sizeof(char) * strLength);
	
	}

	strcpy(str, szText);

	
	Texture* t = NULL;

	if (method)
	{
		// 하기전에 백업
		float gr, gg, gb, ga;
		getRGBA(gr, gg, gb, ga);	
		float size = getStringSize();
		float border = getStringBorder();
		float r, g, b, a, br, bg, bb, ba;
		getStringRGBA(r, g, b, a);
		getStringBorderRGBA(br, bg, bb, ba);
		
		t = method(str); // method가 있다면 그 method에서 문자열을 텍스처로 만듬...


		// 다시 원상태로 복구
		setRGBA(gr, gg, gb, ga);
		setStringSize(size);
		setStringBorder(border);
		setStringRGBA(r, g, b, a);
		setStringBorderRGBA(br, bg, bb, ba);
		
	}

	else
	{
		// 없을시 직접 문자를 텍스처로 만듬
		iGraphics* g = iGraphics::instance();
		iSize size = sizeOfString(str);
		g->init(size);
		g->drawString(0, 0, TOP | LEFT, str);	
		tex = g->getTexture();
	}
#if 0
	if (tex)
		freeImage(tex);
	tex = t;
#else
	if(tex)
		glDeleteTextures(1, &tex ->texID);
	else
	{
		tex = (Texture*)malloc(sizeof(Texture));
		tex->retainCount = 1;
	}
	tex->texID = t->texID;
	tex->width = t->width;
	tex->height = t->height;
	tex->potWidth = t->potWidth;
	tex->potHeight = t->potHeight;

	//tex->retainCount = t->retainCount;
	
	free(t);

#endif
}

void iStrTex::paint(int x, int y, int anc)
{
	drawImage(tex,x,y,anc);
}

void iStrTex::paint(int x, int y, int anc, const char* szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);

	char szText[1024];
	_vsnprintf(szText, sizeof(szText), szFormat, args);
	va_end(args);

	setString(szText);
	drawImage(tex, x, y, anc);
}

void iStrTex::paint(int x, int y, float rateX, float rateY, int anc)
{
	drawImage(tex, x, y, 0, 0, tex->width, tex->height, anc, rateX, rateY, 2, 0, REVERSE_NONE);
}

void iStrTex::paint(int x, int y, float rateX, float rateY, int anc, const char* szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);

	char szText[1024];
	_vsnprintf(szText, sizeof(szText), szFormat, args);
	va_end(args);

	drawImage(tex, x, y, 0, 0, tex->width, tex->height, anc, rateX, rateY, 2, 0, REVERSE_NONE);
}

