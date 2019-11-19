#pragma once

#include "KVector3.h"
#include "KMatrix4.h"
#include "KQuaternion.h"
#include "KPrimitive.h"

class KCube : public KPrimitive
{
public:
    KCube();
    virtual ~KCube();
    void SetIndexBuffer();
    void SetVertexBuffer();
    void DrawOnCanvas( HDC hdc );
};//class KCube
