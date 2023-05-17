#ifndef _KPOLYGONSHAPE_H_
#define _KPOLYGONSHAPE_H_

#include "KShape.h"
#include "KMath.h"
#include "KVectorUtil.h"

struct KPolygonShape : public KShape
{
	void Initialize();
	bool IsValid() const;
	void SetRotation(float radians);
	KShape::Type GetType() const;
	void SetBox(float halfWidth, float halfHeight);
	void Set(KVector2* vertices, uint32 count);
	void FindConvexHull(KVector2 points[], int n, std::vector<KVector2>& convexHullPoints);

	std::vector<KVector2> m_vertices;
	std::vector<KVector2> m_normals;
};

#endif // _KPOLYGONSHAPE_H_
