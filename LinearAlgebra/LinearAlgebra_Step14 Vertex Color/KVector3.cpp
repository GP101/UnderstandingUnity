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

float KVector3::Length() const
{
    return sqrtf(x*x + y*y + z*z);
}

void KVector3::Normalize( )
{
    const float len = Length();
    x /= len;
    y /= len;
    z /= len;
}