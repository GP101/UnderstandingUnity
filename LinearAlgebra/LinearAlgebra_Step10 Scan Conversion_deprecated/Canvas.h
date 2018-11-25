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

#include "Point.h"
#include "KRgb.h"
#include <string>
#include <set>

//inline int round(float a)
//{
//    if (a>0)
//        return int(a + .5);
//    else
//        return int(a - .5);
//}

struct ScannedResult
{
    ScannedResult(int X, int Y, KRgb const& Col) : x(X), y(Y), col(Col) {}

    bool operator<(ScannedResult const& rhs) const { return (y<rhs.y) || ((y == rhs.y) && (x<rhs.x)); }

    int x, y;
    KRgb col;
};

class Canvas
{
public:
                        Canvas(const Canvas&) = delete;
                        Canvas(int resoH = 512, int resoV = 512) : hReso(resoH), vReso(resoV), rgbs(0)  { init(); }
                        ~Canvas() { delete[] rgbs; }

    KRgb&               Pixel(int row, int col) { return pixel(row, col); } //! return a reference to the pixel at (row, col).

    void                Clear(KRgb const& clear_col); //! Clear the whole canvas to the given clear color.

    void                ScanLineSegment(int x1, int y1, KRgb c1, int x2, int y2, KRgb c2
                            , std::set<ScannedResult>* output);

    void                FillTriangle(int x1, int y1, KRgb const col1, int x2, int y2, KRgb const col2
                            , int x3, int y3, KRgb const col3);
    int                 GetWidth() const { return hReso; }
    int                 GetHeight() const { return vReso; }

private:
    void                init();
    KRgb&               pixel(int row, int col) { return *(rgbs + row * hReso + col); }
    void                scanLineSegment(int x1, int y1, KRgb const col1, int x2, int y2, KRgb const col2
                            , std::set<ScannedResult>* output);

    int                 hReso;
    int                 vReso; //! Resolutions (pixel #)at horizontal and vertical directions.
    KRgb*               rgbs;
};

#endif
