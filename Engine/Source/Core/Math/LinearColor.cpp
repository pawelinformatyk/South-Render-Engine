#include "LinearColor.h"

namespace South
{

SLinearColor::SLinearColor(const double RGBA) : R(RGBA), G(RGBA), B(RGBA), A(RGBA)
{
}

SLinearColor::SLinearColor(const double R, const double G, const double B, const double A) : R(R), G(G), B(B), A(A)
{
}

SLinearColor SLinearColor::Rand()
{
    // #TODO: Random utils

    const double X = std::rand() / RAND_MAX;
    const double Y = std::rand() / RAND_MAX;
    const double Z = std::rand() / RAND_MAX;
    const double A = std::rand() / RAND_MAX;

    return {X, Y, Z, A};
}

} // namespace South