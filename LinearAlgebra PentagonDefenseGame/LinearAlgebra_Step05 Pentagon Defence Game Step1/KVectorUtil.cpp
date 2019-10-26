#include "stdafx.h"
#include "KVectorUtil.h"
#include "KMatrix2.h"
#define _USE_MATH_DEFINES
#include <cmath>

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

KVector2 KVectorUtil::ScreenToWorld(const KVector2& v0_)
{
    KMatrix2 m0;
    m0.Set( g_basis2.basis0, g_basis2.basis1 );
    KMatrix2 m1;
    m1.Set( g_screenCoordinate.axis0, g_screenCoordinate.axis1 );

    // Vscreen = Mscreen * Mworld * Vworld
    // MworldInv * MscreenInv * Vscreen = Vworld
    KVector2 v = v0_ - g_screenCoordinate.origin;
    m1 = m1.GetInverse();
    m0 = m0.GetInverse();
    v = m0 * m1 * v;
    return v;
}

void KVectorUtil::DrawLine(HDC hdc, const KVector2& v0_, const KVector2& v1_, int lineWidth, int penStyle, COLORREF color_)
{
    KVector2 v0 = g_basis2.Transform(v0_);
    KVector2 v1 = g_basis2.Transform(v1_);
    v0 = g_screenCoordinate.Transform(v0);
    v1 = g_screenCoordinate.Transform(v1);

    HPEN hpen = CreatePen(penStyle, lineWidth, color_);
    HGDIOBJ original = SelectObject(hdc, hpen);
    {
        MoveToEx(hdc, (int)v0.x, (int)v0.y, nullptr);
        LineTo(hdc, (int)v1.x, (int)v1.y);
    }
    SelectObject(hdc, original);
    DeleteObject(hpen);
}

void KVectorUtil::DrawAxis(HDC hdc, int numHorizontalGrid, int numVerticalGrid, COLORREF color1, COLORREF color2)
{
    {
        KVector2 v0 = KVector2(0, -numVerticalGrid / 2);
        KVector2 v1 = KVector2(0, numVerticalGrid / 2);
        KVectorUtil::DrawLine(hdc, v0, v1, 2, PS_SOLID, color1 );
    }

    {
        KVector2 v0 = KVector2(-numHorizontalGrid / 2, 0);
        KVector2 v1 = KVector2(numHorizontalGrid / 2, 0);
        KVectorUtil::DrawLine(hdc, v0, v1, 2, PS_SOLID, color2 );
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

void KVectorUtil::DrawCircle( HDC hdc, const KVector2& center, float radius, int numSegment
    , int lineWidth, int penStyle, COLORREF color)
{
    const double dt = (2.0 * M_PI) / numSegment;
    double theta = 0;
    const KVector2 p0 = KVector2(radius, 0.0f);
    KVector2 p1 = p0;
    KVector2 p2;
    for (int i = 0; i <= numSegment; ++i)
    {
        KMatrix2 m;
        theta += dt;
        m.SetRotation((float)theta);
        p2 = m * p0;
        DrawLine(hdc, p1+center, p2+center, lineWidth, penStyle, color);
        p1 = p2;
    }
}
