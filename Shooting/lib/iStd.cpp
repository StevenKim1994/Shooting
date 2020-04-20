#include "iStd.h"

uint8 float2uint8(float f)
{
    return f * 0xFF;
}

static Bitmap* bitmap;
//static
Graphics* g;
static Graphics* gFromHDC = NULL;
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

//void setG(Graphics* _g)
//{
//    g = _g;
//}
//
//Graphics* getG()
//{
//    return g;
//}

void loadLib(HDC hDC)
{
    devSize = iSizeMake(DEVSIZE_WIDTH, DEVSIZE_HEIGHT);
    setBackBuffer(&bitmap, &g, devSize);

    gFromHDC = new Graphics(hDC);
    gFromHDC->Clear(Color(255, 0, 0, 0));

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
    delete bitmap;
    delete g;
    delete gFromHDC;

    free(keys);
}

void drawLib(Method_Paint method)
{
    DWORD d = GetTickCount();
    float delta = (d - prevTickCount)/1000.f;
    prevTickCount = d;

    // g : bitmap
    method(delta);
    keyDown = 0;

    // gFromHDC 
    int width = bitmap->GetWidth();
    int height = bitmap->GetHeight();
    Rect destinationRect(viewport.origin.x, viewport.origin.y,
        viewport.size.width, viewport.size.height);
    gFromHDC->DrawImage(
        bitmap,
        destinationRect,
        0, 0, width, height,
        UnitPixel);
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
    if(gFromHDC)
        gFromHDC->Clear(Color(255, 0, 0, 0));

    float r0 = devSize.width / devSize.height;

    monitorSizeW = width;
    monitorSizeH = height;
    float r1 = 1.0f * width / height;

    if (r0 < r1)// 모니터 가로가 큰 경우
    {
        viewport.origin.y = 0; viewport.size.height = height;
        viewport.size.width = devSize.width * height / devSize.height;
        viewport.origin.x = (width - viewport.size.width) / 2;
    }
    else if (r0 > r1)// 모니터 가로가 작은 경우(세로 긴 경우)
    {
        viewport.origin.x = 0; viewport.size.width = width;
        viewport.size.height = devSize.height * width / devSize.width;
        viewport.origin.y = (height - viewport.size.height) / 2;
    }
    else// 정비율
    {
        viewport = iRectMake(0, 0, width, height);
    }
}


void setRGBA(float r, float g, float b, float a)
{
    _r = r;
    _g = g;
    _b = b;
    _a = a;
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
    drawLine(sp.x, sp.y, ep.x, ep.y);
}

void drawLine(float x0, float y0, float x1, float y1)
{
    Pen pen(Color(  float2uint8(_a),
                    float2uint8(_r),
                    float2uint8(_g),
                    float2uint8(_b) ));
    g->DrawLine(&pen, x0, y0, x1, y1);
}

void drawRect(float x, float y, float width, float height, float radius)
{
    Pen pen(Color(  float2uint8(_a),
                    float2uint8(_r),
                    float2uint8(_g),
                    float2uint8(_b)));

    GraphicsPath path;
    path.AddLine(x + radius, y, x + width - (radius * 2), y);
    path.AddArc(x + width - (radius * 2), y, radius * 2, radius * 2, 270, 90);
    path.AddLine(x + width, y + radius, x + width, y + height - (radius * 2));
    path.AddArc(x + width - (radius * 2), y + height - (radius * 2), radius * 2, radius * 2, 0, 90);
    path.AddLine(x + width - (radius * 2), y + height, x + radius, y + height);
    path.AddArc(x, y + height - (radius * 2), radius * 2, radius * 2, 90, 90);
    path.AddLine(x, y + height - (radius * 2), x, y + radius);
    path.AddArc(x, y, radius * 2, radius * 2, 180, 90);
    path.CloseFigure();

    g->DrawPath(&pen, &path);
}

void drawRect(iRect rt, float radius)
{
    drawRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}


void fillRect(float x, float y, float width, float height, float radius)
{
    SolidBrush brush(Color( float2uint8(_a),
                            float2uint8(_r),
                            float2uint8(_g),
                            float2uint8(_b)));
    GraphicsPath path;
    path.AddLine(x + radius, y, x + width - (radius * 2), y);
    path.AddArc(x + width - (radius * 2), y, radius * 2, radius * 2, 270, 90);
    path.AddLine(x + width, y + radius, x + width, y + height - (radius * 2));
    path.AddArc(x + width - (radius * 2), y + height - (radius * 2), radius * 2, radius * 2, 0, 90);
    path.AddLine(x + width - (radius * 2), y + height, x + radius, y + height);
    path.AddArc(x, y + height - (radius * 2), radius * 2, radius * 2, 90, 90);
    path.AddLine(x, y + height - (radius * 2), x, y + radius);
    path.AddArc(x, y, radius * 2, radius * 2, 180, 90);
    path.CloseFigure();

    g->FillPath(&brush, &path);
}

