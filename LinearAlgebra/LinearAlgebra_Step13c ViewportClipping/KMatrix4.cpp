#include "stdafx.h"
#include "KMatrix4.h"
#include <math.h>

KMatrix4::KMatrix4()
{
    SetIdentity();
}//KMatrix4::KMatrix4()

KMatrix4::~KMatrix4()
{
}//KMatrix4::~KMatrix4()

double& KMatrix4::operator()(int iRow, int iCol)
{
    return m_afElements[iRow][iCol];
}//KMatrix4::operator()

KMatrix4 KMatrix4::operator*( KMatrix4& mRight)
{
    KMatrix4 mRet;

    mRet.SetZero();
    for (int i=0; i<4; ++i)
    {
        for (int j=0; j<4; ++j)
        {
            for (int k=0; k<4; ++k)
            {
                mRet(i,j) += m_afElements[i][k] * mRight(k,j);
            }//for
        }//for
    }//for

    return mRet;
}//KMatrix4::operator*()

KVector3 KMatrix4::operator*( KVector3& vLeft)
{
    KVector3 vRet;

    vRet.x = vLeft.x * m_afElements[0][0] +
               vLeft.y * m_afElements[0][1] +
               vLeft.z * m_afElements[0][2] +
               m_afElements[0][3];
    vRet.y = vLeft.x * m_afElements[1][0] +
               vLeft.y * m_afElements[1][1] +
               vLeft.z * m_afElements[1][2] +
               m_afElements[1][3];
    vRet.z = vLeft.x * m_afElements[2][0] +
               vLeft.y * m_afElements[2][1] +
               vLeft.z * m_afElements[2][2] +
               m_afElements[2][3];
    const double w = vLeft.x * m_afElements[3][0] +
        vLeft.y * m_afElements[3][1] +
        vLeft.z * m_afElements[3][2] +
        m_afElements[3][3];
    vRet.x /= w; // homogeneous divide
    vRet.y /= w;
    vRet.z /= w;
    return vRet;
}//KMatrix4::operator*()

KMatrix4 KMatrix4::operator+( KMatrix4& mRight)
{
    KMatrix4 mRet;

    for (int i=0; i<4; ++i)
    {
        for (int j=0; j<4; ++j)
        {
            mRet(i,j) = m_afElements[i][j] + mRight(i,j);
        }//for
    }//for

    return mRet;
}//KMatrix4::operator+()

KMatrix4& KMatrix4::operator=( KMatrix4& mRight)
{
    memcpy( m_afElements, mRight.m_afElements, sizeof(m_afElements) );
    return *this;
}//KMatrix4::operator=()

KMatrix4 KMatrix4::SetZero()
{
    memset( m_afElements, 0, sizeof(m_afElements) );

    return *this;
}//KMatrix4::SetZero()

KMatrix4 KMatrix4::SetIdentity()
{
    SetZero();

    m_afElements[0][0] =
    m_afElements[1][1] =
    m_afElements[2][2] =
    m_afElements[3][3] = 1.0;

    return *this;
}//KMatrix4::SetIdentity()

KMatrix4 KMatrix4::SetRotationX( double fRadian)
{
    SetIdentity();
    m_afElements[1][1] = cos(fRadian);
    m_afElements[1][2] = -sin(fRadian);
    m_afElements[2][1] = sin(fRadian);
    m_afElements[2][2] = cos(fRadian);
    return *this;
}//KMatrix4::SetRotationX()

KMatrix4 KMatrix4::SetRotationY( double fRadian)
{
    SetIdentity();
    m_afElements[0][0] = cos(fRadian);
    m_afElements[0][2] = sin(fRadian);
    m_afElements[2][0] = -sin(fRadian);
    m_afElements[2][2] = cos(fRadian);
    return *this;
}//KMatrix4::SetRotationY()

KMatrix4 KMatrix4::SetRotationZ( double fRadian)
{
    SetIdentity();
    m_afElements[0][0] = cos(fRadian);
    m_afElements[0][1] = -sin(fRadian);
    m_afElements[1][0] = sin(fRadian);
    m_afElements[1][1] = cos(fRadian);
    return *this;
}//KMatrix4::SetRotationZ()

KMatrix4 KMatrix4::SetScale( double fxScale, double fyScale, double fzScale)
{
    SetIdentity();
    m_afElements[0][0] = fxScale;
    m_afElements[1][1] = fyScale;
    m_afElements[2][2] = fzScale;

    return *this;
}//KMatrix4::SetScale()

KMatrix4 KMatrix4::SetTranslation( double x, double y, double z)
{
    SetIdentity();
    m_afElements[0][3] = x;
    m_afElements[1][3] = y;
    m_afElements[2][3] = z;

    return *this;
}//KMatrix4::SetTranslation()

KMatrix4 KMatrix4::SetOrientation( const KVector3& xaxis, const KVector3& yaxis, const KVector3& zaxis )
{
    SetIdentity();
    m_afElements[0][0] = xaxis.x; // first column
    m_afElements[1][0] = xaxis.y;
    m_afElements[2][0] = xaxis.z;
    m_afElements[0][1] = yaxis.x; // second column
    m_afElements[1][1] = yaxis.y;
    m_afElements[2][1] = yaxis.z;
    m_afElements[0][2] = zaxis.x; // third column
    m_afElements[1][2] = zaxis.y;
    m_afElements[2][2] = zaxis.z;

    return *this;
}//KMatrix4::SetOrientation()

KMatrix4 KMatrix4::LookAtRH( KVector3 eye, KVector3 target, KVector3 up )
{
    KVector3 zaxis = ( target - eye ).Normalize(); // forward
    KVector3 xaxis = Cross( up, zaxis ).Normalize(); // right
    KVector3 yaxis = Cross( zaxis, xaxis ); // up

    KVector3 xtranspose{ xaxis.x, yaxis.x, zaxis.x };
    KVector3 ytranspose{ xaxis.y, yaxis.y, zaxis.y };
    KVector3 ztranspose{ xaxis.z, yaxis.z, zaxis.z };

    KMatrix4 orientationInv;
    orientationInv.SetOrientation( xtranspose, ytranspose, ztranspose );

    // T(v)^-1 == T(-v)
    KMatrix4 translationInv;
    translationInv.SetTranslation( -eye.x, -eye.y, -eye.z );

    *this = orientationInv * translationInv;
    return *this;
}//KMatrix4::LookAtRH()

KMatrix4 KMatrix4::SetProjection( double d )
{
    SetZero( );
    m_afElements[0][0] = 1;
    m_afElements[1][1] = 1;
    m_afElements[2][2] = 1;
    m_afElements[3][2] = 1.0 / d;
    m_afElements[3][3] = 1;

    return *this;
}//KMatrix4::SetProjection

KVector3 KMatrix4::GetBasis( int index ) const
{
    KVector3 v;
    v.x = m_afElements[0][index];
    v.y = m_afElements[1][index];
    v.z = m_afElements[2][index];
    return v;
}
