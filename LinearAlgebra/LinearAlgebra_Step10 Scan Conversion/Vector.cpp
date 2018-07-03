#include "stdafx.h"
#include "Vector.h"
#include <math.h> // step1

CVector::CVector()
{
    m_x = 0;
    m_y = 0;
    m_z = 0;
}//CVector::CVector()

CVector::~CVector()
{
}//CVector::~CVector()

CVector::CVector(float x, float y, float z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}//CVector::CVector()

void CVector::Translate(float tx, float ty, float tz)
{
    m_x += tx;
    m_y += ty;
    m_z += tz;
}//CVector::Translate()

void CVector::Scale(float sx, float sy, float sz)
{
    m_x *= sx;
    m_y *= sy;
    m_z *= sz;
}//CVector::Scale()

void CVector::RotateX(float theta)
{
    float tx,ty,tz;

    tx = m_x;
    ty = m_y*cosf(theta) - m_z*sinf(theta);
    tz = m_y*sinf(theta) + m_z*cosf(theta);

    m_y = ty;
    m_z = tz;
}//CVector::RotateX()

void CVector::RotateY(float theta)
{
    float tx,ty,tz;

    tx =  m_x*cosf(theta) + m_z*sinf(theta);
    ty =  m_y;
    tz = -m_x*sinf(theta) + m_z*cosf(theta);

    m_x = tx;
    m_z = tz;
}//CVector::RotateY()

void CVector::RotateZ(float theta)
{
    float tx,ty,tz;

    tx = m_x*cosf(theta) - m_y*sinf(theta);
    ty = m_x*sinf(theta) + m_y*cosf(theta);
    tz = m_z;

    m_x = tx;
    m_y = ty;
}//CVector::RotateZ()
