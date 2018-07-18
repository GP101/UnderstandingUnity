#pragma once
#include "KVector3.h"
#include "KMatrix4.h"

class KPolygon
{
private:
    int         m_indexBuffer[100];
    int         m_sizeIndex;
    KVector3    m_vertexBuffer[100];
    int         m_sizeVertex;
    COLORREF    m_color;

public:
    KPolygon();
	~KPolygon();

    void SetIndexBuffer();
    void SetVertexBuffer();
    void Render(HDC hdc);
	void SetColor(COLORREF color) { m_color = color; }

    void Transform(KMatrix4& mat);
};//class KPolygon
