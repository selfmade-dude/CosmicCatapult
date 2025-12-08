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

inline constexpr Vector2 operator*(const Vector2 &v, double s)
{
    return Vector2{ v.x * s, v.y * s };
}

inline constexpr Vector2 operator/(const Vector2 &v, double s)
{
    return Vector2{ v.x / s, v.y / s };
}

inline constexpr Vector2 operator+(const Vector2 &a, const Vector2 &b)
{
    return Vector2{ a.x + b.x, a.y + b.y };
}

inline constexpr Vector2 operator-(const Vector2 &a, const Vector2 &b)
{
    return Vector2{ a.x - b.x, a.y - b.y };
}

inline constexpr Vector2 operator+(const Vector2 &v, double s)
{
    return Vector2{ v.x + s, v.y + s };
}