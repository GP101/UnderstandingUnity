#include "stdafx.h"
#include "KVector3.h"
#include <math.h> // step1

KVector3::KVector3(float x_, float y_, float z_)
{
    x = x_;
    y = y_;
    z = z_;
}//KVector3::KVector3()

KVector3::~KVector3()
{
}//KVector3::~KVector3()

void KVector3::Translate(float tx, float ty, float tz)
{
    x += tx;
    y += ty;
    z += tz;
}//KVector3::Translate()

void KVector3::Scale(float sx, float sy, float sz)
{
    x *= sx;
    y *= sy;
    z *= sz;
}//KVector3::Scale()

void KVector3::RotateX(float theta)
{
    float tx,ty,tz;

    tx = x;
    ty = y*cosf(theta) - z*sinf(theta);
    tz = y*sinf(theta) + z*cosf(theta);

    y = ty;
    z = tz;
}//KVector3::RotateX()

void KVector3::RotateY(float theta)
{
    float tx,ty,tz;

    tx =  x*cosf(theta) + z*sinf(theta);
    ty =  y;
    tz = -x*sinf(theta) + z*cosf(theta);

    x = tx;
    z = tz;
}//KVector3::RotateY()

void KVector3::RotateZ(float theta)
{
    float tx,ty,tz;

    tx = x*cosf(theta) - y*sinf(theta);
    ty = x*sinf(theta) + y*cosf(theta);
    tz = z;

    x = tx;
    y = ty;
}//KVector3::RotateZ()

float KVector3::Length() const
{
    return sqrtf(x*x + y*y + z*z);
}
