#include "KGen.h"
#include <math.h>
#include "KGraphics2D.h"


// local macros
#define MIN(a,b)        ((a)<(b)?(a):(b))
#define MAX(a,b)        ((a)>(b)?(a):(b))
#define SWAP(a,b,t)     {(t)=(a); (a)=(b); (b)=(t);}


CONSTRUCTOR KGraphics2D::KGraphics2D()
{
    m_fpPutPixel = NULL;
    m_fpMemSet = NULL;
    SetClipRect( 0, 0, 0, 0 );
    m_width = 0;
}

DESTRUCTOR KGraphics2D::~KGraphics2D()
{
}

void KGraphics2D::SetClipRect( int left, int top, int right, int bottom )
{
    m_min_clip_x = left;
    m_max_clip_x = right;
    m_min_clip_y = top;
    m_max_clip_y = bottom;
}

void KGraphics2D::SetCallback( void( __cdecl *fpPutPixel )( int, int, int )
    , void( __cdecl *fpMemSet )( int, int ) /*= NULL*/ )
{
    m_fpPutPixel = fpPutPixel;
    m_fpMemSet = fpMemSet;
}

void KGraphics2D::Bresenham( int xStart, int yStart, int xEnd, int yEnd, int value )
{
    // clipping line
    ClipLine( xStart, yStart, xEnd, yEnd );

    int     i;
    int     distance;
    int     errX, errY;
    int     deltaX, deltaY;
    int     incX, incY;

    errX = 0;
    errY = 0;
    deltaX = xEnd - xStart;
    deltaY = yEnd - yStart;

    if( deltaX > 0 )
        incX = 1;
    else if( deltaX == 0 )
        incX = 0;
    else {
        incX = -1;
        deltaX = -deltaX;
    }//if.. else if.. else..

    if( deltaY > 0 )
        incY = 1;
    else if( deltaY == 0 )
        incY = 0;
    else {
        incY = -1;
        deltaY = -deltaY;
    }//if.. else if.. else..

    //deltaX = abs(deltaX);
    //deltaY = abs(deltaY);

    if( deltaX > deltaY )
        distance = deltaX;
    else
        distance = deltaY;

    for( i = 0; i <= distance + 1; ++i ) {
        PutPixel( xStart, yStart, value );
        errX += deltaX;
        errY += deltaY;
        if( errX > distance ) {
            errX -= distance;
            xStart += incX;
        }//if
        if( errY > distance ) {
            errY -= distance;
            yStart += incY;
        }//if
    }//for
}

void KGraphics2D::Rectangle( int x1, int y1, int x2, int y2, int value )
{
    Bresenham( x1, y1, x2, y1, value );
    Bresenham( x1, y2, x2, y2, value );
    Bresenham( x1, y1, x1, y2, value );
    Bresenham( x2, y1, x2, y2, value );
}

int KGraphics2D::_IsPointIn( int x, int y )
{
    if( x < m_min_clip_x || x > m_max_clip_x )
        return 0;
    if( y < m_min_clip_y || y > m_max_clip_y )
        return 0;
    return 1;
}

void KGraphics2D::_CirclePlotPoints( int xCenter, int yCenter, int x, int y, int value )
{
    if( _IsPointIn( xCenter + x, yCenter + y ) )
        PutPixel( xCenter + x, yCenter + y, value );
    if( _IsPointIn( xCenter - x, yCenter + y ) )
        PutPixel( xCenter - x, yCenter + y, value );
    if( _IsPointIn( xCenter + x, yCenter - y ) )
        PutPixel( xCenter + x, yCenter - y, value );
    if( _IsPointIn( xCenter - x, yCenter - y ) )
        PutPixel( xCenter - x, yCenter - y, value );
    if( _IsPointIn( xCenter + y, yCenter + x ) )
        PutPixel( xCenter + y, yCenter + x, value );
    if( _IsPointIn( xCenter - y, yCenter + x ) )
        PutPixel( xCenter - y, yCenter + x, value );
    if( _IsPointIn( xCenter + y, yCenter - x ) )
        PutPixel( xCenter + y, yCenter - x, value );
    if( _IsPointIn( xCenter - y, yCenter - x ) )
        PutPixel( xCenter - y, yCenter - x, value );
}

void KGraphics2D::CircleMidPoint( int xCenter, int yCenter, int radius, int value )
{
    int x = 0;
    int y = radius;
    int p = 1 - radius;

    // Plot first set of points
    _CirclePlotPoints( xCenter, yCenter, x, y, value );
    while( x < y ) {
        ++x;
        if( p < 0 )
            p += 2 * x + 1;
        else {
            --y;
            p += 2 * ( x - y ) + 1;
        }//if.. else..
        _CirclePlotPoints( xCenter, yCenter, x, y, value );
    }//while
}

