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

#ifndef _KWORLD_H_
#define _KWORLD_H_

#include "KMath.h"

// http://gamedev.tutsplus.com/tutorials/implementation/how-to-create-a-custom-2d-physics-engine-friction-scene-and-jump-table/
struct KWorld
{
	KWorld(float dt, uint32 iterations) : m_dt(dt), m_iterations(iterations) {}

	void Step();
	void Render();
	KRigidbody *Add(KShape *shape, int32 x, int32 y);
	void Clear();

	float m_dt;
	uint32 m_iterations;
	std::vector<KRigidbody*> m_bodies;
};

#endif // _KWORLD_H_