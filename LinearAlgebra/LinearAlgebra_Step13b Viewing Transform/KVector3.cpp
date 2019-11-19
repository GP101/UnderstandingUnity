#include "stdafx.h"
#include "KVector3.h"
#include <math.h> // step1

KVector3::KVector3( double x_, double y_, double z_)
{
    x = x_;
    y = y_;
    z = z_;
}//KVector3::KVector3()

KVector3::~KVector3()
{
}//KVector3::~KVector3()

double KVector3::Length() const
{
    return sqrt( x*x + y * y + z * z );
}

KVector3 KVector3::Normalize() const
{
    const double len = Length();
    KVector3 temp( x / len, y / len, z / len );
    return temp;
}