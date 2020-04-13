#include "iStd.h"

uint8 float2uint8(float f)
{
    return f * 0xFF;
}


int monitorSizeW, monitorSizeH;
iSize devSize;
iRect viewport;
static float _r, _g, _b, _a;

char* _stringName;
float _stringSize;
float _stringR, _stringG, _stringB, _stringA;
float _stringBorder;
float _stringBorderR, _stringBorderG, _stringBorderB, _stringBorderA;

DWORD prevTickCount;

static bool* keys;
static uint32 keyDown, keyStat;
uint32 getKeyDown() { return keyDown; }
bool getKeyDown(uint32 key) { return keyDown & key; }
uint32 getKeyStat() { return keyStat; }
bool getKeyStat(uint32 key) { return keyStat & key; }


void loadLib(HDC hDC)
{
    setupOpenGL(true,hDC);
    initOpenGL();
    devSize = iSizeMake(DEVSIZE_WIDTH, DEVSIZE_HEIGHT);
    reshapeOpenGL(monitorSizeW, monitorSizeH);
	//monitorSizeW,H : App.cpp

 
    _r = 1.0f;
    _g = 1.0f;
    _b = 1.0f;
    _a = 1.0f;

    _stringName = NULL;
    setStringName("궁서체");
    setStringSize(10);
    setStringRGBA(1, 1, 1, 1);
    setStringBorder(1);
    setStringBorderRGBA(0, 0, 0, 1);

    glEnable(GL_LINE_SMOOTH);
	
    prevTickCount = GetTickCount();

    keys = (bool*)calloc(sizeof(bool), 256);
    keyDown = 0;
    keyStat = 0;

    srand(time(NULL));
    void sRandom();
    sRandom();
}

void freeLib()
{
    setupOpenGL(false, NULL);

    free(keys);
}


void drawLib(Method_Paint method)
{
    DWORD d = GetTickCount();
    float delta = (d - prevTickCount)/1000.f;
    prevTickCount = d;
	
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

    method(delta);

    keyDown = 0;   


}

static void keyLib(uint32& key, iKeyState stat, int c)
{
    if (stat == iKeyStateBegan)
    {
        switch (c) {
        case 'a':case 'A':
        case VK_LEFT:   key |= keyboard_left; break;
        case 'd':case 'D':
        case VK_RIGHT:  key |= keyboard_right; break;
        case 'w':case 'W':
        case VK_UP:     key |= keyboard_up; break;
        case 's':case 'S':
        case VK_DOWN:   key |= keyboard_down; break;

        case VK_SPACE:  key |= keyboard_space; break;
        }
    }
    else if (stat == iKeyStateEnded)
    {
        switch (c) {
        case 'a':case 'A':
        case VK_LEFT:   key &= ~keyboard_left; break;
        case 'd':case 'D':
        case VK_RIGHT:  key &= ~keyboard_right; break;
        case 'w':case 'W':
        case VK_UP:     key &= ~keyboard_up; break;
        case 's':case 'S':
        case VK_DOWN:   key &= ~keyboard_down; break;

        case VK_SPACE:  key &= ~keyboard_space; break;
        }
    }
}

void keyLib(iKeyState stat, int c)
{
    if (stat == iKeyStateBegan)
    {
        xprintf("%d\n", c);
        if (keys[c]==false)
            keyLib(keyDown, stat, c);
        keys[c] = true;
    }
    else if (stat == iKeyStateEnded)
    {
        keys[c] = false;
    }
    keyLib(keyStat, stat, c);
}

void resizeLib(int width, int height)
{
    reshapeOpenGL(width, height);
 
}


void setRGBA(float r, float g, float b, float a)
{
    _r = r;
    _g = g;
    _b = b;
    _a = a;
}

void setLineWidth(float lineWidth)
{
    glLineWidth(lineWidth);
}

