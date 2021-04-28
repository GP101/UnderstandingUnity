/*
	Copyright (c) 2013 Randy Gaul http://RandyGaul.net

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:
	  1. The origin of this software must not be misrepresented; you must not
		 claim that you wrote the original software. If you use this software
		 in a product, an acknowledgment in the product documentation would be
		 appreciated but is not required.
	  2. Altered source versions must be plainly marked as such, and must not be
		 misrepresented as being the original software.
	  3. This notice may not be removed or altered from any source distribution.
*/

#include "KPhysicsEngine.h"
#include "KMath.h"
#include "KCircleShape.h"
#include "KPolygonShape.h"


CollisionCallback g_collLookup[KShape::eCount][KShape::eCount] =
{
  { CircletoCircle, CircletoPolygon },
  { PolygontoCircle, PolygontoPolygon },
};


void CircletoCircle(KManifold *m, KRigidbody *a, KRigidbody *b)
{
	KCircleShape *A = reinterpret_cast<KCircleShape *>(a->shape);
	KCircleShape *B = reinterpret_cast<KCircleShape *>(b->shape);

	// Calculate translational vector, which is normal
	KVector2 normal = b->position - a->position;

	float dist_sqr = normal.LengthSquared();
	float radius = A->radius + B->radius;

	// Not in contact
	if (dist_sqr >= radius * radius)
	{
		m->contact_count = 0;
		return;
	}

	float distance = std::sqrt(dist_sqr);

	m->contact_count = 1;

	if (distance == 0.0f)
	{
		m->penetration = A->radius;
		m->normal = KVector2(1, 0);
		m->contacts[0] = a->position;
	}
	else
	{
		m->penetration = radius - distance;
		m->normal = normal / distance; // Faster than using Normalized since we already performed sqrt
		m->contacts[0] = m->normal * A->radius + a->position;
	}
}

void CircletoPolygon(KManifold *m, KRigidbody *a, KRigidbody *b)
{
	KCircleShape *A = reinterpret_cast<KCircleShape *>      (a->shape);
	KPolygonShape *B = reinterpret_cast<KPolygonShape *>(b->shape);

	m->contact_count = 0;

	// Transform circle center to Polygon model space
	KVector2 center = a->position;
	center = B->u.Transpose() * (center - b->position);

	// Find edge with minimum penetration
	// Exact concept as using support points in Polygon vs Polygon
	float separation = -FLT_MAX;
	unsigned int faceNormal = 0;
	for (uint32 i = 0; i < B->m_vertexCount; ++i)
	{
		float s = KVector2::Dot(B->m_normals[i], center - B->m_vertices[i]);

		if (s > A->radius)
			return;

		if (s > separation)
		{
			separation = s;
			faceNormal = i;
		}
	}

	// Grab face's vertices
	KVector2 v1 = B->m_vertices[faceNormal];
	uint32 i2 = faceNormal + 1 < B->m_vertexCount ? faceNormal + 1 : 0;
	KVector2 v2 = B->m_vertices[i2];

	// Check to see if center is within polygon
	if (separation < EPSILON)
	{
		m->contact_count = 1;
		m->normal = -(B->u * B->m_normals[faceNormal]);
		m->contacts[0] = m->normal * A->radius + a->position;
		m->penetration = A->radius;
		return;
	}

	// Determine which voronoi region of the edge center of circle lies within
	float dot1 = KVector2::Dot(center - v1, v2 - v1);
	float dot2 = KVector2::Dot(center - v2, v1 - v2);
	m->penetration = A->radius - separation;

	// Closest to v1
	if (dot1 <= 0.0f)
	{
		if (KVector2::DistSquared(center, v1) > A->radius * A->radius)
			return;

		m->contact_count = 1;
		KVector2 n = v1 - center;
		n = B->u * n;
		n.Normalize();
		m->normal = n;
		v1 = B->u * v1 + b->position;
		m->contacts[0] = v1;
	}

	// Closest to v2
	else if (dot2 <= 0.0f)
	{
		if (KVector2::DistSquared(center, v2) > A->radius * A->radius)
			return;

		m->contact_count = 1;
		KVector2 n = v2 - center;
		v2 = B->u * v2 + b->position;
		m->contacts[0] = v2;
		n = B->u * n;
		n.Normalize();
		m->normal = n;
	}

	// Closest to face
	else
	{
		KVector2 n = B->m_normals[faceNormal];
		if (KVector2::Dot(center - v1, n) > A->radius)
			return;

		n = B->u * n;
		m->normal = -n;
		m->contacts[0] = m->normal * A->radius + a->position;
		m->contact_count = 1;
	}
}

