#pragma once
#include "KVector3.h"

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

    //{{ seojt: step2
    void RotateX(float theta);
    void RotateY(float theta);
    void Translate(float tx, float ty, float tz);
    //}} seojt: step2
};//class KPolygon
