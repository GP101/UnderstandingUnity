#include "stdafx.h"
#include "KVectorUtil.h"
#include <cassert>
#include <algorithm>
#include <cwchar>
#include <vector>
#include <cfloat>

KBasis2             KVectorUtil::g_basis2;
KScreenCoordinate   KVectorUtil::g_screenCoordinate;

namespace
{
    void*   g_frameBits = nullptr;
    int     g_frameWidth = 0;
    int     g_frameHeight = 0;
    int     g_frameStride = 0;
    std::vector<float> g_depthBuffer;

    static bool PointInConvexQuad(int px, int py, const int qx[4], const int qy[4])
    {
        auto cross = [](int ax, int ay, int bx, int by) -> __int64
        {
            return (__int64)ax * by - (__int64)ay * bx;
        };

        bool allNonNeg = true;
        bool allNonPos = true;
        for (int i = 0; i < 4; ++i)
        {
            const int x0 = qx[i];
            const int y0 = qy[i];
            const int x1 = qx[(i + 1) & 3];
            const int y1 = qy[(i + 1) & 3];
            const int dx = x1 - x0;
            const int dy = y1 - y0;
            const int fx = px - x0;
            const int fy = py - y0;
            const __int64 s = cross(dx, dy, fx, fy);
            if (s < 0) allNonNeg = false;
            if (s > 0) allNonPos = false;
        }
        return allNonNeg || allNonPos;
    }
}

void KVectorUtil::SetFrameBuffer(void* bits, int width, int height, int strideBytes)
{
    g_frameBits = bits;
    g_frameWidth = width;
    g_frameHeight = height;
    g_frameStride = strideBytes;
    if (bits && width > 0 && height > 0)
        g_depthBuffer.assign((size_t)width * (size_t)height, FLT_MAX);
    else
        g_depthBuffer.clear();
}

void KVectorUtil::ClearDepthBuffer(float farZ)
{
    if (g_depthBuffer.empty())
        return;
    std::fill(g_depthBuffer.begin(), g_depthBuffer.end(), farZ);
}

void KVectorUtil::ClearFrameBufferWhite()
{
    if (!g_frameBits || g_frameWidth <= 0 || g_frameHeight <= 0 || g_frameStride <= 0)
        return;
    const size_t nbytes = (size_t)g_frameStride * (size_t)g_frameHeight;
    memset(g_frameBits, 0xFF, nbytes);
}

void KVectorUtil::SetScreenCoordinate(const KScreenCoordinate& screenCoord)
{
    g_screenCoordinate = screenCoord;
}

void KVectorUtil::SetBasis2(const KBasis2& basis2)
{
    g_basis2 = basis2;
}

void KVectorUtil::OutText(HDC hdc, int x, int y, const WCHAR* text, COLORREF color)
{
    if (!text)
        return;
    const int len = (int)wcslen(text);
    const int prevBk = SetBkMode(hdc, TRANSPARENT);
    const COLORREF prevColor = SetTextColor(hdc, color);
    TextOutW(hdc, x, y, text, len);
    SetTextColor(hdc, prevColor);
    SetBkMode(hdc, prevBk);
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
    KVector2 v[4] = { KVector2(x, y)
        , KVector2(x, y + 1)
        , KVector2(x + 1, y + 1)
        , KVector2(x + 1, y) };

    int qx[4];
    int qy[4];
    for (int i = 0; i < 4; ++i)
    {
        KVector2 v0 = g_basis2.Transform(v[i]);
        v0 = g_screenCoordinate.Transform(v0);
        qx[i] = (int)v0.x;
        qy[i] = (int)v0.y;
    }

    const DWORD bgra = (DWORD)color.GetB()
        | ((DWORD)color.GetG() << 8)
        | ((DWORD)color.GetR() << 16)
        | ((DWORD)color.GetA() << 24);

    if (g_frameBits && g_frameWidth > 0 && g_frameHeight > 0 && g_frameStride >= 4)
    {
        int xmin = qx[0], xmax = qx[0], ymin = qy[0], ymax = qy[0];
        for (int i = 1; i < 4; ++i)
        {
            xmin = (std::min)(xmin, qx[i]);
            xmax = (std::max)(xmax, qx[i]);
            ymin = (std::min)(ymin, qy[i]);
            ymax = (std::max)(ymax, qy[i]);
        }

        xmin = (std::max)(xmin, 0);
        ymin = (std::max)(ymin, 0);
        xmax = (std::min)(xmax, g_frameWidth - 1);
        ymax = (std::min)(ymax, g_frameHeight - 1);

        if (xmin > xmax || ymin > ymax)
            return;

        BYTE* const base = static_cast<BYTE*>(g_frameBits);
        for (int py = ymin; py <= ymax; ++py)
        {
            DWORD* row = reinterpret_cast<DWORD*>(base + (size_t)py * (size_t)g_frameStride);
            for (int px = xmin; px <= xmax; ++px)
            {
                if (PointInConvexQuad(px, py, qx, qy))
                    row[px] = bgra;
            }
        }
        return;
    }

    Gdiplus::Graphics graphics(hdc);
    Gdiplus::Point point[4];
    for (int i = 0; i < 4; ++i)
    {
        point[i].X = qx[i];
        point[i].Y = qy[i];
    }
    Gdiplus::SolidBrush brush(color);
    graphics.FillPolygon(&brush, point, 4);
}