void convertGrey(uint8* rgba, int width, int height, int potWidth)
{
	for(int j =0; j < height; j++)
	{
		for(int i = 0; i< width; i++)
		{
            uint8* c = &rgba[potWidth * 4 * j + 4 * i];
            uint8 grey = (299 * c[0] + 587 * c[1] + 114* c[2]) / 1000.f;
            c[0] = grey;
            c[1] = grey;
            c[2] = grey;
			
		}
	}
	
}

Texture* createGreyImage(const char* szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);

    char szText[1024];
    _vsnprintf(szText, sizeof(szText), szFormat, args);
    va_end(args);

    wchar_t* ws = utf8_to_utf16(szText);
    Bitmap* bmp = new Bitmap(ws);
    free(ws);

    int width, height;
    uint8* rgba = bmp2rgba(bmp, width, height);

    delete bmp;
    convertGrey(rgba, width, height, nextPot(width));
    Texture* tex = createImageWithRGBA(rgba, width, height);
    free(rgba);

    return tex;
}

Texture** createDivideImage(int wNum, int hNum, const char* szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);

    char szText[1024];
    _vsnprintf(szText, sizeof(szText), szFormat, args);
    va_end(args);
    
    wchar_t* ws = utf8_to_utf16(szText);
    Bitmap* bmp = new Bitmap(ws);
    free(ws);

    int width, height;
    uint8* rgba = bmp2rgba(bmp, width, height);
    delete bmp;

	


    int potWidth = nextPot(width);
    int potHeight = nextPot(height);
	
    int num = wNum * hNum;

    int w = width / wNum;
    int h = height / hNum;
    int potW = nextPot(w); // OpenGL에서는 2의 승수로 저장되기 때문에 크기를 2의 승수로
    int potH = nextPot(h); // 3일때 4로... 5일떄 8로...

    Texture** texs = (Texture**)malloc(sizeof(Texture*) * num);
#if 1
    int* RGBA = (int*)rgba; // 방금 쪼갠거
    int* buf = (int*)calloc(sizeof(int) , potW * potH); // 위에껄 2의 승수로 저장할 버퍼 크기를 4바이트로 써야하기때매 쉽게 int포인터로 함

	for(int j = 0; j<hNum; j++)
	{
        for (int i = 0; i < wNum; i++)
        {
            //buf <= RGBA
            // w * i , h * j 만큼

            for (int n = 0; n < h; n++)
            {
                for (int m = 0; m < w; m++)
                {

                   // buf[(potW * n) + m] = RGBA[potWidth * (h * j + n) + w * i + m]; // potW를 해주는 이유는 2의 승수로 저장하는 특성떄문임.
                                          //RGBA[potWidth * (h * j + n) + w * i + m]; // potWidth를 곱하는 이유는 세로로 봤을땐 한줄마다의 오프셋이 potwidth 이기 때문임.

                    memcpy(&buf[potW * n], &RGBA[potWidth * (h * j + n) + w * i], sizeof(int) * w);
                	
                }
				texs[wNum* j + i] = createImageWithRGBA((GLubyte*)buf, w, h);
            }


			 
		}
	}

	
#endif

#if 0

    uint8* buf = (uint8*)malloc(sizeof(uint8) * potW * potH * 4);
	

    for(int j = 0; j<hNum; j++)
    {
        for (int i = 0; i < wNum; i++)
        {
            memset(buf, 0x00, sizeof(uint8) * potW * potH * 4);
            for (int k = 0; k < h; k++)
                memcpy(&buf[potW * 4 * k], &rgba[potWidth * 4 * (h * j + k) + w * 4 * i], sizeof(uint8) * w * 4);

            //rgba => buf (이미지를 각각 쪼개서 버퍼에 저장함)
            texs[wNum* j +i] = createImageWithRGBA(buf, w, h);
        }
    }

#endif 
    free(buf);
	
    return texs;
}

