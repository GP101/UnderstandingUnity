// LinearAlgebra.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "LinearAlgebra.h"
#include "KVectorUtil.h"
#include <MMSystem.h>
#include "KMatrix2.h"
#include <objidl.h>
#include <gdiplus.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <windowsx.h>
#include <memory>
#include "KBaseObject.h"
#include "KBullet.h"
#include <vector>
#include <list>
#include "KMyGame.h"
#include "jacobi_pd.hpp"


class KMyGame2 : public KMyGame
{
private:
    std::vector<KVector2>   bulletPosition;
    std::vector<double>     m_eigenValues{ 0,0 };
    std::vector<std::vector<double>> 
                            m_eigenVectors;

private:
    void _CalculatePCA();

public:
    virtual void OnInitialize()
    {
    }
    virtual void OnFinalize()
    {
    }
    virtual void OnLButtonDown(int x, int y)
    {
        std::shared_ptr<KBullet> pb = KMyGame::CreateObject<KBullet>();
        KVector2 pos = KVectorUtil::ScreenToWorld(KVector2(x, y));
        pb->SetPosition(pos);
        bulletPosition.push_back(pos);
        _CalculatePCA();
    }
    virtual void OnUpdate(float fElapsedTime) override;
    virtual void OnRender(HDC hdc) override;
    virtual void OnCommand(int commandId) override;
};

void KMyGame2::_CalculatePCA()
{
    //variable declaration
    double x_square = 0;
    double y_square = 0;
    double x_variance;
    double y_variance;
    double covariancePoints = 0;
    double covariance;

    for (unsigned int i = 0; i < bulletPosition.size(); i++)
    {
        //square x and y
        x_square += bulletPosition[i].x * bulletPosition[i].x;
        y_square += bulletPosition[i].y * bulletPosition[i].y;
        //covariance - sum of all positions / by size
        covariancePoints += (bulletPosition[i].x * bulletPosition[i].y);
    }

    //Calculate variance  |square x/y divided by size
    x_variance = x_square / bulletPosition.size();
    y_variance = y_square / bulletPosition.size();
    //Calculate covariance
    covariance = covariancePoints / bulletPosition.size();

    int n = 2;
    std::vector<std::vector<double>> matrices; // matrix to diagonalize
    jacobi_pd::Jacobi<double> eigen_calc(n);

    matrices.push_back(std::vector<double>{x_variance, covariance});
    matrices.push_back(std::vector<double>{covariance, y_variance});

    //Eigenvectors
    m_eigenVectors.push_back(std::vector<double>{0, 0});
    m_eigenVectors.push_back(std::vector<double>{0, 0});

    eigen_calc.Diagonalize(matrices, OUT m_eigenValues, OUT m_eigenVectors);
}

void KMyGame2::OnUpdate(float fElapsedTime)
{
}

void KMyGame2::OnRender(HDC hdc)
{
    if (m_eigenVectors.size() >= 2)
    {
        KVector2 Eigen;
        KVector2 EigenReverse;
        if (m_eigenValues[0] > m_eigenValues[1])
        {
            Eigen.x = float(m_eigenValues[0] * m_eigenVectors[0][0]);
            Eigen.y = float(m_eigenValues[0] * m_eigenVectors[0][1]);
        }
        else
        {
            Eigen.x = float(m_eigenValues[1] * m_eigenVectors[1][0]);
            Eigen.y = float(m_eigenValues[1] * m_eigenVectors[1][1]);
        }
        EigenReverse = -1 * Eigen;
        COLORREF color = RGB(0, 0, 0);
        KVectorUtil::DrawLine(hdc, EigenReverse, Eigen, 3, PS_SOLID, color);
    }
}

void KMyGame2::OnCommand(int commandId)
{
    if (commandId == ID_FILE_RESETDATA)
    {
    }
    else if (commandId == ID_FILE_DRAWPCA)
    {
        //_CalculatePCA();
    }
}

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
