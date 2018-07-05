#include "stdafx.h"
#include "KPolygon.h"
#include "KVectorUtil.h"

void DrawIndexedPrimitive( HDC hdc
    , int* m_indexBuffer            // index buffer
    , int primitiveCounter          // primitive counter
    , KVertex* m_vertexBuffer      // vertex buffer
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
        normal = Cross( m_vertexBuffer[i0].pos - m_vertexBuffer[i1].pos, m_vertexBuffer[i0].pos - m_vertexBuffer[i2].pos );
        KVector3 forward(0, 0, 1);
        int penStyle = PS_SOLID;
        int penWidth = 3;
        if (Dot(forward, normal) > 0)
        {
            penStyle = PS_DOT;
            penWidth = 1;
        }

        // draw triangle
        KVectorUtil::DrawLine( hdc, m_vertexBuffer[i0].pos.x, m_vertexBuffer[i0].pos.y
            , m_vertexBuffer[i1].pos.x, m_vertexBuffer[i1].pos.y, penWidth, penStyle, color );
        KVectorUtil::DrawLine( hdc, m_vertexBuffer[i1].pos.x, m_vertexBuffer[i1].pos.y
            , m_vertexBuffer[i2].pos.x, m_vertexBuffer[i2].pos.y, penWidth, penStyle, color );
        KVectorUtil::DrawLine( hdc, m_vertexBuffer[i2].pos.x, m_vertexBuffer[i2].pos.y
            , m_vertexBuffer[i0].pos.x, m_vertexBuffer[i0].pos.y, penWidth, penStyle, color );

		// advance to next primitive
        counter += 3;
    }//for
}//DrawIndexedPrimitive()

void DrawIndexedPrimitive( Canvas& canvas
    , int* m_indexBuffer            // index buffer
    , int primitiveCounter          // primitive counter
    , KVertex* m_vertexBuffer      // vertex buffer
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
        normal = Cross( m_vertexBuffer[i0].pos - m_vertexBuffer[i1].pos, m_vertexBuffer[i0].pos - m_vertexBuffer[i2].pos );
        normal.Normalize();
        KVector3 forward( 0, 0, -1 );
        float dot = Dot( forward, normal );
        float shade = Dot( forward, m_normalBuffer[i] );
        const bool isVisible = (dot > 0);

        color[0] = GetRValue( color_ ) * shade;
        color[1] = GetGValue( color_ ) * shade;
        color[2] = GetBValue( color_ ) * shade;

        // draw triangle
        if(isVisible == true)
        {
            canvas.FillTriangle( m_vertexBuffer[i0].pos.x, m_vertexBuffer[i0].pos.y, color
                , m_vertexBuffer[i1].pos.x, m_vertexBuffer[i1].pos.y, color
                , m_vertexBuffer[i2].pos.x, m_vertexBuffer[i2].pos.y, color );
            canvas.ScanLineSegment( m_vertexBuffer[i0].pos.x, m_vertexBuffer[i0].pos.y, KRgb( 0, 0, 0 )
                , m_vertexBuffer[i1].pos.x, m_vertexBuffer[i1].pos.y, KRgb( 0, 0, 0 ), nullptr );
            canvas.ScanLineSegment( m_vertexBuffer[i1].pos.x, m_vertexBuffer[i1].pos.y, KRgb( 0, 0, 0 )
                , m_vertexBuffer[i2].pos.x, m_vertexBuffer[i2].pos.y, KRgb( 0, 0, 0 ), nullptr );
            canvas.ScanLineSegment( m_vertexBuffer[i2].pos.x, m_vertexBuffer[i2].pos.y, KRgb( 0, 0, 0 )
                , m_vertexBuffer[i0].pos.x, m_vertexBuffer[i0].pos.y, KRgb( 0, 0, 0 ), nullptr );
        }
        // advance to next primitive
        counter += 3;
    }//for
}//DrawIndexedPrimitive()

