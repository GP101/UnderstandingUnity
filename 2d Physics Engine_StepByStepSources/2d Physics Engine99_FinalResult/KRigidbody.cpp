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

#include "KPhysicsEngine.h"

KRigidbody::KRigidbody(KShape *shape_, int32 x, int32 y)
	: shape(shape_->Clone())
{
	shape->body = this;
	position.Set((float)x, (float)y);
	velocity.Set(0, 0);
	angularVelocity = 0;
	torque = 0;
	rotation = Random(-PI, PI);
	force.Set(0, 0);
	staticFriction = 0.5f;
	dynamicFriction = 0.3f;
	restitution = 0.2f;
	shape->Initialize();
}

void KRigidbody::ApplyForce(const KVector2& f)
{
	force += f;
}

void KRigidbody::ApplyImpulse(const KVector2& impulse, const KVector2& contactVector)
{
	velocity += m_invMass * impulse;
	angularVelocity += m_invI * KVector2::Cross(contactVector, impulse);
}

void KRigidbody::SetStatic(void)
{
	m_I = 0.0f;
	m_invI = 0.0f;
	m_mass = 0.0f;
	m_invMass = 0.0f;
}

void KRigidbody::SetRotation(float radians)
{
	rotation = radians;
	shape->SetRotation(radians);
}