uint8* convertReflect(uint8* rgba, int width, int& height, int potWidth, float rateY)
{
	// ex : 강가에 비친 게임 오브젝트의 모습을 구현하기 위한 ...
    // rateY 1.4
    //
    int newHeight = height * rateY;
    int newPotHeight = nextPot(newHeight);
    uint8* buf = (uint8*)calloc(sizeof(uint8), potWidth * newPotHeight * 4);

	memcpy(buf, rgba, sizeof(uint8)*potWidth*height*4);
    int remainHeight = newPotHeight - height;

	for(int j = 0; j <remainHeight; j++)
	{
        int J = height * (1.0f * j / remainHeight);
        uint8 a = 0xFF * (1.0f - 1.0f * j / remainHeight);
		
		for(int i =0; i<width; i++)
		{
            uint8* dst = &buf[potWidth * 4 * (height + j) * 4 * i];
            uint8* src = &rgba[potWidth * 4 * (height - 1 - J) + 4 * i];
            memcpy(dst, src, sizeof(uint8) * 3);
            dst[3] = a;
		}
	}
 
    height = newHeight;

    return buf;
}
Texture* createReflectImage(float rateY, const char* szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);

    char szText[1024];
    _vsnprintf(szText, sizeof(szText), szFormat, args);
    va_end(args);

    wchar_t* ws = utf8_to_utf16(szText);
    Bitmap* bmp = new Bitmap(ws);
    free(ws);

    int width, height;
    uint8* rgba = bmp2rgba(bmp, width, height);

    delete bmp;

	uint8* tmp = convertReflect(rgba, width, height, nextPot(width), rateY);
    free(rgba);
    Texture* tex = createImageWithRGBA(tmp, width, height);
    free(tmp);
    
    return tex;
	
}

void getRGBA(float& r, float& g, float& b, float& a)
{
    r = _r;
    g = _g;
    b = _b;
    a = _a;
}
void drawLine(iPoint sp, iPoint ep)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    iPoint position[2] =
    {
       sp,ep
    };
    float color[2][4] = { {_r,_g,_b,_a} , {_r,_g,_b,_a} };

    glVertexPointer(2, GL_FLOAT, 0, position);
    glColorPointer(4, GL_FLOAT, 0, color);
    glDrawArrays(GL_LINES, 0, 2);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void drawLine(float x0, float y0, float x1, float y1)
{
    drawLine(iPointMake(x0, y0), iPointMake(x1, y1));
}

static void drawPoly(iPoint* poly, int num, bool fill)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
	//float color[num][4];
    float color[4][4] = { {_r,_g,_b,_a } , {_r,_g,_b,_a } , {_r,_g,_b,_a }, {_r,_g,_b,_a } };

	glVertexPointer(2, GL_FLOAT, 0, poly);
    glColorPointer(4, GL_FLOAT, 0, color);

    glDrawArrays(fill ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, num);
    //glDrawElements;
	
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
	
}

void drawRect(float x, float y, float width, float height, float radius)
{
    iPoint p[4] = {
         {x, y},                 // top|left
         {x, y + height},        // bottom|left
         {x + width, y + height},// bottom|right
         {x + width, y}          // top|right
    };
    drawPoly(p, 4, false);
}


void drawRect(iRect rt, float radius) // opengl1.x 대에서 radius는 무시함
{
    fillRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}


void fillRect(float x, float y, float width, float height, float radius)
{
    iPoint p[4] = { {x,y},{ x,y + height}, {x + width, y + height} , {x + width, height} };
    drawPoly(p, 4, true);
}

void fillRect(iRect rt, float radius)
{
    fillRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}

uint8* bmp2rgba(Bitmap* bmp, int& width, int& height)
{
    width = bmp->GetWidth();
    height = bmp->GetHeight();

    int potWidht = nextPot(width);
    int potHeight = nextPot(height);

    Rect rt(0, 0, width, height);
    BitmapData bmpData;
	bmp->LockBits(&rt, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);
    int stride = bmpData.Stride / 4;
	uint8* argb= (uint8*)bmpData.Scan0;
    uint8* rgba = (uint8*)calloc(sizeof(uint8) , potWidht * potHeight * 4);

    for(int j = 0; j < height; j++)
    {
	    for(int i =0 ; i <width; i++)
	    {
            uint8* dst = &rgba[potWidht * 4 * j + 4 * i]; // OpenGL에서는 2의 승수
            uint8* src = &argb[stride * 4 * j + 4 * i]; // Gdi+에서는 4단위로

            //dst[0] = src[1];
            //dst[1] = src[2];
            //dst[2] = src[3];
            //dst[3] = src[0];
            //memcpy(dst, &src[1], 3); 
			//dst[3] = src[0];

            dst[0] = src[2]; // bgra 임.. 버그인듯
            dst[1] = src[1];
            dst[2] = src[0];
            dst[3] = src[3];
	    }   
    }
	
    bmp->UnlockBits(&bmpData);
	
    return rgba;
}