void DrawIndexedPrimitive( Canvas& canvas
    , int* m_indexBuffer            // index buffer
    , int primitiveCounter          // primitive counter
    , KVertex* m_vertexBuffer      // vertex buffer
    , KVector3* m_normalBuffer      // normal buffer
    , COLORREF color_
    , Bitmap& bitmap ) // texture buffer
{
    int   i0, i1, i2;
    int   counter = 0;

    KRgb color;
    color[0] = GetRValue( color_ );
    color[1] = GetGValue( color_ );
    color[2] = GetBValue( color_ );

    KRgb uv[3];
    for(int i = 0; i < primitiveCounter; ++i)
    {
        // get index
        i0 = m_indexBuffer[counter];
        i1 = m_indexBuffer[counter + 1];
        i2 = m_indexBuffer[counter + 2];

        KVector3 normal;
        normal = Cross( m_vertexBuffer[i0].pos - m_vertexBuffer[i1].pos, m_vertexBuffer[i0].pos - m_vertexBuffer[i2].pos );
        normal.Normalize( );
        KVector3 forward( 0, 0, -1 );
        float dot = Dot( forward, normal );
        float shade = Dot( forward, m_normalBuffer[i] );
        const bool isVisible = ( dot > 0 );

        color[0] = GetRValue( color_ ) * shade;
        color[1] = GetGValue( color_ ) * shade;
        color[2] = GetBValue( color_ ) * shade;

        uv[0][0] = m_vertexBuffer[i0].u * 255.0f;
        uv[0][1] = m_vertexBuffer[i0].v * 255.0f;
        uv[0][2] = 0.0f;
        uv[1][0] = m_vertexBuffer[i1].u * 255.0f;
        uv[1][1] = m_vertexBuffer[i1].v * 255.0f;
        uv[1][2] = 0.0f;
        uv[2][0] = m_vertexBuffer[i2].u * 255.0f;
        uv[2][1] = m_vertexBuffer[i2].v * 255.0f;
        uv[2][2] = 0.0f;

        // draw triangle
        if(isVisible == true)
        {
            canvas.FillTriangle( m_vertexBuffer[i0].pos.x, m_vertexBuffer[i0].pos.y, uv[0]
                , m_vertexBuffer[i1].pos.x, m_vertexBuffer[i1].pos.y, uv[1]
                , m_vertexBuffer[i2].pos.x, m_vertexBuffer[i2].pos.y, uv[2], bitmap, color );
            canvas.ScanLineSegment( m_vertexBuffer[i0].pos.x, m_vertexBuffer[i0].pos.y, KRgb( 0, 0, 0 )
                , m_vertexBuffer[i1].pos.x, m_vertexBuffer[i1].pos.y, KRgb( 0, 0, 0 ), nullptr );
            canvas.ScanLineSegment( m_vertexBuffer[i1].pos.x, m_vertexBuffer[i1].pos.y, KRgb( 0, 0, 0 )
                , m_vertexBuffer[i2].pos.x, m_vertexBuffer[i2].pos.y, KRgb( 0, 0, 0 ), nullptr );
            canvas.ScanLineSegment( m_vertexBuffer[i2].pos.x, m_vertexBuffer[i2].pos.y, KRgb( 0, 0, 0 )
                , m_vertexBuffer[i0].pos.x, m_vertexBuffer[i0].pos.y, KRgb( 0, 0, 0 ), nullptr );
        }
        // advance to next primitive
        counter += 3;
    }//for
}//DrawIndexedPrimitive()

KPolygon::KPolygon()
{
    m_sizeIndex  = 0;
    m_sizeVertex = 0;
    m_color = RGB(255, 255, 255);
    m_primitiveCount = 0;
}//KPolygon::KPolygon()

KPolygon::~KPolygon()
{
}//KPolygon::~KPolygon()

