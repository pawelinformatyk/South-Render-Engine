#pragma once

namespace South
{

// #TODO: Check tranparency
class SLinearColor
{
public:
    double R = 0;
    double G = 0;
    double B = 0;
    double A = 0;

    SLinearColor() = default;
    SLinearColor(double RGBA);
    SLinearColor(double R, double G, double B, double A = 1);

    static SLinearColor Rand();
};

} // namespace South