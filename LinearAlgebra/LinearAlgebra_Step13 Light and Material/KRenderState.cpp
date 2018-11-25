#include "stdafx.h"
#include "KRenderState.h"

CONSTRUCTOR KRenderState::KRenderState()
{
}

DESTRUCTOR KRenderState::~KRenderState()
{
}

void KRenderState::SetLight(KLight light)
{
    m_light = light;
}

void KRenderState::SetMaterial(KMaterial mtrl)
{
    m_material = mtrl;
}

KLight KRenderState::GetLight()
{
    return m_light;
}

KMaterial KRenderState::GetMaterial()
{
    return m_material;
}
