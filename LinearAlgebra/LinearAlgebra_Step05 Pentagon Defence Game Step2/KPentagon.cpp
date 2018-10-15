#include "KPentagon.h"
#include "KVectorUtil.h"

extern HDC     g_hdc;
extern HWND    g_hwnd;

KPentagon::KPentagon()
{
}

KPentagon::~KPentagon()
{
}

void KPentagon::OnUpdate(float fElapsedTime_)
{
    POINT mousePoint;
    GetCursorPos(&mousePoint);
    ScreenToClient(g_hwnd, &mousePoint);
    KVector2 vmouse = KVectorUtil::ScreenToWorld(KVector2(mousePoint.x, mousePoint.y));
    KVector2 vdir = vmouse;
    vdir.Normalize();
    m_cannonDir = vdir;
}

void KPentagon::OnRender()
{
	KVectorUtil::DrawCircle(g_hdc, KVector2(0, 0), 1, 5);
	KVectorUtil::DrawLine(g_hdc, KVector2(0, 0), m_cannonDir * 1.5f, 2, PS_DASH);
}