Texture* createImage(const char* szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);

    char szText[1024];
    _vsnprintf(szText, sizeof(szText), szFormat, args);
    va_end(args);

    wchar_t* ws = utf8_to_utf16(szText);
    Bitmap* bmp = new Bitmap(ws);
    free(ws);

    int width, height;
    uint8* rgba = bmp2rgba(bmp, width, height);

    delete bmp;

    Texture* tex = createImageWithRGBA(rgba, width, height);
    free(rgba);

	return tex;
}
void freeImage(Texture* tex)
{
    if (tex->retainCount > 1)
    {
        tex->retainCount--;
        return;
    }
    
    glDeleteTextures(1, &tex->texID); // 복수일수도 잇어서 주소값을 넘긴다.
    free(tex);
}

void drawImage(Texture* tex, int x, int y, int anc)
{
    drawImage(tex, x, y,
        0, 0, tex->width, tex->height,
        anc, 1.0f, 1.0f, 2, 0, REVERSE_NONE);
}

void drawImage(Texture* tex, int x, int y,
    int ix, int iy, int iw, int ih, int anc,
    float ratX, float ratY,
    int xyz, float degree, int reverse)
{
    int width = tex->width * ratX;
    int height = tex->height * ratY;
    switch (anc) {
    case TOP | LEFT:                                         break;
    case TOP | HCENTER:     x -= width / 2;                  break;
    case TOP | RIGHT:       x -= width;                      break;
    case VCENTER | LEFT:                    y -= height / 2; break;
    case VCENTER | HCENTER: x -= width / 2; y -= height / 2; break;
    case VCENTER | RIGHT:   x -= width;     y -= height / 2; break;
    case BOTTOM | LEFT:                     y -= height;     break;
    case BOTTOM | HCENTER:  x -= width / 2; y -= height;     break;
    case BOTTOM | RIGHT:    x -= width;     y -= height;     break;
    }

    iPoint position[4] = {
        {x, y},             // top|left
        {x, y + height},      // bottom|left,  
        {x + width, y},       // top|right
        {x + width, y + height} // bottom|right
    };
#if 0
    iPoint coordinate[4] = {
        {0.0, 0.0},
        {0.0, tex->height / tex->potHeight},
        {tex->width / tex->potWidth, 0.0},
        {tex->width / tex->potWidth, tex->height / tex->potHeight}
    };
#else
    iPoint coordinate[4] = {
        {ix / tex->potWidth, iy / tex->potHeight},
        {ix / tex->potWidth, (iy + ih) / tex->potHeight},
        {(ix + iw) / tex->potWidth, iy / tex->potHeight},
        {(ix + iw) / tex->potWidth, (iy + ih) / tex->potHeight}
    };
#endif
    float color[4][4] = {
        {_r, _g, _b, _a},
        {_r, _g, _b, _a},
        {_r, _g, _b, _a },
        {_r, _g, _b, _a} };
    if (reverse == REVERSE_WIDTH)
    {
        iPoint t;
        for (int i = 0; i < 2; i++)
        {
            t = position[i];
            position[i] = position[i + 2];
            position[i + 2] = t;
        }
    }
    else if (reverse == REVERSE_HEIGHT)
    {
        iPoint t;
        for (int i = 0; i < 2; i++)
        {
            t = position[2 * i];
            position[2 * i] = position[2 * i + 1];
            position[2 * i + 1] = t;
        }
    }

    glPushMatrix();
    if (degree)
    {
        iPoint t = iPointMake(x + width / 2, y + height / 2);
        for (int i = 0; i < 4; i++)
            position[i] -= t;
        glTranslatef(t.x, t.y, 0);

        float _xyz[3] = { 0, 0, 0 };
        _xyz[xyz] = 1.0f;
        while (degree > 360) degree -= 360;
        degree = 360 - degree;
        glRotatef(degree, _xyz[0], _xyz[1], _xyz[2]);
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex->texID);

    glVertexPointer(2, GL_FLOAT, 0, position);
    glTexCoordPointer(2, GL_FLOAT, 0, coordinate);
    glColorPointer(4, GL_FLOAT, 0, color);

#if 1
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#elif 1
    uint8 indices[4] = { 0, 1, 2, 3 };
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
#else
    uint8 indices[6] = { 0, 1, 2,  1, 2, 3 };
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
#endif

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();

#if 0
    Image* img = (Image*)tex->texID;
    Rect destinationRect(x, y, width, height);
#if 0
    g->DrawImage(
        img,
        destinationRect,
        ix, iy, iw, ih,
        UnitPixel);
#endif
    iPoint destPoint[3] = {
        {x, y}, {x + width, y}, {x, y + height}
    };
    if (reverse == REVERSE_WIDTH)
    {
        destPoint[2].x = destPoint[1].x;

        iPoint t = destPoint[0];
        destPoint[0] = destPoint[1];
        destPoint[1] = t;
    }
    else if (reverse == REVERSE_HEIGHT)
    {
        destPoint[1].y = destPoint[2].y;

        iPoint t = destPoint[0];
        destPoint[0] = destPoint[2];
        destPoint[2] = t;
    }

    if (xyz == 0)// x축으로 회전
    {
        destPoint[0].y =
            destPoint[1].y = y + height / 2 - height / 2 * _cos(degree);
        destPoint[2].y = y + height / 2 + height / 2 * _cos(degree);
    }
    else if (xyz == 1)// y축으로 회전
    {
        destPoint[0].x =
            destPoint[2].x = x + width / 2 - width / 2 * _cos(degree);
        destPoint[1].x = x + width / 2 + width / 2 * _cos(degree);
}
    else if (xyz == 2)// z축으로 회전
    {
        iPoint t = iPointMake(x + width / 2, y + height / 2);
        for (int i = 0; i < 3; i++)
            destPoint[i] = iPointRotate(destPoint[i], t, degree);
    }

    ColorMatrix matrix = { _r,   0.0f, 0.0f, 0.0f, 0.0f,
                          0.0f, _g,   0.0f, 0.0f, 0.0f,
                          0.0f, 0.0f, _b,   0.0f, 0.0f,
                          0.0f, 0.0f, 0.0f, _a,   0.0f,
                          0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    ImageAttributes attr;
    attr.SetColorMatrix(&matrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
    g->DrawImage(img,
        (PointF*)destPoint, 3,
        ix, iy, iw, ih, UnitPixel, &attr);
#endif
}



void setClip(int x, int y, int width, int height)
{
    if (x == 0 && y == 0 && width == 0 && height == 0)
        glDisable(GL_SCISSOR_TEST);

    else
    {
        glEnable(GL_SCISSOR_TEST);

    	// monitor 영역 #bug
    	
        glScissor(x, y, width, height);
    }
    
}

void setClip(iRect rt)
{
    setClip(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height);
}

void setStringName(const char* name)
{
    if (_stringName)
    {
        if (strcmp(_stringName, name) == 0)
            return;
        free(_stringName);
    }

    char* s = (char*)calloc(sizeof(char), 1 + strlen(name));
    strcpy(s, name);
    _stringName = s;
}

const char* getStringName() { return _stringName; }
float getStringSize() { return _stringSize; }

void setStringSize(float size)
{
    _stringSize = size;
}

void setStringRGBA(float r, float g, float b, float a)
{
    _stringR = r;
    _stringG = g;
    _stringB = b;
    _stringA = a;
}

void getStringRGBA(float& r, float& g, float& b, float& a)
{
    r = _stringR;
    g = _stringG;
    b = _stringB;
    a = _stringA;
}

void setStringBorder(float size)
{
    _stringBorder = size;
}

float getStringBorder()
{
    return _stringBorder;
}

void setStringBorderRGBA(float r, float g, float b, float a)
{
    _stringBorderR = r;
    _stringBorderG = g;
    _stringBorderB = b;
    _stringBorderA = a;
}
void getStringBorderRGBA(float& r, float& g, float& b, float& a)
{
    r = _stringBorderR;
    g = _stringBorderG;
    b = _stringBorderB;
    a = _stringBorderA;
}

iRect rectOfString(uint8* rgba, int width, int height)
{
    iRect rt;

    // 왼쪽에서 오른쪽 찾아감
    for (int i = 0; i < width; i++)
    {
        bool found = false;
        for (int j = 0; j < height; j++)
        {
            if (rgba[width * 4 * j + 4 * i + 3])
            {
                rt.origin.x = i;
                found = true;
                break;
            }
        }
        if (found)
            break;
    }

    // 오른쪽에서 왼쪽 찾아감
    for (int i = width - 1; i > -1; i--)
    {
        bool found = false;
        for (int j = 0; j < height; j++)
        {
            if (rgba[width * 4 * j + 4 * i + 3])
            {
                rt.size.width = i - rt.origin.x - 1;
                found = true;
                break;
            }
        }
        if (found)
            break;
    }

    // 위에서 아래로 찾아감
    for (int j = 0; j < height; j++)
    {
        bool found = false;
        for (int i = 0; i < width; i++)
        {
            if (rgba[width * 4 * j + 4 * i + 3])
            {
                rt.origin.y = j;
                found = true;
                break;
            }
        }
        if (found)
            break;
    }

    // 아래에서 위로 찾아감
    for (int j = height - 1; j > -1; j--)
    {
        bool found = false;
        for (int i = 0; i < width; i++)
        {
            if (rgba[width * 4 * j + 4 * i + 3])
            {
                rt.size.height = j - rt.origin.y - 1;
                found = true;
                break;
            }
        }
        if (found)
            break;
    }

    return rt;
}

iSize sizeOfString(const char* szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);

    char szText[1024];
    _vsnprintf(szText, sizeof(szText), szFormat, args);
    va_end(args);

    wchar_t* wStringName = utf8_to_utf16(_stringName);
    wchar_t* wStr = utf8_to_utf16(szText);

    GraphicsPath path;
    FontFamily  fontFamily(wStringName);
    PointF      pointF(0, 0);
    StringFormat sf;// top|left
    //sf.SetFormatFlags();

    GraphicsPath p;
    p.AddString(wStr, lstrlenW(wStr), &fontFamily, FontStyleRegular,
        _stringSize, pointF, &sf);
    Rect rt;
    p.GetBounds(&rt);

    free(wStringName);
    free(wStr);

    return iSizeMake(rt.Width, rt.Height);
}



#define W 32
#define R 16
#define P 0
#define M1 13
#define M2 9
#define M3 5

#define MAT0POS(t,v) (v^(v>>t))
#define MAT0NEG(t,v) (v^(v<<(-(t))))
#define MAT3NEG(t,v) (v<<(-(t)))
#define MAT4NEG(t,b,v) (v ^ ((v<<(-(t))) & b))

#define V0            STATE[state_i                   ]
#define VM1           STATE[(state_i+M1) & 0x0000000fU]
#define VM2           STATE[(state_i+M2) & 0x0000000fU]
#define VM3           STATE[(state_i+M3) & 0x0000000fU]
#define VRm1          STATE[(state_i+15) & 0x0000000fU]
#define VRm2          STATE[(state_i+14) & 0x0000000fU]
#define newV0         STATE[(state_i+15) & 0x0000000fU]
#define newV1         STATE[state_i                 ]
#define newVRm1       STATE[(state_i+14) & 0x0000000fU]

#define FACT 2.32830643653869628906e-10

static unsigned int state_i = 0;
static unsigned int STATE[R];
static unsigned int z0, z1, z2;

void drawString(int x, int y, int anc, const char* szFormat, ...)
{
	//#bug
}

void sRandom() {
    int j;
    state_i = 0;
    for (j = 0; j < R; j++)
        STATE[j] = rand();
}

uint32 random()
{
#if 1
    return rand();
#else
    z0 = VRm1;
    z1 = MAT0NEG(-16, V0) ^ MAT0NEG(-15, VM1);
    z2 = MAT0POS(11, VM2);
    newV1 = z1 ^ z2;
    newV0 = MAT0NEG(-2, z0) ^ MAT0NEG(-18, z1) ^ MAT3NEG(-28, z2) ^ MAT4NEG(-5, 0xda442d24U, newV1);
    state_i = (state_i + 15) & 0x0000000fU;
    return STATE[state_i] * FACT;
#endif
}

float _sin(float degree)
{
    return sin(M_PI / 180 * degree);
}

float _cos(float degree)
{
    return cos(M_PI / 180 * degree);
}

float linear(float r, float a, float b)
{
    return a + (b - a) * r;
}

iPoint linear(float r, iPoint a, iPoint b)
{
    return a + (b - a) * r;
}

float easeIn(float r, float a, float b)
{
    return a + (b - a) * _sin(90 * r);
}

iPoint easeIn(float r, iPoint a, iPoint b)
{
    return a + (b - a) * _sin(90 * r);
}

float easeOut(float r, float a, float b)
{
    return a + (b - a) * (1 + _sin(270 + 90 * r));
    //return a + (b - a) * r * r;
}

iPoint easeOut(float r, iPoint a, iPoint b)
{
    return a + (b - a) * (1 + _sin(270 + 90 * r));
    //return a + (b - a) * r * r;
}

float easeInOut(float r, float a, float b)
{
    if (r < 0.5f)
        easeIn(r * 2.0f, a, a + (b - a) / 2);
    return easeOut((r - 0.5f) * 2.0f, a + (b - a) / 2, b);
}

iPoint easeInOut(float r, iPoint a, iPoint b)
{
    if (r < 0.5f)
        easeIn(r * 2.0f, a, a + (b - a) / 2);
    return easeOut((r - 0.5f) * 2.0f, a + (b - a) / 2, b);
}

bool containPoint(iPoint p, iRect rt)
{
    if (p.x < rt.origin.x ||
        p.x > rt.origin.x + rt.size.width ||
        p.y < rt.origin.y ||
        p.y > rt.origin.y + rt.size.height)
        return false;
    return true;
}

bool containRect(iRect src, iRect dst)
{
    if (src.origin.x + src.size.width < dst.origin.x ||
        src.origin.x > dst.origin.x + dst.size.width ||
        src.origin.y + src.size.height < dst.origin.y ||
        src.origin.y > dst.origin.y + dst.size.height)
        return false;

    return true;
}

char* loadFile(const char* filePath, int& length)
{
    FILE* pf = fopen(filePath, "rb");

    fseek(pf, 0, SEEK_END);
    length = ftell(pf);
    char* buf = (char*)calloc(sizeof(char), 1 + length);
    fseek(pf, 0, SEEK_SET);//rewind(pf);
    fread(buf, length, 1, pf);

    fclose(pf);

    return buf;
}

void saveFile(const char* filePath, char* buf, int bufLength)
{
    FILE* pf = fopen(filePath, "wb");

    fwrite(buf, bufLength, 1, pf);

    fclose(pf);
}



// 한점에서 선분까지의 거리
float getDistanceLine1(iPoint p, iPoint sp, iPoint ep)
{
    iPoint n = ep - sp;
    float len = sqrtf(n.x * n.x + n.y * n.y);
    n /= len;

    iPoint m = p - sp;
    iPoint proj = n * max(0.0f, min((m.x * n.x + m.y * n.y), len));

    return iPointLength(m - proj);
}