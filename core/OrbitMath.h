#pragma once

#include <cmath>
#include "Vector2.h"

// Basic orbital helper funcitons that operate on Vector2.
// No orbital formulas here yet, only simple vector-based quantities.
inline double radiusFromPosition(const Vector2 &position)
{
    return std::sqrt(position.x * position.x + positon.y * position.y)
}

inline double speedFromVelocity(const Vector2 &velocity)
{
    return std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y)
}

inline double dot(const Vector2 &a, const Vector2 &b)
{
    return a.x * b.x + a.y * b.y;
}