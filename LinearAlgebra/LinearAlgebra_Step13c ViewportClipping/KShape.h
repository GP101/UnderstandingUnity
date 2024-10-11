/*
	Windows porting in 2021: jintaeks@dongseo.ac.kr
*/

#ifndef SHAPE_H
#define SHAPE_H

#include "KMath.h"
#include <memory>

struct KShape;
typedef std::shared_ptr<KShape>	KShapePtr;
struct KShape
{
	enum Type
	{
		eCircle,
		ePoly,
		eCount
	};

public:
	KShape()
	{
		r = Random(0.2, 0.8);
		g = Random(0.2, 0.8);
		b = Random(0.2, 0.8);
	}
	virtual void Initialize() = 0;
	virtual void SetRotation(double radians) = 0;
	virtual Type GetType() const = 0;
	void SetPosition(const KVector2& pos_) { position = pos_; }

public:
	KMatrix2 rotation; // Orientation matrix from model to world
	KVector2 position;
	// Store a color in RGB format
	double r, g, b;
};

#endif // SHAPE_H