void KVectorUtil::PutPixelDepth(HDC hdc, int x, int y, Gdiplus::Color color, float z)
{
    if (g_depthBuffer.empty() || g_frameWidth <= 0 || g_frameHeight <= 0)
    {
        PutPixel(hdc, x, y, color);
        return;
    }

    KVector2 v[4] = { KVector2(x, y)
        , KVector2(x, y + 1)
        , KVector2(x + 1, y + 1)
        , KVector2(x + 1, y) };

    int qx[4];
    int qy[4];
    for (int i = 0; i < 4; ++i)
    {
        KVector2 v0 = g_basis2.Transform(v[i]);
        v0 = g_screenCoordinate.Transform(v0);
        qx[i] = (int)v0.x;
        qy[i] = (int)v0.y;
    }

    const DWORD bgra = (DWORD)color.GetB()
        | ((DWORD)color.GetG() << 8)
        | ((DWORD)color.GetR() << 16)
        | ((DWORD)color.GetA() << 24);

    if (g_frameBits && g_frameStride >= 4)
    {
        int xmin = qx[0], xmax = qx[0], ymin = qy[0], ymax = qy[0];
        for (int i = 1; i < 4; ++i)
        {
            xmin = (std::min)(xmin, qx[i]);
            xmax = (std::max)(xmax, qx[i]);
            ymin = (std::min)(ymin, qy[i]);
            ymax = (std::max)(ymax, qy[i]);
        }

        xmin = (std::max)(xmin, 0);
        ymin = (std::max)(ymin, 0);
        xmax = (std::min)(xmax, g_frameWidth - 1);
        ymax = (std::min)(ymax, g_frameHeight - 1);

        if (xmin > xmax || ymin > ymax)
            return;

        BYTE* const base = static_cast<BYTE*>(g_frameBits);
        for (int py = ymin; py <= ymax; ++py)
        {
            DWORD* row = reinterpret_cast<DWORD*>(base + (size_t)py * (size_t)g_frameStride);
            for (int px = xmin; px <= xmax; ++px)
            {
                if (!PointInConvexQuad(px, py, qx, qy))
                    continue;
                const size_t di = (size_t)py * (size_t)g_frameWidth + (size_t)px;
                if (z >= g_depthBuffer[di])
                    continue;
                g_depthBuffer[di] = z;
                row[px] = bgra;
            }
        }
        return;
    }

    PutPixel(hdc, x, y, color);
}

void KVectorUtil::_ScanLineLow(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color)
{
    auto sign = [](float delta){ return delta > 0.f ? 1.0f : -1.0f; };

    float deltax = float(x1 - x0);
    float deltay = float(y1 - y0);
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
            y = y + int(sign(deltay) * 1.0f);
            error = error - 1.0f;
        }
    }
}

void KVectorUtil::_ScanLineHigh(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color)
{
    auto sign = [](float delta){ return delta > 0.f ? 1.0f : -1.0f; };

    float deltax = float(x1 - x0);
    float deltay = float(y1 - y0);
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
            x = x + int(sign(deltax) * 1.0f);
            error = error - 1.0f;
        }
    }
}

