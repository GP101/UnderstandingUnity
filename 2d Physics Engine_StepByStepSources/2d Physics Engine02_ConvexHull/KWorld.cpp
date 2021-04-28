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
}
