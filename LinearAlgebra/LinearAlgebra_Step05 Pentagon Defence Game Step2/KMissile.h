#pragma once
#include "KBaseObject.h"
class KMissile : public KBaseObject
{
public:
	KMissile();
	virtual ~KMissile();

	virtual void OnUpdate(float fElapsedTime_);
	virtual void OnRender();
};

