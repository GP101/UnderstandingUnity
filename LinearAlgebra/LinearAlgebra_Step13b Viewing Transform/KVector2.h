#pragma once
#include <math.h>

class KVector2
{
public:
    static KVector2 zero;
    static KVector2 one;
    static KVector2 right;
    static KVector2 up;

    static KVector2 Lerp(const KVector2& begin, const KVector2& end, double ratio);

public:
    double  x;
    double  y;

public:
    KVector2( double tx=0.0, double ty=0.0) { x = tx; y = ty; }
    KVector2(int tx, int ty) { x = (double)tx; y = (double)ty; }
    double Length() const
    {
        return sqrt( x*x + y * y );
    }
    KVector2 Normalize() const
    {
        const double length = Length();
        KVector2 temp{ x / length,y / length };
        return temp;
    }
    KVector2 operator+() const
    {
        return *this;
    }
    KVector2 operator-() const
    {
        return KVector2(-x, -y);
    }
};

inline KVector2 operator+(const KVector2& lhs, const KVector2& rhs)
{
    KVector2 temp(lhs.x + rhs.x, lhs.y + rhs.y);
    return temp;
}

inline KVector2 operator*( double scalar, const KVector2& rhs)
{
    KVector2 temp(scalar*rhs.x, scalar*rhs.y);
    return temp;
}

inline KVector2 operator*(const KVector2& lhs, double scalar)
{
    KVector2 temp(scalar*lhs.x, scalar*lhs.y);
    return temp;
}
