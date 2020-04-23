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

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    hDC = GetDC(hWnd);

    RECT rect;
    GetClientRect(hWnd, &rect);
    monitorSizeW = rect.right - rect.left;
    monitorSizeH = rect.bottom - rect.top;

    loadLib(hDC);
    loadGame();
    loadCursor();

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
    case WM_SIZE:
        // client rect
        resizeLib(LOWORD(lParam), HIWORD(lParam));

        drawLib(drawGame);
        SwapBuffers(hDC);
        break;
    case WM_SIZING:case WM_MOVE:
    {
        // window rect
        RECT rect;
        GetClientRect(hWnd, &rect);
        resizeLib(rect.right-rect.left, rect.bottom-rect.top);

        drawLib(drawGame);
        SwapBuffers(hDC);
    }
        break;


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
