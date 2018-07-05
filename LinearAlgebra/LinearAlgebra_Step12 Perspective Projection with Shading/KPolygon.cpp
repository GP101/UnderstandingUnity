#include "stdafx.h"
#include "KPolygon.h"
#include "KVectorUtil.h"

void DrawIndexedPrimitive( HDC hdc
    , int* m_indexBuffer            // index buffer
    , int primitiveCounter          // primitive counter
    , KVector3* m_vertexBuffer      // vertex buffer
    , COLORREF color )
{
    int   i0, i1, i2;
    int   counter = 0;

    for (int i = 0; i < primitiveCounter; ++i)
    {
        // get index
        i0 = m_indexBuffer[counter];
        i1 = m_indexBuffer[counter + 1];
        i2 = m_indexBuffer[counter + 2];

        KVector3 normal;
        normal = Cross(m_vertexBuffer[i0] - m_vertexBuffer[i1], m_vertexBuffer[i0] - m_vertexBuffer[i2]);
        KVector3 forward(0, 0, 1);
        int penStyle = PS_SOLID;
        int penWidth = 3;
        if (Dot(forward, normal) > 0)
        {
            penStyle = PS_DOT;
            penWidth = 1;
        }

        // draw triangle
        KVectorUtil::DrawLine(hdc, m_vertexBuffer[i0].x, m_vertexBuffer[i0].y
            , m_vertexBuffer[i1].x, m_vertexBuffer[i1].y, penWidth, penStyle, color );
        KVectorUtil::DrawLine(hdc, m_vertexBuffer[i1].x, m_vertexBuffer[i1].y
            , m_vertexBuffer[i2].x, m_vertexBuffer[i2].y, penWidth, penStyle, color );
        KVectorUtil::DrawLine(hdc, m_vertexBuffer[i2].x, m_vertexBuffer[i2].y
            , m_vertexBuffer[i0].x, m_vertexBuffer[i0].y, penWidth, penStyle, color );

		// advance to next primitive
        counter += 3;
    }//for
}//DrawIndexedPrimitive()

void DrawIndexedPrimitive( Canvas& canvas
    , int* m_indexBuffer            // index buffer
    , int primitiveCounter          // primitive counter
    , KVector3* m_vertexBuffer      // vertex buffer
    , KVector3* m_normalBuffer      // normal buffer
    , COLORREF color_ )
{
    int   i0, i1, i2;
    int   counter = 0;

    KRgb color;
    color[0] = GetRValue( color_ );
    color[1] = GetGValue( color_ );
    color[2] = GetBValue( color_ );
    for(int i = 0; i < primitiveCounter; ++i)
    {
        // get index
        i0 = m_indexBuffer[counter];
        i1 = m_indexBuffer[counter + 1];
        i2 = m_indexBuffer[counter + 2];

        KVector3 normal;
        normal = Cross( m_vertexBuffer[i0] - m_vertexBuffer[i1], m_vertexBuffer[i0] - m_vertexBuffer[i2] );
        normal.Normalize();
        KVector3 forward( 0, 0, -1 );
        float dot = Dot( forward, normal );
        float shade = Dot( forward, m_normalBuffer[i] );
        const bool isVisible = (dot > 0);

        color[0] = 0;
        color[1] = 0;
        color[2] = shade * 192.0f + 64.0f;;

        // draw triangle
        if(isVisible == true)
        {
            canvas.FillTriangle( m_vertexBuffer[i0].x, m_vertexBuffer[i0].y, color
                , m_vertexBuffer[i1].x, m_vertexBuffer[i1].y, color
                , m_vertexBuffer[i2].x, m_vertexBuffer[i2].y, color );
            canvas.ScanLineSegment( m_vertexBuffer[i0].x, m_vertexBuffer[i0].y, KRgb(0,0,0)
                , m_vertexBuffer[i1].x, m_vertexBuffer[i1].y, KRgb( 0, 0, 0 ), nullptr );
            canvas.ScanLineSegment( m_vertexBuffer[i1].x, m_vertexBuffer[i1].y, KRgb( 0, 0, 0 )
                , m_vertexBuffer[i2].x, m_vertexBuffer[i2].y, KRgb( 0, 0, 0 ), nullptr );
            canvas.ScanLineSegment( m_vertexBuffer[i2].x, m_vertexBuffer[i2].y, KRgb( 0, 0, 0 )
                , m_vertexBuffer[i0].x, m_vertexBuffer[i0].y, KRgb( 0, 0, 0 ), nullptr );
        }
        // advance to next primitive
        counter += 3;
    }//for
}//DrawIndexedPrimitive()

