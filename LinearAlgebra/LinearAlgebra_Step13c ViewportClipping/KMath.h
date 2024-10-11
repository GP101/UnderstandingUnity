#pragma once
#include <cmath>
#include <cassert>
#include <algorithm>
#include "KVector2.h"
#include "KMatrix2.h"

typedef signed int		int32;
typedef unsigned int	uint32;
typedef float			float32;

const double			PI = 3.141592741;
const double			EPSILON = 0.0001;

#ifndef constructor
#define constructor
#endif

#ifndef destructor
#define destructor
#endif

#ifndef VIRTUAL
#define VIRTUAL
#endif

inline bool IsEqual(double a, double b)
{
	return std::abs(a - b) <= EPSILON;
}

inline bool IsZero(double a)
{
	return std::abs(a) <= 0.00001;
}

inline double Square(double a)
{
	return a * a;
}

inline double Clamp(double min, double max, double a)
{
	if (a < min) return min;
	if (a > max) return max;
	return a;
}

inline int Round(double a)
{
	return (int)(a + 0.5);
}

inline double Random(double min, double max)
{
	double a = (double)rand();
	a /= RAND_MAX;
	a = (max - min) * a + min;
	return a;
}
