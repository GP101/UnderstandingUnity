#pragma once
#include <windows.h>
#include "KVector2.h"
#include "KBasis2.h"
#include "KScreenCoordinate.h"
#include <objidl.h>
#include <gdiplus.h>

namespace KVectorUtil
{
    extern KScreenCoordinate    g_screenCoordinate;
    extern KBasis2  g_basis2;

    void SetScreenCoordinate(const KScreenCoordinate& screenCoord);
    void SetBasis2(const KBasis2& basis2);
    void DrawLine(HDC hdc, const KVector2& v0, const KVector2& v1
        , int lineWidth = 1, int penStyle = PS_SOLID, COLORREF color = RGB(0, 0, 0));
    void DrawLine(HDC hdc, float x0, float y0, float x1, float y1
        , int lineWidth = 1, int penStyle = PS_SOLID, COLORREF color = RGB(0, 0, 0));
    void DrawAxis(HDC hdc, int numHorizontalGrid, int numVerticalGrid , COLORREF color = RGB(64, 64, 64));
    void DrawGrid(HDC hdc, int numHorizontalGrid, int numVerticalGrid, COLORREF color = RGB(64, 64, 64));
    void PutPixel(HDC hdc, int x, int y, Gdiplus::Color color = Gdiplus::Color::Black);
}
