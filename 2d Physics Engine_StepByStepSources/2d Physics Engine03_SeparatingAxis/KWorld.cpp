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

void IntegrateForces(KRigidbody *b, float dt)
{
	if (b->m_invMass == 0.0f)
		return;

	b->velocity += (b->force * b->m_invMass + g_gravity) * dt;
	b->angularVelocity += b->torque * b->m_invI * dt;
}

void IntegrateVelocity(KRigidbody *b, float dt)
{
	if (b->m_invMass == 0.0f)
		return;

	b->position += b->velocity * dt;
	b->rotation += b->angularVelocity * dt;
	b->SetRotation(b->rotation);
}

//void KWorld::Step(void)
//{
//	// Generate new collision info
//	m_contacts.clear();
//	for (uint32 i = 0; i < m_bodies.size(); ++i)
//	{
//		KRigidbody *A = m_bodies[i];
//
//		for (uint32 j = i + 1; j < m_bodies.size(); ++j)
//		{
//			KRigidbody *B = m_bodies[j];
//			if (A->m_invMass == 0 && B->m_invMass == 0)
//				continue;
//			KManifold m(A, B);
//			m.Solve();
//			if (m.contact_count)
//				m_contacts.emplace_back(m);
//		}
//	}
//
//	// Integrate forces
//	for (uint32 i = 0; i < m_bodies.size(); ++i)
//		IntegrateForces(m_bodies[i], m_dt);
//
//	// Initialize collision
//	for (uint32 i = 0; i < m_contacts.size(); ++i)
//		m_contacts[i].Initialize();
//
//	// Solve collisions
//	//for (uint32 j = 0; j < m_iterations; ++j)
//	//	for (uint32 i = 0; i < m_contacts.size(); ++i)
//	//		m_contacts[i].ApplyImpulse();
//
//	// Integrate velocities
//	//for (uint32 i = 0; i < m_bodies.size(); ++i)
//	//	IntegrateVelocity(m_bodies[i], m_dt);
//
//	// Correct positions
//	//for (uint32 i = 0; i < m_contacts.size(); ++i)
//	//	m_contacts[i].PositionalCorrection();
//
//	// Clear all forces
//	for (uint32 i = 0; i < m_bodies.size(); ++i)
//	{
//		KRigidbody *b = m_bodies[i];
//		b->force.Set(0, 0);
//		b->torque = 0;
//	}
//}

void KWorld::Render(void)
{
	for (uint32 i = 0; i < m_bodies.size(); ++i)
	{
		KRigidbody *b = m_bodies[i];
		b->shape->Draw();
	}

	if (g_drawPenetration == true)
	{
		COLORREF color = RGB(255, 0, 0);
		std::vector<KVector2> points;

		color = RGB(255, 0, 255);
		points.clear();
		for (uint32 i = 0; i < m_contacts.size(); ++i)
		{
			KManifold& m = m_contacts[i];
			KVector2 n = m.normal;
			for (uint32 j = 0; j < m.contact_count; ++j)
			{
				KVector2 c = m.contacts[j];
				points.push_back(KVector2(c.x, c.y));
				n *= 1.0f;
				c += n;
				points.push_back(KVector2(c.x, c.y));
			}
		}
		KVectorUtil::DrawLine(g_hdc, points, 2, 0, color, 2);
	}
}

KRigidbody* KWorld::Add(KShape *shape, int32 x, int32 y)
{
	assert(shape);
	KRigidbody *b = new KRigidbody(shape, x, y);
	m_bodies.push_back(b);
	return b;
}

void KWorld::Clear()
{
	m_bodies.clear();
	m_contacts.clear();
}
