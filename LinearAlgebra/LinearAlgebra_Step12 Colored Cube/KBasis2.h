#pragma once
#include "KVector2.h"

class KBasis2
{
public:
    KVector2 basis0;
    KVector2 basis1;

public:
    KBasis2() { basis0 = KVector2(1, 0); basis1 = KVector2(0, 1); }
    void SetInfo(const KVector2& tbasis0, const KVector2& tbasis1 )
    {
        basis0 = tbasis0;
        basis1 = tbasis1;
    }
    KVector2 Transform(const KVector2& input)
    {
        KVector2 t0 = input.x*basis0;
        KVector2 t1 = input.y*basis1;
        KVector2 temp(t0.x + t1.x,  t0.y + t1.y);
        return temp;
    }
};
