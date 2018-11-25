#pragma once
#include "KVector3.h"

class KMatrix4
{
public:
    float   m_afElements[4][4];

	KMatrix4();
	~KMatrix4();

    /// access matrix element (iRow,iCol)
    /// @param  iRow: row index
    /// @param  iCol: column index
    /// @return reference of element (iRow,iCol)
    float& operator()(int iRow, int iCol);
    KMatrix4 operator*(KMatrix4& mRight);
    KVector3 operator*(KVector3& vRight);
    KMatrix4 operator+(KMatrix4& mRight);
    KMatrix4& operator=(KMatrix4& mRight);

    KMatrix4 SetZero();
    KMatrix4 SetIdentity();
    KMatrix4 SetRotationX(float fRadian);
    KMatrix4 SetRotationY(float fRadian);
    KMatrix4 SetRotationZ(float fRadian);
    KMatrix4 SetScale(float fxScale, float fyScale, float fzScale);
    KMatrix4 SetTranslation(float x, float y, float z);

    KMatrix4 SetProjection(float d);
};//class KMatrix4
