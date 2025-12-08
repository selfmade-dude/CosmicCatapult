#pragma once

#include <cmath>
#include "OrbitMath.h"
#include "State2D.h"
#include "Vector2.h"

// Compute gravitational acceleration at a given position
inline Vector2 gravitationalAcceleration(const Vector2 &position, double mu)
{
    const double r = radiusFromPosition(position);

    if (r == 0.0)
    {
        return Vector2(0.0,0.0);
    }

    const double r2 = r * r;
    const double factor = -mu / (r * r * r);

    return Vector2(factor * position.x, factor * position.y);
}

inline State2D derivatives(const State2D &state, double mu)
{
    State2D result;

    result.position = state.velocity;

    result.velocity = gravitationalAcceleration(state.position, mu);

    return result;
}