#if !defined(AFX_VECTOR_H__ED0A4977_9BC3_11D5_82EE_008048E10474__INCLUDED_)
#define AFX_VECTOR_H__ED0A4977_9BC3_11D5_82EE_008048E10474__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVector
{
public:
	CVector();
	virtual ~CVector();

    CVector(float x, float y, float z);

    float m_x;
    float m_y;
    float m_z;

    void Translate(float tx, float ty, float tz);
    void Scale(float sx, float sy, float sz);
    void RotateX(float theta);
    void RotateY(float theta);
    void RotateZ(float theta);
};//class CVector

#endif // !defined(AFX_VECTOR_H__ED0A4977_9BC3_11D5_82EE_008048E10474__INCLUDED_)
