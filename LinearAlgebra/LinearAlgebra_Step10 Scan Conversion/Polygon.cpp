#include "stdafx.h"
#include "Polygon.h"

void DrawIndexedPrimitive( HDC hdc,
        int*        m_indexBuffer,      // index buffer
        int         primitiveCounter,   // primitive counter
        CVector*    m_vertexBuffer )    // vertex buffer
{
    int   i1, i2;
    int   counter = 0;

    for (int i=0; i<primitiveCounter; ++i)
    {
		// get index
        i1 = m_indexBuffer[counter];
        i2 = m_indexBuffer[counter+1];

		// draw line
        MoveToEx( hdc, int(m_vertexBuffer[i1].m_x),
            int(m_vertexBuffer[i1].m_y), NULL );
        LineTo( hdc, int(m_vertexBuffer[i2].m_x),
            int(m_vertexBuffer[i2].m_y) );

		// advance to next primitive
        counter += 2;
    }//for
}//DrawIndexedPrimitive()


CPolygon::CPolygon()
{
    m_sizeIndex  = 0;
    m_sizeVertex = 0;
}//CPolygon::CPolygon()

CPolygon::~CPolygon()
{
}//CPolygon::~CPolygon()

void CPolygon::SetIndexBuffer()
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
}//CPolygon::SetIndexBuffer()

void CPolygon::SetVertexBuffer()
{
    m_vertexBuffer[0] = CVector(   0.0f, 100.0f, 0);
    m_vertexBuffer[1] = CVector(   0.0f,   0.0f, 0);
    m_vertexBuffer[2] = CVector( 100.0f,   0.0f, 0);
    m_vertexBuffer[3] = CVector( 100.0f, 100.0f, 0);
    m_sizeVertex = 4;
}//CPolygon::SetVertexBuffer()

void CPolygon::Render(HDC hdc)
{
    ::DrawIndexedPrimitive(
        hdc,
        m_indexBuffer,      // index buffer
        6,                  // primitive counter
        m_vertexBuffer );   // vertex buffer
}//CPolygon::Render()
