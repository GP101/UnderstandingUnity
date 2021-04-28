#ifndef _KCIRCLESHAPE_H_
#define _KCIRCLESHAPE_H_

#include "KShape.h"
#include "KRigidbody.h"
#include "KMath.h"
#include "KVectorUtil.h"

struct KCircleShape : public KShape
{
	KCircleShape(float r);
	KShape *Clone() const;
	void Initialize();
	void ComputeMass(float density);
	void SetRotation(float radians);
	void Draw() const;
	KShape::Type GetType() const;
};

#endif // _KCIRCLESHAPE_H_
