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
    KVector2 v = v0_ - g_screenCoordinate.origin; // inverse translation
    KMatrix2 m1Inv = m1.GetInverse();
    KMatrix2 m0Inv = m0.GetInverse();
    v = m0Inv * m1Inv * v;
    return v;
}

void KVectorUtil::DrawLine(HDC hdc, const KVector2& v0_, const KVector2& v1_, int lineWidth, int penStyle, COLORREF color_)
{
	KMatrix2    basis;
	KMatrix2    screen;
	basis.Set(g_basis2.basis0, g_basis2.basis1);
	screen.Set(g_screenCoordinate.axis0, g_screenCoordinate.axis1);

	KVector2 v0;// = g_basis2.Transform(v0_);
	KVector2 v1;// = g_basis2.Transform(v1_);
	v0 = screen * basis * v0_;
	v1 = screen * basis * v1_;

    //v0 = g_screenCoordinate.Transform(v0);
    //v1 = g_screenCoordinate.Transform(v1);
	v0 = v0 + g_screenCoordinate.origin;
	v1 = v1 + g_screenCoordinate.origin;

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

float KVectorUtil::LengthSquared(const KVector2& a, const KVector2& b)
{
    KVector2 t = b - a;
    return t.x * t.x + t.y * t.y;
}

float KVectorUtil::Length(const KVector2& a, const KVector2& b)
{
    return sqrtf(LengthSquared(a, b));
}

float KVectorUtil::PointLinesegmentDistance(KVector2 p, KVector2 v, KVector2 w)
{
    // Return minimum distance between line segment vw and point p
    const float l2 = LengthSquared(v, w);  // i.e. |w-v|^2 -  avoid a sqrt
    if (l2 <= 0.0001f) return Length(p, v);   // v == w case
    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line. 
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    // We clamp t from [0,1] to handle points outside the segment vw.
    const float t = __max(0, __min(1, KVector2::Dot(p - v, w - v) / l2));
    const KVector2 projection = v + t * (w - v);  // Projection falls on the segment
    return Length(p, projection);
}