void KVectorUtil::ScanLine(HDC hdc, int x0, int y0, int x1, int y1, Gdiplus::Color color)
{
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) {
            _ScanLineLow(hdc, x1, y1, x0, y0, color);
        }
        else {
            _ScanLineLow(hdc, x0, y0, x1, y1, color);
        }

    }
    else {
        if (y0 > y1) {
            _ScanLineHigh(hdc, x1, y1, x0, y0, color);
        }
        else {
            _ScanLineHigh(hdc, x0, y0, x1, y1, color);
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
        PutPixel(hdc, x, y, color);
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
        PutPixel(hdc, x, y, color);
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
            _ScanPlotLineLow(hdc, x1, y1, x0, y0, color);
        }
        else {
            _ScanPlotLineLow(hdc, x0, y0, x1, y1, color);
        }

    } 
    else {
        if (y0 > y1) {
            _ScanPlotLineHigh(hdc, x1, y1, x0, y0, color);
        }
        else {
            _ScanPlotLineHigh(hdc, x0, y0, x1, y1, color);
        }
    }
}

/*
Scan Conversion and Filling Triangle Algorithms
*/
void KVectorUtil::ScanLineSegment(HDC hdc, int x1, int y1, KRgb c1, int x2, int y2, KRgb c2
    , std::set<ScannedResult>* output)
{
    bool swap_xy = false;
    bool flip_y = false;

    auto _scanLineSegment = [&hdc, &swap_xy, &flip_y](int x1, int y1, KRgb const col1
        , int x2, int y2, KRgb const col2
        , std::set<ScannedResult>* output)
    {
        assert(x2 - x1 >= 0 && x2 - x1 >= y2 - y1 && y2 - y1 >= 0);

        bool horizontal = y1 == y2;
        bool diagonal = y2 - y1 == x2 - x1;
        float scale = 1.0f / (x2 - x1);

        KRgb col = col1, col_step = (col2 - col1) * scale * .99999f;

        int y = y1;
        float yy = (float)y1, y_step = (y2 - y1) * scale;

        for (int x = x1; x <= x2; ++x, col += col_step)
        {
            int X(x), Y(y);
            if (swap_xy) std::swap(X, Y);
            if (flip_y)  Y = -Y;

            if (output)
                output->insert(ScannedResult(X, Y, col));
            else {
                PutPixel(hdc, X, Y, col.GetGdiColor());
            }

            if (!horizontal)
            {
                if (diagonal) y++;
                else y = int(round(yy += y_step));
            }
        }
    };

    if (x1 > x2)
    {
        std::swap(x1, x2); std::swap(y1, y2); std::swap(c1, c2);
    }
    flip_y = (y1 > y2);
    if (flip_y)
    {
        y1 = -y1, y2 = -y2;
    }
    swap_xy = (y2 - y1 > x2 - x1);
    if (swap_xy)
    {
        std::swap(x1, y1), std::swap(x2, y2);
    }
    _scanLineSegment(x1, y1, c1, x2, y2, c2, output);
}

void KVectorUtil::ScanLineSegment(HDC hdc, int x1, int y1, KRgb c1, float z1, int x2, int y2, KRgb c2, float z2
    , std::set<ScannedResult>* output, float depthBias)
{
    bool swap_xy = false;
    bool flip_y = false;

    auto _scanLineSegment = [&hdc, &swap_xy, &flip_y, depthBias](int x1, int y1, KRgb const col1, float z1_
        , int x2, int y2, KRgb const col2, float z2_
        , std::set<ScannedResult>* output)
    {
        assert(x2 - x1 >= 0 && x2 - x1 >= y2 - y1 && y2 - y1 >= 0);

        bool horizontal = y1 == y2;
        bool diagonal = y2 - y1 == x2 - x1;
        float scale = 1.0f / (x2 - x1);

        KRgb col = col1, col_step = (col2 - col1) * scale * .99999f;
        float z = z1_;
        const float z_step = (z2_ - z1_) * scale * .99999f;

        int y = y1;
        float yy = (float)y1, y_step = (y2 - y1) * scale;

        for (int x = x1; x <= x2; ++x, col += col_step, z += z_step)
        {
            int X(x), Y(y);
            if (swap_xy) std::swap(X, Y);
            if (flip_y)  Y = -Y;

            if (output)
                output->insert(ScannedResult(X, Y, col, z));
            else
                PutPixelDepth(hdc, X, Y, col.GetGdiColor(), z - depthBias);

            if (!horizontal)
            {
                if (diagonal) y++;
                else y = int(round(yy += y_step));
            }
        }
    };

    if (x1 > x2)
    {
        std::swap(x1, x2); std::swap(y1, y2); std::swap(c1, c2);
        std::swap(z1, z2);
    }
    flip_y = (y1 > y2);
    if (flip_y)
    {
        y1 = -y1, y2 = -y2;
    }
    swap_xy = (y2 - y1 > x2 - x1);
    if (swap_xy)
    {
        std::swap(x1, y1), std::swap(x2, y2);
    }
    _scanLineSegment(x1, y1, c1, z1, x2, y2, c2, z2, output);
}