int KGraphics2D::ClipLine( int& x1, int& y1, int& x2, int& y2 )
{
    // internal clipping codes
#define CLIP_CODE_C  0x0000
#define CLIP_CODE_N  0x0008
#define CLIP_CODE_S  0x0004
#define CLIP_CODE_E  0x0002
#define CLIP_CODE_W  0x0001

#define CLIP_CODE_NE 0x000a
#define CLIP_CODE_SE 0x0006
#define CLIP_CODE_NW 0x0009 
#define CLIP_CODE_SW 0x0005

    int xc1 = x1;
    int yc1 = y1;
    int xc2 = x2;
    int yc2 = y2;

    int p1_code = 0;
    int p2_code = 0;

    int temp;

    // determine codes for p1 and p2
    if( y1 < m_min_clip_y )
        p1_code |= CLIP_CODE_N;
    else if( y1 > m_max_clip_y )
        p1_code |= CLIP_CODE_S;

    if( x1 < m_min_clip_x )
        p1_code |= CLIP_CODE_W;
    else if( x1 > m_max_clip_x )
        p1_code |= CLIP_CODE_E;

    if( y2 < m_min_clip_y )
        p2_code |= CLIP_CODE_N;
    else if( y2 > m_max_clip_y )
        p2_code |= CLIP_CODE_S;

    if( x2 < m_min_clip_x )
        p2_code |= CLIP_CODE_W;
    else if( x2 > m_max_clip_x )
        p2_code |= CLIP_CODE_E;

    // try and trivially reject
    if( ( p1_code & p2_code ) )
        return 0;

    // test for totally visible, if so leave points untouched
    if( p1_code == 0 && p2_code == 0 )
        return 1;

    // determine end clip point for p1
    switch( p1_code ) {
    case CLIP_CODE_C:
        break;

    case CLIP_CODE_N:
        {
            yc1 = m_min_clip_y;
            temp = ( m_min_clip_y - y1 ) * ( x2 - x1 ) / ( y2 - y1 ); // for speed-up! - seojt
            xc1 = int( x1 + 0.5f + temp );
        } 
        break;
    case CLIP_CODE_S:
        {
            yc1 = m_max_clip_y;
            temp = ( m_max_clip_y - y1 ) * ( x2 - x1 ) / ( y2 - y1 );
            xc1 = int( x1 + 0.5f + temp );
        }
        break;

    case CLIP_CODE_W:
        {
            xc1 = m_min_clip_x;
            temp = ( m_min_clip_x - x1 ) * ( y2 - y1 ) / ( x2 - x1 );
            yc1 = int( y1 + 0.5f + temp );
        }
        break;

    case CLIP_CODE_E:
        {
            xc1 = m_max_clip_x;
            temp = ( m_max_clip_x - x1 ) * ( y2 - y1 ) / ( x2 - x1 );
            yc1 = int( y1 + 0.5f + temp );
        }
        break;

    // these cases are more complex, must compute 2 intersections
    case CLIP_CODE_NE:
        {
            // north hline intersection
            yc1 = m_min_clip_y;
            temp = ( m_min_clip_y - y1 ) * ( x2 - x1 ) / ( y2 - y1 );
            xc1 = int( x1 + 0.5f + temp );

            // test if intersection is valid, of so then done, else compute next
            if( xc1 < m_min_clip_x || xc1 > m_max_clip_x ) {
                // east vline intersection
                xc1 = m_max_clip_x;
                temp = ( m_max_clip_x - x1 ) * ( y2 - y1 ) / ( x2 - x1 );
                yc1 = int( y1 + 0.5f + temp );
            } // end if
        }
        break;

    case CLIP_CODE_SE:
        {
            // south hline intersection
            yc1 = m_max_clip_y;
            temp = ( m_max_clip_y - y1 ) * ( x2 - x1 ) / ( y2 - y1 );
            xc1 = int( x1 + 0.5f + temp );

            // test if intersection is valid, of so then done, else compute next
            if( xc1 < m_min_clip_x || xc1 > m_max_clip_x ) {
                // east vline intersection
                xc1 = m_max_clip_x;
                temp = ( m_max_clip_x - x1 ) * ( y2 - y1 ) / ( x2 - x1 );
                yc1 = int( y1 + 0.5f + temp );
            } // end if
        }
        break;

    case CLIP_CODE_NW:
        {
            // north hline intersection
            yc1 = m_min_clip_y;
            temp = ( m_min_clip_y - y1 ) * ( x2 - x1 ) / ( y2 - y1 );
            xc1 = int( x1 + 0.5f + temp );

            // test if intersection is valid, of so then done, else compute next
            if( xc1 < m_min_clip_x || xc1 > m_max_clip_x ) {
                xc1 = m_min_clip_x;
                temp = ( m_min_clip_x - x1 ) * ( y2 - y1 ) / ( x2 - x1 );
                yc1 = int( y1 + 0.5f + temp );
            } // end if
        }
        break;

    case CLIP_CODE_SW:
        {
            // south hline intersection
            yc1 = m_max_clip_y;
            temp = ( m_max_clip_y - y1 ) * ( x2 - x1 ) / ( y2 - y1 );
            xc1 = int( x1 + 0.5f + temp );

            // test if intersection is valid, of so then done, else compute next
            if( xc1 < m_min_clip_x || xc1 > m_max_clip_x ) {
                xc1 = m_min_clip_x;
                temp = ( m_min_clip_x - x1 ) * ( y2 - y1 ) / ( x2 - x1 );
                yc1 = int( y1 + 0.5f + temp );
            } // end if
        }
        break;
    }//switch

    // determine clip point for p2
    switch( p2_code ) {
    case CLIP_CODE_C: 
        break;

    case CLIP_CODE_N:
        {
            yc2 = m_min_clip_y;
            xc2 = x2 + ( m_min_clip_y - y2 ) * ( x1 - x2 ) / ( y1 - y2 );
        }
        break;

    case CLIP_CODE_S:
        {
            yc2 = m_max_clip_y;
            xc2 = x2 + ( m_max_clip_y - y2 ) * ( x1 - x2 ) / ( y1 - y2 );
        }
        break;

    case CLIP_CODE_W:
        {
            xc2 = m_min_clip_x;
            yc2 = y2 + ( m_min_clip_x - x2 ) * ( y1 - y2 ) / ( x1 - x2 );
        }
        break;

    case CLIP_CODE_E:
        {
            xc2 = m_max_clip_x;
            yc2 = y2 + ( m_max_clip_x - x2 ) * ( y1 - y2 ) / ( x1 - x2 );
        }
        break;

    // these cases are more complex, must compute 2 intersections
    case CLIP_CODE_NE:
        {
            // north hline intersection
            yc2 = m_min_clip_y;
            temp = ( m_min_clip_y - y2 ) * ( x1 - x2 ) / ( y1 - y2 );
            xc2 = int( x2 + 0.5f + temp );

            // test if intersection is valid, of so then done, else compute next
            if( xc2 < m_min_clip_x || xc2 > m_max_clip_x ) {
                // east vline intersection
                xc2 = m_max_clip_x;
                temp = ( m_max_clip_x - x2 ) * ( y1 - y2 ) / ( x1 - x2 );
                yc2 = int( y2 + 0.5f + temp );
            } // end if
        }
        break;

    case CLIP_CODE_SE:
        {
            // south hline intersection
            yc2 = m_max_clip_y;
            temp = ( m_max_clip_y - y2 ) * ( x1 - x2 ) / ( y1 - y2 );
            xc2 = int( x2 + 0.5f + temp );

            // test if intersection is valid, of so then done, else compute next
            if( xc2 < m_min_clip_x || xc2 > m_max_clip_x ) {
                // east vline intersection
                xc2 = m_max_clip_x;
                temp = ( m_max_clip_x - x2 ) * ( y1 - y2 ) / ( x1 - x2 );
                yc2 = int( y2 + 0.5f + temp );
            } // end if
        }
        break;

    case CLIP_CODE_NW:
        {
            // north hline intersection
            yc2 = m_min_clip_y;
            temp = ( m_min_clip_y - y2 ) * ( x1 - x2 ) / ( y1 - y2 );
            xc2 = int( x2 + 0.5f + temp );

            // test if intersection is valid, of so then done, else compute next
            if( xc2 < m_min_clip_x || xc2 > m_max_clip_x ) {
                xc2 = m_min_clip_x;
                temp = ( m_min_clip_x - x2 ) * ( y1 - y2 ) / ( x1 - x2 );
                yc2 = int( y2 + 0.5f + temp );
            } // end if
        }
        break;

    case CLIP_CODE_SW:
        {
            // south hline intersection
            yc2 = m_max_clip_y;
            temp = ( m_max_clip_y - y2 ) * ( x1 - x2 ) / ( y1 - y2 );
            xc2 = int( x2 + 0.5f + temp );

            // test if intersection is valid, of so then done, else compute next
            if( xc2 < m_min_clip_x || xc2 > m_max_clip_x ) {
                xc2 = m_min_clip_x;
                temp = ( m_min_clip_x - x2 ) * ( y1 - y2 ) / ( x1 - x2 );
                yc2 = int( y2 + 0.5f + temp );
            } // end if
        }
        break;
    }//switch

    // do bounds check
    if( ( xc1 < m_min_clip_x ) || ( xc1 > m_max_clip_x ) ||
        ( yc1 < m_min_clip_y ) || ( yc1 > m_max_clip_y ) ||
        ( xc2 < m_min_clip_x ) || ( xc2 > m_max_clip_x ) ||
        ( yc2 < m_min_clip_y ) || ( yc2 > m_max_clip_y ) )
    {
        return 0;
    }//if

    // store vars back
    x1 = xc1;
    y1 = yc1;
    x2 = xc2;
    y2 = yc2;

    return 1;
}

