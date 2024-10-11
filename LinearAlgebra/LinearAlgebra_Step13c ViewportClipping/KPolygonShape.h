#ifndef _KPOLYGONSHAPE_H_
#define _KPOLYGONSHAPE_H_

#include "KShape.h"
#include "KMath.h"
#include "KVectorUtil.h"
#include "KMatrix3.h"

struct KPolygonShape : public KShape
{
	void Initialize();
	bool IsValid() const;
	void SetRotation(double radians);
	KShape::Type GetType() const;
	void SetBox(double halfWidth, double halfHeight);
	void Set(KVector2* vertices, uint32 count);
	void FindConvexHull(KVector2 points[], int n, std::vector<KVector2>& convexHullPoints);
	void Transform(const KMatrix3& transform);

	std::vector<KVector2> m_vertices;
	std::vector<KVector2> m_normals;
};

#endif // _KPOLYGONSHAPE_H_
