#pragma once

#include "KHermiteCurve.h"
#include "KVector2.h"


///-----------------------------------------------------------------------------
/// @class  KHermiteSpline2
/// @brief  2-dimensional Hermite spline
/// @see    KHermiteCurve
///
class KHermiteSpline2
{
private:
    KHermiteCurve   m_aHermiteCurve[2];

public:
    KHermiteSpline2(){}
    KHermiteSpline2( const KVector2& p0, const KVector2& p1, const KVector2& dp0, const KVector2& dp1 )
    {
        m_aHermiteCurve[ 0 ].Construct( p0.x, p1.x, dp0.x, dp1.x );
        m_aHermiteCurve[ 1 ].Construct( p0.y, p1.y, dp0.y, dp1.y );
    }

    ~KHermiteSpline2(){}

    void Construct( const KVector2& p0, const KVector2& p1, const KVector2& dp0, const KVector2& dp1 )
    {
        m_aHermiteCurve[ 0 ].Construct( p0.x, p1.x, dp0.x, dp1.x );
        m_aHermiteCurve[ 1 ].Construct( p0.y, p1.y, dp0.y, dp1.y );
    }

    const KVector2 GetPosition( float u_ ) const
    {
        return KVector2( m_aHermiteCurve[ 0 ].CalculateX( u_ ), m_aHermiteCurve[ 1 ].CalculateX( u_ ) );
    }

    const KVector2 GetTangent( float u_ ) const
    {
        return KVector2( m_aHermiteCurve[ 0 ].CalculateDxDu( u_ ), m_aHermiteCurve[ 1 ].CalculateDxDu( u_ ) );
    }

    const KVector2 GetCurvature( float u_ ) const
    {
        return KVector2( m_aHermiteCurve[ 0 ].CalculateDx2D2u( u_ ), m_aHermiteCurve[ 1 ].CalculateDx2D2u( u_ ) );
    }
};//class KHermiteSpline2
