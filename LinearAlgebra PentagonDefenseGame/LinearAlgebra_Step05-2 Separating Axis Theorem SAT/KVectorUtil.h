#pragma once
#include <windows.h>
#include "KVector2.h"
#include "KBasis2.h"
#include "KScreenCoordinate.h"

namespace KVectorUtil
{
    extern KScreenCoordinate    g_screenCoordinate;
    extern KBasis2  g_basis2;

    void SetScreenCoordinate(const KScreenCoordinate& screenCoord);
    void SetBasis2(const KBasis2& basis2);
    KVector2 ScreenToWorld(const KVector2& v0_);
    void DrawLine(HDC hdc, const KVector2& v0, const KVector2& v1
        , int lineWidth = 1, int penStyle = PS_SOLID, COLORREF color = RGB(0,0,0) );
    void DrawAxis(HDC hdc, int numHorizontalGrid, int numVerticalGrid , COLORREF color1 = RGB(64, 64, 64), COLORREF color2 = RGB(64, 64, 64));
    void DrawGrid(HDC hdc, int numHorizontalGrid, int numVerticalGrid, COLORREF color = RGB(64, 64, 64));
    void DrawCircle(HDC hdc, const KVector2& center, float radius, int numSegment
        , int lineWidth = 1, int penStyle = PS_SOLID, COLORREF color = RGB(0, 0, 0));
    float LengthSquared(const KVector2& a, const KVector2& b);
    float Length(const KVector2& a, const KVector2& b);
    float PointLinesegmentDistance(KVector2 p, KVector2 v, KVector2 w);
}
