#include "stdafx.h"
#include "Canvas.h"
#include <vector>
#include <algorithm>
#include <cassert>

bool Canvas::Create(int horizontalPixel, int verticalPixel)
{
    assert(horizontalPixel >= 0);
    assert(verticalPixel >= 0);
    m_hReso = horizontalPixel;
    m_vReso = verticalPixel;
    init();
    return true;
}

void Canvas::Destroy()
{
    _Release();
}

void Canvas::_Release()
{
    if(m_rgbs != nullptr)
    {
        delete[] m_rgbs;
        m_rgbs = nullptr;
    }
}

void Canvas::init()
{
    const int N = m_hReso*m_vReso;

    _Release();
    m_rgbs = new KRgb[N];
    memset( m_rgbs, 0, sizeof(KRgb)*N );
}

void Canvas::Clear(KRgb const& clear_col)
{
    for(int col = 0; col < m_hReso; ++col)
    {
        for(int row = 0; row < m_vReso; ++row)
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

        if (output)
            output->insert( ScannedResult( X, Y, col ) );
        else
            Pixel( Y + m_vReso / 2, X + m_hReso / 2 ) = col;

        if (!horizontal)
        {
            if (diagonal) y++;
            else y = int(round(yy += y_step));
        }
    }
}

void Canvas::scanLineSegment( int x1, int y1, KRgb const col1
    , int x2, int y2, KRgb const col2
    , std::set<ScannedResult>* output
    , Bitmap& bitmap
    , KRgb shadeColor )
{
    assert( x2 - x1 >= 0 && x2 - x1 >= y2 - y1 && y2 - y1 >= 0 );

    const int bitmapWidth = bitmap.GetWidth( ) - 1;
    const int bitmapHeight= bitmap.GetHeight( ) - 1;

    bool
        horizontal = y1 == y2,
        diagonal = y2 - y1 == x2 - x1;

    float scale = 1.0f / ( x2 - x1 );

    //KRgb col_step2 = (col2 - col1) * scale;
    KRgb col = col1, col_step = ( col2 - col1 ) * scale * .99999f;

    int y = y1;
    float yy = (float)y1, y_step = ( y2 - y1 ) * scale;

    for(int x = x1; x <= x2; ++x, col += col_step)
    {
        int X( x ), Y( y );
        if(swap_xy) std::swap( X, Y );
        if(flip_y)  Y = -Y;

        if(output)
            output->insert( ScannedResult( X, Y, col ) );
        else
        {
            const float u = __min( __max( col[0] / 255.0f, 0 ), 1.0f );
            const float v = __min( __max( col[1] / 255.0f, 0 ), 1.0f );
            Color color;
            bitmap.GetPixel( u * bitmapWidth, v * bitmapHeight, &color );

            KRgb colTemp;
            colTemp[0] = (color.GetR( ) + shadeColor[0]) / 2.0f;
            colTemp[1] = (color.GetG( ) + shadeColor[1]) / 2.0f;
            colTemp[2] = (color.GetB( ) + shadeColor[2]) / 2.0f;

            Pixel( Y + m_vReso / 2, X + m_hReso / 2 ) = colTemp;
        }//if.. else..

        if(!horizontal)
        {
            if(diagonal) y++;
            else y = int( round( yy += y_step ) );
        }
    }
}

void Canvas::ScanLineSegment( int x1, int y1, KRgb c1, int x2, int y2, KRgb c2
    , std::set<ScannedResult>* output )
{
    if(x1 > x2) // always scan convert from left to right.
    {
        std::swap( x1, x2 ); std::swap( y1, y2 ); std::swap( c1, c2 );
    }
    flip_y = ( y1 > y2 );
    if(flip_y) // always scan convert from down to up.
    {
        y1 = -y1, y2 = -y2;
    }
    swap_xy = ( y2 - y1 > x2 - x1 );
    if(swap_xy) // and always scan convert a line with <= 45 deg to x-axis.
    {
        std::swap( x1, y1 ), std::swap( x2, y2 );
    }
    scanLineSegment( x1, y1, c1, x2, y2, c2, output );
}

