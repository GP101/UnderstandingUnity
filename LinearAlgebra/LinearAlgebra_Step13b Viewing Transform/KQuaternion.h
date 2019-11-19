#pragma once
#include <cmath>

class KQuaternion
{
public:
    const static KQuaternion Zero;
    const static KQuaternion  One;
    const static KQuaternion xHat;
    const static KQuaternion yHat;
    const static KQuaternion zHat;

public:
    double w;
    double x;
    double y;
    double z;

public: // Constructors
    KQuaternion( );
    KQuaternion( const KQuaternion& Q );
    KQuaternion( const double w0, const double x0, const double y0, const double z0 );

    inline KQuaternion operator/( const double t ) const { return KQuaternion( w / t, x / t, y / t, z / t ); }
    KQuaternion operator*( const KQuaternion& Q ) const;
    inline KQuaternion operator/( const KQuaternion& Q ) const { return ( *this )*( Q.inverse( ) ); }

    inline KQuaternion inverse( ) const { return conjugate( ) / normsquared( ); }
    inline KQuaternion conjugate( ) const { return KQuaternion( w, -x, -y, -z ); }
    inline double     normsquared( ) const { return ( w*w + x*x + y*y + z*z ); }
};
