#include "KPolygonShape.h"
#include "KMath.h"

extern HDC     g_hdc;


void KPolygonShape::Initialize(void)
{
	ComputeMass(1.0f);
}

bool KPolygonShape::IsValid() const
{
	return body != nullptr;
}

KShape* KPolygonShape::Clone(void) const
{
	KPolygonShape *poly = new KPolygonShape();
	poly->u = u;
	for (uint32 i = 0; i < m_vertexCount; ++i)
	{
		poly->m_vertices[i] = m_vertices[i];
		poly->m_normals[i] = m_normals[i];
	}
	poly->m_vertexCount = m_vertexCount;
	return poly;
}

void KPolygonShape::ComputeMass(float density)
{
	// Calculate centroid and moment of inertia
	KVector2 c(0.0f, 0.0f); // centroid
	float area = 0.0f;
	float I = 0.0f;
	const float k_inv3 = 1.0f / 3.0f;

	for (uint32 i1 = 0; i1 < m_vertexCount; ++i1)
	{
		// Triangle vertices, third vertex implied as (0, 0)
		KVector2 p1(m_vertices[i1]);
		uint32 i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
		KVector2 p2(m_vertices[i2]);

		float D = KVector2::Cross(p1, p2);
		float triangleArea = 0.5f * D;

		area += triangleArea;

		// Use area to weight the centroid average, not just vertex position
		c += triangleArea * k_inv3 * (p1 + p2);

		float intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
		float inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
		I += (0.25f * k_inv3 * D) * (intx2 + inty2);
	}

	c *= 1.0f / area;

	// Translate vertices to centroid (make the centroid (0, 0)
	// for the polygon in model space)
	// Not really necessary, but I like doing this anyway
	for (uint32 i = 0; i < m_vertexCount; ++i)
		m_vertices[i] -= c;

	body->m_mass = density * area;
	body->m_invMass = (body->m_mass) ? 1.0f / body->m_mass : 0.0f;
	body->m_I = I * density;
	body->m_invI = body->m_I ? 1.0f / body->m_I : 0.0f;
}

void KPolygonShape::SetRotation(float radians)
{
	u.Set(radians);
}

void KPolygonShape::Draw(void) const
{
	COLORREF color;
	if (body == nullptr)
		color = RGB(0, 0, 0);
	else
		color = RGB(r*255.f, g * 255.f, b*255.f);
	std::vector<KVector2> points;
	for (uint32 i = 0; i < m_vertexCount; ++i)
	{
		KVector2 v = body->position + u * m_vertices[i];
		points.push_back(KVector2(v.x, v.y));
	}
	KVectorUtil::DrawLine(g_hdc, points, 2, 0, color, 1); // 1: line loop
}

KShape::Type KPolygonShape::GetType() const
{
	return ePoly;
}

// Half width and half height
void KPolygonShape::SetBox(float hw, float hh)
{
	m_vertexCount = 4;
	m_vertices[0].Set(-hw, -hh);
	m_vertices[1].Set(hw, -hh);
	m_vertices[2].Set(hw, hh);
	m_vertices[3].Set(-hw, hh);
	m_normals[0].Set(0.0f, -1.0f);
	m_normals[1].Set(1.0f, 0.0f);
	m_normals[2].Set(0.0f, 1.0f);
	m_normals[3].Set(-1.0f, 0.0f);
}

void KPolygonShape::Set(KVector2 *vertices, uint32 count)
{
	// No hulls with less than 3 vertices (ensure actual polygon)
	assert(count > 2 && count <= MaxPolyVertexCount);
	count = __min((int32)count, MaxPolyVertexCount);

	// Find the right most point on the hull
	int32 rightMost = 0;
	float highestXCoord = vertices[0].x;
	for (uint32 i = 1; i < count; ++i)
	{
		float x = vertices[i].x;
		if (x > highestXCoord)
		{
			highestXCoord = x;
			rightMost = i;
		}

		// If matching x then take farthest negative y
		else if (x == highestXCoord)
			if (vertices[i].y < vertices[rightMost].y)
				rightMost = i;
	}

	int32 hull[MaxPolyVertexCount];
	int32 outCount = 0;
	int32 indexHull = rightMost;

	for (;;)
	{
		hull[outCount] = indexHull;

		// Search for next index that wraps around the hull
		// by computing cross products to find the most counter-clockwise
		// vertex in the set, given the previos hull index
		int32 nextHullIndex = 0;
		for (int32 i = 1; i < (int32)count; ++i)
		{
			// Skip if same coordinate as we need three unique
			// points in the set to perform a cross product
			if (nextHullIndex == indexHull)
			{
				nextHullIndex = i;
				continue;
			}

			// Cross every set of three unique vertices
			// Record each counter clockwise third vertex and add
			// to the output hull
			// See : http://www.oocities.org/pcgpe/math2d.html
			KVector2 e1 = vertices[nextHullIndex] - vertices[hull[outCount]];
			KVector2 e2 = vertices[i] - vertices[hull[outCount]];
			float c = KVector2::Cross(e1, e2);
			if (c < 0.0f)
				nextHullIndex = i;

			// Cross product is zero then e vectors are on same line
			// therefor want to record vertex farthest along that line
			if (c == 0.0f && e2.LengthSquared() > e1.LengthSquared())
				nextHullIndex = i;
		}

		++outCount;
		indexHull = nextHullIndex;

		// Conclude algorithm upon wrap-around
		if (nextHullIndex == rightMost)
		{
			m_vertexCount = outCount;
			break;
		}
	}

	// Copy vertices into shape's vertices
	for (uint32 i = 0; i < m_vertexCount; ++i)
		m_vertices[i] = vertices[hull[i]];

	// Compute face normals
	for (uint32 i1 = 0; i1 < m_vertexCount; ++i1)
	{
		uint32 i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
		KVector2 face = m_vertices[i2] - m_vertices[i1];

		// Ensure no zero-length edges, because that's bad
		assert(face.LengthSquared() > EPSILON * EPSILON);

		// Calculate normal with 2D cross product between vector and scalar
		m_normals[i1] = KVector2(face.y, -face.x);
		m_normals[i1].Normalize();
	}
}

// The extreme point along a direction within a polygon
KVector2 KPolygonShape::GetSupportPoint(const KVector2& dir)
{
	float bestProjection = -FLT_MAX;
	KVector2 bestVertex;

	for (uint32 i = 0; i < m_vertexCount; ++i)
	{
		KVector2 v = m_vertices[i];
		float projection = KVector2::Dot(v, dir);

		if (projection > bestProjection)
		{
			bestVertex = v;
			bestProjection = projection;
		}
	}

	return bestVertex;
}
