#pragma once
#include "KBaseObject.h"
#include "KVector2.h"

class KPentagon : public KBaseObject
{
private:
    KVector2            m_cannonDir;

public:
	CONSTRUCTOR         KPentagon();
	DESTRUCTOR          virtual ~KPentagon();

	virtual void        OnUpdate(float fElapsedTime_);
	virtual void        OnRender(HDC hdc);

    KVector2            GetCannorDir() { return m_cannonDir; }
};
