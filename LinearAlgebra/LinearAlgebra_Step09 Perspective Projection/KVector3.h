#pragma once

class KVector3
{
public:
    float x;
    float y;
    float z;

public:
    KVector3(float x=0.0f, float y=0.0f, float z=0.0f);
    KVector3(int tx, int ty, int tz) { x = (float)tx; y = (float)ty; z = (float)tz; }
    ~KVector3();

    float Length() const;
};//class KVector3

inline KVector3 operator+(const KVector3& lhs, const KVector3& rhs)
{
    KVector3 temp(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    return temp;
}

inline KVector3 operator-(const KVector3& lhs, const KVector3& rhs)
{
    KVector3 temp(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    return temp;
}
