#pragma once
#include "KVector2.h"

class KScreenCoordinate
{
public:
    KVector2 axis0;
    KVector2 axis1;
    KVector2 origin;

public:
    KScreenCoordinate() { axis0 = KVector2(1, 0); axis1 = KVector2(0, -1); origin = KVector2(0, 0); }
    void SetInfo(const KVector2& taxis0, const KVector2& taxis1, const KVector2& torigin)
    {
        axis0 = taxis0;
        axis1 = taxis1;
        origin = torigin;
    }
    void SetOrigin(const KVector2& origin_) { origin = origin_;  }
    KVector2 Transform(const KVector2& input)
    {
        KVector2 t0 = input.x*axis0;
        KVector2 t1 = input.y*axis1;
        KVector2 temp(t0.x + t1.x + origin.x, t0.y + t1.y + origin.y);
        return temp;
    }
};
