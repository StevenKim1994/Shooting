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

#if (OPENGL == 0)
    loadLib(hDC); // GDI;
    loadGame();
#else
    setupOpenGL(true, hDC); // OPENGL;
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
#if (OPENGL == 0)
            drawLib(drawGame);
#else
            void testDot();
            void testLine();
            void testRect();
            void testCircle();
            void testTexture();
            void testTextureforImage();
        	
            //testDot();
            //testLine();
            //testCircle();
            //testTexture();
            //testTextureforImage();
            setAntiAliasParameters(false);
        	
            glColor3f(1, 0, 0);
            glBegin(GL_QUAD_STRIP);
            glVertex2f(-0.3, 0.3);
        	
            glVertex2f(0.3, 0.3);
            glColor3f(0, 1, 0);
            glVertex2f(0.3, -0.3);
            glColor3f(1, 1, 0);
            glVertex2f(-0.3, -0.3);
            glEnd();
		
#endif
            SwapBuffers(hDC);
        }
    }

#if (OPENGL == 0)
    freeLib();
    freeGame();
#else
    setupOpenGL(false, NULL);
#endif

    DestroyWindow(hWnd);
    endGdiplus(gpToken);

    return (int)msg.wParam;
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
#if (OPENGL==0)
        resizeLib(LOWORD(lParam), HIWORD(lParam));
#else
        reshapeOpenGL(LOWORD(lParam), HIWORD(lParam));
#endif
    	break;

#if (OPENGL == 0)
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

void testDot()
{
    glClearColor(0, 0, 0, 1);
    //glClearColor(random() % 256 / 255.f, random() % 256 / 255.f, random() % 256 / 255.f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY); // 정점별로 색 지정 

    glColor4f(1, 0, 0, 1); // 이건 정점별로 지정한 색에 색을 곱해줌. 기본 지정한 색은 0,0,0,0(WHITE)
	
    iPoint position[3] = { {-0.5, -0.5},{0.0, 0.0},{0.5,0.5} };
    //float color[3][4] = { {1,0,0,1}, {0,0,1,1}, {1,0,1,1} };
	
    glVertexPointer(2, GL_FLOAT, 0, position);
    //glColorPointer(4, GL_FLOAT, 0, color);
    glPointSize(5);
	
    glDrawArrays(GL_POINTS, 0, 3);
    //glDrawElements;
	
	
        
    glDisableClientState(GL_VERTEX_ARRAY);
    
}

void testLine()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);

    iPoint vertices[2];
   // vertices[0] = iPointMake(-0.5, -0.5);
   // vertices[1] = iPointMake(0.0, 0.0);
   //
    vertices[0] = iPointMake(50, 50);
    vertices[1] = iPointMake(devSize.width-50, devSize.height-50);
#if 0
	//left right : -1 ~ 1
    // bottom top : -1 ~ 1
    //
    devSize = iSizeMake(1000, 800);

	for(int i = 0; i < 2; i++)
	{
        iPoint& p = vertices[i];

        p.x = p.x / devSize.width * 2 - 1;// 0 ~ devSize.width = > -1~1;
        p.y = p.y / devSize.height * 2 - 1;
	}

#else
	devSize = iSizeMake(1000, 800);
    iPoint position[2] = { {100,100},{devSize.width - 100, devSize.height - 100} };

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, devSize.width, devSize.height, 0, 0, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    iPoint c = (position[0] + position[1]) / 2;
    glTranslatef(c.x, c.y, 0);
    glRotatef(30, 0, 0, 1);

#endif
	
    glVertexPointer(2, GL_FLOAT, 0, vertices);
	
	
    glLineWidth(5);
    glEnable(GL_LINE_SMOOTH);
	
    glDrawArrays(GL_LINES, 0, 2);
	
    glDisableClientState(GL_VERTEX_ARRAY);
}
	

void testRect()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
	
    iPoint position[4] = { { -0.3, -0.3 }, { 0.3, -0.3 }, {0.3,0.3}, { 0.0, 0.3 } };

    float color[4][4] = { {1,1,1,0},{1,0,0,1},{0,1,0,1},{0,0,1,0} };


    glVertexPointer(2, GL_FLOAT, 0, position);
    glColorPointer(4, GL_FLOAT, 0, color);
	
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
}

