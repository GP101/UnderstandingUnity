#include "KBullet.h"
#include "KVectorUtil.h"
#include "KVector2.h"

CONSTRUCTOR KBullet::KBullet()
{
	m_position = KVector2(0, 0);
}

DESTRUCTOR KBullet::~KBullet()
{
}

void KBullet::OnUpdate(float fElapsedTime_)
{
	m_position = m_position + m_velocity * fElapsedTime_ + 0.5f * m_acceleration * fElapsedTime_ * fElapsedTime_;
}

void KBullet::OnRender(HDC hdc)
{
	KVectorUtil::DrawCircle(hdc, m_position, 0.1f, 4);
}