void Canvas::ScanLineSegment( int x1, int y1, KRgb c1, int x2, int y2, KRgb c2
    , std::set<ScannedResult>* output, Bitmap& bitmap, KRgb shadeColor )
{
    if(x1 > x2) // always scan convert from left to right.
    {
        std::swap( x1, x2 ); std::swap( y1, y2 ); std::swap( c1, c2 );
    }
    flip_y = ( y1 > y2 );
    if(flip_y) // always scan convert from down to up.
    {
        y1 = -y1, y2 = -y2;
    }
    swap_xy = ( y2 - y1 > x2 - x1 );
    if(swap_xy) // and always scan convert a line with <= 45 deg to x-axis.
    {
        std::swap( x1, y1 ), std::swap( x2, y2 );
    }
    scanLineSegment( x1, y1, c1, x2, y2, c2, output, bitmap, shadeColor );
}

void Canvas::FillTriangle(int x1, int y1, KRgb const col1
    , int x2, int y2, KRgb const col2
    , int x3, int y3, KRgb const col3)

{
    static int debugCounter = 0;
    static int debugCallCounter = 0;

    debugCounter = 0;
    debugCallCounter += 1;
    std::set<ScannedResult> scanned_pnts; // sorted in y val, and in x val if y val the same.
    ScanLineSegment(x1, y1, col1, x2, y2, col2, &scanned_pnts);
    ScanLineSegment(x2, y2, col2, x3, y3, col3, &scanned_pnts);
    ScanLineSegment(x3, y3, col3, x1, y1, col1, &scanned_pnts);

    int cur_yval = m_vReso / 2; // initialize to an invalid value.
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
                //debugCounter += 1;
                //if(debugCounter == debugCallCounter)
                //    return;

                same_yval.clear();
            }
            cur_yval = y;
        }
        same_yval.insert(*it);
    }

    if(same_yval.size( )) // draw last horizontal line
    {
        std::set<ScannedResult>::iterator it1 = same_yval.begin( ), it2 = --same_yval.end( );
        ScanLineSegment( it1->x, cur_yval, it1->col,
            it2->x, cur_yval, it2->col, nullptr );
    }

    debugCallCounter = 0;
}

void Canvas::FillTriangle( int x1, int y1, KRgb const col1
    , int x2, int y2, KRgb const col2
    , int x3, int y3, KRgb const col3, Bitmap& bitmap, KRgb shadeColor )

{
    static int debugCounter = 0;
    static int debugCallCounter = 0;

    debugCounter = 0;
    debugCallCounter += 1;
    std::set<ScannedResult> scanned_pnts; // sorted in y val, and in x val if y val the same.
    ScanLineSegment( x1, y1, col1, x2, y2, col2, &scanned_pnts );
    ScanLineSegment( x2, y2, col2, x3, y3, col3, &scanned_pnts );
    ScanLineSegment( x3, y3, col3, x1, y1, col1, &scanned_pnts );

    int cur_yval = m_vReso / 2; // initialize to an invalid value.
    std::set<ScannedResult> same_yval; // of the scanned result.

    for(std::set<ScannedResult>::iterator it = scanned_pnts.begin( ); it != scanned_pnts.end( ); ++it)
    {
        int y = it->y;
        if(y != cur_yval)
        {
            if(same_yval.size( ))
            {
                std::set<ScannedResult>::iterator it1 = same_yval.begin( ), it2 = --same_yval.end( );
                ScanLineSegment( it1->x, cur_yval, it1->col,
                    it2->x, cur_yval, it2->col, nullptr, bitmap, shadeColor );
                //debugCounter += 1;
                //if(debugCounter == debugCallCounter)
                //    return;

                same_yval.clear( );
            }
            cur_yval = y;
        }
        same_yval.insert( *it );
    }

    if(same_yval.size( )) // draw last horizontal line
    {
        std::set<ScannedResult>::iterator it1 = same_yval.begin( ), it2 = --same_yval.end( );
        ScanLineSegment( it1->x, cur_yval, it1->col,
            it2->x, cur_yval, it2->col, nullptr, bitmap, shadeColor );
    }

    debugCallCounter = 0;
}
