#pragma once

struct Vector2
{
    double x = 0.0;
    double y = 0.0;

    Vector2() = default;

    Vector2(double x_, double y_)
        : x(x_), y(y_)
    {}
};