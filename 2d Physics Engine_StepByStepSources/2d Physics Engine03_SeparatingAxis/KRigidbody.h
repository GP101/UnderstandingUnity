/*
	Copyright (c) 2013 Randy Gaul http://RandyGaul.net

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:
	  1. The origin of this software must not be misrepresented; you must not
		 claim that you wrote the original software. If you use this software
		 in a product, an acknowledgment in the product documentation would be
		 appreciated but is not required.
	  2. Altered source versions must be plainly marked as such, and must not be
		 misrepresented as being the original software.
	  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BODY_H
#define BODY_H
#include "KMath.h"

struct KShape;

// http://gamedev.tutsplus.com/tutorials/implementation/how-to-create-a-custom-2d-physics-engine-the-core-engine/
struct KRigidbody
{
	KRigidbody(KShape *shape_, int32 x, int32 y);
	void ApplyForce(const KVector2& f);
	void ApplyImpulse(const KVector2& impulse, const KVector2& contactVector);
	void SetStatic();
	void SetRotation(float radians);

	KVector2 position;
	KVector2 velocity;

	float angularVelocity;
	float torque;
	float rotation; // radians

	KVector2 force;

	// Set by shape
	float m_I;  // moment of inertia
	float m_invI; // inverse inertia
	float m_mass;  // mass
	float m_invMass; // inverse mass

	// http://gamedev.tutsplus.com/tutorials/implementation/how-to-create-a-custom-2d-physics-engine-friction-scene-and-jump-table/
	float staticFriction;
	float dynamicFriction;
	float restitution;

	// KShape interface
	KShape* shape;
};

#endif // BODY_H