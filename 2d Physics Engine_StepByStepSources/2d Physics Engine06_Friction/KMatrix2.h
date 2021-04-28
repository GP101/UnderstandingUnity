#pragma once
#include "KVector2.h"
#include <math.h>

class KMatrix2
{
public:
    static KMatrix2 zero;
    static KMatrix2 identity;
public:
	union
	{
		struct
		{
			float   _00, _01;
			float   _10, _11;
		};
		float m[2][2];
		float v[4];
	};

public:
    KMatrix2()
    {
        _00 = 1.0f;
        _01 = 0.0f;
        _10 = 0.0f;
        _11 = 1.0f;
    }
	KMatrix2(float radian) { SetRotation(radian); }
	KMatrix2(float e00, float e01, float e10, float e11)
	{
		_00 = e00;
		_01 = e01;
		_10 = e10;
		_11 = e11;
	}
	void Set(float e00, float e01, float e10, float e11)
	{
		_00 = e00;
		_01 = e01;
		_10 = e10;
		_11 = e11;
	}
	void Set(const KVector2& basis0, const KVector2& basis1)
	{
		_00 = basis0.x;
		_01 = basis1.x;
		_10 = basis0.y;
		_11 = basis1.y;
	}
	void Set(float radian)
	{
		SetRotation(radian);
	}

    void SetIdentity()
    {
        _00 = 1.0f; _01 = 0.0f;
        _10 = 0.0f; _11 = 1.0f;
    }

    void SetRotation(float theta)
    {
		const float c = cosf(theta);
		const float s = sinf(theta);
        _00 = c; _01 = -s;
        _10 = s; _11 = c;
    }

    void SetShear(float shearXParallelToY, float shearYParallelToX)
    {
        _00 = 1.0f; _01 = shearYParallelToX;
        _10 = shearXParallelToY; _11 = 1.0f;
    }

    bool GetBasis(KVector2& basis_, int basisIndexFrom0_)
    {
        if (basisIndexFrom0_ == 0) {
            basis_.x = _00;
            basis_.y = _10;
        }
        else if (basisIndexFrom0_ == 1)
        {
            basis_.x = _01;
            basis_.y = _11;
        }
        else
        {
            return false;
        }

        return true;
    }
    float GetDeterminant() const
    {
        return _00 * _11 - _01 * _10;
    }
    KMatrix2 GetInverse() const;

	KMatrix2 Abs() const
	{
		return KMatrix2( std::abs(_00), std::abs(_01), std::abs(_10), std::abs(_11));
	}

	KVector2 AxisX() const
	{
		return KVector2(_00, _10);
	}

	KVector2 AxisY() const
	{
		return KVector2(_01, _11);
	}

	KMatrix2 Transpose() const
	{
		return KMatrix2(_00, _10, _01, _11);
	}

};

inline KVector2 operator*(const KVector2& v, const KMatrix2& m)
{
    KVector2 temp;
    temp.x = v.x*m._00 + v.y*m._10;
    temp.y = v.x*m._01 + v.y*m._11;
    return temp;
}

inline KVector2 operator*(const KMatrix2& m, const KVector2& v)
{
    KVector2 temp;
    temp.x = m._00*v.x + m._01*v.y;
    temp.y = m._10*v.x + m._11*v.y;
    return temp;
}

inline KMatrix2 operator*(float scalar, const KMatrix2& m)
{
    KMatrix2 temp;
    temp._00 = scalar*m._00;
    temp._01 = scalar*m._01;
    temp._10 = scalar*m._10;
    temp._11 = scalar*m._11;
    return temp;
}

// composition: matrix-matrix multiplication
inline KMatrix2 operator*(const KMatrix2& m0, const KMatrix2& m1)
{
    KMatrix2 temp;
    temp._00 = m0._00*m1._00 + m0._01*m1._10;
    temp._01 = m0._00*m1._01 + m0._01*m1._11;
    temp._10 = m0._10*m1._00 + m0._11*m1._10;
    temp._11 = m0._10*m1._01 + m0._11*m1._11;
    return temp;
}
