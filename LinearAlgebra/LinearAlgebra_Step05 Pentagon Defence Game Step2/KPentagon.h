#pragma once
#include "KBaseObject.h"
#include "KVector2.h"

class KPentagon : public KBaseObject
{
private:
    KVector2    m_cannonDir;

public:
	KPentagon();
	virtual ~KPentagon();

	virtual void OnUpdate(float fElapsedTime_);
	virtual void OnRender();

    KVector2 GetCannorDir() { return m_cannonDir; }
};

