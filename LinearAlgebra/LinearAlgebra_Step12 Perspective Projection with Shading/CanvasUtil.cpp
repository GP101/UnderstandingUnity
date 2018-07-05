#include "stdafx.h"
#include "CanvasUtil.h"
#include <objidl.h>
#include <gdiplus.h>

using namespace Gdiplus;

void CanvasUtil::DrawCanvas( HDC hdc, Canvas& canvas, float originScreenX, float originScreenY, float dotPixelSize )
{
    Graphics    graphics( hdc );

    const int canvasWidth = canvas.GetWidth( );
    const int canvasHeight = canvas.GetHeight( );

    const float left = originScreenX - canvasWidth * dotPixelSize / 2;
    float top = originScreenY + canvasHeight * dotPixelSize / 2;

    for(int col = 0; col < canvasWidth; ++col)
    {
        for(int row = 0; row < canvasHeight; ++row)
        {
            const float x = left + col * dotPixelSize;
            const float y = top - row * dotPixelSize;
            const KRgb rgb = canvas.Pixel( row, col );
            SolidBrush  brush( Color( rgb[0], rgb[1], rgb[2] ) );
            graphics.FillRectangle( &brush, x, y, dotPixelSize, dotPixelSize );
        }
    }
}