void PolygontoCircle(KManifold *m, KRigidbody *a, KRigidbody *b)
{
	CircletoPolygon(m, b, a);
	m->normal = -m->normal;
}

float FindAxisLeastPenetration(uint32 *faceIndex, KPolygonShape *A, KPolygonShape *B)
{
	float bestDistance = -FLT_MAX;
	uint32 bestIndex;

	for (uint32 i = 0; i < A->m_vertexCount; ++i)
	{
		// Retrieve a face normal from A
		KVector2 n = A->m_normals[i];
		KVector2 nw = A->u * n;

		// Transform face normal into B's model space
		KMatrix2 buT = B->u.Transpose();
		n = buT * nw;

		// Retrieve support point from B along -n
		KVector2 s = B->GetSupportPoint(-n);

		// Retrieve vertex on face from A, transform into
		// B's model space
		KVector2 v = A->m_vertices[i];
		v = A->u * v + A->body->position;
		v -= B->body->position;
		v = buT * v;

		// Compute penetration distance (in B's model space)
		float d = KVector2::Dot(n, s - v);

		// Store greatest distance
		if (d > bestDistance)
		{
			bestDistance = d;
			bestIndex = i;
		}
	}

	*faceIndex = bestIndex;
	return bestDistance;
}

void FindIncidentFace(KVector2 *v, KPolygonShape *RefPoly, KPolygonShape *IncPoly, uint32 referenceIndex)
{
	KVector2 referenceNormal = RefPoly->m_normals[referenceIndex];

	// Calculate normal in incident's frame of reference
	referenceNormal = RefPoly->u * referenceNormal; // To world space
	referenceNormal = IncPoly->u.Transpose() * referenceNormal; // To incident's model space

	// Find most anti-normal face on incident polygon
	int32 incidentFace = 0;
	float minDot = FLT_MAX;
	for (uint32 i = 0; i < IncPoly->m_vertexCount; ++i)
	{
		float dot = KVector2::Dot(referenceNormal, IncPoly->m_normals[i]);
		if (dot < minDot)
		{
			minDot = dot;
			incidentFace = i;
		}
	}

	// Assign face vertices for incidentFace
	v[0] = IncPoly->u * IncPoly->m_vertices[incidentFace] + IncPoly->body->position;
	incidentFace = incidentFace + 1 >= (int32)IncPoly->m_vertexCount ? 0 : incidentFace + 1;
	v[1] = IncPoly->u * IncPoly->m_vertices[incidentFace] + IncPoly->body->position;
}

int32 Clip(KVector2 n, float c, KVector2 *face)
{
	uint32 sp = 0;
	KVector2 out[2] = {
	  face[0],
	  face[1]
	};

	// Retrieve distances from each endpoint to the line
	// d = ax + by - c
	float d1 = KVector2::Dot(n, face[0]) - c;
	float d2 = KVector2::Dot(n, face[1]) - c;

	// If negative (behind plane) clip
	if (d1 <= 0.0f) out[sp++] = face[0];
	if (d2 <= 0.0f) out[sp++] = face[1];

	// If the points are on different sides of the plane
	if (d1 * d2 < 0.0f) // less than to ignore -0.0f
	{
		// Push interesection point
		float alpha = d1 / (d1 - d2);
		out[sp] = face[0] + alpha * (face[1] - face[0]);
		++sp;
	}

	// Assign our new converted values
	face[0] = out[0];
	face[1] = out[1];

	assert(sp != 3);

	return sp;
}