int KGraphics2D::FastDistance2D( int x, int y )
{
    // first compute the absolute value of x,y
    if( x < 0 ) x = -x;
    if( y < 0 ) y = -y;
    //x = abs(x);
    //y = abs(y);

    // compute the minimum of x,y
    int mn = MIN( x, y );

    // return the distance
    return ( x + y - ( mn >> 1 ) - ( mn >> 2 ) + ( mn >> 4 ) );
}

float KGraphics2D::FastDistance3D( float fx, float fy, float fz )
{
    int temp;  // used for swaping
    int x, y, z; // used for algorithm

    // make sure values are all positive
    x = int( fabs( fx ) * 1024.0f );
    y = int( fabs( fy ) * 1024.0f );
    z = int( fabs( fz ) * 1024.0f );

    // sort values
    if (y < x) SWAP(x, y, temp);
    if (z < y) SWAP(y, z, temp);
    if (y < x) SWAP(x, y, temp);
    const int dist = ( z + 11 * ( y >> 5 ) + ( x >> 2 ) );

    // compute distance with 8% error
    return (float)( dist >> 10 );
}

void KGraphics2D::_DrawHLine( int x, int y, int width, int value )
{
    for( int i = 0; i < width; ++i )
        PutPixel( x + i, y, value );
}

