#pragma once
#include <windows.h>
#include "KVector2.h"
#include "KBasis2.h"
#include "KScreenCoordinate.h"
#include <objidl.h>
#include <gdiplus.h>
#include "KRgb.h"
#include <set>
#include <vector>

namespace KVectorUtil
{
    extern KScreenCoordinate    g_screenCoordinate;
    extern KBasis2  g_basis2;

    void SetScreenCoordinate(const KScreenCoordinate& screenCoord);
    void SetBasis2(const KBasis2& basis2);
    KVector2 ScreenToWorld(const KVector2& v0_);
    KVector2 WorldToScreen(const KVector2& v0);
    void DrawLine(HDC hdc, const KVector2& v0, const KVector2& v1
        , int lineWidth = 1, int penStyle = PS_SOLID, COLORREF color = RGB(0, 0, 0));
    void DrawLine(HDC hdc, float x0, float y0, float x1, float y1
        , int lineWidth = 1, int penStyle = PS_SOLID, COLORREF color = RGB(0, 0, 0));
    void DrawAxis(HDC hdc, int numHorizontalGrid, int numVerticalGrid , COLORREF color = RGB(64, 64, 64));
    void DrawGrid(HDC hdc, int numHorizontalGrid, int numVerticalGrid, COLORREF color = RGB(64, 64, 64));
    void DrawCircle(HDC hdc, const KVector2& center, float radius, int numSegment
        , int lineWidth = 1, int penStyle = PS_SOLID, COLORREF color = RGB(0, 0, 0));

    void PutPixel(HDC hdc, int x, int y, Gdiplus::Color color = Gdiplus::Color::Black);

    void _ScanLineLow(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);
    void _ScanLineHigh(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);
    void ScanLine(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);

    void ScanPlotLineOctant0(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);
    void _ScanPlotLineLow(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);
    void _ScanPlotLineHigh(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);
    void ScanPlotLine(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);
    void ScanMidpointCircle(HDC hdc, int x_centre, int y_centre, int r, Gdiplus::Color color = Gdiplus::Color::Black);

    void ScanLineSegment(HDC hdc, int x1, int y1, KRgb c1, int x2, int y2, KRgb c2
        , std::set<ScannedResult>* output);
    void FillTriangle(HDC hdc, int x1, int y1, KRgb const col1
        , int x2, int y2, KRgb const col2
        , int x3, int y3, KRgb const col3);

    float LengthSquared(const KVector2& a, const KVector2& b);
    float Length(const KVector2& a, const KVector2& b);
    float PointLinesegmentDistance(KVector2 p, KVector2 v0, KVector2 v1);
    float PointLineDistance(KVector2 p, KVector2 v0, KVector2 v1);
    /// check whether point p is in the convex polygon of 'points'
    bool IsPointInPolygon(const KVector2& p, const std::vector<KVector2>& points);
    bool IsPointInPolygon(const KVector2& p, const KVector2* points, int numPoints);
    bool LineSegementsIntersect(KVector2 p0, KVector2 p1, KVector2 q0, KVector2 q1
        , KVector2& out, bool considerCollinearOverlapAsIntersect = false);
    int LineSegmentPolygonIntersection(const KVector2& p0, const KVector2& p1, const std::vector<KVector2>& points);
    KVector2 GetGeoCenter(const KVector2* points, int vertexCount);
    KVector2 GetGeoCenter(const std::vector<KVector2>& points);
    void Clip(const std::vector<KVector2>& inPoints, const KVector2 p0, const KVector2 p1
        , std::vector<KVector2>& outPoints);
    void ScanDrawPolygon(HDC hdc, std::vector<KVector2>& points, COLORREF color);
    /// <summary>
    /// check whether vector bc is rotated CCW or CW with respect to ab.
    /// </summary>
    /// <param name="a"> first point </param>
    /// <param name="b"> second point </param>
    /// <param name="c"> thrid point </param>
    /// <returns> 0: colinear, 1: Clockwise, 2: Counter-Clockwise </returns>
    int GetDirection(const KVector2& a, const KVector2& b, const KVector2& c);
}
