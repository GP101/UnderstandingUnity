#include "stdafx.h"
#include "KCube.h"
#include "KVectorUtil.h"
#include "KRgb.h"
#include <algorithm>
#include "KMaterial.h"
#include "KRenderState.h"

KCube::KCube()
{
    SetIndexBuffer();
    SetVertexBuffer();
    m_color = RGB( 255, 0, 255 );
}

KCube::~KCube()
{
}

void KCube::SetIndexBuffer()
{
    int buffer[] = {
        0,2,1,
        2,0,3,
        3,6,2,
        6,3,7,
        7,5,6,
        5,7,4,
        4,1,5,
        1,4,0,
        4,3,0,
        3,4,7,
        1,6,5,
        6,1,2
    };

    for (int i=0; i<_countof(buffer); ++i)
    {
        m_indexBuffer[i] = buffer[i];
    }//for
    m_sizeIndex = _countof(buffer);
}

void KCube::SetVertexBuffer()
{
    m_vertexBuffer[0] = KVector3(-0.5, -0.5, +0.5);
    m_vertexBuffer[1] = KVector3(-0.5, +0.5, +0.5);
    m_vertexBuffer[2] = KVector3(+0.5, +0.5, +0.5);
    m_vertexBuffer[3] = KVector3(+0.5, -0.5, +0.5);
    m_vertexBuffer[4] = KVector3(-0.5, -0.5, -0.5);
    m_vertexBuffer[5] = KVector3(-0.5, +0.5, -0.5);
    m_vertexBuffer[6] = KVector3(+0.5, +0.5, -0.5);
    m_vertexBuffer[7] = KVector3(+0.5, -0.5, -0.5);
    m_sizeVertex = 8;
}

void KCube::DrawOnCanvas( HDC hdc )
{
    KMaterial mtrl;
    mtrl.m_diffuse = KRgb( 1, 0, 1 ); //m_color;
    mtrl.m_specular = KRgb( 0, 0, 0 );
    KRenderState::GetInstance().SetMaterial( mtrl );

    ::ScanDrawIndexedPrimitive(
        hdc,
        m_indexBuffer,      // index buffer
        12,                 // primitive(triangle) counter
        m_vertexBuffer,     // vertex buffer
        m_color );
}//KPolygon::Render()
