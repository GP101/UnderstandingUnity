#pragma once
#include <windows.h>
#include "KVector2.h"
#include "KBasis2.h"
#include "KScreenCoordinate.h"
#include <objidl.h>
#include <gdiplus.h>
#include "KRgb.h"
#include <set>

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

    void _ScanLineLow(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);
    void _ScanLineHigh(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);
    void ScanLine(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);

    void ScanPlotLineOctant0(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);
    void _ScanPlotLineLow(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);
    void _ScanPlotLineHigh(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);
    void ScanPlotLine(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);

    void ScanLineSegment(HDC hdc, int x1, int y1, KRgb c1, int x2, int y2, KRgb c2
        , std::set<ScannedResult>* output);
    void FillTriangle(HDC hdc, int x1, int y1, KRgb const col1
        , int x2, int y2, KRgb const col2
        , int x3, int y3, KRgb const col3);
}