void KVectorUtil::FillTriangle(HDC hdc, int x1, int y1, KRgb const col1
    , int x2, int y2, KRgb const col2
    , int x3, int y3, KRgb const col3)

{
    std::set<ScannedResult> scanned_pnts;
    ScanLineSegment(hdc, x1, y1, col1, x2, y2, col2, &scanned_pnts);
    ScanLineSegment(hdc, x2, y2, col2, x3, y3, col3, &scanned_pnts);
    ScanLineSegment(hdc, x3, y3, col3, x1, y1, col1, &scanned_pnts);

    int cur_yval = INT_MIN;
    std::set<ScannedResult> same_yval;

    for (std::set<ScannedResult>::iterator it = scanned_pnts.begin(); it != scanned_pnts.end(); ++it)
    {
        int y = it->y;
        if (y != cur_yval)
        {
            if (same_yval.size())
            {
                std::set<ScannedResult>::iterator it1 = same_yval.begin(), it2 = --same_yval.end();
                ScanLineSegment(hdc, it1->x, cur_yval, it1->col,
                    it2->x, cur_yval, it2->col, nullptr);

                same_yval.clear();
            }
            cur_yval = y;
        }
        same_yval.insert(*it);
    }
    if (same_yval.size())
    {
        std::set<ScannedResult>::iterator it1 = same_yval.begin(), it2 = --same_yval.end();
        ScanLineSegment(hdc, it1->x, cur_yval, it1->col
            , it2->x, cur_yval, it2->col, nullptr);
    }
}

void KVectorUtil::FillTriangle(HDC hdc, int x1, int y1, KRgb const col1, float z1
    , int x2, int y2, KRgb const col2, float z2
    , int x3, int y3, KRgb const col3, float z3)
{
    std::set<ScannedResult> scanned_pnts;
    ScanLineSegment(hdc, x1, y1, col1, z1, x2, y2, col2, z2, &scanned_pnts);
    ScanLineSegment(hdc, x2, y2, col2, z2, x3, y3, col3, z3, &scanned_pnts);
    ScanLineSegment(hdc, x3, y3, col3, z3, x1, y1, col1, z1, &scanned_pnts);

    int cur_yval = INT_MIN;
    std::set<ScannedResult> same_yval;

    for (std::set<ScannedResult>::iterator it = scanned_pnts.begin(); it != scanned_pnts.end(); ++it)
    {
        int y = it->y;
        if (y != cur_yval)
        {
            if (same_yval.size())
            {
                std::set<ScannedResult>::iterator it1 = same_yval.begin(), it2 = --same_yval.end();
                ScanLineSegment(hdc, it1->x, cur_yval, it1->col, it1->z,
                    it2->x, cur_yval, it2->col, it2->z, nullptr);

                same_yval.clear();
            }
            cur_yval = y;
        }
        same_yval.insert(*it);
    }
    if (same_yval.size())
    {
        std::set<ScannedResult>::iterator it1 = same_yval.begin(), it2 = --same_yval.end();
        ScanLineSegment(hdc, it1->x, cur_yval, it1->col, it1->z,
            it2->x, cur_yval, it2->col, it2->z, nullptr);
    }
}

