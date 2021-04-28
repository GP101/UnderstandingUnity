#pragma once
#include <math.h>
#include <cmath>

class KVector2
{
public:
	static float EPSILON;
	static float PI;

    static KVector2 zero;
    static KVector2 one;
    static KVector2 right;
    static KVector2 up;

    static KVector2 Lerp(const KVector2& begin, const KVector2& end, float ratio);
    static float Dot(const KVector2& left, const KVector2& right);
    static float DistSquared(const KVector2& a, const KVector2& b);
    static float Dist(const KVector2& a, const KVector2& b);
	static float Determinant(const KVector2&a, const KVector2 &b);
	static bool IsCCW(const KVector2&a, const KVector2 &b);
	static KVector2 Min(const KVector2& a, const KVector2& b);
	static KVector2 Max(const KVector2& a, const KVector2& b);
	static KVector2 Cross(const KVector2& v, float a);
	static KVector2 Cross(float a, const KVector2& v);
	static float Cross(const KVector2& a, const KVector2& b);

public:
	union
	{
		float v[2];
		float m[1][1];

		struct
		{
			float   x;
			float   y;
		};
	};

public:
    KVector2(float tx=0.0f, float ty=0.0f) { x = tx; y = ty; }
	void Set(float x_, float y_)
	{
		x = x_;
		y = y_;
	}
    float Length() const
    {
        return sqrtf(x*x + y*y);
    }
	float LengthSquared() const
	{
		return x * x + y * y;
	}
    void Normalize()
    {
        const float length = Length();
		if (length > EPSILON)
		{
			x = x / length;
			y = y / length;
		}
    }

	void Rotate(float radians )
	{
		float c = std::cos(radians);
		float s = std::sin(radians);

		float tx = x * c - y * s;
		float ty = x * s + y * c;

		x = tx;
		y = ty;
	}

    float& operator[](int i)
    {
        return v[i];
    }

	KVector2 operator-() const
	{
		return KVector2(-x, -y);
	}

	KVector2 operator*=(float scalar)
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

inline float operator*(const KVector2& left, const KVector2& right)
{
	return left.x * right.x + left.y * right.y;
}

inline KVector2 operator*(float scalar, const KVector2& rhs)
{
    KVector2 temp(scalar*rhs.x, scalar*rhs.y);
    return temp;
}

inline KVector2 operator*(const KVector2& lhs, float scalar)
{
	KVector2 temp(lhs.x*scalar, lhs.y*scalar);
	return temp;
}

inline KVector2 operator/(const KVector2& lhs, float scalar)
{
	KVector2 temp(lhs.x/scalar, lhs.y/scalar);
	return temp;
}
