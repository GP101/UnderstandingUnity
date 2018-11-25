#pragma once

#include "KVector3.h"
#include "KMatrix4.h"
#include "KRgb.h"

class KPolygon
{
private:
    int         m_indexBuffer[100];
    int         m_sizeIndex;
    KVector3    m_vertexBuffer[100];
    int         m_sizeVertex;
    KRgb        m_color;
    int         m_primitiveCount;

public:
    KPolygon();
	~KPolygon();

    void SetIndexBuffer();
    void SetVertexBuffer();
    void Render(HDC hdc);
	void SetColor(KRgb color) { m_color = color; }
    void Transform(KMatrix4& mat);
    void DrawOnCanvas( HDC hdc );
};//class KPolygon
