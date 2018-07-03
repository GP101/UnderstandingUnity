#if !defined(AFX_POLYGON_H__ED0A4978_9BC3_11D5_82EE_008048E10474__INCLUDED_)
#define AFX_POLYGON_H__ED0A4978_9BC3_11D5_82EE_008048E10474__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vector.h"

class CPolygon
{
public:
	CPolygon();
	virtual ~CPolygon();

    int     m_indexBuffer[100];
    int     m_sizeIndex;
    CVector m_vertexBuffer[100];
    int     m_sizeVertex;

    void SetIndexBuffer();
    void SetVertexBuffer();
    void Render(HDC hdc);
};//class CPolygon

#endif // !defined(AFX_POLYGON_H__ED0A4978_9BC3_11D5_82EE_008048E10474__INCLUDED_)
