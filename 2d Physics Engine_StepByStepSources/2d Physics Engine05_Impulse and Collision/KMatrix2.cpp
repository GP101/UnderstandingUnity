#include "KMatrix2.h"

static KMatrix2 zero = KMatrix2( 0.0f, 0.0f, 0.0f, 0.0f );
static KMatrix2 identity = KMatrix2( 1.0f, 0.0f, 0.0f, 1.0f);

KMatrix2 KMatrix2::GetInverse() const
{
    KMatrix2 t;
    t.Set(_11, -_01, -_10, _00);
    float det = GetDeterminant();
    t = (1.0f / det) * t;
    return t;
}
