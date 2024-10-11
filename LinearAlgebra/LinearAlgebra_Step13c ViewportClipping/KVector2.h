#pragma once
#include <math.h>
#include <cmath>

#pragma warning( disable : 26495 )

class KVector2
{
public:
	static double EPSILON;
	static double PI;

	static KVector2 zero;
	static KVector2 one;
	static KVector2 right;
	static KVector2 up;

	static bool IsZero(double a);
	static KVector2 Lerp(const KVector2& begin, const KVector2& end, double ratio);
	static double Dot(const KVector2& left, const KVector2& right);
	static double DistSquared(const KVector2& a, const KVector2& b);
	static double Dist(const KVector2& a, const KVector2& b);
	static double Determinant(const KVector2& a, const KVector2& b);
	static bool IsCCW(const KVector2& a, const KVector2& b);
	static int GetDirection(const KVector2& a, const KVector2& b, const KVector2& c);
	static KVector2 Min(const KVector2& a, const KVector2& b);
	static KVector2 Max(const KVector2& a, const KVector2& b);
	static KVector2 Cross(const KVector2& v, double a);
	static KVector2 Cross(double a, const KVector2& v);
	static double Cross(const KVector2& a, const KVector2& b);

public:
	union
	{
		double v[2];
		double m[1][1];

		struct
		{
			double  x;
			double	y;
		};
	};

public:
	KVector2(double tx = 0.0, double ty = 0.0) { x = tx; y = ty; }
	void Set(double x_, double y_)
	{
		x = x_;
		y = y_;
	}
	double Length() const
	{
		return sqrt(x * x + y * y);
	}
	double LengthSquared() const
	{
		return x * x + y * y;
	}
	void Normalize()
	{
		const double length = Length();
		if (length > EPSILON)
		{
			x = x / length;
			y = y / length;
		}
	}

	void Rotate(double radians)
	{
		double c = std::cos(radians);
		double s = std::sin(radians);

		double tx = x * c - y * s;
		double ty = x * s + y * c;

		x = tx;
		y = ty;
	}

	double& operator[](int i)
	{
		return v[i];
	}

	KVector2 operator-() const
	{
		return KVector2(-x, -y);
	}

	KVector2 operator*=(double scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	KVector2 operator+=(const KVector2 rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	KVector2 operator-=(const KVector2 rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
};

inline KVector2 operator+(const KVector2& lhs, const KVector2& rhs)
{
	KVector2 temp(lhs.x + rhs.x, lhs.y + rhs.y);
	return temp;
}

inline KVector2 operator-(const KVector2& lhs, const KVector2& rhs)
{
	KVector2 temp(lhs.x - rhs.x, lhs.y - rhs.y);
	return temp;
}

inline double operator*(const KVector2& left, const KVector2& right)
{
	return left.x * right.x + left.y * right.y;
}

inline KVector2 operator*(double scalar, const KVector2& rhs)
{
	KVector2 temp(scalar * rhs.x, scalar * rhs.y);
	return temp;
}

inline KVector2 operator*(const KVector2& lhs, double scalar)
{
	KVector2 temp(lhs.x * scalar, lhs.y * scalar);
	return temp;
}

inline KVector2 operator/(const KVector2& lhs, double scalar)
{
	KVector2 temp(lhs.x / scalar, lhs.y / scalar);
	return temp;
}
