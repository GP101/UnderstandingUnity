#include "KPentagon.h"
#include "KVectorUtil.h"

extern HWND    g_hwnd;

CONSTRUCTOR KPentagon::KPentagon()
{
}

DESTRUCTOR KPentagon::~KPentagon()
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

void KPentagon::OnRender(HDC hdc)
{
	KVectorUtil::DrawCircle(hdc, KVector2(0, 0), 1, 5);
	KVectorUtil::DrawLine(hdc, KVector2(0, 0), m_cannonDir * 1.5f, 2, PS_DASH);
}
