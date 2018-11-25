#pragma once

#include "KVector3.h"
#include "KMatrix4.h"
#include "KRgb.h"
#include <objidl.h>
#include <gdiplus.h>

using namespace Gdiplus;

struct KVertex
{
    KVector3            pos;
    float               u;
    float               v;
};

class KPolygon
{
private:
    int         m_indexBuffer[100];
    int         m_sizeIndex;
    KVertex     m_vertexBuffer[100];
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
    void DrawOnCanvas( HDC hdc, Bitmap& bitmap );
};//class KPolygon
