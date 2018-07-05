#include "stdafx.h"
#include "Canvas.h"
#include <vector>
#include <algorithm>
#include <cassert>

void Canvas::init()
{
    const int N = hReso*vReso;

    delete[] rgbs;
    rgbs = new KRgb[N];
    memset(rgbs, 0, sizeof(KRgb)*N);
}

void Canvas::Clear(KRgb const& clear_col)
{
    for(int col = 0; col < hReso; ++col)
    {
        for (int row = 0; row < vReso; ++row)
        {
            pixel(row, col) = clear_col;
        }
    }//for
}

/*
Scan Conversion and Filling Triangle Algorithms
*/

// if swap_xy, flip the line around y=x line(i.e. the diagonal line).
// if flip_y,  flip the line around y=0 line(i.e, x axis)
static bool swap_xy = false, flip_y = false;

void Canvas::scanLineSegment(int x1, int y1, KRgb const col1
    , int x2, int y2, KRgb const col2
    , std::set<ScannedResult>* output)
{
    assert(x2 - x1 >= 0 && x2 - x1 >= y2 - y1 && y2 - y1 >= 0);

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

        //col[0] = __min(__max(col[0], 0), 255);
        //col[1] = __min(__max(col[1], 0), 255);
        //col[2] = __min(__max(col[2], 0), 255);
        if (output)
            output->insert(ScannedResult(X, Y, col));
        else
            Pixel(Y + vReso / 2, X + hReso / 2) = col;

        if (!horizontal)
        {
            if (diagonal) y++;
            else y = int(round(yy += y_step));
        }
    }
}

void Canvas::ScanLineSegment(int x1, int y1, KRgb c1, int x2, int y2, KRgb c2
    , std::set<ScannedResult>* output)
{
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
    scanLineSegment(x1, y1, c1, x2, y2, c2, output);
}

void Canvas::FillTriangle(int x1, int y1, KRgb const col1
    , int x2, int y2, KRgb const col2
    , int x3, int y3, KRgb const col3)

{
    std::set<ScannedResult> scanned_pnts; // sorted in y val, and in x val if y val the same.
    ScanLineSegment(x1, y1, col1, x2, y2, col2, &scanned_pnts);
    ScanLineSegment(x2, y2, col2, x3, y3, col3, &scanned_pnts);
    ScanLineSegment(x3, y3, col3, x1, y1, col1, &scanned_pnts);

    int cur_yval = vReso / 2; // initialize to an invalid value.
    std::set<ScannedResult> same_yval; // of the scanned result.

    for (std::set<ScannedResult>::iterator it = scanned_pnts.begin(); it != scanned_pnts.end(); ++it)
    {
        int y = it->y;
        if (y != cur_yval)
        {
            if (same_yval.size())
            {
                std::set<ScannedResult>::iterator it1 = same_yval.begin(), it2 = --same_yval.end();
                ScanLineSegment(it1->x, cur_yval, it1->col,
                    it2->x, cur_yval, it2->col, nullptr);

                same_yval.clear();
            }
            cur_yval = y;
        }
        same_yval.insert(*it);
    }
    if(same_yval.size( ))
    {
        std::set<ScannedResult>::iterator it1 = same_yval.begin( ), it2 = --same_yval.end( );
        ScanLineSegment( it1->x, cur_yval, it1->col
            , it2->x, cur_yval, it2->col, nullptr );
    }
}
