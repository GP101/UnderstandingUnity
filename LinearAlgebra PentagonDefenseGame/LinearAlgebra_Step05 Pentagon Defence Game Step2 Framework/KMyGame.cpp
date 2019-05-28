#include "stdafx.h"
#include "KMyGame.h"
#include "LinearAlgebra.h"
#include "KVectorUtil.h"
#include <MMSystem.h>
#include "KMatrix2.h"
#include <objidl.h>
#include <gdiplus.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <windowsx.h>
#include <memory>
#include "KBaseObject.h"
#include "KPentagon.h"
#include "KMissile.h"
#include "KBullet.h"
#include <vector>
#include <list>
#include "KMyGame.h"

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#pragma comment(lib,"winmm.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
// _20180519_jintaeks
HWND    g_hwnd = NULL;
HDC     g_hdc = 0;
HBITMAP g_hBitmap = 0;
RECT    g_clientRect;
int     g_mouseLButtonDown = 0;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void                Initialize();
void                Finalize();
void                OnSize();
void                OnIdle(float fElapsedTime_);
void                OnLButtonDown(int x, int y);
void                OnLButtonUp();


std::shared_ptr<KMyGame>     KMyGame::instance = nullptr;

CONSTRUCTOR KMyGame::KMyGame()
{
}

DESTRUCTOR KMyGame::~KMyGame()
{
}

int KMyGame::Run(HINSTANCE hInstance, int nCmdShow)
{
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LINEARALGEBRA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LINEARALGEBRA));

    Initialize();

    DWORD dwOldTime = ::timeGetTime();

    MSG msg;

    // Main message loop:
    while (true)
    {
        ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        const DWORD dwNewTime = ::timeGetTime();
        const BOOL bIsTranslateMessage = TranslateAccelerator(msg.hwnd, hAccelTable, &msg);
        if (!bIsTranslateMessage)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }//if

        OnIdle(float(dwNewTime - dwOldTime) / 1000.f);
        Sleep(10);

        dwOldTime = dwNewTime;

        if (msg.message == WM_QUIT)
        {
            break;
        }//if
    }//while

    Finalize();
    GdiplusShutdown(gdiplusToken);

    return (int)msg.wParam;
}

void KMyGame::Initialize()
{
    OnInitialize();
}

void KMyGame::Finalize()
{
    OnFinalize();
}

void KMyGame::_AddGameObject(KBaseObjectPtr sp)
{
    m_objectManager.push_back(sp);
}

void KMyGame::Update(float fElapsedTime_)
{
    for (unsigned int i = 0; i < m_objectManager.size(); ++i)\
    {
        KBaseObjectPtr pb = m_objectManager[i];
        pb->OnUpdate(fElapsedTime_);
    }

    for (unsigned int i = 0; i < m_objectManager.size(); ++i)\
    {
        KBaseObjectPtr pb = m_objectManager[i];
        pb->OnLateUpdate(fElapsedTime_);
    }
}

void KMyGame::Render()
{
    for (unsigned int i = 0; i < m_objectManager.size(); ++i)\
    {
        KBaseObjectPtr pb = m_objectManager[i];
        pb->OnRender( g_hdc );
    }
}

void KMyGame::LButtonDown(int x, int y)
{
    OnLButtonDown(x, y);
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LINEARALGEBRA));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LINEARALGEBRA);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    // _20180519_jintaeks
    g_hwnd = hWnd;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


//_20180519_jintaeks
void OnPaint(HDC hdc)
{
    //KVectorUtil::DrawLine(hdc, KVector2(0, 0), KVector2(3, 2), 1, PS_DASH, RGB(0, 0, 255));
    //KVectorUtil::DrawLine(hdc, KVector2(0, 0), KVector2(2, 3), 2, PS_SOLID, RGB(255, 0, 0));
    Graphics graphics(hdc);
    Pen      pen(Color(255, 0, 0, 255));
    graphics.DrawLine(&pen, 0, 0, 200, 100);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
                       int wmId = LOWORD(wParam);
                       // Parse the menu selections:
                       switch (wmId)
                       {
                       case IDM_ABOUT:
                           DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                           break;
                       case IDM_EXIT:
                           DestroyWindow(hWnd);
                           break;
                       default:
                           return DefWindowProc(hWnd, message, wParam, lParam);
                       }
    }
        break;
    case WM_PAINT:
    {
                     PAINTSTRUCT ps;
                     HDC hdc = BeginPaint(hWnd, &ps);
                     // TODO: Add any drawing code that uses hdc here...
                     // _20180519_jintaeks
                     OnPaint(hdc);
                     EndPaint(hWnd, &ps);
    }
        break;
    case WM_SIZE:
        OnSize();
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
        OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_LBUTTONUP:
        OnLButtonUp();
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