void PolygontoPolygon(KManifold *m, KRigidbody *a, KRigidbody *b)
{
	auto BiasGreaterThan = [](float a, float b) -> bool
	{
		const float k_biasRelative = 0.95f;
		const float k_biasAbsolute = 0.01f;
		return a >= b * k_biasRelative + a * k_biasAbsolute;
	};

	KPolygonShape *A = reinterpret_cast<KPolygonShape *>(a->shape);
	KPolygonShape *B = reinterpret_cast<KPolygonShape *>(b->shape);
	m->contact_count = 0;

	// Check for a separating axis with A's face planes
	uint32 faceA;
	float penetrationA = FindAxisLeastPenetration(&faceA, A, B);
	if (penetrationA >= 0.0f)
		return;

	// Check for a separating axis with B's face planes
	uint32 faceB;
	float penetrationB = FindAxisLeastPenetration(&faceB, B, A);
	if (penetrationB >= 0.0f)
		return;

	uint32 referenceIndex;
	bool flip; // Always point from a to b

	KPolygonShape *RefPoly; // Reference
	KPolygonShape *IncPoly; // Incident

	// Determine which shape contains reference face
	if (BiasGreaterThan(penetrationA, penetrationB))
	{
		RefPoly = A;
		IncPoly = B;
		referenceIndex = faceA;
		flip = false;
	}
	else
	{
		RefPoly = B;
		IncPoly = A;
		referenceIndex = faceB;
		flip = true;
	}

	// World space incident face
	KVector2 incidentFace[2];
	FindIncidentFace(incidentFace, RefPoly, IncPoly, referenceIndex);

	//        y
	//        ^  ->n       ^
	//      +---c ------posPlane--
	//  x < | i |\
    //      +---+ c-----negPlane--
	//             \       v
	//              r
	//
	//  r : reference face
	//  i : incident poly
	//  c : clipped point
	//  n : incident normal

	// Setup reference face vertices
	KVector2 v1 = RefPoly->m_vertices[referenceIndex];
	referenceIndex = referenceIndex + 1 == RefPoly->m_vertexCount ? 0 : referenceIndex + 1;
	KVector2 v2 = RefPoly->m_vertices[referenceIndex];

	// Transform vertices to world space
	v1 = RefPoly->u * v1 + RefPoly->body->position;
	v2 = RefPoly->u * v2 + RefPoly->body->position;

	// Calculate reference face side normal in world space
	KVector2 sidePlaneNormal = (v2 - v1);
	sidePlaneNormal.Normalize();

	// Orthogonalize
	KVector2 refFaceNormal(sidePlaneNormal.y, -sidePlaneNormal.x);

	// ax + by = c
	// c is distance from origin
	float refC = KVector2::Dot(refFaceNormal, v1);
	float negSide = -KVector2::Dot(sidePlaneNormal, v1);
	float posSide = KVector2::Dot(sidePlaneNormal, v2);

	// Clip incident face to reference face side planes
	if (Clip(-sidePlaneNormal, negSide, incidentFace) < 2)
		return; // Due to floating point error, possible to not have required points

	if (Clip(sidePlaneNormal, posSide, incidentFace) < 2)
		return; // Due to floating point error, possible to not have required points

	  // Flip
	m->normal = flip ? -refFaceNormal : refFaceNormal;

	// Keep points behind reference face
	uint32 cp = 0; // clipped points behind reference face
	float separation = KVector2::Dot(refFaceNormal, incidentFace[0]) - refC;
	if (separation <= 0.0f)
	{
		m->contacts[cp] = incidentFace[0];
		m->penetration = -separation;
		++cp;
	}
	else
		m->penetration = 0;

	separation = KVector2::Dot(refFaceNormal, incidentFace[1]) - refC;
	if (separation <= 0.0f)
	{
		m->contacts[cp] = incidentFace[1];

		m->penetration += -separation;
		++cp;

		// Average penetration
		m->penetration /= (float)cp;
	}

	m->contact_count = cp;
}
