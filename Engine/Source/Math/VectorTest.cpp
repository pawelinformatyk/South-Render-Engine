#include "sthpch.h"

namespace South
{

void Test()
{
    const Vector Vec;
    const Vector Vec2(1., 2, 4);
    const Vector Vec3 = Vec;
    const Vector Vec4 = Vec2;

    STH_INFO("MathTest End");
}

}