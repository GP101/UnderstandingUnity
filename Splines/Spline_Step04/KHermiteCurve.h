/** @file   KHermiteCurve.h
    @desc   interface of HermiteCurve
    @since  December 3, 2001

    @date   jintaeks on 2007-09-06, 12:17
*/

#pragma once

///-----------------------------------------------------------------------------
/// @Name   class KHermiteCurve
/// @Desc   represent a Hermite curve of one dimension
///
class KHermiteCurve
{
public:
    float   m_a;
    float   m_b;
    float   m_c;
    float   m_d;

public:
    KHermiteCurve(){}
    ~KHermiteCurve(){}

    /// @see 이재규, "C로 배우는 알고리즘2", p.1083
    /// @param  p1: first point
    /// @param  p2: second point
    /// @param  v1: slope on first point
    /// @param  v2: slope on second point
    KHermiteCurve(float p1, float p2, float v1, float v2)
    {
        Construct( p1, p2, v1, v2 );
    }
   
    /// set coefficients of a Hermite curve
    void Construct(float p1, float p2, float v1, float v2)
    {
        m_d = p1;
        m_c = v1;
        m_b = -3.0f*p1 + 3.0f*p2 - 2.0f*v1 - v2;
        m_a =  2.0f*p1 - 2.0f*p2 + v1 + v2;
    }

    /// get second derivative with respect to u
    /// used to get curvature
    float CalculateDx2D2u(float u) const
    {
        return 6.0f*m_a*u + 2.0f*m_b;
    }

    /// get first derivative with respect to u
    /// used to get tangent vector
    /// differentiation of equation t^3 + t^2 + t + 1
    float CalculateDxDu(float u) const
    {
        return 3.0f*m_a*u*u + 2.0f*m_b*u + m_c;
    }

    /// get valud of parameter u
    float CalculateX(float u) const
    {
        float uu, uuu;
        uu = u * u;
        uuu = uu * u;
        return m_a*uuu + m_b*uu + m_c*u + m_d;
    }
};//class KHermiteCurve
