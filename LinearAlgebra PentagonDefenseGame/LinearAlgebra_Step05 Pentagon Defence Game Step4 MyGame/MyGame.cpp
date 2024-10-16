// LinearAlgebra.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "KMyGame.h"
#include "KPentagon.h"
#include "KBullet.h"

#ifdef _DEBUG
#pragma comment(lib, "LinearAlgebra Step05 Pentagon Defence Game Step3 Library Debug.lib")
#else
#pragma comment(lib, "LinearAlgebra Step05 Pentagon Defence Game Step3 Library Release.lib")
#endif

class KMyGame2 : public KMyGame
{
private:
    std::shared_ptr<KPentagon>  
                        m_pentagon;

public:
    virtual void        OnInitialize()
                        {
                            m_pentagon = KMyGame::CreateObject<KPentagon>();
                        }
    virtual void        OnFinalize()
                        {
                        }
    virtual void        OnLButtonDown(int x, int y)
                        {
                            std::shared_ptr<KBullet> pb = KMyGame::CreateObject<KBullet>();
    
                            KVector2 dir = m_pentagon->GetCannorDir();
                            KVector2 velocity = dir * 3.0f;
                            pb->SetPosition(dir * 1.5f);
                            pb->SetVelocity(velocity);
                        }
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    KMyGame::CreateInstance<KMyGame2>();
    int wParam = KMyGame::GetInstance()->Run(hInstance, nCmdShow);

    return (int)wParam;
}