void fillRect(iRect rt, float radius)
{
    fillRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}

uint8* bmp2rgba(Bitmap* bmp, int& width, int& height)
{
    width = bmp->GetWidth();
    height = bmp->GetHeight();
    int potWidth = nextPOT(width);
    int potHeight = nextPOT(height);

    BitmapData bmpData;
    Rect rt(0, 0, width, height);
    bmp->LockBits(&rt, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);

    int stride = bmpData.Stride / 4;
    uint8* argb = (uint8*)bmpData.Scan0;
    uint8* rgba = (uint8*)calloc(sizeof(uint8), potWidth * potHeight * 4);
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            uint8* dst = &rgba[potWidth * 4 * j + 4 * i];
            uint8* src = &argb[stride * 4 * j + 4 * i];
            dst[0] = src[2];
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
    Image* img = new Image(ws);
    free(ws);

    Texture* tex = (Texture*)malloc(sizeof(Texture));
    tex->texID = img;
    tex->width = img->GetWidth();
    tex->height = img->GetHeight();
    //tex->potWidth;
    //tex->potHeight;
    tex->retainCount = 1;

    return tex;
}
void freeImage(Texture* tex)
{
    if (tex->retainCount > 1)
    {
        tex->retainCount--;
        return;
    }
    delete tex->texID;
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
    Image* img = (Image*)tex->texID;
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
    case BOTTOM | RIGHT:    x -= width;     y -= height;     break; }

    Rect destinationRect(x, y, width, height);
#if 0
    g->DrawImage(
        img,
        destinationRect,
        ix, iy, iw, ih,
        UnitPixel);
#endif
    iPoint destPoint[3] = {
        {x, y}, {x+width, y}, {x, y+height}
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

    ColorMatrix matrix = {_r,   0.0f, 0.0f, 0.0f, 0.0f,
                          0.0f, _g,   0.0f, 0.0f, 0.0f,
                          0.0f, 0.0f, _b,   0.0f, 0.0f,
                          0.0f, 0.0f, 0.0f, _a,   0.0f,
                          0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    ImageAttributes attr;
    attr.SetColorMatrix(&matrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
    g->DrawImage(img,
        (PointF*)destPoint, 3,
        ix, iy, iw, ih, UnitPixel, &attr);
}

void setClip(int x, int y, int width, int height)
{
    Region region(Rect(x, y, width, height));
    g->SetClip(&region, CombineModeReplace);
}

void setClip(iRect rt)
{
    Region region(Rect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height));
    g->SetClip(&region, CombineModeReplace);
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

void drawString(int x, int y, int anc, const char* szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);

    char szText[1024];
    _vsnprintf(szText, sizeof(szText), szFormat, args);
    va_end(args);

    iSize size = sizeOfString(szText);
    float width = size.width;
    float height = size.height;
    switch (anc) {
    case TOP | LEFT:                                         break;
    case TOP | HCENTER:     x -= width / 2;                  break;
    case TOP | RIGHT:       x -= width;                      break;
    case VCENTER | LEFT:                    y -= height / 2; break;
    case VCENTER | HCENTER: x -= width / 2; y -= height / 2; break;
    case VCENTER | RIGHT:   x -= width;     y -= height / 2; break;
    case BOTTOM | LEFT:                     y -= height;     break;
    case BOTTOM | HCENTER:  x -= width / 2; y -= height;     break;
    case BOTTOM | RIGHT:    x -= width;     y -= height;     break; }

    wchar_t* wStr = utf8_to_utf16(szText);

    GraphicsPath path;
    FontFamily  fontFamily;
    PointF      pointF(x, y);
    StringFormat sf;

    int fontStyle;
    checkFontFamily(&fontFamily, fontStyle);

    path.AddString(wStr, lstrlenW(wStr), &fontFamily, fontStyle, 
        _stringSize, pointF, &sf);

    SolidBrush brush(Color( float2uint8(_stringA),
                            float2uint8(_stringR),
                            float2uint8(_stringG),
                            float2uint8(_stringB)));
    g->FillPath(&brush, &path);

    if (_stringBorder)
    {
        Pen pen(Color(  float2uint8(_stringBorderA),
                        float2uint8(_stringBorderR),
                        float2uint8(_stringBorderG),
                        float2uint8(_stringBorderB)));
        pen.SetWidth(_stringBorder);
        g->DrawPath(&pen, &path);
    }

    free(wStr);
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