void KGraphics2D::DrawTopTri( int x1, int y1, int x2, int y2, int x3, int y3, int color )
{
    float dx_right;     // the dx/dy ratio of the right edge of line
    float dx_left;      // the dx/dy ratio of the left edge of line
    float xs, xe;       // the starting and ending points of the edges
    float height;       // the height of the triangle

    int temp_x;         // used during sorting as temps
    int temp_y;
    int right;          // used by clipping
    int left;

    // destination address of next scanline
    //UCHAR*  dest_addr = NULL;

    // test order of x1 and x2
    if( x2 < x1 ) {
        temp_x = x2;
        x2 = x1;
        x1 = temp_x;
    } // end if swap

    // compute delta's
    height = float( y3 - y1 );

    dx_left = ( x3 - x1 ) / height;
    dx_right = ( x3 - x2 ) / height;

    // set starting points
    xs = (float)x1;
    xe = (float)x2 + 0.5f;

    // perform y clipping
    if( y1 < m_min_clip_y ) {
        // compute new xs and ys
        xs = xs + dx_left  * (float)( -y1 + m_min_clip_y );
        xe = xe + dx_right * (float)( -y1 + m_min_clip_y );

        // reset y1
        y1 = m_min_clip_y;
    } // end if top is off screen

    if( y3 > m_max_clip_y )
        y3 = m_max_clip_y;

    // compute starting address in video memory
    //dest_addr = dest_buffer + y1 * m_width;

    // test if x clipping is needed
    if( x1 >= m_min_clip_x && x1 <= m_max_clip_x &&
        x2 >= m_min_clip_x && x2 <= m_max_clip_x &&
        x3 >= m_min_clip_x && x3 <= m_max_clip_x ) {
        // draw the triangle
        for( temp_y = y1; temp_y <= y3; temp_y++/*, dest_addr += m_width*/ ) {
            //memset((UCHAR *)dest_addr+(unsigned int)xs,
            //    color,(unsigned int)(xe-xs+1));
            _DrawHLine( (int)xs, temp_y, (int)( xe - xs + 1.0f ), color );

            // adjust starting point and ending point
            xs += dx_left;
            xe += dx_right;
        } // end for
    } // end if no x clipping needed
    else {
        // clip x axis with slower version

        // draw the triangle
        for( temp_y = y1; temp_y <= y3; temp_y++/*, dest_addr += m_width*/ ) {
            // do x clip
            left = (int)xs;
            right = (int)xe;

            // adjust starting point and ending point
            xs += dx_left;
            xe += dx_right;

            // clip line
            if( left < m_min_clip_x ) {
                left = m_min_clip_x;

                if( right < m_min_clip_x )
                    continue;
            }//if

            if( right > m_max_clip_x ) {
                right = m_max_clip_x;

                if( left > m_max_clip_x )
                    continue;
            }//if

            //memset((UCHAR  *)dest_addr+(unsigned int)left,
            //    color,(unsigned int)(right-left+1));
            _DrawHLine( left, temp_y, right - left + 1, color );
        } // end for
    } // end else x clipping needed
}

