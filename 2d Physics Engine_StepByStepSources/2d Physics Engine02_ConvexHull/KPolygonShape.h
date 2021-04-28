#ifndef _KPOLYGONSHAPE_H_
#define _KPOLYGONSHAPE_H_

#include "KShape.h"
#include "KRigidbody.h"
#include "KMath.h"
#include "KVectorUtil.h"

#define MaxPolyVertexCount 64


struct KPolygonShape : public KShape
{
	void Initialize();
	bool IsValid() const;
	KShape *Clone() const;
	void ComputeMass(float density);
	void SetRotation(float radians);
	void Draw() const;
	KShape::Type GetType() const;
	// Half width and half height
	void SetBox(float hw, float hh);
	void Set(KVector2 *vertices, uint32 count);
	// The extreme point along a direction within a polygon
	KVector2 GetSupportPoint(const KVector2& dir);

	uint32 m_vertexCount = 0;
	KVector2 m_vertices[MaxPolyVertexCount];
	KVector2 m_normals[MaxPolyVertexCount];
};

#endif // _KPOLYGONSHAPE_H_
