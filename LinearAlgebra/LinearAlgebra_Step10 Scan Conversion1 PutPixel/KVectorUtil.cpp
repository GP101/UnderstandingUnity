#include "stdafx.h"
#include "KVectorUtil.h"

KBasis2             KVectorUtil::g_basis2;
KScreenCoordinate   KVectorUtil::g_screenCoordinate;

void KVectorUtil::SetScreenCoordinate(const KScreenCoordinate& screenCoord)
{
    g_screenCoordinate = screenCoord;
}

void KVectorUtil::SetBasis2(const KBasis2& basis2)
{
    g_basis2 = basis2;
}

void KVectorUtil::DrawLine(HDC hdc, const KVector2& v0_, const KVector2& v1_, int lineWidth, int penStyle, COLORREF color)
{
    KVector2 v0 = g_basis2.Transform(v0_);
    KVector2 v1 = g_basis2.Transform(v1_);
    v0 = g_screenCoordinate.Transform(v0);
    v1 = g_screenCoordinate.Transform(v1);

    HPEN hpen = CreatePen(penStyle, lineWidth, color);
    HGDIOBJ original = SelectObject(hdc, hpen);
    {
        MoveToEx(hdc, (int)v0.x, (int)v0.y, nullptr);
        LineTo(hdc, (int)v1.x, (int)v1.y);
    }
    SelectObject(hdc, original);
    DeleteObject(hpen);
}

void KVectorUtil::DrawLine(HDC hdc, float x0, float y0, float x1, float y1
    , int lineWidth, int penStyle, COLORREF color)
{
    DrawLine(hdc, KVector2(x0, y0), KVector2(x1, y1), lineWidth, penStyle, color);
}

void KVectorUtil::DrawAxis(HDC hdc, int numHorizontalGrid, int numVerticalGrid, COLORREF color)
{
    {
        KVector2 v0 = KVector2(0, -numVerticalGrid / 2);
        KVector2 v1 = KVector2(0, numVerticalGrid / 2);
        KVectorUtil::DrawLine(hdc, v0, v1, 2, PS_SOLID, color );
    }

    {
        KVector2 v0 = KVector2(-numHorizontalGrid / 2, 0);
        KVector2 v1 = KVector2(numHorizontalGrid / 2, 0);
        KVectorUtil::DrawLine(hdc, v0, v1, 2, PS_SOLID, color );
    }
}

void KVectorUtil::DrawGrid( HDC hdc, int numHorizontalGrid, int numVerticalGrid, COLORREF color)
{
    int hbegin = int(-numHorizontalGrid / 2.0f - 0.5f);
    for (int count = 0; count <= numHorizontalGrid; ++count) {
        KVector2 v0 = KVector2(hbegin, 0) + KVector2(0, -numVerticalGrid / 2);
        KVector2 v1 = KVector2(hbegin, 0) + KVector2(0, numVerticalGrid / 2);
        KVectorUtil::DrawLine(hdc, v0, v1, 1, PS_DOT, color );
        hbegin += 1;
    }

    int vbegin = int(-numVerticalGrid / 2.0f - 0.5f);
    for (int count = 0; count <= numVerticalGrid; ++count) {
        KVector2 v0 = KVector2(0,vbegin) + KVector2( -numHorizontalGrid / 2, 0 );
        KVector2 v1 = KVector2(0,vbegin)+ KVector2( numHorizontalGrid / 2, 0);
        KVectorUtil::DrawLine(hdc, v0, v1, 1, PS_DOT, color);
        vbegin += 1;
    }
}

void KVectorUtil::PutPixel(HDC hdc, int x, int y, Gdiplus::Color color)
{
    Gdiplus::Graphics    graphics(hdc);
    const float dotWidth = g_screenCoordinate.axis0.Length();
    const float dotHeight = g_screenCoordinate.axis1.Length();

    Gdiplus::Point  point[4];
    KVector2 v[4] = { KVector2(x, y)
        , KVector2(x, y+1)
        , KVector2(x+1, y+1)
        , KVector2(x + 1, y) };
    for (int i = 0; i < 4; ++i)
    {
        KVector2 v0 = g_basis2.Transform(v[i]);
        v0 = g_screenCoordinate.Transform(v0);
        point[i].X = (int)v0.x;
        point[i].Y = (int)v0.y;
    }

    Gdiplus::SolidBrush  brush(color);
    graphics.FillPolygon(&brush, point, 4);
    //graphics.FillRectangle(&brush, v0.x, v0.y, dotWidth, dotHeight);
}
