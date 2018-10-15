#include "KBullet.h"
#include "KVectorUtil.h"
#include "KVector2.h"

extern HWND    g_hwnd;
extern HDC     g_hdc;

KBullet::KBullet()
{
	m_position = KVector2(0, 0);
}

KBullet::~KBullet()
{
}

void KBullet::OnUpdate(float fElapsedTime_)
{
	m_position = m_position + m_velocity * fElapsedTime_;
}

void KBullet::OnRender()
{
	KVectorUtil::DrawCircle(g_hdc, m_position, 0.1f, 4);
}
