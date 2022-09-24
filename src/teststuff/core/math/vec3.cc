#include "Vec3.h"

void Vec3::lerp(const Vec3 &v1, const Vec3 &v2, const float l)
{
    if (l <= 0.0f)
        (*this) = v1;
    else if (l >= 1.0f)
        (*this) = v2;
    else
        (*this) = v1 + l * (v2 - v1);
}
