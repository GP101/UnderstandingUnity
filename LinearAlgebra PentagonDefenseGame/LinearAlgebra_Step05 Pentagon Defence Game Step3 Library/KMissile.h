#pragma once
#include "KBaseObject.h"

class KMissile : public KBaseObject
{
public:
	CONSTRUCTOR         KMissile();
	DESTRUCTOR virtual  ~KMissile();

	virtual void        OnUpdate(float fElapsedTime_);
	virtual void        OnRender(HDC hdc);
};
