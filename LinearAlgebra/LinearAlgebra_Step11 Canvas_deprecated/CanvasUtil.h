#pragma once
#include "Canvas.h"
#include <windows.h>

namespace CanvasUtil
{
    void DrawCanvas( HDC hdc, Canvas& canvas, float originScreenX, float originScreenY, float dotPixelSize );
}
