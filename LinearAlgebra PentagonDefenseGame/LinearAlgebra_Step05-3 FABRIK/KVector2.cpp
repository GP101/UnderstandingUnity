#include "stdafx.h"
#include "KVector2.h"

KVector2 KVector2::zero = KVector2(0, 0);
KVector2 KVector2::one = KVector2(1, 1);
KVector2 KVector2::right = KVector2(1, 0);
KVector2 KVector2::up = KVector2(0, 1);

KVector2 KVector2::Lerp(const KVector2& begin, const KVector2& end, float ratio_)
{
    float ratio = __min(1, __max(0, ratio_));
    KVector2 temp;
    temp.x = begin.x + (end.x - begin.x) * ratio;
    temp.y = begin.y + (end.y - begin.y) * ratio;
    return temp;
}

float KVector2::Dot(const KVector2& left, const KVector2& right)
{
    return left.x * right.x + left.y * right.y;
}//KVector2::Dot()

float KVector2::LengthSquared(const KVector2& a, const KVector2& b)
{
    KVector2 t = b - a;
    return t.x * t.x + t.y * t.y;
}

float KVector2::Length(const KVector2& a, const KVector2& b)
{
    return sqrtf(LengthSquared(a, b));
}
