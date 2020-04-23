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
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOWERDEFENSE));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_SHOOTING);
    wc.lpszClassName = szWindowClass;
    wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wc);

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, // 이부분이 윈도우창에 대한 기본설정을 지정하는 인자매크로임
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    hDC = GetDC(hWnd);

    RECT rect;
    GetClientRect(hWnd, &rect);
    monitorSizeW = rect.right - rect.left;
    monitorSizeH = rect.bottom - rect.top;

    loadLib(hDC);
    loadGame();
    loadCursor();
    isFullscreen = false; // 창모드로 설정

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

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
            drawLib(drawGame);
            drawCursor(0.0f); // #bug
            SwapBuffers(hDC);
        }
    }

    freeLib();
    freeGame();
    freeCursor();

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
    case WM_GETMINMAXINFO:
    {
        if (win_border_width == 0 || win_border_height == 0)
            break;

        float rate = devSize.width / devSize.height;
        float width = monitorSizeW * DEV_MIN_RATE;
        float height = width / rate;

        MINMAXINFO* mmi = (MINMAXINFO*)lParam;
        mmi->ptMinTrackSize.x = width + win_border_width;
        mmi->ptMinTrackSize.y = height + win_border_height;

        break;
    }
    case WM_CREATE:
    {
        RECT rt0, rt1;
        GetWindowRect(hWnd, &rt0);
        GetClientRect(hWnd, &rt1);
        win_border_width = (rt0.right - rt0.left) - (rt1.right - rt1.left);
        win_border_height = (rt0.bottom - rt0.top) - (rt1.bottom - rt1.top);
        
        break;
    }


    case WM_SIZE:
    {
        // client rect
        resizeLib(LOWORD(lParam), HIWORD(lParam));

        drawLib(drawGame);
        SwapBuffers(hDC);
        break;
    }
    case WM_SIZING:
    {
        // window rect
        RECT& rect = *reinterpret_cast<LPRECT>(lParam); //  == RECT* rt = (RECT*)lParam;
        
        enforceResolution((int)wParam, rect,win_border_width, win_border_height);
        //RECT rect;
        //GetClientRect(hWnd, &rect);
        resizeLib(rect.right - rect.left, rect.bottom - rect.top);

        drawLib(drawGame);
        SwapBuffers(hDC);
        break;
    }

    case WM_MOVE:
    {

        // window rect
        RECT rect;
        GetClientRect(hWnd, &rect);
        resizeLib(rect.right-rect.left, rect.bottom-rect.top);

        drawLib(drawGame);
        SwapBuffers(hDC);
        break;
    }


    case WM_LBUTTONDOWN:
        cursor = convertCoordinate(LOWORD(lParam), HIWORD(lParam));
        keyGame(iKeyStateBegan, cursor);
        break;
    case WM_MOUSEMOVE:
        cursor = convertCoordinate(LOWORD(lParam), HIWORD(lParam));
        keyGame(iKeyStateMoved, cursor);
        break;
    case WM_LBUTTONUP:
        cursor = convertCoordinate(LOWORD(lParam), HIWORD(lParam));
        keyGame(iKeyStateEnded,cursor);
        break;
    case WM_SETCURSOR: // 마우스 커서가 프로그램 창에 들어왔을때
        
        // 실제 게임이 표시되는 영역에 마우스 좌표가 들어왔다면...
        if (updateCursor(LOWORD(lParam) == HTCLIENT))
            return true;

        break;

    //case WM_CHAR:
    //    break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
            goFullscreen();
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

//---------------------------------------------------
//              cursor 
//---------------------------------------------------

static Texture* texCursor;
iPoint cursor;
static bool bCursor;

void loadCursor()
{
    texCursor = createImage("assets/cursor.png");
    cursor = iPointZero;
    bCursor = false;
}

void freeCursor()
{
    freeImage(texCursor);
}

void drawCursor(float dt)
{
    if( bCursor) // 커서가 화면에 들어왔을떄만
    { 
        drawImage(texCursor, cursor.x, cursor.y, TOP | LEFT);
    }
}

bool updateCursor(bool inClient)
{
    if (bCursor == inClient)
        return false;

    bCursor = inClient;

    //ShowCursor(bCursor ? FALSE : TRUE); // 게임커서 이미지가 보이면 시스템 커서 안보이게.
    
    // 게임을 사용중일떄 다른 프로그램에서 이 시스템 커서를 수정하는 경우 레퍼런스 카운터가 달라지기 때문에 밑의 코드가 필요함.
    if (bCursor)
    {
        while (1)
        {
            int n = ShowCursor(FALSE);

            if (n < 0) break;
        }
    }
    
    else
    {
        while (1)
        {
            int n = ShowCursor(TRUE);

            if (n > -1) break;

        }
    }



    return true;
}

