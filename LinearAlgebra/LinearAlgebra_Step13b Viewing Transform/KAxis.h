#pragma once
#include "KMatrix4.h"

class KAxis
{
private:
    KVector3    m_right; // x-axis
    KVector3    m_up; // y-axis
    KVector3    m_forward; // z-axis
    KVector3    m_position; // axis position
    double      m_length;

public:
    KAxis();
    ~KAxis();
    void Render( HDC hdc );
    void InitPositionDirection();
    void Transform( KMatrix4& mat );
};

