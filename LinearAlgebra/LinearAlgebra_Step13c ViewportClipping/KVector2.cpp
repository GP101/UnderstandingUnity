#include "stdafx.h"
#include "KVector2.h"
#include <algorithm>

double KVector2::EPSILON = 0.0001;
double KVector2::PI = 3.141592741;

KVector2 KVector2::zero = KVector2(0, 0);
KVector2 KVector2::one = KVector2(1, 1);
KVector2 KVector2::right = KVector2(1, 0);
KVector2 KVector2::up = KVector2(0, 1);


bool KVector2::IsZero(double a)
{
	return std::abs(a) <= 0.00001f;
}

KVector2 KVector2::Lerp(const KVector2& begin, const KVector2& end, double ratio_)
{
	double ratio = __min(1, __max(0, ratio_));
	KVector2 temp;
	temp.x = begin.x + (end.x - begin.x) * ratio;
	temp.y = begin.y + (end.y - begin.y) * ratio;
	return temp;
}

double KVector2::Dot(const KVector2& left, const KVector2& right)
{
	return left.x * right.x + left.y * right.y;
}//KVector2::Dot()

double KVector2::DistSquared(const KVector2& a, const KVector2& b)
{
	KVector2 t = a - b;
	return t.x * t.x + t.y * t.y;
}

double KVector2::Dist(const KVector2& a, const KVector2& b)
{
	return sqrt(DistSquared(a, b));
}

double KVector2::Determinant(const KVector2& a, const KVector2& b)
{
	return a.x * b.y - b.x * a.y;
}

bool KVector2::IsCCW(const KVector2& a, const KVector2& b)
{
	return KVector2::Determinant(a, b) > 0.0f;
}

int KVector2::GetDirection(const KVector2& a, const KVector2& b, const KVector2& c)
{
	double val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
	if (IsZero(val))
		return 0;    //colinear
	else if (val > 0)
		return 1;    //clockwise direction

	return 2; //counter-clockwise direction
}

KVector2 KVector2::Min(const KVector2& a, const KVector2& b)
{
	return KVector2(__min(a.x, b.x), __min(a.y, b.y));
}

KVector2 KVector2::Max(const KVector2& a, const KVector2& b)
{
	return KVector2(__max(a.x, b.x), __max(a.y, b.y));
}

KVector2 KVector2::Cross(const KVector2& v, double a)
{
	// v is rotated 90-degree CW
	return KVector2(a * v.y, -a * v.x);
}

KVector2 KVector2::Cross(double a, const KVector2& v)
{
	// v is rotated 90-degree CCW
	return KVector2(-a * v.y, a * v.x);
}

double KVector2::Cross(const KVector2& a, const KVector2& b)
{
	return a.x * b.y - a.y * b.x;
}
