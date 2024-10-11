#include "stdafx.h"
#include "KAxis.h"
#include "KPrimitive.h"

KAxis::KAxis()
{
    m_length = 2.0;
}

KAxis::~KAxis()
{
}

void KAxis::Render( HDC hdc )
{
    COLORREF colorRed = RGB( 255, 0, 0 );
    COLORREF colorGreen = RGB( 0, 255, 0 );
    COLORREF colorBlue = RGB( 0, 0, 255 );

    ::DrawPrimitive( hdc, m_position, m_right, colorRed );
    ::DrawPrimitive( hdc, m_position, m_up, colorGreen );
    ::DrawPrimitive( hdc, m_position, m_forward, colorBlue );
}

void KAxis::InitPositionDirection()
{
    m_position = KVector3( 0, 0, 0 );
    m_right = KVector3( 1, 0, 0 ) * m_length;
    m_up = KVector3( 0, 1, 0 )* m_length;
    m_forward = KVector3( 0, 0, 1 )* m_length;
}

void KAxis::Transform( KMatrix4& mat )
{
    m_position = mat * m_position;
    m_right = mat * m_right;
    m_up = mat * m_up;
    m_forward = mat * m_forward;
}
