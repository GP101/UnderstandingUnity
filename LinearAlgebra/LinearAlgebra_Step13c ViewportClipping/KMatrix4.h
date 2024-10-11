#pragma once
#include "KVector3.h"

class KMatrix4
{
public:
    double  m_afElements[4][4];

	KMatrix4();
	~KMatrix4();

    /// access matrix element (iRow,iCol)
    /// @param  iRow: row index
    /// @param  iCol: column index
    /// @return reference of element (iRow,iCol)
    double& operator()(int iRow, int iCol);
    KMatrix4 operator*(KMatrix4& mRight);
    KVector3 operator*( KVector3& vRight);
    KMatrix4 operator+( KMatrix4& mRight);
    KMatrix4& operator=( KMatrix4& mRight);

    KMatrix4 SetZero();
    KMatrix4 SetIdentity();
    KMatrix4 SetRotationX( double fRadian);
    KMatrix4 SetRotationY( double fRadian);
    KMatrix4 SetRotationZ( double fRadian);
    KMatrix4 SetScale( double fxScale, double fyScale, double fzScale);
    KMatrix4 SetTranslation( double x, double y, double z );
    KMatrix4 SetOrientation( const KVector3& xaxis, const KVector3& yaxis, const KVector3& zaxis );
    KMatrix4 LookAtRH( KVector3 eye, KVector3 target, KVector3 up );

    KMatrix4 SetProjection( double d);
    KVector3 GetBasis( int index ) const;
};//class KMatrix4
