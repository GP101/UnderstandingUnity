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

float& KMatrix4::operator()(int iRow, int iCol)
{
    return m_afElements[iRow][iCol];
}//KMatrix4::operator()

KMatrix4 KMatrix4::operator*(KMatrix4& mRight)
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

KVector3 KMatrix4::operator*(KVector3& vLeft)
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
    const float w = vLeft.x * m_afElements[3][0] +
        vLeft.y * m_afElements[3][1] +
        vLeft.z * m_afElements[3][2] +
        m_afElements[3][3];
    vRet.x /= w; // homogeneous divide
    vRet.y /= w;
    vRet.z /= w;
    return vRet;
}//KMatrix4::operator*()

KMatrix4 KMatrix4::operator+(KMatrix4& mRight)
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

KMatrix4& KMatrix4::operator=(KMatrix4& mRight)
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
    m_afElements[3][3] = 1.f;

    return *this;
}//KMatrix4::SetIdentity()

KMatrix4 KMatrix4::SetRotationX(float fRadian)
{
    SetIdentity();
    m_afElements[1][1] = cosf(fRadian);
    m_afElements[1][2] = -sinf(fRadian);
    m_afElements[2][1] = sinf(fRadian);
    m_afElements[2][2] = cosf(fRadian);
    return *this;
}//KMatrix4::SetRotationX()

KMatrix4 KMatrix4::SetRotationY(float fRadian)
{
    SetIdentity();
    m_afElements[0][0] = cosf(fRadian);
    m_afElements[0][2] = sinf(fRadian);
    m_afElements[2][0] = -sinf(fRadian);
    m_afElements[2][2] = cosf(fRadian);
    return *this;
}//KMatrix4::SetRotationY()

KMatrix4 KMatrix4::SetRotationZ(float fRadian)
{
    SetIdentity();
    m_afElements[0][0] = cosf(fRadian);
    m_afElements[0][1] = -sinf(fRadian);
    m_afElements[1][0] = sinf(fRadian);
    m_afElements[1][1] = cosf(fRadian);
    return *this;
}//KMatrix4::SetRotationZ()

KMatrix4 KMatrix4::SetScale(float fxScale, float fyScale, float fzScale)
{
    SetIdentity();
    m_afElements[0][0] = fxScale;
    m_afElements[1][1] = fyScale;
    m_afElements[2][2] = fzScale;

    return *this;
}//KMatrix4::SetScale()

KMatrix4 KMatrix4::SetTranslation(float x, float y, float z)
{
    SetIdentity();
    m_afElements[0][3] = x;
    m_afElements[1][3] = y;
    m_afElements[2][3] = z;

    return *this;
}//KMatrix4::SetTranslation()
