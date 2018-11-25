#pragma once
#include "KGen.h"
#include "KRgb.h"
#include "KVector3.h"

class KLight
{
public:
    enum LightType
    {
        LT_DIRECTIONAL,
        LT_POINT,
        LT_SPOT,
    };

    KRgb                m_ambient = KRgb(0, 0, 0);
    KRgb                m_diffuse = KRgb(1, 1, 1);
    KRgb                m_specular = KRgb(0, 0, 0);
    KRgb                m_emissive = KRgb(0, 0, 0);
    LightType           m_lightType = LT_DIRECTIONAL;
    KVector3            m_dir = KVector3(0, 0, -1);

public:
    CONSTRUCTOR         KLight();
    DESTRUCTOR          ~KLight();
};

