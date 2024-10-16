// LinearAlgebra.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "LinearAlgebra.h"
//_20180519_jintaeks
#include "KVectorUtil.h"
#include <MMSystem.h>
#include <objidl.h>
#include <gdiplus.h>
#include "KMatrix3.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "KRenderState.h"
#include "KCube.h"
#include "KQuaternion.h"
#include "KAxis.h"

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

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void                Initialize();
void                Finalize();
void                OnSize();
void                OnUpdate( double fElapsedTime_);
void                OnRender(HDC hdc, double fElapsedTime_);
void                OnIdle(double fElapsedTime_);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup( &gdiplusToken, &gdiplusStartupInput, NULL );

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LINEARALGEBRA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
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

        OnIdle(double(dwNewTime - dwOldTime) / 1000.0);
        Sleep(10);

        dwOldTime = dwNewTime;

        if (msg.message == WM_QUIT)
        {
            break;
        }//if
    }//while

    Finalize();
    GdiplusShutdown( gdiplusToken );

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LINEARALGEBRA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LINEARALGEBRA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
            OnPaint( hdc );
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_SIZE:
        OnSize();
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
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
}//Initialize()

void Finalize()
{
    if(g_hdc != 0) {
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

    const int pixelWidth = 4;
    KVector2 origin;
    origin.x = iWidth / 2.0f;
    origin.y = iHeight / 2.0f;
    KVectorUtil::g_screenCoordinate.SetInfo( KVector2( pixelWidth, 0 ), KVector2( 0, -pixelWidth ), origin );

    HDC hdc = ::GetDC(g_hwnd);
    g_hdc = CreateCompatibleDC(hdc);
    g_hBitmap = CreateCompatibleBitmap(hdc, iWidth, iHeight);
    SelectObject(g_hdc, g_hBitmap);
}//OnSize()

void OnUpdate( double fElapsedTime_)
{
}

void OnRender(HDC hdc, double fElapsedTime_)
{
    char text[] = "Press arrow keys to pan camera";
    RECT rect;
    rect.left = rect.top = 0;
    rect.right = rect.bottom = 300;
    DrawTextA( hdc, text, strlen(text), &rect, DT_CENTER );

    const double dotWidth = KVectorUtil::g_screenCoordinate.axis0.Length( );
    const double dotHeight = KVectorUtil::g_screenCoordinate.axis1.Length( );

    static double fRotDelta = 0.0;
    fRotDelta += fElapsedTime_;

    KLight light;
    light.m_ambient = KRgb(0.1f, 0.1f, 0.1f);
    light.m_diffuse = KRgb(1, 1, 1);
    light.m_specular = KRgb(0, 0, 0);
    KRenderState::GetInstance().SetLight(light);

    KCube           cube;
    KMatrix4        matRotY;
    KMatrix4        matTrans;
    KMatrix4        matScale;
    KMatrix4        matWorld;
    KMatrix4        matView;
    KMatrix4        matProjection;

    static double   s_fTheta = 0.0;
    s_fTheta += fElapsedTime_ * 0.5;

    matRotY.SetRotationY(s_fTheta);
    matTrans.SetTranslation( 0.0, 0.0, 0.0 );
    matScale.SetScale( 30, 30, 30 );
    matWorld = matTrans * matRotY * matScale;

    static double cameraX = 0.0;
    static double cameraY = 0.0;
    static double cameraZ = -10.0;
    if (GetAsyncKeyState( VK_LEFT )) {
        cameraX -= 10.0;
    }
    if (GetAsyncKeyState( VK_RIGHT )) {
        cameraX += 10.0;
    }
    if (GetAsyncKeyState( VK_UP )) {
        cameraY += 10.0;
    }
    if (GetAsyncKeyState( VK_DOWN )) {
        cameraY -= 10.0;
    }
    if (GetAsyncKeyState( VK_PRIOR )) {
        cameraZ += 10.0;
    }
    if (GetAsyncKeyState( VK_NEXT )) {
        cameraZ -= 10.0;
    }
    cameraZ = __max( -100.0, __min( cameraZ, -30 ) );
    matView.LookAtRH( KVector3( cameraX, cameraY, cameraZ ), KVector3( cameraX, cameraY, 0.0 ), KVector3( 0, 1, 0 ) );
    //matView.LookAtRH( KVector3( cameraX, cameraY, cameraZ ), KVector3( 0.0, 0.0, 0.0 ), KVector3(0,1,0) );

    matProjection.SetProjection( 200.f );

    cube.Transform( matWorld );
    cube.Transform( matView );
    cube.Transform( matProjection );
    cube.DrawOnCanvas(hdc);

    KAxis axis;
    axis.InitPositionDirection();
    axis.Transform( matWorld );
    axis.Transform( matView );
    axis.Transform( matProjection );
    axis.Render( hdc );
}

void OnIdle( double fElapsedTime_)
{
    const int iWidth = g_clientRect.right - g_clientRect.left + 1;
    const int iHeight = g_clientRect.bottom - g_clientRect.top + 1;

    OnUpdate(fElapsedTime_);
    HDC hdc = ::GetDC(g_hwnd);

    HBRUSH brush;
    brush = CreateSolidBrush(RGB(255, 255, 255));
    HGDIOBJ oldBrush = SelectObject(g_hdc, brush);
    Rectangle(g_hdc, 0, 0, iWidth, iHeight);

    {
        OnRender( g_hdc, fElapsedTime_);
    }

    BitBlt(hdc, 0, 0, iWidth, iHeight, g_hdc, 0, 0, SRCCOPY);
    SelectObject(g_hdc, oldBrush);
    DeleteObject(brush);

    ::ReleaseDC(g_hwnd, hdc);
}//OnIdle()
