#include "stdafx.h"
#include "KVectorUtil.h"

void KVectorUtil::DrawLine(HDC hdc, const KVector2& v0, const KVector2& v1)
{
    MoveToEx(hdc, (int)v0.x, (int)v0.y, nullptr);
    LineTo(hdc, (int)v1.x, (int)v1.y);
}