void KGraphics2D::DrawBottomTri( int x1, int y1, int x2, int y2, int x3, int y3, int color )
{
    float dx_right;     // the dx/dy ratio of the right edge of line
    float dx_left;      // the dx/dy ratio of the left edge of line
    float xs, xe;       // the starting and ending points of the edges
    float height;       // the height of the triangle

    int temp_x;         // used during sorting as temps
    int temp_y;
    int right;          // used by clipping
    int left;

    // destination address of next scanline
    //UCHAR*  dest_addr;

    // test order of x1 and x2
    if( x3 < x2 ) {
        temp_x = x2;
        x2 = x3;
        x3 = temp_x;
    } // end if swap

    // compute delta's
    height = float( y3 - y1 );

    dx_left = ( x2 - x1 ) / height;
    dx_right = ( x3 - x1 ) / height;

    // set starting points
    xs = (float)x1;
    xe = (float)x1; // +(float)0.5;

    // perform y clipping
    if( y1 < m_min_clip_y ) {
        // compute new xs and ys
        xs = xs + dx_left  * (float)( -y1 + m_min_clip_y );
        xe = xe + dx_right * (float)( -y1 + m_min_clip_y );

        // reset y1
        y1 = m_min_clip_y;
    } // end if top is off screen

    if( y3 > m_max_clip_y )
        y3 = m_max_clip_y;

    // compute starting address in video memory
    //dest_addr = dest_buffer+y1*m_width;

    // test if x clipping is needed
    if( x1 >= m_min_clip_x && x1 <= m_max_clip_x &&
        x2 >= m_min_clip_x && x2 <= m_max_clip_x &&
        x3 >= m_min_clip_x && x3 <= m_max_clip_x ) {
        // draw the triangle
        for( temp_y = y1; temp_y <= y3; ++temp_y/*,dest_addr+=m_width*/ ) {
            //memset((UCHAR  *)dest_addr+(unsigned int)xs,
            //    color,(unsigned int)(xe-xs+1));
            _DrawHLine( (int)xs, temp_y, (int)( xe - xs + 1.0f ), color );

            // adjust starting point and ending point
            xs += dx_left;
            xe += dx_right;
        } // end for
    } // end if no x clipping needed
    else {
        // clip x axis with slower version

        // draw the triangle
        for( temp_y = y1; temp_y <= y3; ++temp_y/*,dest_addr+=m_width*/ ) {
            // do x clip
            left = (int)xs;
            right = (int)xe;

            // adjust starting point and ending point
            xs += dx_left;
            xe += dx_right;

            // clip line
            if( left < m_min_clip_x ) {
                left = m_min_clip_x;

                if( right < m_min_clip_x )
                    continue;
            }//if

            if( right > m_max_clip_x ) {
                right = m_max_clip_x;

                if( left > m_max_clip_x )
                    continue;
            }//if

            //memset((UCHAR  *)dest_addr+(unsigned int)left,
            //    color,(unsigned int)(right-left+1));
            _DrawHLine( left, temp_y, right - left + 1, color );
        } // end for
    } // end else x clipping needed
}

void KGraphics2D::DrawTriangle2D( int x1, int y1, int x2, int y2, int x3, int y3, int color )
{
    int temp_x; // used for sorting
    int temp_y;
    int new_x;

    // test for h lines and v lines
    if( ( x1 == x2 && x2 == x3 ) || ( y1 == y2 && y2 == y3 ) )
        return;

    // sort p1,p2,p3 in ascending y order
    if( y2 < y1 ) {
        temp_x = x2;
        temp_y = y2;
        x2 = x1;
        y2 = y1;
        x1 = temp_x;
        y1 = temp_y;
    } // end if

    // now we know that p1 and p2 are in order
    if( y3 < y1 ) {
        temp_x = x3;
        temp_y = y3;
        x3 = x1;
        y3 = y1;
        x1 = temp_x;
        y1 = temp_y;
    } // end if

    // finally test y3 against y2
    if( y3 < y2 ) {
        temp_x = x3;
        temp_y = y3;
        x3 = x2;
        y3 = y2;
        x2 = temp_x;
        y2 = temp_y;
    } // end if

    // do trivial rejection tests for clipping
    if( y3 < m_min_clip_y || y1 > m_max_clip_y ||
        ( x1 < m_min_clip_x && x2 < m_min_clip_x && x3 < m_min_clip_x ) ||
        ( x1 > m_max_clip_x && x2 > m_max_clip_x && x3 > m_max_clip_x ) )
        return;

    // test if top of triangle is flat
    if( y1 == y2 ) {
        DrawTopTri( x1, y1, x2, y2, x3, y3, color/*, dest_buffer, m_width*/ );
    } // end if
    else if( y2 == y3 ) {
        DrawBottomTri( x1, y1, x2, y2, x3, y3, color/*, dest_buffer, m_width*/ );
    } // end if bottom is flat
    else {
        // general triangle that's needs to be broken up along long edge
        new_x = x1 + (int)( 0.5f + (float)( y2 - y1 ) * (float)( x3 - x1 ) / (float)( y3 - y1 ) );

        // draw each sub-triangle
        DrawBottomTri( x1, y1, new_x, y2, x2, y2, color/*, dest_buffer, m_width*/ );
        DrawTopTri( x2, y2, new_x, y2, x3, y3, color/*, dest_buffer, m_width*/ );
    } // end else
}

