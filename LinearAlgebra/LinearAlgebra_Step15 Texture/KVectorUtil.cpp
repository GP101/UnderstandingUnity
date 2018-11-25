#include "stdafx.h"
#include "KVectorUtil.h"
#include <cassert>

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
    // if swap_xy, flip the line around y=x line(i.e. the diagonal line).
    // if flip_y,  flip the line around y=0 line(i.e, x axis)
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

        //KRgb col_step2 = (col2 - col1) * scale;
        KRgb col = col1, col_step = (col2 - col1) * scale * .99999f;

        int y = y1;
        float yy = (float)y1, y_step = (y2 - y1) * scale;

        for (int x = x1; x <= x2; ++x, col += col_step)
        {
            int X(x), Y(y);
            if (swap_xy) std::swap(X, Y);
            if (flip_y)  Y = -Y;

            //col[0] = __min(__max(col[0], 0), 255);
            //col[1] = __min(__max(col[1], 0), 255);
            //col[2] = __min(__max(col[2], 0), 255);
            if (output)
                output->insert(ScannedResult(X, Y, col));
            else {
                //Pixel(Y + vReso / 2, X + hReso / 2) = col;
                PutPixel(hdc, X, Y, col.GetGdiColor());
            }

            if (!horizontal)
            {
                if (diagonal) y++;
                else y = int(round(yy += y_step));
            }
        }
    };

    if (x1 > x2) // always scan convert from left to right.
    {
        std::swap(x1, x2); std::swap(y1, y2); std::swap(c1, c2);
    }
    flip_y = (y1 > y2);
    if (flip_y) // always scan convert from down to up.
    {
        y1 = -y1, y2 = -y2;
    }
    swap_xy = (y2 - y1 > x2 - x1);
    if (swap_xy) // and always scan convert a line with <= 45 deg to x-axis.
    {
        std::swap(x1, y1), std::swap(x2, y2);
    }
    _scanLineSegment(x1, y1, c1, x2, y2, c2, output);
}

void KVectorUtil::FillTriangle(HDC hdc, int x1, int y1, KRgb const col1
    , int x2, int y2, KRgb const col2
    , int x3, int y3, KRgb const col3)

{
    std::set<ScannedResult> scanned_pnts; // sorted in y val, and in x val if y val the same.
    ScanLineSegment(hdc, x1, y1, col1, x2, y2, col2, &scanned_pnts);
    ScanLineSegment(hdc, x2, y2, col2, x3, y3, col3, &scanned_pnts);
    ScanLineSegment(hdc, x3, y3, col3, x1, y1, col1, &scanned_pnts);

    int cur_yval = INT_MIN;// vReso / 2; // initialize to an invalid value.
    std::set<ScannedResult> same_yval; // of the scanned result.

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

void KVectorUtil::ScanLineSegment( HDC hdc, int x1, int y1, KRgb c1, int x2, int y2, KRgb c2
    , std::set<ScannedResult>* output, Bitmap& bitmap, KRgb shadeColor)
{
    // if swap_xy, flip the line around y=x line(i.e. the diagonal line).
    // if flip_y,  flip the line around y=0 line(i.e, x axis)
    bool swap_xy = false;
    bool flip_y = false;

    auto _scanLineSegment = [&hdc, &swap_xy, &flip_y](int x1, int y1, KRgb const col1
        , int x2, int y2, KRgb const col2
        , std::set<ScannedResult>* output
        , Bitmap& bitmap
        , KRgb shadeColor)
    {
        assert(x2 - x1 >= 0 && x2 - x1 >= y2 - y1 && y2 - y1 >= 0);

        const int bitmapWidth = bitmap.GetWidth() - 1;
        const int bitmapHeight = bitmap.GetHeight() - 1;

        bool
            horizontal = y1 == y2,
            diagonal = y2 - y1 == x2 - x1;

        float scale = 1.0f / (x2 - x1);

        //KRgb col_step2 = (col2 - col1) * scale;
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
            else
            {
                const float u = __min(__max(col[0], 0), 1.0f);
                const float v = __min(__max(col[1], 0), 1.0f);
                Color color;
                bitmap.GetPixel(u * bitmapWidth, v * bitmapHeight, &color);

                KRgb colTemp;
                colTemp[0] = (color.GetR()/255.f + shadeColor[0]) / 2.0f;
                colTemp[1] = (color.GetG()/255.f + shadeColor[1]) / 2.0f;
                colTemp[2] = (color.GetB()/255.f + shadeColor[2]) / 2.0f;

                //Pixel(Y + m_vReso / 2, X + m_hReso / 2) = colTemp;
                PutPixel(hdc, X, Y, colTemp.GetGdiColor());
            }//if.. else..

            if (!horizontal)
            {
                if (diagonal) y++;
                else y = int(round(yy += y_step));
            }
        }
    };

    if (x1 > x2) // always scan convert from left to right.
    {
        std::swap(x1, x2); std::swap(y1, y2); std::swap(c1, c2);
    }
    flip_y = (y1 > y2);
    if (flip_y) // always scan convert from down to up.
    {
        y1 = -y1, y2 = -y2;
    }
    swap_xy = (y2 - y1 > x2 - x1);
    if (swap_xy) // and always scan convert a line with <= 45 deg to x-axis.
    {
        std::swap(x1, y1), std::swap(x2, y2);
    }
    _scanLineSegment( x1, y1, c1, x2, y2, c2, output, bitmap, shadeColor);
}

void KVectorUtil::FillTriangle(HDC hdc, int x1, int y1, KRgb const col1
    , int x2, int y2, KRgb const col2
    , int x3, int y3, KRgb const col3, Bitmap& bitmap, KRgb shadeColor)

{
    static int debugCounter = 0;
    static int debugCallCounter = 0;

    debugCounter = 0;
    debugCallCounter += 1;
    std::set<ScannedResult> scanned_pnts; // sorted in y val, and in x val if y val the same.
    ScanLineSegment(hdc, x1, y1, col1, x2, y2, col2, &scanned_pnts);
    ScanLineSegment(hdc, x2, y2, col2, x3, y3, col3, &scanned_pnts);
    ScanLineSegment(hdc, x3, y3, col3, x1, y1, col1, &scanned_pnts);

    int cur_yval = INT_MIN; // initialize to an invalid value.
    std::set<ScannedResult> same_yval; // of the scanned result.

    for (std::set<ScannedResult>::iterator it = scanned_pnts.begin(); it != scanned_pnts.end(); ++it)
    {
        int y = it->y;
        if (y != cur_yval)
        {
            if (same_yval.size())
            {
                std::set<ScannedResult>::iterator it1 = same_yval.begin(), it2 = --same_yval.end();
                ScanLineSegment(hdc, it1->x, cur_yval, it1->col,
                    it2->x, cur_yval, it2->col, nullptr, bitmap, shadeColor);
                //debugCounter += 1;
                //if(debugCounter == debugCallCounter)
                //    return;

                same_yval.clear();
            }
            cur_yval = y;
        }
        same_yval.insert(*it);
    }

    if (same_yval.size()) // draw last horizontal line
    {
        std::set<ScannedResult>::iterator it1 = same_yval.begin(), it2 = --same_yval.end();
        ScanLineSegment(hdc, it1->x, cur_yval, it1->col,
            it2->x, cur_yval, it2->col, nullptr, bitmap, shadeColor);
    }

    debugCallCounter = 0;
}
