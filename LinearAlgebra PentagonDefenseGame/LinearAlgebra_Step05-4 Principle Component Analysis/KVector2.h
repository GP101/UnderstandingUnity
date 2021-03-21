#pragma once
#include <math.h>

class KVector2
{
public:
    static KVector2 zero;
    static KVector2 one;
    static KVector2 right;
    static KVector2 up;

    static KVector2 Lerp(const KVector2& begin, const KVector2& end, float ratio);
    static float Dot(const KVector2& left, const KVector2& right);
    static float LengthSquared(const KVector2& a, const KVector2& b);
    static float Length(const KVector2& a, const KVector2& b);

public:
    float   x;
    float   y;

public:
    KVector2(float tx=0.0f, float ty=0.0f) { x = tx; y = ty; }
    KVector2(int tx, int ty) { x = (float)tx; y = (float)ty; }
    float Length() const
    {
        return sqrtf(x*x + y*y);
    }
    void Normalize()
    {
        const float length = Length();
        x = x / length;
        y = y / length;
    }
    float& operator[](int i)
    {
        if (i == 0)
            return x;
        return y;
    }
};

inline KVector2 operator+(const KVector2& lhs, const KVector2& rhs)
{
    KVector2 temp(lhs.x + rhs.x, lhs.y + rhs.y);
    return temp;
}

inline KVector2 operator-(const KVector2& lhs, const KVector2& rhs)
{
    KVector2 temp(lhs.x - rhs.x, lhs.y - rhs.y);
    return temp;
}

inline KVector2 operator*(float scalar, const KVector2& rhs)
{
    KVector2 temp(scalar*rhs.x, scalar*rhs.y);
    return temp;
}

inline KVector2 operator*(const KVector2& lhs, float scalar)
{
    KVector2 temp(scalar*lhs.x, scalar*lhs.y);
    return temp;
}
