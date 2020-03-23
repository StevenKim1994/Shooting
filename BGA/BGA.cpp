// BGA.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "BGA.h"

#include <math.h>

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
    hInstance = hInst;

    WNDCLASSEXW wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BGA));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = MAKEINTRESOURCEW(IDC_BGA);
    wc.lpszClassName = szWindowClass;
    wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wc);

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    hDC = GetDC(hWnd);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    runWnd = true;
    MSG msg;
    while (runWnd)
    {
        if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            void test();
            test();
            SwapBuffers(hDC);
        }
    }

    DestroyWindow(hWnd);

    return (int) msg.wParam;
}

void test()
{
    RECT rt;
    GetClientRect(hWnd, &rt);
    Rectangle(hDC, rt.left + 5, rt.top + 5, rt.right - 5, rt.bottom - 5);

    RECT rect = { 100, 100, 300, 200 };
    static float dt = 0.01f;
    dt += 0.1f;
    if (dt > 1.0f)
        dt = 0.0f;
    float d = 100 * sin(180 / 3.141592 * dt);
    rect.left += d;
    rect.right += d;
    DrawText(hDC, TEXT("AAA"), -1, &rect, DT_CENTER | DT_WORDBREAK);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
        break;
    case WM_MOUSEMOVE:
        break;
    case WM_LBUTTONUP:
        break;

    case WM_CLOSE:
        if (IDYES==MessageBoxW(NULL, TEXT("너 못생겼어?"), TEXT("알림"), MB_YESNO))
        {
            runWnd = false;
        }
        break;

    case WM_CHAR:
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

