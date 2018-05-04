// jintaeks on 2017-11-15_21-35
#pragma once

#include "KGen.h"
#include <windows.h>


class KGraphics2D
{
public:
    struct VERTEX2DF
    {
        float           x;
        float           y;
    };

    // a 2D polygon
    struct POLYGON2D
    {
        int             state;    // state of polygon
        int             num_verts;// number of vertices
        int             x0;
        int             y0;   // position of center of polygon  
        int             xv;
        int             yv;   // initial velocity
        DWORD           color;    // could be index or PALETTENTRY
        VERTEX2DF*      vlist;    // pointer to vertex list
    };

private:
    // PutPixel callback function
    void( *m_fpPutPixel )( int x, int y, int value );
    void( *m_fpMemSet )( int offset, int value );

    // clipping rectangle 
    int                 m_min_clip_x;
    int                 m_max_clip_x;
    int                 m_min_clip_y;
    int                 m_max_clip_y;
    int                 m_width; // width of image

private:
    void                _DrawHLine( int x, int y, int width, int value ); // draw horizontal line
    void                _CirclePlotPoints( int xCenter, int yCenter, int x, int y, int value );
    int                 _IsPointIn( int x, int y ); // is point in clipping region?

public:
    // static member functions
    static int          FastDistance2D( int x, int y );
    static float        FastDistance3D( float fx, float fy, float fz );

public:
    void                PutPixel( int x, int y, int value ) { m_fpPutPixel( x, y, value ); }
    void                MemSet( int offset, int value ) { m_fpMemSet( offset, value ); }
    void                Bresenham( int xStart, int yStart, int xEnd, int yEnd, int value ); // draw line
    void                Rectangle( int x1, int y1, int x2, int y2, int value ); // draw rectangle
    void                CircleMidPoint( int xCenter, int yCenter, int radius, int value );

    void                DrawTopTri( int x1, int y1, int x2, int y2, int x3, int y3, int value );
    void                DrawBottomTri( int x1, int y1, int x2, int y2, int x3, int y3, int value );
    void                DrawTriangle2D( int x1, int y1, int x2, int y2, int x3, int y3, int value );
    void                DrawFilledPolygon2D( POLYGON2D* poly );

    // helper
    void                SetCallback( void( *fpPutPixel )( int, int, int )
                            , void( *fpMemSet )( int, int ) = NULL );
    void                SetMemPitch( int pitch ) { m_width = pitch; }
    void                SetClipRect( int x_min, int y_min, int x_max, int y_max );
    int                 ClipLine( int& x1, int& y1, int& x2, int& y2 ); // most internally used
    
    // constructor and destructor
    CONSTRUCTOR         KGraphics2D();
    DESTRUCTOR          ~KGraphics2D();
};// class KGraphics2D


/* you can use like this:

    #include <stdio.h>
    #include <memory.h>
    #include <conio.h>
    #include "StringEx.h"
    #include "Fov2D.h"

    #define CLIP_X1     10 
    #define CLIP_Y1     7
    #define CLIP_X2     60
    #define CLIP_Y2     15

    int g_board[24][79];


    void PutPixelCallback(int x, int y, int value)
    {   // you don't need to clip test. it's clipped
        g_board[y][x] = value;
    }//PutPixelCallback()

    void MemSetCallback(int offset, int value)
    {
        if (offset < 79*24) // clipping test
            *(g_board[0] + offset) = value;
    }//MemSetCallback()

    void InitBoard()
    {
        memset( g_board, '.', sizeof(g_board) );
    }//InitBoard()

    void DrawBoard()
    {
        int         row, col;
        CStringEx   str;

        for (row=0; row<24; ++row) {
            str = "";
            for (col=0; col<79; ++col) {
                str += g_board[row][col];
            }//for
            printf("%s\n", str.GetBuffer() );
        }//for
    }//DrawBoard()

    void main()
    {
        InitBoard();
    
        KGraphics2D fov;
        fov.SetCallback( PutPixelCallback, MemSetCallback );
        fov.SetMemPitch( 79 );

        fov.SetClipRect(CLIP_X1, CLIP_Y1, CLIP_X2, CLIP_Y2);
        fov.Rectangle(CLIP_X1, CLIP_Y1, CLIP_X2, CLIP_Y2, 'o');

        fov.SetClipRect(0, 0, 78, 23);
        fov.DrawTriangle2D(2,2, 40,10, 20,21, 'T');


        VERTEX2DF   vertex[6]={{15,-5},{5,-5},{0,-11},{-5,-5},{-15,-5},{0,10}};
        POLYGON2D   poly={0, 6, 39,12, 0,0, 'R', NULL};
          //              ^state
          //                 ^num verts
          //                    ^center
          //                           ^direction
          //                                ^color
          //                                   ^vertex list

        poly.vlist = vertex;

        fov.DrawFilledPolygon2D(&poly);

        DrawBoard();
    }//main()
*/
