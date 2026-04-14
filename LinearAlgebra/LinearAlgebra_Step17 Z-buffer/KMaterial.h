#pragma once
#include "KGen.h"
#include "KRgb.h"

class KMaterial
{
public:
    KRgb                m_ambient = KRgb(0,0,0);
    KRgb                m_diffuse = KRgb(1,1,1);
    KRgb                m_specular = KRgb(0,0,0);
    KRgb                m_emissive = KRgb(0,0,0);
public:
    CONSTRUCTOR         KMaterial();
    DESTRUCTOR          ~KMaterial();
};