void Initialize()
{
    if (KMyGame::GetInstance() != nullptr)
        KMyGame::GetInstance()->Initialize();
}//Initialize()

void Finalize()
{
    if (KMyGame::GetInstance() != nullptr)
        KMyGame::GetInstance()->Finalize();

    if (g_hdc != 0) {
        DeleteDC(g_hdc);
        g_hdc = 0;
    }//if
    if (g_hBitmap != 0) {
        DeleteObject(g_hBitmap);
        g_hBitmap = 0;
    }//if
}//Finalize()

void OnSize()
{
    Finalize();

    ::GetClientRect(g_hwnd, &g_clientRect);
    const int iWidth = g_clientRect.right - g_clientRect.left + 1;
    const int iHeight = g_clientRect.bottom - g_clientRect.top + 1;

    KVector2 origin;
    origin.x = iWidth / 2.0f;
    origin.y = iHeight / 2.0f;
    KVectorUtil::g_screenCoordinate.SetInfo(KVector2(50, 0), KVector2(0, -50), origin);

    HDC hdc = ::GetDC(g_hwnd);
    g_hdc = CreateCompatibleDC(hdc);
    g_hBitmap = CreateCompatibleBitmap(hdc, iWidth, iHeight);
    SelectObject(g_hdc, g_hBitmap);
}//OnSize()

void OnIdle(float fElapsedTime_)
{
    KMyGame::GetInstance()->Update(fElapsedTime_);

    const int iWidth = g_clientRect.right - g_clientRect.left + 1;
    const int iHeight = g_clientRect.bottom - g_clientRect.top + 1;

    HDC hdc = ::GetDC(g_hwnd);

    HBRUSH brush;
    brush = CreateSolidBrush(RGB(255, 255, 255));
    HGDIOBJ oldBrush = SelectObject(g_hdc, brush);
    Rectangle(g_hdc, 0, 0, iWidth, iHeight);

    {
        static float s_rotDelta = 0.0f;
        s_rotDelta += fElapsedTime_ * 0.2f;
        KVector2    xaxis(1, 0);
        KVector2    yaxis(0, 1);
        KMatrix2    m;
        m.SetRotation(s_rotDelta);
        xaxis = m * xaxis;
        yaxis = m * yaxis;
        KBasis2     basis2;
        //basis2.SetInfo(KVector2(1, 0), KVector2(0, 1));
        basis2.SetInfo(xaxis, yaxis);
        KVectorUtil::SetBasis2(basis2);

        KVectorUtil::DrawGrid(g_hdc, 50, 50);
        KVectorUtil::DrawAxis(g_hdc, 50, 50, RGB(255, 0, 0), RGB(255, 0, 0));
    }

    KMyGame::GetInstance()->Render();

    BitBlt(hdc, 0, 0, iWidth, iHeight, g_hdc, 0, 0, SRCCOPY);
    SelectObject(g_hdc, oldBrush);
    DeleteObject(brush);

    RECT rect;
    rect.left = rect.top = 0;
    rect.right = rect.bottom = 100;
    DrawTextA(hdc, "Hello", 5, &rect, DT_CENTER);

    ::ReleaseDC(g_hwnd, hdc);
}//OnIdle()

void OnLButtonDown(int x, int y)
{
    if (g_mouseLButtonDown != 1) {
        KMyGame::GetInstance()->LButtonDown(x, y);
    }
    g_mouseLButtonDown = 1;
}

void OnLButtonUp()
{
    g_mouseLButtonDown = 0;
}
