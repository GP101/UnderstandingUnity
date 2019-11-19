#pragma once

class KVector3
{
public:
    double  x;
    double  y;
    double  z;

public:
    KVector3( double x=0.0, double y=0.0, double z=0.0);
    KVector3(int tx, int ty, int tz) { x = (double)tx; y = (double)ty; z = (double)tz; }
    ~KVector3();

    double Length() const;
    KVector3 Normalize() const;
    KVector3 operator-()
    {
        KVector3 temp( -x, -y, -z );
        return temp;
    }
};//class KVector3

inline KVector3 operator+(const KVector3& lhs, const KVector3& rhs)
{
    KVector3 temp(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    return temp;
}

inline KVector3 operator-( const KVector3& lhs, const KVector3& rhs )
{
    KVector3 temp( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
    return temp;
}

inline KVector3 operator*( double scaler, const KVector3& rhs )
{
    KVector3 temp( scaler*rhs.x, scaler*rhs.y, scaler*rhs.z );
    return temp;
}

inline KVector3 operator*( const KVector3& lhs, double scaler )
{
    KVector3 temp( scaler*lhs.x, scaler*lhs.y, scaler*lhs.z );
    return temp;
}

inline double Dot(const KVector3& lhs, const KVector3& rhs)
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
