#include "stdafx.h"
#include "KPolygon.h"
#include "KVectorUtil.h"

void DrawIndexedPrimitive( HDC hdc
    , int* m_indexBuffer            // index buffer
    , int primitiveCounter          // primitive counter
    , KVector3* m_vertexBuffer      // vertex buffer
    , COLORREF color )
{
    int   i1, i2;
    int   counter = 0;

    for (int i=0; i<primitiveCounter; ++i)
    {
		// get index
        i1 = m_indexBuffer[counter];
        i2 = m_indexBuffer[counter+1];

		// draw line
        KVectorUtil::DrawLine(hdc, m_vertexBuffer[i1].x, m_vertexBuffer[i1].y
            , m_vertexBuffer[i2].x, m_vertexBuffer[i2].y, 2, PS_SOLID, color );

		// advance to next primitive
        counter += 2;
    }//for
}//DrawIndexedPrimitive()


KPolygon::KPolygon()
{
    m_sizeIndex  = 0;
    m_sizeVertex = 0;
    m_color = RGB(0, 0, 255);
}//KPolygon::KPolygon()

KPolygon::~KPolygon()
{
}//KPolygon::~KPolygon()

void KPolygon::SetIndexBuffer()
{
    int buffer[] = {0,1,
                    1,3,
                    3,0,
                    1,2,
                    2,3,
                    3,1};
    for (int i=0; i<12; ++i)
    {
        m_indexBuffer[i] = buffer[i];
    }//for
    m_sizeIndex = 12;
}//KPolygon::SetIndexBuffer()

void KPolygon::SetVertexBuffer()
{
    m_vertexBuffer[0] = KVector3(-5.0f, 5.0f, 0.0f);
    m_vertexBuffer[1] = KVector3(-5.0f, -5.0f, 0.0f);
    m_vertexBuffer[2] = KVector3(5.0f, -5.0f, 0.0f);
    m_vertexBuffer[3] = KVector3(5.0f, 5.0f, 0.0f);
    m_sizeVertex = 4;
}//KPolygon::SetVertexBuffer()

void KPolygon::Render(HDC hdc)
{
    ::DrawIndexedPrimitive(
        hdc,
        m_indexBuffer,      // index buffer
        6,                  // primitive counter
        m_vertexBuffer,     // vertex buffer
        m_color);
}//KPolygon::Render()
