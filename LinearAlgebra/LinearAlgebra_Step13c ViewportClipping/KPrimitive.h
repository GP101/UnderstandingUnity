#pragma once
#include <windows.h>
#include "KVector3.h"
#include "KMatrix4.h"
#include "KQuaternion.h"
#include "KRgb.h"

void DrawIndexedPrimitive( HDC hdc
    , int* m_indexBuffer            // index buffer
    , int primitiveCounter          // primitive counter
    , KVector3* m_vertexBuffer      // vertex buffer
    , COLORREF color );

void ScanDrawIndexedPrimitive( HDC hdc
    , int* m_indexBuffer            // index buffer
    , int primitiveCounter          // primitive counter
    , KVector3* m_vertexBuffer      // vertex buffer
    , KRgb color_ );

void DrawPrimitive( HDC hdc
    , KVector3 vBegin
    , KVector3 vEnd
    , COLORREF color );

class KPrimitive
{
protected:
    int         m_indexBuffer[100];
    int         m_sizeIndex;
    KVector3    m_vertexBuffer[100];
    int         m_sizeVertex;
    COLORREF    m_color;

public:
    KPrimitive();
    virtual ~KPrimitive();

    void Render( HDC hdc );
    void SetColor( COLORREF color ) { m_color = color; }
    void Transform( KMatrix4& mat );
    void Rotate( const KQuaternion& q );
};
