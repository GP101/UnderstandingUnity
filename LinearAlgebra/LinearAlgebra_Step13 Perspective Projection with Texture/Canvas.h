/**
*\file		Canvas.h
*
*\brief	The Canvas class is a discrete simulation of the real life canvas.
*
*\author	Xianming Chen
*\site http://www.cs.utah.edu/~xchen/columbia/session2/
*\date      modified by _20180703_jintaeks
*/

#ifndef _CANVAS_H_
#define _CANVAS_H_

#include "KGen.h"
#include "Point.h"
#include "KRgb.h"
#include <string>
#include <set>
#include <objidl.h>
#include <gdiplus.h>

using namespace Gdiplus;

//inline int round(float a)
//{
//    if (a>0)
//        return int(a + .5);
//    else
//        return int(a - .5);
//}

struct ScannedResult
{
    CONSTRUCTOR         ScannedResult(int X, int Y, KRgb const& Col) : x(X), y(Y), col(Col) {}

    bool                operator<(ScannedResult const& rhs) const { return (y<rhs.y) || ((y == rhs.y) && (x<rhs.x)); }

    int                 x;
    int                 y;
    KRgb                col;
};

class Canvas
{
public:
    CONSTRUCTOR         Canvas(const Canvas&) = delete; // prevent copy constructor.
    CONSTRUCTOR         Canvas( ) : m_hReso( 0 ), m_vReso( 0 ), m_rgbs( nullptr )  {}
    DESTRUCTOR          ~Canvas() { _Release(); }

    bool                Create(int horizontalPixel, int verticalPixel);
    void                Destroy();
    KRgb&               Pixel(int row, int col) { return pixel(row, col); } //! return a reference to the pixel at (row, col).

    void                Clear(KRgb const& clear_col); //! Clear the whole canvas to the given clear color.

    void                ScanLineSegment( int x1, int y1, KRgb c1, int x2, int y2, KRgb c2
                            , std::set<ScannedResult>* output );
    void                ScanLineSegment( int x1, int y1, KRgb c1, int x2, int y2, KRgb c2
                            , std::set<ScannedResult>* output, Bitmap& bitmap, KRgb shadeColor );

    void                FillTriangle( int x1, int y1, KRgb const col1, int x2, int y2, KRgb const col2
                            , int x3, int y3, KRgb const col3 );
    void                FillTriangle( int x1, int y1, KRgb const col1, int x2, int y2, KRgb const col2
                            , int x3, int y3, KRgb const col3, Bitmap& bitmap, KRgb shadeColor );
    int                 GetWidth( ) const { return m_hReso; }
    int                 GetHeight( ) const { return m_vReso; }

private:
    void                init();
    KRgb&               pixel( int row, int col ) { return *( m_rgbs + row * m_hReso + col ); }
    void                scanLineSegment( int x1, int y1, KRgb const col1, int x2, int y2, KRgb const col2
                            , std::set<ScannedResult>* output );
    void                scanLineSegment( int x1, int y1, KRgb const col1, int x2, int y2, KRgb const col2
                            , std::set<ScannedResult>* output, Bitmap& bitmap, KRgb shadeColor );
    void                _Release( );

    int                 m_hReso; //! Resolutions (pixel #)at horizontal and vertical directions.
    int                 m_vReso;
    KRgb*               m_rgbs;
};

#endif
