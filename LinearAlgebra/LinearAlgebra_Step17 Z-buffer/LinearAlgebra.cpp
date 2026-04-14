// LinearAlgebra.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "LinearAlgebra.h"
//_20180519_jintaeks
#include "KVectorUtil.h"
#include <MMSystem.h>
#include "KPolygon.h"
#include <objidl.h>
#include <gdiplus.h>
#include "KMatrix3.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "KRenderState.h"

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
Bitmap* g_image = nullptr;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void                Initialize();
void                Finalize();
static void         ReleaseBackBuffer();
void                OnSize();
void                OnUpdate(float fElapsedTime_);
void                OnRender(HDC hdc, float fElapsedTime_);
void                OnIdle(float fElapsedTime_);

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

        OnIdle(float(dwNewTime - dwOldTime) / 1000.f);
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
    //g_image.FromFile( L"imageService.jpg" );
    g_image = new Bitmap( L"imageService.jpg" );
}//Initialize()

static void ReleaseBackBuffer()
{
    if (g_hdc != 0) {
        DeleteDC(g_hdc);
        g_hdc = 0;
    }
    if (g_hBitmap != 0) {
        DeleteObject(g_hBitmap);
        g_hBitmap = 0;
    }
}

void Finalize()
{
    if (g_image != nullptr)
    {
        delete g_image;
        g_image = nullptr;
    }

    ReleaseBackBuffer();
}//Finalize()

void OnSize()
{
    // Only recreate the DIB section / compatible bitmap. The texture (g_image) must stay
    // valid so ScanLineSegment / DrawOnCanvas keep a consistent Bitmap for UV sampling.
    ReleaseBackBuffer();

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

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = iWidth;
    bmi.bmiHeader.biHeight = -iHeight; // top-down, row 0 = top of client
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* dibBits = nullptr;
    g_hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &dibBits, nullptr, 0);
    SelectObject(g_hdc, g_hBitmap);

    const int stride = ((iWidth * 32 + 31) / 32) * 4;
    KVectorUtil::SetFrameBuffer(dibBits, iWidth, iHeight, stride);

    ::ReleaseDC(g_hwnd, hdc);
}//OnSize()

void OnUpdate(float fElapsedTime_)
{
}

void OnRender(HDC hdc, float fElapsedTime_)
{
    const float dotWidth = KVectorUtil::g_screenCoordinate.axis0.Length( );
    const float dotHeight = KVectorUtil::g_screenCoordinate.axis1.Length( );

    static float fRotDelta = 0.0f;
    fRotDelta += fElapsedTime_;

    KLight light;
    light.m_ambient = KRgb(0.1f, 0.1f, 0.1f);
    light.m_diffuse = KRgb(1, 1, 1);
    light.m_specular = KRgb(0, 0, 0);
    KRenderState::GetInstance().SetLight(light);

    KPolygon        poly;
    KMatrix4        matRotX;
    KMatrix4        matRotY;
    KMatrix4        matTrans;
    KMatrix4        matTransform;
    KMatrix4        matProjection;
    static float    s_fTheta = 0.0f;

    s_fTheta += fElapsedTime_ * 0.5f;
    matRotX.SetRotationX(s_fTheta);
    matRotY.SetRotationY(s_fTheta);
    matProjection.SetProjection(200.f);

    poly.SetIndexBuffer();

    // Cube A (farther along +Z in model space before projection overlap)
    matTrans.SetTranslation(10.f, 10.f, -18.f);
    matTransform = matTrans * matRotY * matRotX;
    poly.SetVertexBuffer();
    poly.Transform(matTransform);
    poly.Transform(matProjection);
    poly.DrawOnCanvas(hdc, *g_image);

    // Cube B (nearer) — z-buffer should hide parts of A behind B where they overlap
    matTrans.SetTranslation(-20.f, 10.f, 18.f);
    matTransform = matTrans * matRotY * matRotX;
    poly.SetVertexBuffer();
    poly.Transform(matTransform);
    poly.Transform(matProjection);
    poly.DrawOnCanvas(hdc, *g_image);

    //KVectorUtil::DrawGrid( hdc, 300, 300 );
    KVectorUtil::DrawAxis(hdc, 300, 300);

    static float s_fpsAccumTime = 0.f;
    static int s_fpsFrameCount = 0;
    static WCHAR s_fpsText[64] = L"FPS: --";

    s_fpsAccumTime += fElapsedTime_;
    s_fpsFrameCount += 1;
    if (s_fpsAccumTime >= 0.5f)
    {
        const float fps = s_fpsFrameCount / s_fpsAccumTime;
        swprintf_s(s_fpsText, L"FPS: %.1f", fps);
        s_fpsAccumTime = 0.f;
        s_fpsFrameCount = 0;
    }
    KVectorUtil::OutText(hdc, 8, 8, s_fpsText, RGB(0, 0, 0));

    // GDI+ image test. _20180705_jintaeks
    //Graphics    graphics( hdc );
    //graphics.DrawImage( g_image, 0, 0, 100, 100 );
}

void OnIdle(float fElapsedTime_)
{
    const int iWidth = g_clientRect.right - g_clientRect.left + 1;
    const int iHeight = g_clientRect.bottom - g_clientRect.top + 1;

    OnUpdate(fElapsedTime_);
    HDC hdc = ::GetDC(g_hwnd);

    KVectorUtil::ClearFrameBufferWhite();
    KVectorUtil::ClearDepthBuffer();

    {
        OnRender( g_hdc, fElapsedTime_);
    }

    BitBlt(hdc, 0, 0, iWidth, iHeight, g_hdc, 0, 0, SRCCOPY);

    ::ReleaseDC(g_hwnd, hdc);
}//OnIdle()
