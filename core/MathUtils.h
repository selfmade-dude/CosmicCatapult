#pragma once

#include <cmath>

namespace math
{
    constexpr double pi = 3.14159265358979323846;

    // Convert degrees -> radians
    constexpr double deg2rad(double deg)
    {
        return deg * (pi/180.0);
    }

    // Convert radians -> degrees
    constexpr double rad2deg(double rad)
    {
        return rad * (180.0 / pi);
    }
}