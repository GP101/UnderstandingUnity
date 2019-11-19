#include "stdafx.h"
#include "KPrimitive.h"
#include "KVectorUtil.h"
#include "KRgb.h"
#include <algorithm>
#include "KRenderState.h"

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
        normal = Cross( m_vertexBuffer[i0] - m_vertexBuffer[i1], m_vertexBuffer[i0] - m_vertexBuffer[i2] );
        KVector3 forward( 0, 0, 1 );
        int penStyle = PS_SOLID;
        int penWidth = 3;
        if (Dot( forward, normal ) > 0)
        {
            penStyle = PS_DOT;
            penWidth = 1;
        }

        // draw triangle
        KVectorUtil::DrawLine( hdc, m_vertexBuffer[i0].x, m_vertexBuffer[i0].y
            , m_vertexBuffer[i1].x, m_vertexBuffer[i1].y, penWidth, penStyle, color );
        KVectorUtil::DrawLine( hdc, m_vertexBuffer[i1].x, m_vertexBuffer[i1].y
            , m_vertexBuffer[i2].x, m_vertexBuffer[i2].y, penWidth, penStyle, color );
        KVectorUtil::DrawLine( hdc, m_vertexBuffer[i2].x, m_vertexBuffer[i2].y
            , m_vertexBuffer[i0].x, m_vertexBuffer[i0].y, penWidth, penStyle, color );

        // advance to next primitive
        counter += 3;
    }//for
}//DrawIndexedPrimitive()

void ScanDrawIndexedPrimitive( HDC hdc
    , int* m_indexBuffer            // index buffer
    , int primitiveCounter          // primitive counter
    , KVector3* m_vertexBuffer      // vertex buffer
    , KRgb color_ )
{
    int   i0, i1, i2;
    int   counter = 0;

    KLight light = KRenderState::GetInstance().GetLight();
    KMaterial mtrl = KRenderState::GetInstance().GetMaterial();

    for (int i = 0; i < primitiveCounter; ++i)
    {
        // get index
        i0 = m_indexBuffer[counter];
        i1 = m_indexBuffer[counter + 1];
        i2 = m_indexBuffer[counter + 2];

        KVector3 normal;
        normal = Cross( m_vertexBuffer[i0] - m_vertexBuffer[i1], m_vertexBuffer[i0] - m_vertexBuffer[i2] );
        normal = normal.Normalize();
        KVector3 forward( 0, 0, 1 );
        double dot = Dot( forward, normal );
        double shade = Dot(light.m_dir, normal);
        const bool isVisible = (dot < 0);

        KRgb color = light.m_ambient + light.m_diffuse * mtrl.m_diffuse * shade + light.m_specular * mtrl.m_specular;
        color[0] = __min(1.0f, color[0]);
        color[1] = __min(1.0f, color[1]);
        color[2] = __min(1.0f, color[2]);

        // draw triangle
        if (isVisible == true)
        {
            KVectorUtil::FillTriangle( hdc, m_vertexBuffer[i0].x, m_vertexBuffer[i0].y, color
                , m_vertexBuffer[i1].x, m_vertexBuffer[i1].y, color
                , m_vertexBuffer[i2].x, m_vertexBuffer[i2].y, color );
            KVectorUtil::ScanLineSegment( hdc, m_vertexBuffer[i0].x, m_vertexBuffer[i0].y, KRgb( 0, 0, 0 )
                , m_vertexBuffer[i1].x, m_vertexBuffer[i1].y, KRgb( 0, 0, 0 ), nullptr );
            KVectorUtil::ScanLineSegment( hdc, m_vertexBuffer[i1].x, m_vertexBuffer[i1].y, KRgb( 0, 0, 0 )
                , m_vertexBuffer[i2].x, m_vertexBuffer[i2].y, KRgb( 0, 0, 0 ), nullptr );
            KVectorUtil::ScanLineSegment( hdc, m_vertexBuffer[i2].x, m_vertexBuffer[i2].y, KRgb( 0, 0, 0 )
                , m_vertexBuffer[i0].x, m_vertexBuffer[i0].y, KRgb( 0, 0, 0 ), nullptr );
        }
        // advance to next primitive
        counter += 3;
    }//for
}//ScanDrawIndexedPrimitive()

void DrawPrimitive( HDC hdc
    , KVector3 vBegin
    , KVector3 vEnd
    , COLORREF color )
{
    const int penStyle = PS_SOLID;
    const int penWidth = 3;
    Gdiplus::Color c = color;
    //KVectorUtil::ScanLine( hdc, (int)vBegin.x, (int)vBegin.y, (int)vEnd.x, (int)vEnd.y, c );
    KVectorUtil::DrawLine( hdc, (int)vBegin.x, (int)vBegin.y, (int)vEnd.x, (int)vEnd.y, 1, PS_SOLID, color );
}//DrawPrimitive()

//////////////////////////////////////////////////////////////////////////
// KPrimitive members
//
KPrimitive::KPrimitive()
{
    m_sizeIndex = 0;
    m_sizeVertex = 0;
    m_color = RGB( 0, 0, 0 );
}

KPrimitive::~KPrimitive()
{
}

void KPrimitive::Render( HDC hdc )
{
    ::DrawIndexedPrimitive(
        hdc,
        m_indexBuffer,      // index buffer
        12,                 // primitive(triangle) counter
        m_vertexBuffer,     // vertex buffer
        m_color );
}

void KPrimitive::Transform( KMatrix4& mat )
{
    for (int i = 0; i < m_sizeVertex; ++i)
    {
        m_vertexBuffer[i] = mat * m_vertexBuffer[i];
    }//for
}


void KPrimitive::Rotate( const KQuaternion& q )
{
    const KQuaternion qConjugate = q.conjugate();
    for (int i = 0; i < m_sizeVertex; ++i)
    {
        KQuaternion qPoint = KQuaternion( 0, m_vertexBuffer[i].x, m_vertexBuffer[i].y, m_vertexBuffer[i].z );
        KQuaternion qResult = q * qPoint * qConjugate;
        m_vertexBuffer[i] = KVector3( qResult.x, qResult.y, qResult.z );
    }//for
}
