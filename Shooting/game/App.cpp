#include "App.h"

#include "Game.h"

HINSTANCE hInstance;
HWND hWnd;
HDC hDC;
bool runWnd;

WCHAR szTitle[100] = TEXT("BGA");    // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[100] = TEXT("AGB");       // 기본 창 클래스 이름입니다.

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInst,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    ULONG_PTR gpToken = startGdiplus();

    hInstance = hInst;

    WNDCLASSEXW wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHOOTING));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_SHOOTING);
    wc.lpszClassName = szWindowClass;
    wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wc);

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    hDC = GetDC(hWnd);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

#if (OPENGL==0)
    loadLib(hDC);
    loadGame();
#else
    setupOpenGL(true, hDC);
    initOpenGL();
#endif

    runWnd = true;
    MSG msg;
    while (runWnd)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
#if (OPENGL==0)
            drawLib(drawGame);
#else
            void test();
            test();
#endif
            SwapBuffers(hDC);
        }
    }

#if (OPENGL==0)
    freeLib();
    freeGame();
#else
    setupOpenGL(false, NULL);
#endif

    DestroyWindow(hWnd);
    endGdiplus(gpToken);

    return (int)msg.wParam;
}

void testDot()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    //glColor4f(1, 0, 0, 1);

    iPoint position[3] = { {-0.5, -0.5}, {0.0, 0.0}, {0.5, 0.5} };
    float color[3][4] = { {1, 0, 0, 1}, {0, 0, 1, 1}, {1, 0, 1, 1} };
    glVertexPointer(2, GL_FLOAT, 0, position);
    glColorPointer(4, GL_FLOAT, 0, color);
    glPointSize(5);

    glDrawArrays(GL_POINTS, 0, 3);
    //glDrawElements;

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void testLine()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);

#if 0
    iPoint position[2] = { {-0.5, -0.5}, {0.0, 0.0} };
#elif 0
    devSize = iSizeMake(1000, 800);
    iPoint position[2] = { {50, 50}, {devSize.width-50, devSize.height-50} };
    // left right : -1 ~ 1
    // bottom top : -1 ~ 1
    for (int i = 0; i < 2; i++)
    {
        iPoint* p = &position[i];

        // 0 ~ devSize.width => -1 ~ 1
        p->x = p->x / devSize.width * 2 - 1;
        // 0 ~ devSize.width => -1 ~ 1
        p->y = p->y / devSize.height * 2 - 1;
    }
#else
    devSize = iSizeMake(1000, 800);
    iPoint position[2] = { {100, 100},
    {devSize.width - 100, devSize.height - 100} };

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, devSize.width, devSize.height, 0, 0, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

#endif
    glVertexPointer(2, GL_FLOAT, 0, position);
    glLineWidth(5);
    glEnable(GL_LINE_SMOOTH);

    glDrawArrays(GL_LINES, 0, 2);
    //glDrawElements;

    glDisableClientState(GL_VERTEX_ARRAY);
}

void testPoli()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    iPoint position[4] = { {-0.3, -0.3}, {0.3, -0.3},
    {0.3, 0.3}, {0.0, 0.3}};
    float color[4][4] = { {1, 1, 1, 1}, {1, 0, 0, 1},
    {0, 1, 0, 1}, { 0, 0, 1, 1}
    };
    glVertexPointer(2, GL_FLOAT, 0, position);
    glColorPointer(4, GL_FLOAT, 0, color);

    //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    //glDrawElements;

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void testCircle()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    iPoint position[360];
    float color[360][4];
    for (int i = 0; i < 360; i++)
    {
        position[i] = iPointRotate(iPointMake(0.2, 0), iPointMake(0, 0), i);
        float* c = color[i];
        c[0] = (i+1)/360.;
        c[1] = ((i + 91) % 360 )/360.;
        c[2] = ((i +181) % 360) / 360.;
        c[3] = 1;
    }
    glVertexPointer(2, GL_FLOAT, 0, position);
    glColorPointer(4, GL_FLOAT, 0, color);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 360);
    //glDrawArrays(GL_LINE_LOOP, 0, 360);
    //glDrawElements;

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

uint32 nextPot(uint32 x)
{
    return x;
}
void test()
{
    static Texture* tex = NULL;
    if (tex == NULL)
    {
        setAntiAliasParameters(true);
#if 0
        uint8 rgba[16] = {
            255, 0, 0, 255,     0, 255, 0, 255,
            0, 0, 255, 255,     255, 255, 0, 255
        };
        int width = 2, height = 2;

        tex = createImageWithRGBA(rgba, width, height);
#else
        wchar_t* wPath = utf8_to_utf16("assets/ex.png");
        Bitmap* bmp = new Bitmap(wPath);
        free(wPath);

        int width, height;
        uint8* rgba = bmp2rgba(bmp, width, height);
        delete bmp;

        tex = createImageWithRGBA(rgba, width, height);
        free(rgba);
#endif
    }

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, (GLuint)tex->texID);

    static float delta = 0.0f;
    delta += 1/60.;
    float s = 0.2 * _sin(360 * delta);

    iPoint position[4] = {
        // top|left,   bottom|left,  top|right,  bottom|right
        //{-0.3, 0.3},   {-0.3, -0.3}, {0.3, 0.3}, {0.3, -0.3}
        {-0.3-s, 0.3+s},   {-0.3-s, -0.3-s},
        {0.3+s, 0.3+s}, {0.3+s, -0.3-s}
    };
    iPoint coordinate[4] = {
        //{0.0, 1.0},   {0.0, 0.0},    {1.0, 1.0}, {1.0, 0.0}
        {0.0, 0.0},   {0.0, 1.0},    {1.0, 0.0}, {1.0, 1.0}
    };
    glVertexPointer(2, GL_FLOAT, 0, position);
    glTexCoordPointer(2, GL_FLOAT, 0, coordinate);

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
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

iPoint convertCoordinate(int x, int y)
{
    RECT rt;
    //GetWindowRect(hWnd, &rt);
    GetClientRect(hWnd, &rt);

    iPoint r;
    r.x = x - rt.left;
    r.y = y - rt.top;

    r.x = (r.x - viewport.origin.x) * devSize.width / viewport.size.width;
    r.y = (r.y - viewport.origin.y) * devSize.height / viewport.size.height;

    return r;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
#if OPENGL==0
        resizeLib(LOWORD(lParam), HIWORD(lParam));
#else
        reshapeOpenGL(LOWORD(lParam), HIWORD(lParam));
#endif
        break;

#if (OPENGL==0)
    case WM_LBUTTONDOWN:
        keyGame(iKeyStateBegan, convertCoordinate(LOWORD(lParam), HIWORD(lParam)));
        break;
    case WM_MOUSEMOVE:
        keyGame(iKeyStateMoved, convertCoordinate(LOWORD(lParam), HIWORD(lParam)));
        break;
    case WM_LBUTTONUP:
        keyGame(iKeyStateEnded, convertCoordinate(LOWORD(lParam), HIWORD(lParam)));
        break;
#endif

    //case WM_CHAR:
    //    break;
    case WM_KEYDOWN:
        keyLib(iKeyStateBegan, wParam);
        break;
    case WM_KEYUP:
        keyLib(iKeyStateEnded, wParam);
        break;

    case WM_CLOSE:
        if (IDYES == MessageBoxW(NULL, TEXT("너 못생겼어?"), TEXT("알림"), MB_YESNO))
        {
            runWnd = false;
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

