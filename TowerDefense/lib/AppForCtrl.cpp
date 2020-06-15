
#if (TOOL ==1)
#include "AppForCtrl.h"
#include "iWindow.h"
#include "Script.h"

HINSTANCE hInstance;
HWND hWnd;
HDC hDC;
bool runWnd;

WCHAR szTitle[100] = TEXT("TOOL");    // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[100] = TEXT("TOOL");       // 기본 창 클래스 이름입니다.

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInst,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    HANDLE handle = startOnlyRun(szWindowClass);
    
    if (handle == NULL)
    {
        MessageBox(NULL, TEXT("중복실행"), TEXT("Title"), MB_OK);
        return 0;
    }

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
    wc.lpszMenuName = MAKEINTRESOURCEW(IDC_TOWERDEFENSE);
    wc.lpszClassName = szWindowClass;
    wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wc);

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    hDC = GetDC(hWnd);

    
    setupOpenGL(true);
    loadScript(hWnd);
    //testCtrlSystem(hWnd,hInstance);

    RECT rect;
    GetClientRect(hWnd, &rect);
    monitorSizeW = rect.right - rect.left;
    monitorSizeH = rect.bottom - rect.top;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TOWERDEFENSE));

    runWnd = true;
    MSG msg;
    while (runWnd)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            static DWORD prevTickCount = GetTickCount();
            DWORD d = GetTickCount();
            float delta = (d - prevTickCount) / 1000.f;
            prevTickCount = d;
       
            updateOpenGL(delta);
        }
    }

    
    freeScript();
    setupOpenGL(false);

    DestroyWindow(hWnd);
    endGdiplus(gpToken);
    endOnlyRun(handle);

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

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result; 

    switch (message)
    {
    case WM_DROPFILES:
   
        dragScript(wParam, lParam);
        break;
   
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORBTN:
    case WM_CTLCOLORDLG:
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
        result = colorScrip(wParam, lParam);//colorScript(wParam, lParam);
        if (result)
            return result;
        break;
    


    case WM_COMMAND:
    {
        updateScript(wParam, lParam);

        int wmID = LOWORD(wParam);

       
        switch (wmID)
        {
            
        case IDM_OPEN:
        {
            break;
        }



        case IDM_ABOUT:
        {
                DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
        }

        }
    }

    case WM_CREATE:
    {
       
        break;
    }

    case WM_GETMINMAXINFO:
    {
        break;
    }

    case WM_SIZE:

        break;
    case WM_SIZING:
    {

        break;
    }
    case WM_MOVE:
    {
     
        break;
    }

    case WM_LBUTTONDOWN:
       
        break;
    case WM_MOUSEMOVE:
       
        break;
    case WM_LBUTTONUP:
      
        break;
    case WM_KEYDOWN:
        

 
        break;
    case WM_KEYUP:
        
        break;

    case WM_CLOSE:
        
       
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam); // 윈도우 화면 갱신!
    }

    return 0;
}
#endif
