#pragma once

class KVector2
{
public:
    float   x;
    float   y;

public:
    KVector2(float tx, float ty) { x = tx; y = ty; }
    KVector2(int tx, int ty) { x = (float)tx; y = (float)ty; }
};

inline KVector2 operator+(const KVector2& lhs, const KVector2& rhs)
{
    KVector2 temp(lhs.x + rhs.x, lhs.y + rhs.y);
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
