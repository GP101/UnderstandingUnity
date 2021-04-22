// LinearAlgebra.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "LinearAlgebra.h"
//_20180519_jintaeks
#include "KVectorUtil.h"
#include <MMSystem.h>
#include "KMatrix2.h"
#include <objidl.h>
#include <gdiplus.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <windowsx.h>
#include "KVector2.h"
#include <vector>
#include <limits>

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

using KPolygon = std::vector<KVector2>;

struct CollInfo
{
	KVector2 p0;
	KVector2 p1;
	float	 fMpvLen = 0.0f; // length of Minimum Push Vector
	//float	 fp1MpvLen = 0.0f; // length of Minimum Push Vector
	bool     p0Valid;
	bool     p1Valid;
	KVector2 normal0;
	KVector2 normal1;

	CollInfo()
	{
		p0Valid = false;
		p1Valid = false;
	}
};

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void                Initialize();
void                Finalize();
void                OnSize();
void                OnRender(HDC hdc);
void                OnIdle(float fElapsedTime_);
void                OnLButtonDown( int x, int y );
void                DrawPolygon(HDC hdc, const KPolygon& a, COLORREF color);
bool                Intersect(const KPolygon& a, const KPolygon& b, CollInfo* pColl0 = nullptr, CollInfo* pColl1=nullptr);
bool				_IntersectInternal(const KPolygon& a, const KPolygon& b, CollInfo* pOutCollInfo = nullptr);


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
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

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
    GdiplusShutdown(gdiplusToken);

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
    case WM_LBUTTONDOWN:
        OnLButtonDown( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
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
    KVectorUtil::g_screenCoordinate.SetInfo(KVector2(60, 0), KVector2(0, -60), origin);

    HDC hdc = ::GetDC(g_hwnd);
    g_hdc = CreateCompatibleDC(hdc);
    g_hBitmap = CreateCompatibleBitmap(hdc, iWidth, iHeight);
    SelectObject(g_hdc, g_hBitmap);
}//OnSize()

void OnRender(HDC hdc)
{
    POINT mousePoint;
    GetCursorPos(&mousePoint);
    ScreenToClient(g_hwnd, &mousePoint);
    KVector2 vmouse = KVectorUtil::ScreenToWorld(KVector2(mousePoint.x, mousePoint.y));
    //KVector2 vdir = vmouse;
    //vdir.Normalize();
    //KVectorUtil::DrawLine(hdc, KVector2(0, 0), vdir* 1.5f, 2, PS_DASH);

	//KPolygon a{ KVector2(1,1), KVector2(0,2), KVector2(-2,-1) };// for debugging
	KPolygon a{ KVector2(1,1), KVector2(0,2), KVector2(-2,-1), KVector2(-1, -2), KVector2(1.5f, -1.0f) };
    KPolygon b{ KVector2(1,1), KVector2(-1.0f,1.5f), KVector2(-1.5f,-1.3f), KVector2(0.5f,-1.0f), KVector2(2,0) };
    for (auto& v : b)
    {
        v = v + vmouse;
    }

	CollInfo coll0, coll1;
    const bool isIntersect = Intersect(a, b, &coll0, &coll1);
	//const bool isMousePointInPolygon = KVectorUtil::IsPointInPolygon(vmouse, a);
    DrawPolygon(hdc, a, RGB(0,0,0));
    DrawPolygon(hdc, b, isIntersect ? RGB(255, 0, 0) : RGB(0, 0, 0));
	if (coll0.p0Valid)
	{
		KVectorUtil::DrawArrow(hdc, coll0.p0, coll0.p0 + coll0.normal0 * coll0.fMpvLen, 0.1f, 4, PS_SOLID, RGB(0, 0, 255));
	}
	if (coll0.p1Valid)
	{
		KVectorUtil::DrawArrow(hdc, coll0.p1, coll0.p1 + coll0.normal1 * coll0.fMpvLen, 0.1f, 4, PS_SOLID, RGB(0, 0, 255));
	}
	if (coll1.p0Valid)
	{
		KVectorUtil::DrawArrow(hdc, coll1.p0, coll1.p0 + coll1.normal0 * coll1.fMpvLen, 0.1f, 2, PS_SOLID, RGB(255, 0, 255));
	}
	if (coll1.p1Valid)
	{
		KVectorUtil::DrawArrow(hdc, coll1.p1, coll1.p1 + coll1.normal1 * coll1.fMpvLen, 0.1f, 2, PS_SOLID, RGB(255, 0, 255));
	}
}

void OnIdle(float fElapsedTime_)
{
    const int iWidth = g_clientRect.right - g_clientRect.left + 1;
    const int iHeight = g_clientRect.bottom - g_clientRect.top + 1;

    HDC hdc = ::GetDC(g_hwnd);

    HBRUSH brush;
    brush = CreateSolidBrush(RGB(255, 255, 255));
    HGDIOBJ oldBrush = SelectObject(g_hdc, brush);
    Rectangle(g_hdc, 0, 0, iWidth, iHeight);

    {
        KBasis2     basis2;
        basis2.SetInfo(KVector2(1, 0), KVector2(0, 1));
        KVectorUtil::SetBasis2(basis2);

        KVectorUtil::DrawGrid(g_hdc, 40, 20);
        KVectorUtil::DrawAxis(g_hdc, 42, 22, RGB(255,0,0), RGB(0,255,0));
    }

    OnRender(g_hdc);

    BitBlt(hdc, 0, 0, iWidth, iHeight, g_hdc, 0, 0, SRCCOPY);
    SelectObject(g_hdc, oldBrush);
    DeleteObject(brush);

    ::ReleaseDC(g_hwnd, hdc);
}//OnIdle()

