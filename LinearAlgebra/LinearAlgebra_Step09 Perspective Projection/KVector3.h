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

    void Translate(float tx, float ty, float tz);
    void Scale(float sx, float sy, float sz);
    void RotateX(float theta);
    void RotateY(float theta);
    void RotateZ(float theta);
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

inline float Dot(const KVector3& lhs, const KVector3& rhs)
{
    return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}

inline KVector3 Cross(const KVector3& u, const KVector3& v)
{
    KVector3 temp;
    temp.x = u.y*v.z - u.z*v.y;
    temp.y = u.z*v.x - u.x*v.z;
    temp.z = u.x*v.y - u.y*v.x;
    return temp;
}
