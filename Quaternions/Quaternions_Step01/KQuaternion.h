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

    KQuaternion& operator=( const KQuaternion& Q ) { w = Q.w; x = Q.x; y = Q.y; z = Q.z; return *this; }
    KQuaternion& operator+=( const KQuaternion& Q ) { w += Q.w; x += Q.x; y += Q.y; z += Q.z; return *this; }

    inline KQuaternion operator-( ) const { return KQuaternion( -w, -x, -y, -z ); }
    inline KQuaternion operator+( const double t ) const { return KQuaternion( w + t, x, y, z ); }
    inline KQuaternion operator-( const double t ) const { return KQuaternion( w - t, x, y, z ); }
    inline KQuaternion operator*( const double t ) const { return KQuaternion( w*t, x*t, y*t, z*t ); }
    inline KQuaternion operator/( const double t ) const { return KQuaternion( w / t, x / t, y / t, z / t ); }
    inline KQuaternion operator+( const KQuaternion& Q ) const { return KQuaternion( w + Q.w, x + Q.x, y + Q.y, z + Q.z ); }
    inline KQuaternion operator-( const KQuaternion& Q ) const { return KQuaternion( w - Q.w, x - Q.x, y - Q.y, z - Q.z ); }
    KQuaternion operator*( const KQuaternion& Q ) const;
    inline KQuaternion operator/( const KQuaternion& Q ) const { return ( *this )*( Q.inverse( ) ); }

    inline KQuaternion cross( const KQuaternion& Q ) const { return KQuaternion( 0, -z*Q.y + y*Q.z, z*Q.x - x*Q.z, -y*Q.x + x*Q.y ); }
    //inline KQuaternion commutator( const KQuaternion& Q ) const { return KQuaternion( 0, -2 * z*Q.y + 2 * y*Q.z, 2 * z*Q.x - 2 * x*Q.z, -2 * y*Q.x + 2 * x*Q.y ); }
    inline double     dot( const KQuaternion& Q ) const { return x*Q.x + y*Q.y + z*Q.z; }
    inline double     abs( ) const { return std::sqrt( normsquared( ) ); }
    inline KQuaternion sqrt( ) const { const double Abs = abs( ); return ( *this / Abs + 1 )*std::sqrt( Abs / ( 2 + 2 * w / Abs ) ); }
    //inline double     angle( ) const { return 2 * logRotor( ).abs( ); }
    inline KQuaternion inverse( ) const { return conjugate( ) / normsquared( ); }
    inline KQuaternion conjugate( ) const { return KQuaternion( w, -x, -y, -z ); }
    inline KQuaternion normalized( ) const { return ( *this ) / abs( ); }
    inline double     normsquared( ) const { return ( w*w + x*x + y*y + z*z ); }
};
