#include "stdafx.h"
#include "KMatrix3.h"

static KMatrix3 zero = KMatrix3(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
static KMatrix3 identity = KMatrix3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f );

KMatrix3 KMatrix3::GetInverse() const
{
	// lambda
	auto GetMinor = [](float a, float b, float c, float d)
	{
		return a * d - b * c;
	};
	const float det = GetDeterminant();
	KMatrix3 adj;
	// Get minor and apply cofactor
	const float m11 = +GetMinor(_22, _23, _32, _33);
	const float m12 = -GetMinor(_21, _23, _31, _33);
	const float m13 = +GetMinor(_21, _22, _31, _32);
	const float m21 = -GetMinor(_12, _13, _32, _33);
	const float m22 = +GetMinor(_11, _13, _31, _33);
	const float m23 = -GetMinor(_11, _12, _31, _32);
	const float m31 = +GetMinor(_12, _13, _22, _23);
	const float m32 = -GetMinor(_11, _13, _21, _23);
	const float m33 = +GetMinor(_11, _12, _21, _22);
	// set Adjugate matrix
	adj.Set(
		m11, m21, m31,
		m12, m22, m32,
		m13, m23, m33);
	return (1.0f / det) * adj;
}
