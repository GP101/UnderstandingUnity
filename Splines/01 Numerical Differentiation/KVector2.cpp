#include "stdafx.h"
#include "KVector2.h"

KVector2 KVector2::zero = KVector2(0, 0);
KVector2 KVector2::one = KVector2(1, 1);
KVector2 KVector2::right = KVector2(1, 0);
KVector2 KVector2::up = KVector2(0, 1);

KVector2 KVector2::Lerp(const KVector2& begin, const KVector2& end, double ratio_)
{
    double ratio = __min(1, __max(0, ratio_));
    KVector2 temp;
    temp.x = begin.x + (end.x - begin.x) * ratio;
    temp.y = begin.y + (end.y - begin.y) * ratio;
    return temp;
}