void KPolygon::SetIndexBuffer()
{
    int buffer[] = {
        0 + 0, 3 + 0, 1 + 0,
        1 + 0, 3 + 0, 2 + 0,
        0 + 4, 1 + 4, 3 + 4,
        1 + 4, 2 + 4, 3 + 4,

        0 + 8, 3 + 8, 1 + 8,
        1 + 8, 3 + 8, 2 + 8,
        0 + 12, 1 + 12, 3 + 12,
        1 + 12, 2 + 12, 3 + 12,

        0 + 16, 3 + 16, 1 + 16,
        1 + 16, 3 + 16, 2 + 16,
        0 + 20, 1 + 20, 3 + 20,
        1 + 20, 2 + 20, 3 + 20,
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
    m_vertexBuffer[0] = KVertex{ KVector3( -40.f, +40.f, -40.f ), 0.0f, 1.0f };
    m_vertexBuffer[1] = KVertex{ KVector3( -40.f, -40.f, -40.f ), 0.0f, 0.0f };
    m_vertexBuffer[2] = KVertex{ KVector3( +40.f, -40.f, -40.f ), 1.0f, 0.0f };
    m_vertexBuffer[3] = KVertex{ KVector3( +40.f, +40.f, -40.f ), 1.0f, 1.0f };
    m_vertexBuffer[4] = KVertex{ KVector3( -40.f, +40.f, +40.f ), 0.0f, 1.0f };
    m_vertexBuffer[5] = KVertex{ KVector3( -40.f, -40.f, +40.f ), 0.0f, 0.0f };
    m_vertexBuffer[6] = KVertex{ KVector3( +40.f, -40.f, +40.f ), 1.0f, 0.0f };
    m_vertexBuffer[7] = KVertex{ KVector3( +40.f, +40.f, +40.f ), 1.0f, 1.0f };

    m_vertexBuffer[8] = KVertex{ KVector3( -40.f, -40.f, +40.f ), 0.0f, 1.0f };
    m_vertexBuffer[9] = KVertex{ KVector3( +40.f, -40.f, +40.f ), 0.0f, 0.0f };
    m_vertexBuffer[10] = KVertex{ KVector3( +40.f, -40.f, -40.f ), 1.0f, 0.0f };
    m_vertexBuffer[11] = KVertex{ KVector3( -40.f, -40.f, -40.f ), 1.0f, 1.0f };
    m_vertexBuffer[12] = KVertex{ KVector3( -40.f, +40.f, +40.f ), 0.0f, 1.0f };
    m_vertexBuffer[13] = KVertex{ KVector3( +40.f, +40.f, +40.f ), 0.0f, 0.0f };
    m_vertexBuffer[14] = KVertex{ KVector3( +40.f, +40.f, -40.f ), 1.0f, 0.0f };
    m_vertexBuffer[15] = KVertex{ KVector3( -40.f, +40.f, -40.f ), 1.0f, 1.0f };

    m_vertexBuffer[16] = KVertex{ KVector3( -40.f, +40.f, +40.f ), 0.0f, 1.0f };
    m_vertexBuffer[17] = KVertex{ KVector3( -40.f, -40.f, +40.f ), 0.0f, 0.0f };
    m_vertexBuffer[18] = KVertex{ KVector3( -40.f, -40.f, -40.f ), 1.0f, 0.0f };
    m_vertexBuffer[19] = KVertex{ KVector3( -40.f, +40.f, -40.f ), 1.0f, 1.0f };
    m_vertexBuffer[20] = KVertex{ KVector3( +40.f, +40.f, +40.f ), 0.0f, 1.0f };
    m_vertexBuffer[21] = KVertex{ KVector3( +40.f, -40.f, +40.f ), 0.0f, 0.0f };
    m_vertexBuffer[22] = KVertex{ KVector3( +40.f, -40.f, -40.f ), 1.0f, 0.0f };
    m_vertexBuffer[23] = KVertex{ KVector3( +40.f, +40.f, -40.f ), 1.0f, 1.0f };

    m_sizeVertex = 24;
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
        m_vertexBuffer[i].pos = mat * m_vertexBuffer[i].pos;
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
        normal = Cross( m_vertexBuffer[i0].pos - m_vertexBuffer[i1].pos, m_vertexBuffer[i0].pos - m_vertexBuffer[i2].pos );
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
}

void KPolygon::DrawOnCanvas( Canvas& canvas, Bitmap& bitmap )
{
    ::DrawIndexedPrimitive(
        canvas,
        m_indexBuffer,      // index buffer
        12,                 // primitive(triangle) counter
        m_vertexBuffer,     // vertex buffer
        m_normalBuffer,     // normal buffer
        m_color,
        bitmap );
}
