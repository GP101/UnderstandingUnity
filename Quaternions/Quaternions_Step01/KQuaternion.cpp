#include "stdafx.h"
#include "KQuaternion.h"


const KQuaternion KQuaternion::Zero = KQuaternion( 0.0, 0.0, 0.0, 0.0 );
const KQuaternion KQuaternion::One = KQuaternion( 1.0, 0.0, 0.0, 0.0 );
const KQuaternion KQuaternion::xHat = KQuaternion( 0.0, 1.0, 0.0, 0.0 );
const KQuaternion KQuaternion::yHat = KQuaternion( 0.0, 0.0, 1.0, 0.0 );
const KQuaternion KQuaternion::zHat = KQuaternion( 0.0, 0.0, 0.0, 1.0 );


KQuaternion::KQuaternion( )
: w( 0.0 ), x( 0.0 ), y( 0.0 ), z( 0.0 )
{
}

KQuaternion::KQuaternion( const KQuaternion& Q )
: w( Q.w ), x( Q.x ), y( Q.y ), z( Q.z )
{
}

KQuaternion::KQuaternion( const double w0, const double x0, const double y0, const double z0 )
: w( w0 ), x( x0 ), y( y0 ), z( z0 )
{
}

/// Quaternion multiplication.
KQuaternion KQuaternion::operator*( const KQuaternion& Q ) const
{
    return KQuaternion( w*Q.w - x*Q.x - y*Q.y - z*Q.z
        , w*Q.x + x*Q.w + y*Q.z - z*Q.y
        , w*Q.y - x*Q.z + y*Q.w + z*Q.x
        , w*Q.z + x*Q.y - y*Q.x + z*Q.w );
}
