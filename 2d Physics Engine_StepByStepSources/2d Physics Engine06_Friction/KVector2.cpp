#include "KVector2.h"
#include <algorithm>

float KVector2::EPSILON = 0.0001f;
float KVector2::PI = 3.141592741f;

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

float KVector2::DistSquared(const KVector2& a, const KVector2& b)
{
    KVector2 t = a - b;
    return t.x * t.x + t.y * t.y;
}

float KVector2::Dist(const KVector2& a, const KVector2& b)
{
    return sqrtf(DistSquared(a, b));
}

float KVector2::Determinant(const KVector2&a, const KVector2 &b)
{
	return a.x*b.y - b.x*a.y;
}

bool KVector2::IsCCW(const KVector2&a, const KVector2 &b)
{
	return KVector2::Determinant(a, b) > 0.0f;
}

KVector2 KVector2::Min(const KVector2& a, const KVector2& b)
{
	return KVector2(std::min(a.x, b.x), std::min(a.y, b.y));
}

KVector2 KVector2::Max(const KVector2& a, const KVector2& b)
{
	return KVector2(std::max(a.x, b.x), std::max(a.y, b.y));
}

KVector2 KVector2::Cross(const KVector2& v, float a)
{
	// v is rotated 90-degree CW
	return KVector2(a * v.y, -a * v.x);
}

KVector2 KVector2::Cross(float a, const KVector2& v)
{
	// v is rotated 90-degree CCW
	return KVector2(-a * v.y, a * v.x);
}

float KVector2::Cross(const KVector2& a, const KVector2& b)
{
	return a.x * b.y - a.y * b.x;
}