KPolygon::KPolygon()
{
    m_sizeIndex  = 0;
    m_sizeVertex = 0;
    m_color = RGB(0, 0, 255);
    m_primitiveCount = 0;
}//KPolygon::KPolygon()

KPolygon::~KPolygon()
{
}//KPolygon::~KPolygon()

void KPolygon::SetIndexBuffer()
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
    m_primitiveCount = m_sizeIndex / 3; // number of triangles.
}//KPolygon::SetIndexBuffer()

void KPolygon::SetVertexBuffer()
{
    m_vertexBuffer[0] = KVector3( -20.f,  20.f, -20.f);
    m_vertexBuffer[1] = KVector3( -20.f, -20.f, -20.f);
    m_vertexBuffer[2] = KVector3(  20.f, -20.f, -20.f);
    m_vertexBuffer[3] = KVector3(  20.f,  20.f, -20.f);
    m_vertexBuffer[4] = KVector3( -20.f,  20.f,  20.f);
    m_vertexBuffer[5] = KVector3( -20.f, -20.f,  20.f);
    m_vertexBuffer[6] = KVector3(  20.f, -20.f,  20.f);
    m_vertexBuffer[7] = KVector3(  20.f,  20.f,  20.f);
    m_sizeVertex = 8;
}//KPolygon::SetVertexBuffer()

void KPolygon::Render(HDC hdc)
{
    ::DrawIndexedPrimitive(
        hdc,
        m_indexBuffer,      // index buffer
        12,                   // primitive(triangle) counter
        m_vertexBuffer,     // vertex buffer
        m_color );
}//KPolygon::Render()

//void KPolygon::RotateX(float theta)
//{
//    for (int i=0; i<m_sizeVertex; ++i)
//    {
//        m_vertexBuffer[i].RotateX(theta);
//    }//for
//}//KPolygon::RotateX(float theta)
//
//void KPolygon::RotateY(float theta)
//{
//    for (int i=0; i<m_sizeVertex; ++i)
//    {
//        m_vertexBuffer[i].RotateY(theta);
//    }//for
//}//KPolygon::RotateY(float theta)
//
//void KPolygon::Translate(float tx, float ty, float tz)
//{
//    for (int i=0; i<m_sizeVertex; ++i)
//    {
//        m_vertexBuffer[i].Translate(tx,ty,tz);
//    }//for
//}//KPolygon::Translate(float tx, float ty, float tz)

void KPolygon::Transform(KMatrix4& mat)
{
    for (int i=0; i<m_sizeVertex; ++i)
    {
        m_vertexBuffer[i] = mat * m_vertexBuffer[i];
    }//for
}//KPolygon::Transform()

void KPolygon::UpdateNormal()
{
    int   i0, i1, i2;
    int   counter = 0;

    for(int i = 0; i < m_primitiveCount; ++i)
    {
        // get index
        i0 = m_indexBuffer[counter];
        i1 = m_indexBuffer[counter + 1];
        i2 = m_indexBuffer[counter + 2];

        KVector3 normal;
        normal = Cross( m_vertexBuffer[i0] - m_vertexBuffer[i1], m_vertexBuffer[i0] - m_vertexBuffer[i2] );
        normal.Normalize();
        m_normalBuffer[i] = normal;
        counter += 3;
    }//for
}

void KPolygon::DrawOnCanvas( Canvas& canvas )
{
    ::DrawIndexedPrimitive(
        canvas,
        m_indexBuffer,      // index buffer
        12,                 // primitive(triangle) counter
        m_vertexBuffer,     // vertex buffer
        m_normalBuffer,     // normal buffer
        m_color );
}//KPolygon::Render()