void KGraphics2D::DrawFilledPolygon2D( POLYGON2D* poly )
{
    // you may call ClipLine() for all edges
    // data structure modified
    // or
    // you can clip test in you MemSet() callback function

    int ydiff1, ydiff2;         // difference between starting x and ending x
    int xdiff1, xdiff2;         // difference between starting y and ending y
    int start;                  // starting offset of line between edges
    int length;                 // distance from edge 1 to edge 2
    int errorterm1, errorterm2; // error terms for edges 1 & 2
    int offset1, offset2;       // offset of current pixel in edges 1 & 2
    int count1, count2;         // increment count for edges 1 & 2
    int xunit1, xunit2;         // unit to advance x offset for edges 1 & 2

    // initialize count of number of edges drawn:
    int edgecount = poly->num_verts - 1;

    // determine which vertex is at top of polygon:

    int firstvert = 0;         // start by assuming vertex 0 is at top

    int min_y = (int)poly->vlist[0].y; // find y coordinate of vertex 0

    for( int index = 1; index < poly->num_verts; ++index ) {
        // Search thru vertices
        if( ( poly->vlist[index].y ) < min_y ) {
            // is another vertex higher?
            firstvert = index;
            min_y = (int)poly->vlist[index].y;
        } //if
    } //for

    // finding starting and ending vertices of first two edges:
    int startvert1 = firstvert;      // get starting vertex of edge 1
    int startvert2 = firstvert;      // get starting vertex of edge 2
    int xstart1 = int( poly->vlist[startvert1].x + poly->x0 );
    int ystart1 = int( poly->vlist[startvert1].y + poly->y0 );
    int xstart2 = int( poly->vlist[startvert2].x + poly->x0 );
    int ystart2 = int( poly->vlist[startvert2].y + poly->y0 );
    int endvert1 = startvert1 - 1;           // get ending vertex of edge 1

    if( endvert1 < 0 )
        endvert1 = poly->num_verts - 1;    // check for wrap

    int xend1 = int( poly->vlist[endvert1].x + poly->x0 );      // get x & y coordinates
    int yend1 = int( poly->vlist[endvert1].y + poly->y0 );      // of ending vertices
    int endvert2 = startvert2 + 1;           // get ending vertex of edge 2

    if( endvert2 == ( poly->num_verts ) )
        endvert2 = 0;  // Check for wrap

    int xend2 = int( poly->vlist[endvert2].x + poly->x0 );      // get x & y coordinates
    int yend2 = int( poly->vlist[endvert2].y + poly->y0 );      // of ending vertices

    // draw the polygon:

    while( edgecount > 0 ) {
        // continue drawing until all edges drawn
        offset1 = m_width * ystart1 + xstart1;  // offset of edge 1
        offset2 = m_width * ystart2 + xstart2;  // offset of edge 2

        // initialize error terms
        // for edges 1 & 2
        errorterm1 = 0;
        errorterm2 = 0;

        // get absolute value of
        if( ( ydiff1 = yend1 - ystart1 ) < 0 )
            ydiff1 = -ydiff1;

        // x & y lengths of edges
        if( ( ydiff2 = yend2 - ystart2 ) < 0 )
            ydiff2 = -ydiff2;

        if( ( xdiff1 = xend1 - xstart1 ) < 0 ) {
            // get value of length
            xunit1 = -1;                    // calculate X increment
            xdiff1 = -xdiff1;
        }
        else {
            xunit1 = 1;
        } // end else

        if( ( xdiff2 = xend2 - xstart2 ) < 0 ) {
            // Get value of length
            xunit2 = -1;                   // calculate X increment
            xdiff2 = -xdiff2;
        }
        else {
            xunit2 = 1;
        } // end else

        // choose which of four routines to use
        if( xdiff1 > ydiff1 ) {
            // if x length of edge 1 is greater than y length
            if( xdiff2 > ydiff2 ) {
                // if X length of edge 2 is greater than y length

                // increment edge 1 on X and edge 2 on X:
                count1 = xdiff1;    // count for x increment on edge 1
                count2 = xdiff2;    // count for x increment on edge 2

                while( count1 && count2 ) {
                    // continue drawing until one edge is done
                    // calculate edge 1:
                    while( ( errorterm1 < xdiff1 ) && ( count1 > 0 ) ) {
                        // finished w/edge 1?
                        if( count1-- ) {
                            // count down on edge 1
                            offset1 += xunit1;  // increment pixel offset
                            xstart1 += xunit1;
                        } // end if

                        errorterm1 += ydiff1; // increment error term

                        if( errorterm1 < xdiff1 ) {  // if not more than XDIFF
                            //vbuffer[offset1] = (UCHAR)poly->color; // ...plot a pixel
                            MemSet( offset1, poly->color );
                        } //if

                    } //while

                    errorterm1 -= xdiff1; // if time to increment X, restore error term

                    // calculate edge 2:

                    while( ( errorterm2 < xdiff2 ) && ( count2 > 0 ) ) {
                        // finished w/edge 2?
                        if( count2-- ) {
                            // count down on edge 2
                            offset2 += xunit2;  // increment pixel offset
                            xstart2 += xunit2;
                        } // end if

                        errorterm2 += ydiff2; // increment error term

                        if( errorterm2 < xdiff2 ) {  // if not more than XDIFF
                            //vbuffer[offset2] = (UCHAR)poly->color;  // ...plot a pixel
                            MemSet( offset2, poly->color );
                        } //if

                    } //while

                    errorterm2 -= xdiff2; // if time to increment X, restore error term

                    // draw line from edge 1 to edge 2:

                    length = offset2 - offset1; // determine length of horizontal line

                    if( length < 0 ) { // if negative...
                        length = -length;       // make it positive
                        start = offset2;        // and set START to edge 2
                    }
                    else
                        start = offset1;     // else set START to edge 1

                    for( int index = start; index < start + length + 1; ++index ) {  // From edge to edge...
                        //vbuffer[index] = (UCHAR)poly->color;         // ...draw the line
                        MemSet( index, poly->color );
                    } //for

                    offset1 += m_width;           // advance edge 1 offset to next line
                    ++ystart1;
                    offset2 += m_width;           // advance edge 2 offset to next line
                    ++ystart2;

                } //if

            }
            else {
                // increment edge 1 on X and edge 2 on Y:
                count1 = xdiff1;    // count for X increment on edge 1
                count2 = ydiff2;    // count for Y increment on edge 2

                while( count1 && count2 ) {  // continue drawing until one edge is done
                    // calculate edge 1:
                    while( ( errorterm1 < xdiff1 ) && ( count1 > 0 ) ) { // finished w/edge 1?
                        if( count1-- ) {
                            // count down on edge 1
                            offset1 += xunit1;  // increment pixel offset
                            xstart1 += xunit1;
                        } //if

                        errorterm1 += ydiff1; // increment error term

                        if( errorterm1 < xdiff1 ) {  // If not more than XDIFF
                            //vbuffer[offset1] = (UCHAR)poly->color; // ...plot a pixel
                            MemSet( offset1, poly->color );
                        } //if

                    } //while

                    errorterm1 -= xdiff1; // If time to increment X, restore error term

                    // calculate edge 2:
                    errorterm2 += xdiff2; // increment error term

                    if( errorterm2 >= ydiff2 ) { // if time to increment Y...
                        errorterm2 -= ydiff2;        // ...restore error term
                        offset2 += xunit2;           // ...and advance offset to next pixel
                        xstart2 += xunit2;
                    } // end if

                    count2--;

                    // draw line from edge 1 to edge 2:

                    length = offset2 - offset1; // determine length of horizontal line

                    if( length < 0 ) { // if negative...
                        length = -length;       // ...make it positive
                        start = offset2;        // and set START to edge 2
                    }
                    else
                        start = offset1;        // else set START to edge 1

                    for( int index = start; index < start + length + 1; ++index ) { // from edge to edge
                        //vbuffer[index] = (UCHAR)poly->color;         // ...draw the line
                        MemSet( index, poly->color );
                    } //for

                    offset1 += m_width;           // advance edge 1 offset to next line
                    ++ystart1;
                    offset2 += m_width;           // advance edge 2 offset to next line
                    ++ystart2;
                } //while
            } //if
        }
        else {
            if( xdiff2 > ydiff2 ) {
                // increment edge 1 on Y and edge 2 on X:

                count1 = ydiff1;  // count for Y increment on edge 1
                count2 = xdiff2;  // count for X increment on edge 2

                while( count1 && count2 ) {  // continue drawing until one edge is done
                    // calculate edge 1:

                    errorterm1 += xdiff1; // Increment error term

                    if( errorterm1 >= ydiff1 ) {  // if time to increment Y...
                        errorterm1 -= ydiff1;         // ...restore error term
                        offset1 += xunit1;            // ...and advance offset to next pixel
                        xstart1 += xunit1;
                    } //if

                    --count1;

                    // Calculate edge 2:

                    while( ( errorterm2 < xdiff2 ) && ( count2 > 0 ) ) { // finished w/edge 1?
                        if( count2-- ) { // count down on edge 2
                            offset2 += xunit2;  // increment pixel offset
                            xstart2 += xunit2;
                        } //if

                        errorterm2 += ydiff2; // increment error term

                        if( errorterm2 < xdiff2 ) {  // if not more than XDIFF
                            //vbuffer[offset2] = (UCHAR)poly->color; // ...plot a pixel
                            MemSet( offset2, poly->color );
                        } //if
                    } //while

                    errorterm2 -= xdiff2;  // if time to increment X, restore error term

                    // draw line from edge 1 to edge 2:

                    length = offset2 - offset1; // determine length of horizontal line

                    if( length < 0 ) {    // if negative...
                        length = -length;  // ...make it positive
                        start = offset2;   // and set START to edge 2
                    }
                    else
                        start = offset1;  // else set START to edge 1

                    for( int index = start; index < start + length + 1; ++index ) {// from edge to edge...
                        //vbuffer[index] = (UCHAR)poly->color;      // ...draw the line
                        MemSet( index, poly->color );
                    } // end for index

                    offset1 += m_width;         // advance edge 1 offset to next line
                    ++ystart1;
                    offset2 += m_width;         // advance edge 2 offset to next line
                    ++ystart2;
                } //if
            }
            else {
                // increment edge 1 on Y and edge 2 on Y:
                count1 = ydiff1;  // count for Y increment on edge 1
                count2 = ydiff2;  // count for Y increment on edge 2

                while( count1 && count2 ) {
                    // continue drawing until one edge is done
                    // calculate edge 1:
                    errorterm1 += xdiff1;  // increment error term

                    if( errorterm1 >= ydiff1 ) { // if time to increment Y
                        errorterm1 -= ydiff1;         // ...restore error term
                        offset1 += xunit1;            // ...and advance offset to next pixel
                        xstart1 += xunit1;
                    } //if

                    --count1;

                    // calculate edge 2:
                    errorterm2 += xdiff2;            // increment error term

                    if( errorterm2 >= ydiff2 ) {     // if time to increment Y
                        errorterm2 -= ydiff2;         // ...restore error term
                        offset2 += xunit2;            // ...and advance offset to next pixel
                        xstart2 += xunit2;
                    } //if

                    --count2;

                    // draw line from edge 1 to edge 2:

                    length = offset2 - offset1;  // determine length of horizontal line

                    if( length < 0 ) {
                        // if negative...
                        length = -length;        // ...make it positive
                        start = offset2;         // and set START to edge 2
                    }
                    else
                        start = offset1;         // else set START to edge 1

                    for( int index = start; index < start + length + 1; ++index ) { // from edge to edge
                        //vbuffer[index] = (UCHAR)poly->color;   // ...draw the linee
                        MemSet( index, poly->color );
                    } //for

                    offset1 += m_width;            // advance edge 1 offset to next line
                    ++ystart1;
                    offset2 += m_width;            // advance edge 2 offset to next line
                    ++ystart2;
                } //while
            } //else
        } //if

        // another edge (at least) is complete. Start next edge, if any.
        if( !count1 ) { // if edge 1 is complete...
            --edgecount;           // decrement the edge count
            startvert1 = endvert1;   // make ending vertex into start vertex
            --endvert1;            // and get new ending vertex

            if( endvert1 < 0 )
                endvert1 = poly->num_verts - 1; // check for wrap

            xend1 = int( poly->vlist[endvert1].x + poly->x0 );  // get x & y of new end vertex
            yend1 = int( poly->vlist[endvert1].y + poly->y0 );
        } //if

        if( !count2 ) { // if edge 2 is complete...
            --edgecount;          // decrement the edge count
            startvert2 = endvert2;  // make ending vertex into start vertex
            ++endvert2;           // and get new ending vertex

            if( endvert2 == ( poly->num_verts ) )
                endvert2 = 0;                // check for wrap

            xend2 = int( poly->vlist[endvert2].x + poly->x0 );  // get x & y of new end vertex
            yend2 = int( poly->vlist[endvert2].y + poly->y0 );
        } //if
    } //while
}