void KVectorUtil::ScanLineSegment(HDC hdc, int x1, int y1, KRgb c1, float z1, int x2, int y2, KRgb c2, float z2
    , std::set<ScannedResult>* output, Bitmap& bitmap, KRgb shadeColor)
{
    bool swap_xy = false;
    bool flip_y = false;

    auto _scanLineSegment = [&hdc, &swap_xy, &flip_y](int x1, int y1, KRgb const col1, float z1_
        , int x2, int y2, KRgb const col2, float z2_
        , std::set<ScannedResult>* output
        , Bitmap& bitmap
        , KRgb shadeColor)
    {
        assert(x2 - x1 >= 0 && x2 - x1 >= y2 - y1 && y2 - y1 >= 0);

        const int bitmapWidth = bitmap.GetWidth() - 1;
        const int bitmapHeight = bitmap.GetHeight() - 1;

        bool horizontal = y1 == y2,
            diagonal = y2 - y1 == x2 - x1;

        float scale = 1.0f / (x2 - x1);

        KRgb col = col1, col_step = (col2 - col1) * scale * .99999f;
        float z = z1_;
        const float z_step = (z2_ - z1_) * scale * .99999f;

        int y = y1;
        float yy = (float)y1, y_step = (y2 - y1) * scale;

        for (int x = x1; x <= x2; ++x, col += col_step, z += z_step)
        {
            int X(x), Y(y);
            if (swap_xy) std::swap(X, Y);
            if (flip_y)  Y = -Y;

            if (output)
                output->insert(ScannedResult(X, Y, col, z));
            else
            {
                const float u = __min(__max(col[0], 0), 1.0f);
                const float v = __min(__max(col[1], 0), 1.0f);
                Color color;
                bitmap.GetPixel(int(u * bitmapWidth), int(v * bitmapHeight), &color);

                KRgb colTemp;
                colTemp[0] = (color.GetR()/255.f + shadeColor[0]) / 2.0f;
                colTemp[1] = (color.GetG()/255.f + shadeColor[1]) / 2.0f;
                colTemp[2] = (color.GetB()/255.f + shadeColor[2]) / 2.0f;

                PutPixelDepth(hdc, X, Y, colTemp.GetGdiColor(), z);
            }

            if (!horizontal)
            {
                if (diagonal) y++;
                else y = int(round(yy += y_step));
            }
        }
    };

    if (x1 > x2)
    {
        std::swap(x1, x2); std::swap(y1, y2); std::swap(c1, c2);
        std::swap(z1, z2);
    }
    flip_y = (y1 > y2);
    if (flip_y)
    {
        y1 = -y1, y2 = -y2;
    }
    swap_xy = (y2 - y1 > x2 - x1);
    if (swap_xy)
    {
        std::swap(x1, y1), std::swap(x2, y2);
    }
    _scanLineSegment(x1, y1, c1, z1, x2, y2, c2, z2, output, bitmap, shadeColor);
}

void KVectorUtil::FillTriangle(HDC hdc, int x1, int y1, KRgb const col1, float z1
    , int x2, int y2, KRgb const col2, float z2
    , int x3, int y3, KRgb const col3, float z3, Bitmap& bitmap, KRgb shadeColor)
{
    std::set<ScannedResult> scanned_pnts;
    ScanLineSegment(hdc, x1, y1, col1, z1, x2, y2, col2, z2, &scanned_pnts);
    ScanLineSegment(hdc, x2, y2, col2, z2, x3, y3, col3, z3, &scanned_pnts);
    ScanLineSegment(hdc, x3, y3, col3, z3, x1, y1, col1, z1, &scanned_pnts);

    int cur_yval = INT_MIN;
    std::set<ScannedResult> same_yval;

    for (std::set<ScannedResult>::iterator it = scanned_pnts.begin(); it != scanned_pnts.end(); ++it)
    {
        int y = it->y;
        if (y != cur_yval)
        {
            if (same_yval.size())
            {
                std::set<ScannedResult>::iterator it1 = same_yval.begin(), it2 = --same_yval.end();
                ScanLineSegment(hdc, it1->x, cur_yval, it1->col, it1->z,
                    it2->x, cur_yval, it2->col, it2->z, nullptr, bitmap, shadeColor);

                same_yval.clear();
            }
            cur_yval = y;
        }
        same_yval.insert(*it);
    }

    if (same_yval.size())
    {
        std::set<ScannedResult>::iterator it1 = same_yval.begin(), it2 = --same_yval.end();
        ScanLineSegment(hdc, it1->x, cur_yval, it1->col, it1->z,
            it2->x, cur_yval, it2->col, it2->z, nullptr, bitmap, shadeColor);
    }
}
