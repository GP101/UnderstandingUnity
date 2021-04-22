#pragma once
#include "KVector2.h"
#include <math.h>

class KMatrix2
{
public:
    static KMatrix2 zero;
    static KMatrix2 identity;
public:
    float   _11, _12;
    float   _21, _22;

public:
    KMatrix2(float e11 = 1.0f, float e12 = 0.0f, float e21 = 0.0f, float e22 = 1.0f)
    {
        _11 = e11;
        _12 = e12;
        _21 = e21;
        _22 = e22;
    }
    ~KMatrix2() {}
	void Set(float e11, float e12, float e21, float e22)
	{
		_11 = e11;
		_12 = e12;
		_21 = e21;
		_22 = e22;
	}
	void Set(const KVector2& basis0, const KVector2& basis1)
	{
		_11 = basis0.x;
		_12 = basis1.x;
		_21 = basis0.y;
		_22 = basis1.y;
	}

    void SetIdentity()
    {
        _11 = 1.0f; _12 = 0.0f;
        _21 = 0.0f; _22 = 1.0f;
    }

    void SetRotation(float theta)
    {
        _11 = cosf(theta); _12 = -sinf(theta);
        _21 = sinf(theta); _22 = cosf(theta);
    }

    void SetShear(float shearXParallelToY, float shearYParallelToX)
    {
        _11 = 1.0f; _12 = shearYParallelToX;
        _21 = shearXParallelToY; _22 = 1.0f;
    }

    bool GetBasis(KVector2& basis_, int basisIndexFrom0_)
    {
        if (basisIndexFrom0_ == 0) {
            basis_.x = _11;
            basis_.y = _21;
        }
        else if (basisIndexFrom0_ == 1)
        {
            basis_.x = _12;
            basis_.y = _22;
        }
        else
        {
            return false;
        }

        return true;
    }
    float GetDeterminant() const
    {
        return _11 * _22 - _12 * _21;
    }
    KMatrix2 GetInverse() const;
};

inline KVector2 operator*(const KVector2& v, const KMatrix2& m)
{
    KVector2 temp;
    temp.x = v.x*m._11 + v.y*m._21;
    temp.y = v.x*m._12 + v.y*m._22;
    return temp;
}

inline KVector2 operator*(const KMatrix2& m, const KVector2& v)
{
    KVector2 temp;
    temp.x = m._11*v.x + m._12*v.y;
    temp.y = m._21*v.x + m._22*v.y;
    return temp;
}

inline KMatrix2 operator*(float scalar, const KMatrix2& m)
{
    KMatrix2 temp;
    temp._11 = scalar*m._11;
    temp._12 = scalar*m._12;
    temp._21 = scalar*m._21;
    temp._22 = scalar*m._22;
    return temp;
}

// composition: matrix-matrix mulpiplication
inline KMatrix2 operator*(const KMatrix2& m0, const KMatrix2& m1)
{
    KMatrix2 temp;
    temp._11 = m0._11*m1._11 + m0._12*m1._21;
    temp._12 = m0._11*m1._12 + m0._12*m1._22;
    temp._21 = m0._21*m1._11 + m0._22*m1._21;
    temp._22 = m0._21*m1._12 + m0._22*m1._22;
    return temp;
}