void testCircle()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    iPoint position[720];
   // float color[720][4];

	for(int i = 0; i < 720; i++)
	{
		position[i] = iPointRotate(iPointMake(0.1f, 0), iPointMake(0, 0), i);	

     //   float* c = color[i];

		
   //     c[0] = (i + 1) / 720;
     //   c[1] = ((i + 91) % 720) / 720;
      //  c[2] = ((i + 46) % 720) / 720;
      //  c[3] = ((i + 181) % 720) / 720;
		
	}

    glVertexPointer(2, GL_FLOAT, 0, position);
    //glColorPointer(4, GL_FLOAT, 0, color);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 720);

    glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_COLOR_ARRAY);
}


void testTexture()
{
    static Texture* tex = NULL;

    if (tex == NULL)
    {
        setAntiAliasParameters(true);
        uint8 rgba[16] =
        { 255,0,0,255,
          0,255,0,255,
          0,0,255,255,
          255,255,0,255
        };

        int width = 2, height = 2;
        int potWidth = nextPot(width), potHeight = nextPot(height); // 2의 승수로 만든 width, height;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, potWidth, potHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

        tex = createImageWithRGBA(rgba, width, height);
    }
	
/*    GLuint texID;
    glGenTextures(1, &texID); // generate(create) Texture;
    //glActiveTexture(GL_TEXTURE0); // 첫번째 텍스처에 texID를 쓰겠다라는 의미. 이걸 사용하지 않아도 첫번째 텍스처는 0번임.
	glBindTexture(GL_TEXTURE_2D, texID); // use Texture;
    //glActiveTexture(GL_TEXTURE0+1); // 수가 증가할수록 뒤에 1씩을 더해줌 텍스처 여러개 사용할때에만 두번째 텍스처 사용
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    */

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, (GLuint)tex->texID);
	
    iPoint position[4] = { {-0.3,0.3}, {-0.3,-0.3}, {0.3, 0.3}, {0.3, -0.3}
    };

	// top|left, bottom|left, top|right, bottom|right
    iPoint texCoorinate[4] = { {0.0, 1.0}, {0.0,0.0 }, {1.0,1.0}, {1.0,0.0} };

	
	glVertexPointer(2, GL_FLOAT, 0, position);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoorinate);
	
	//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	//glBindTexture(GL_TEXTURE_2D, 0); // don't use Texture;
    //glDeleteTextures(1, &texID); // destroy Texture;
#if 1
    uint8 indices[4] = { 0,1,2,3 };
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
#else
	uint8 indices[6] = { 0,1,2,    1,2,3 };
    glDrawElements(GL_TRIANGLES, 6 ,GL_UNSIGNED_BYTE, indices);
#endif
	

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, 0); //OpenGL이 구동시에 가르키는 텍스처를 가르킴
    glDisable(GL_TEXTURE_2D);
}

void testTextureforImage()
{
    static Texture* tex = NULL;

    if (tex == NULL)
    {
        setAntiAliasParameters(true);
#if 0
        /* uint8 rgba[16] =
        { 255,0,0,255,
          0,255,0,255,
          0,0,255,255,
          255,255,0,255
        };*/

        int width = 2, height = 2;
        int potWidth = nextPot(width), potHeight = nextPot(height); // 2의 승수로 만든 width, height;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, potWidth, potHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

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

    delta += 1 / 60.0f;

    float s = 0.2f * _sin(360 * delta);

	
    iPoint position[4] = { {-0.3-s,0.3+s}, {-0.3-s,-0.3-s}, {0.3+s, 0.3+s}, {0.3+s, -0.3-s}
    };

    iPoint texCoorinate[4] = { { 0.0, 0.0 }, { 0.0, 1.0 }, { 1.0,0.0 }, { 1.0,1.0 } };//{ {0.0, 1.0}, {0.0,0.0 }, {1.0,1.0}, {1.0,0.0} };


	
    glVertexPointer(2, GL_FLOAT, 0, position);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoorinate);
   
    
#if 1
    uint8 indices[4] = { 0,1,2,3 };
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
#else
    uint8 indices[6] = { 0,1,2,    1,2,3 };
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
#endif


    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, 0); //OpenGL이 구동시에 가르키는 텍스처를 가르킴
    glDisable(GL_TEXTURE_2D);
}