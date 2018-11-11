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
        , KVector2(x, y + 1)
        , KVector2(x + 1, y + 1)
        , KVector2(x + 1, y) };
    for (int i = 0; i < 4; ++i)
    {
        KVector2 v0 = g_basis2.Transform(v[i]);
        v0 = g_screenCoordinate.Transform(v0);
        point[i].X = v0.x;
        point[i].Y = v0.y;
    }

    Gdiplus::SolidBrush  brush(color);
    graphics.FillPolygon(&brush, point, 4);
    //graphics.FillRectangle(&brush, v0.x, v0.y, dotWidth, dotHeight);
}

void KVectorUtil::_ScanLineLow(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color)
{
    auto sign = [](float delta){ return delta > 0.f ? 1.0f : -1.0f; };

    float deltax = x1 - x0;
    float deltay = y1 - y0;
    float deltaerr = abs(deltay / deltax); // Assume deltax != 0 (line is not vertical),
    // note that this division needs to be done in a way that preserves the fractional part
    float error = 0.0f; // No error at start
    int y = y0;
    for (int x = x0; x <= x1; ++x)
    {
        PutPixel(hdc, x, y, color);
        error = error + deltaerr;
        if (error >= 0.5f)
        {
            y = y + sign(deltay) * 1.0f;
            error = error - 1.0f;
        }
    }
}

void KVectorUtil::_ScanLineHigh(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color)
{
    auto sign = [](float delta){ return delta > 0.f ? 1.0f : -1.0f; };

    float deltax = x1 - x0;
    float deltay = y1 - y0;
    float deltaerr = abs(deltax / deltay); // Assume deltax != 0 (line is not vertical),
    // note that this division needs to be done in a way that preserves the fractional part
    float error = 0.0f; // No error at start
    int x = x0;
    for (int y = y0; y <= y1; ++y)
    {
        PutPixel(hdc, x, y, color);
        error = error + deltaerr;
        if (error >= 0.5f)
        {
            x = x + sign(deltax) * 1.0f;
            error = error - 1.0f;
        }
    }
}

void KVectorUtil::ScanLine(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color)
{
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) {
            _ScanLineLow(hdc, x1, y1, x0, y0);
        }
        else {
            _ScanLineLow(hdc, x0, y0, x1, y1);
        }

    }
    else {
        if (y0 > y1) {
            _ScanLineHigh(hdc, x1, y1, x0, y0);
        }
        else {
            _ScanLineHigh(hdc, x0, y0, x1, y1);
        }
    }
}

void KVectorUtil::ScanPlotLineOctant0(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int D = 2 * dy - dx;
    int y = y0;

    for( int x = x0; x <= x1; ++x )
    {
        PutPixel(hdc, x, y, color);
        if (D > 0) {
            y = y + 1;
            D = D - 2 * dx;
        }
        D = D + 2 * dy;
    }
}

void KVectorUtil::_ScanPlotLineLow(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    int D = 2 * dy - dx;
    int y = y0;

    for (int x = x0; x <= x1; ++x)
    {
        PutPixel(hdc, x, y);
        if (D > 0) {
            y = y + yi;
            D = D - 2 * dx;
        }
        D = D + 2 * dy;
    }
}

void KVectorUtil::_ScanPlotLineHigh(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;
    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }

    int D = 2 * dx - dy;
    int x = x0;
    for (int y = y0; y <= y1; ++y) {
        PutPixel( hdc, x, y);
        if (D > 0) {
            x = x + xi;
            D = D - 2 * dy;
        }
        D = D + 2 * dx;
    }
}

void KVectorUtil::ScanPlotLine(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color)
{
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) {
            _ScanPlotLineLow(hdc, x1, y1, x0, y0);
        }
        else {
            _ScanPlotLineLow(hdc, x0, y0, x1, y1);
        }

    } 
    else {
        if (y0 > y1) {
            _ScanPlotLineHigh(hdc, x1, y1, x0, y0);
        }
        else {
            _ScanPlotLineHigh(hdc, x0, y0, x1, y1);
        }
    }
}