void setCurrentMonitor(RECT& rt)
{
    GetWindowRect(hWnd, &rt);

    HMONITOR hMonitor = MonitorFromRect(&rt, MONITOR_DEFAULTTONEAREST); // 이 프로그램 창이 뛰어진 가장 가까운 모니터를 설정
    
    MONITORINFO mi;
    mi.cbSize = sizeof(MONITORINFO);

    GetMonitorInfo(hMonitor, &mi);

    if (mi.dwFlags == MONITORINFOF_PRIMARY) // 주모니터
    {
        rt.left = 0;
        rt.top = 0;
        rt.right = GetSystemMetrics(SM_CXSCREEN);
        rt.bottom = GetSystemMetrics(SM_CYSCREEN);
    }
    else // 보조모니터
    {
        memcpy(&rt, &mi.rcWork, sizeof(RECT)); // &rcWork 현재 활성화된(작업영역)모니터의 영역의 구조체


    }

}


bool isFullscreen;
RECT rtPrev;

void goFullscreen()
{
   
    isFullscreen = !isFullscreen;

    if (isFullscreen)
    {
        GetWindowRect(hWnd, &rtPrev);



        RECT rt;
        setCurrentMonitor(rt);
        int x = rt.left;
        int y = rt.top;
        int w = rt.right - rt.left;
        int h = rt.bottom - rt.top;

        //SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW);
        SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
        SetWindowPos(hWnd, HWND_TOP, x, y, w, h, SWP_SHOWWINDOW);

    }
    else
    {
        int x = rtPrev.left;
        int y = rtPrev.top;
        int w = rtPrev.right - rtPrev.left;
        int h = rtPrev.bottom - rtPrev.top;

        //SetWindowLong(hWnd, GWL_STYLE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
        SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        SetWindowPos(hWnd, HWND_TOP, x, y, w, h, SWP_SHOWWINDOW);
    }


}

int win_border_width = 0 , win_border_height = 0;
void enforceResolution(int edge, RECT& rect, int win_border_width, int win_border_height) // 강제 종횡비
{
    switch (edge)
    {
    case WMSZ_BOTTOM:
    case WMSZ_TOP:
    {
        // w : h = devSize.width : devSize.height
        int h = (rect.bottom - rect.top) - win_border_height;
        int w = h * devSize.width / devSize.height + win_border_width;

        rect.left = (rect.left + rect.right) / 2 - w / 2;
        rect.right = rect.left + w;

        break;  
    }
    case WMSZ_LEFT:
    case WMSZ_RIGHT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = w * devSize.height / devSize.width + win_border_height;

        rect.top = (rect.top + rect.bottom) / 2 - h / 2;
        rect.bottom = rect.top + h;

        break;
    }

    case WMSZ_BOTTOMLEFT:
    {
      /*
        int h = (rect.bottom - rect.top) - win_border_height;
        int w = h * devSize.width / devSize.height + win_border_width;

        rect.left = rect.right - w;
      */ // myCode


        // w : h = devSize.width : devSize.height
        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;

        // if(w/h > devSize.width / devSize.height)
        
        if (w * devSize.width > devSize.width * h) // 가로가 크다면
        {
            w = h * devSize.width / devSize.height + win_border_width;
            rect.left = rect.right - w;
        }
        else // 세로가 크다면
        {
            h = w * devSize.height / devSize.width + win_border_height;
            rect.bottom = rect.top + h;
        }
        

        break;
    }
    case WMSZ_BOTTOMRIGHT:
    {
        /*
        int h = (rect.bottom - rect.top) - win_border_height;
        int w = h * devSize.width / devSize.height + win_border_width;

       
        rect.right = rect.left + w;
        */ // myCode


        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;

        // if(w/h > devSize.width / devSize.height)

        if (w * devSize.width > devSize.width * h) // 가로가 크다면
        {
            w = h * devSize.width / devSize.height + win_border_width;
            rect.right = rect.left + w;
        }
        else // 세로가 크다면
        {
            h = w * devSize.height / devSize.width + win_border_height;
            rect.bottom = rect.top + h;
        }
        break;
    }
    case WMSZ_TOPLEFT:
    {
        /*
        int h = (rect.bottom - rect.top) - win_border_height;
        int w = h * devSize.width / devSize.height + win_border_width;

        rect.left = rect.right - w;
       
        */ // myCode

        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;

        if (w * devSize.height > devSize.width * h)
        {
            w = h * devSize.width / devSize.height + win_border_width;
            rect.left = rect.right - w;
        }
        else
        {
            h = w * devSize.height / devSize.width + win_border_height;
            rect.top = rect.bottom - h;
        }

        break;
    }
    case WMSZ_TOPRIGHT:
    {
        /*
        int h = (rect.bottom - rect.top) - win_border_height;
        int w = h * devSize.width / devSize.height + win_border_width;

        rect.right = rect.left + w;
     
        break;
        */ // myCode

        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;

        if (w * devSize.width > devSize.width * h) // 가로가 크다면
        {
            w = h * devSize.width / devSize.height + win_border_width;
            rect.right = rect.left + w;
        }
        else
        {
            h = w * devSize.height / devSize.width + win_border_height;
            rect.top = rect.bottom - h;
        }

    }
    }
}