void OnLButtonDown(int x, int y)
{
}

void DrawPolygon(HDC hdc, const KPolygon& a, COLORREF color)
{
    const int numVertices = a.size() + 0;
    for (int i = 0; i < numVertices - 1; ++i) {
        KVectorUtil::DrawLine(hdc, a[i], a[i + 1], 2, 0, color);
    }
    KVectorUtil::DrawLine(hdc, a[numVertices-1], a[0], 2, 0, color);
}

bool Intersect(const KPolygon& a, const KPolygon& b, CollInfo* pColl0, CollInfo* pColl1)
{
	const bool isIntersect = _IntersectInternal(a, b, pColl0) && _IntersectInternal(b, a, pColl1);

	return isIntersect;
}

/// https://gist.github.com/nyorain/dc5af42c6e83f7ac6d831a2cfd5fbece
/// Returns whether the two given convex polygons intersect using the
/// separating axis theorem. The given polygons can be in clockwise or
/// counter-clockwise order (does not matter).
bool _IntersectInternal(const KPolygon& a, const KPolygon& b, CollInfo* pColl)
{
	if (pColl != nullptr)
	{
		pColl->fMpvLen = std::numeric_limits<float>::infinity();
		//pColl->fp1MpvLen = std::numeric_limits<float>::infinity();
		pColl->p0Valid = false;
		pColl->p1Valid = false;
	}

    // loop over the vertices(-> edges -> axis) of the first polygon
    for (auto i = 0u; i < a.size() + 0; ++i) {
        // calculate the normal vector of the current edge
        // this is the axis will we check in this loop
        auto current = a[i];
        auto next = a[(i + 1) % a.size()];
        auto edge = next - current;

        KVector2 axis{};
        axis[0] = -edge[1];
        axis[1] = edge[0];
        axis.Normalize();

		KVector2 vMinA, vMaxA;
		KVector2 vMinB, vMaxB;
		KVector2 p0, p1;
		float fMpvLen = 0.f;
		KVector2 axisMinA, axisMaxA;
		KVector2 axisMinB, axisMaxB;
		KVector2 dir0, dir1;

        // loop over all vertices of both polygons and project them
        // onto the axis. We are only interested in max/min projections
        float aMaxProj = -std::numeric_limits<float>::infinity();
        float aMinProj = std::numeric_limits<float>::infinity();
		float bMaxProj = -std::numeric_limits<float>::infinity();
		float bMinProj = std::numeric_limits<float>::infinity();
        for (const auto& v : a) {
			float proj = KVector2::Dot(axis, v);
			if (proj < aMinProj)
			{
				aMinProj = proj;
				vMinA = v;
				axisMinA = axis;
			}
			if (proj > aMaxProj)
			{
				aMaxProj = proj;
				vMaxA = v;
				axisMaxA = axis;
			}
        }

        for (const auto& v : b) {
            auto proj = KVector2::Dot(axis, v);
			if (proj < bMinProj)
			{
				bMinProj = proj;
				vMinB = v;
				axisMinB = axis;
			}
			if (proj > bMaxProj)
			{
				bMaxProj = proj;
				vMaxB = v;
				axisMaxB = axis;
			}
        }

        // now check if the intervals the both polygons projected on the
        // axis overlap. If they don't, we have found an axis of separation and
        // the given polygons cannot overlap
        if (aMaxProj < bMinProj || aMinProj > bMaxProj) {
            return false;
        }
		if (aMaxProj < bMaxProj)
		{
			fMpvLen = aMaxProj - bMinProj;
			p0 = vMinB;
			p1 = vMaxA;
			dir0 = axisMinB;
			dir1 = axisMaxA;
		}
		else
		{
			fMpvLen = bMaxProj - aMinProj;
			p0 = vMaxB;
			p1 = vMinA;
			dir0 = -axisMaxB;
			dir1 = -axisMinA;
		}


		if (pColl != nullptr && fMpvLen < pColl->fMpvLen)
		{
			pColl->fMpvLen = fMpvLen;
			pColl->p0 = p0;
			pColl->p1 = p1;
			dir0.Normalize();
			dir1.Normalize();
			pColl->normal0 = dir0;
			pColl->normal1 = -dir1;
		}
    }

    // at this point, we have checked all axis but found no separating axis
    // which means that the polygons must intersect.
	if (pColl != nullptr)
	{
		pColl->p0Valid = KVectorUtil::IsPointInPolygon(pColl->p0, a);
		pColl->p1Valid = KVectorUtil::IsPointInPolygon(pColl->p1, b);
	}
    return true;
}
