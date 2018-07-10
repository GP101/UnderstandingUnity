#pragma once

#include "KVector3.h"
#include "KMatrix4.h"
#include "Canvas.h"
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
    COLORREF    m_color;
    int         m_primitiveCount;

public:
    KPolygon();
	~KPolygon();

    void SetIndexBuffer();
    void SetVertexBuffer();
    void Render(HDC hdc);
	void SetColor(COLORREF color) { m_color = color; }

    //void RotateX(float theta);
    //void RotateY(float theta);
    //void Translate(float tx, float ty, float tz);

    void Transform(KMatrix4& mat);

    void DrawOnCanvas( Canvas& canvas );
    void DrawOnCanvas( Canvas& canvas, Bitmap& bitmap );
};//class KPolygon
