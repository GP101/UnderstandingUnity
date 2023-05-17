#pragma once
#include <cmath>
#include <cassert>
#include <algorithm>
#include "KVector2.h"
#include "KMatrix2.h"

typedef signed int		int32;
typedef unsigned int	uint32;
typedef float			float32;

const float				PI = 3.141592741f;
const float				EPSILON = 0.0001f;

#ifndef constructor
#define constructor
#endif

#ifndef destructor
#define destructor
#endif

#ifndef VIRTUAL
#define VIRTUAL
#endif

inline bool IsEqual(float a, float b)
{
	return std::abs(a - b) <= EPSILON;
}

inline bool IsZero(float a)
{
	return std::abs(a) <= 0.00001f;
}

inline float Square(float a)
{
	return a * a;
}

inline float Clamp(float min, float max, float a)
{
	if (a < min) return min;
	if (a > max) return max;
	return a;
}

inline int Round(float a)
{
	return (int)(a + 0.5f);
}

inline float Random(float min, float max)
{
	float a = (float)rand();
	a /= RAND_MAX;
	a = (max - min) * a + min;
	return a;
}
