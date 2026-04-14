#pragma once
#include "KGen.h"
#include "KMaterial.h"
#include "KLight.h"

class KRenderState
{
public:
    static KRenderState& GetInstance()
    {
        static KRenderState instance;
        return instance;
    }

private:
    KLight              m_light;
    KMaterial           m_material;

public:
    CONSTRUCTOR         KRenderState();
    DESTRUCTOR          ~KRenderState();

    void                SetLight(KLight light);
    void                SetMaterial(KMaterial mtrl);
    KLight              GetLight();
    KMaterial           GetMaterial();
};
