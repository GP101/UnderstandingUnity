#pragma once
#include "KBaseObject.h"
#include "KVector2.h"

class KBullet :	public KBaseObject
{
public:
	KVector2	m_velocity;
	KVector2	m_acceleration;
	KVector2	m_position;

public:
	KBullet();
	virtual ~KBullet();

	virtual void OnUpdate(float fElapsedTime_);
	virtual void OnRender();

    void SetPosition(const KVector2 pos_)
    {
        m_position = pos_;
    }
    void SetVelocity(const KVector2& v)
	{
		m_velocity = v;
	}
};

