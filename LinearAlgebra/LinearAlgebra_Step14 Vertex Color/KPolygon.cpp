#include "stdafx.h"
#include "KPolygon.h"
#include "KVectorUtil.h"
#include "KRenderState.h"

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

void ScanDrawIndexedPrimitive( HDC hdc
    , int* m_indexBuffer            // index buffer
    , int primitiveCounter          // primitive counter
    , KVertex* m_vertexBuffer      // vertex buffer
    , KRgb color_ )
{
    int   i0, i1, i2;
    int   counter = 0;

    KLight light = KRenderState::GetInstance().GetLight();
    KMaterial mtrl = KRenderState::GetInstance().GetMaterial();

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
        float shade = Dot(light.m_dir, normal);
        const bool isVisible = (dot > 0);

        KRgb color[3];
        color[0] = light.m_ambient + light.m_diffuse * mtrl.m_diffuse * m_vertexBuffer[i0].color * shade
            + light.m_specular * mtrl.m_specular;
        color[0][0] = __min(1.0f, color[0][0]);
        color[0][1] = __min(1.0f, color[0][1]);
        color[0][2] = __min(1.0f, color[0][2]);
        color[1] = light.m_ambient + light.m_diffuse * mtrl.m_diffuse * m_vertexBuffer[i1].color * shade
            + light.m_specular * mtrl.m_specular;
        color[1][0] = __min(1.0f, color[1][0]);
        color[1][1] = __min(1.0f, color[1][1]);
        color[1][2] = __min(1.0f, color[1][2]);
        color[2] = light.m_ambient + light.m_diffuse * mtrl.m_diffuse * m_vertexBuffer[i2].color * shade
            + light.m_specular * mtrl.m_specular;
        color[2][0] = __min(1.0f, color[2][0]);
        color[2][1] = __min(1.0f, color[2][1]);
        color[2][2] = __min(1.0f, color[2][2]);

        // draw triangle
        if(isVisible == true)
        {
            KVectorUtil::FillTriangle(hdc, m_vertexBuffer[i0].pos.x, m_vertexBuffer[i0].pos.y, color[0]
                , m_vertexBuffer[i1].pos.x, m_vertexBuffer[i1].pos.y, color[1]
                , m_vertexBuffer[i2].pos.x, m_vertexBuffer[i2].pos.y, color[2] );
            KVectorUtil::ScanLineSegment( hdc, m_vertexBuffer[i0].pos.x, m_vertexBuffer[i0].pos.y, KRgb(0, 0, 0)
                , m_vertexBuffer[i1].pos.x, m_vertexBuffer[i1].pos.y, KRgb( 0, 0, 0 ), nullptr );
            KVectorUtil::ScanLineSegment( hdc, m_vertexBuffer[i1].pos.x, m_vertexBuffer[i1].pos.y, KRgb(0, 0, 0)
                , m_vertexBuffer[i2].pos.x, m_vertexBuffer[i2].pos.y, KRgb( 0, 0, 0 ), nullptr );
            KVectorUtil::ScanLineSegment( hdc, m_vertexBuffer[i2].pos.x, m_vertexBuffer[i2].pos.y, KRgb(0, 0, 0)
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
    m_color = KRgb(0, 0, 1);
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
    m_vertexBuffer[0]  = KVertex{ KVector3(-30.f, -30.f, +30.f), KRgb(1, 0, 0) };
    m_vertexBuffer[1]  = KVertex{ KVector3(-30.f, +30.f, +30.f), KRgb(0, 1, 0) };
    m_vertexBuffer[2]  = KVertex{ KVector3(+30.f, +30.f, +30.f), KRgb(1, 0, 0) };
    m_vertexBuffer[3]  = KVertex{ KVector3(+30.f, -30.f, +30.f), KRgb(0, 0, 1) };
    m_vertexBuffer[4]  = KVertex{ KVector3(-30.f, -30.f, -30.f), KRgb(1, 0, 0) };
    m_vertexBuffer[5]  = KVertex{ KVector3(-30.f, +30.f, -30.f), KRgb(0, 1, 0) };
    m_vertexBuffer[6]  = KVertex{ KVector3(+30.f, +30.f, -30.f), KRgb(1, 0, 0) };
    m_vertexBuffer[7]  = KVertex{ KVector3(+30.f, -30.f, -30.f), KRgb(0, 0, 1) };

    m_vertexBuffer[8]  = KVertex{ KVector3(-30.f, +30.f, -30.f), KRgb(0, 0, 1) };
    m_vertexBuffer[9]  = KVertex{ KVector3(+30.f, +30.f, -30.f), KRgb(0, 0, 1) };
    m_vertexBuffer[10] = KVertex{ KVector3(+30.f, +30.f, +30.f), KRgb(0, 0, 1) };
    m_vertexBuffer[11] = KVertex{ KVector3(-30.f, +30.f, +30.f), KRgb(0, 0, 1) };
    m_vertexBuffer[12] = KVertex{ KVector3(-30.f, -30.f, -30.f), KRgb(0, 0, 1) };
    m_vertexBuffer[13] = KVertex{ KVector3(+30.f, -30.f, -30.f), KRgb(0, 0, 1) };
    m_vertexBuffer[14] = KVertex{ KVector3(+30.f, -30.f, +30.f), KRgb(0, 0, 1) };
    m_vertexBuffer[15] = KVertex{ KVector3(-30.f, -30.f, +30.f), KRgb(0, 0, 1) };

    m_vertexBuffer[16] = KVertex{ KVector3(-30.f, -30.f, -30.f), KRgb(1, 0, 0) };
    m_vertexBuffer[17] = KVertex{ KVector3(-30.f, +30.f, -30.f), KRgb(1, 0, 0) };
    m_vertexBuffer[18] = KVertex{ KVector3(-30.f, +30.f, +30.f), KRgb(1, 0, 0) };
    m_vertexBuffer[19] = KVertex{ KVector3(-30.f, -30.f, +30.f), KRgb(1, 0, 0) };
    m_vertexBuffer[20] = KVertex{ KVector3(+30.f, -30.f, -30.f), KRgb(1, 0, 0) };
    m_vertexBuffer[21] = KVertex{ KVector3(+30.f, +30.f, -30.f), KRgb(1, 0, 0) };
    m_vertexBuffer[22] = KVertex{ KVector3(+30.f, +30.f, +30.f), KRgb(1, 0, 0) };
    m_vertexBuffer[23] = KVertex{ KVector3(+30.f, -30.f, +30.f), KRgb(1, 0, 0) };

    m_sizeVertex = 24;
}//KPolygon::SetVertexBuffer()

void KPolygon::Render(HDC hdc)
{
    ::DrawIndexedPrimitive(
        hdc,
        m_indexBuffer,      // index buffer
        12,                   // primitive(triangle) counter
        m_vertexBuffer,     // vertex buffer
        m_color.GetGdiColor().ToCOLORREF() );
}//KPolygon::Render()

void KPolygon::Transform(KMatrix4& mat)
{
    for (int i=0; i<m_sizeVertex; ++i)
    {
        m_vertexBuffer[i].pos = mat * m_vertexBuffer[i].pos;
    }//for
}//KPolygon::Transform()

void KPolygon::DrawOnCanvas( HDC hdc )
{
    KMaterial mtrl;
    mtrl.m_diffuse = KRgb(1, 1, 1); //m_color;
    mtrl.m_specular = KRgb(0, 0, 0);
    KRenderState::GetInstance().SetMaterial( mtrl );

    ::ScanDrawIndexedPrimitive( 
        hdc,
        m_indexBuffer,      // index buffer
        12,                 // primitive(triangle) counter
        m_vertexBuffer,     // vertex buffer
        m_color );
}
