#pragma once
#include <windows.h>
#include "KVector2.h"
#include "KBasis2.h"
#include "KScreenCoordinate.h"
#include <objidl.h>
#include <gdiplus.h>
#include "KRgb.h"
#include <set>
#include <objidl.h>
#include <gdiplus.h>

using namespace Gdiplus;

namespace KVectorUtil
{
    extern KScreenCoordinate    g_screenCoordinate;
    extern KBasis2  g_basis2;

    /// 32bpp top-down DIB (BGRA) — set from CreateDIBSection; nullptr disables fast PutPixel path.
    void SetFrameBuffer(void* bits, int width, int height, int strideBytes);
    void ClearFrameBufferWhite();
    /// Clears depth buffer (same pixel grid as frame buffer). Smaller z = nearer (pass if z < stored).
    void ClearDepthBuffer(float farZ = 3.402823466e+38F /* FLT_MAX */);

    void SetScreenCoordinate(const KScreenCoordinate& screenCoord);
    void SetBasis2(const KBasis2& basis2);
    void DrawLine(HDC hdc, const KVector2& v0, const KVector2& v1
        , int lineWidth = 1, int penStyle = PS_SOLID, COLORREF color = RGB(0, 0, 0));
    void DrawLine(HDC hdc, float x0, float y0, float x1, float y1
        , int lineWidth = 1, int penStyle = PS_SOLID, COLORREF color = RGB(0, 0, 0));
    void DrawAxis(HDC hdc, int numHorizontalGrid, int numVerticalGrid , COLORREF color = RGB(64, 64, 64));
    void DrawGrid(HDC hdc, int numHorizontalGrid, int numVerticalGrid, COLORREF color = RGB(64, 64, 64));
    void PutPixel(HDC hdc, int x, int y, Gdiplus::Color color = Gdiplus::Color::Black);
    /// Same as PutPixel but participates in z-buffer when depth buffer is allocated (see SetFrameBuffer).
    void PutPixelDepth(HDC hdc, int x, int y, Gdiplus::Color color, float z);
    void OutText(HDC hdc, int x, int y, const WCHAR* text, COLORREF color = RGB(0, 0, 0));

    void _ScanLineLow(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);
    void _ScanLineHigh(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);
    void ScanLine(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);

    void ScanPlotLineOctant0(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);
    void _ScanPlotLineLow(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);
    void _ScanPlotLineHigh(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);
    void ScanPlotLine(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color = Gdiplus::Color::Black);

    /// When drawing to the framebuffer (output == nullptr), subtracts depthBias from z so
    /// lines sit slightly nearer than filled pixels (smaller z passes the depth test).
    void ScanLineSegment(HDC hdc, int x1, int y1, KRgb c1, float z1, int x2, int y2, KRgb c2, float z2
        , std::set<ScannedResult>* output, float depthBias = 0.f);
    void ScanLineSegment(HDC hdc, int x1, int y1, KRgb c1, int x2, int y2, KRgb c2
        , std::set<ScannedResult>* output);
    void FillTriangle(HDC hdc, int x1, int y1, KRgb const col1, float z1
        , int x2, int y2, KRgb const col2, float z2
        , int x3, int y3, KRgb const col3, float z3);
    void FillTriangle(HDC hdc, int x1, int y1, KRgb const col1
        , int x2, int y2, KRgb const col2
        , int x3, int y3, KRgb const col3);

    void ScanLineSegment(HDC hdc, int x1, int y1, KRgb c1, float z1, int x2, int y2, KRgb c2, float z2
        , std::set<ScannedResult>* output, Bitmap& bitmap, KRgb shadeColor);
    void FillTriangle(HDC hdc, int x1, int y1, KRgb const col1, float z1
        , int x2, int y2, KRgb const col2, float z2
        , int x3, int y3, KRgb const col3, float z3, Bitmap& bitmap, KRgb shadeColor);
}
