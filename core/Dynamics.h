#pragma once

#include <cmath>
#include "OrbitMath.h"
#include "State2.h"
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

inline State2 derivatives(const State2 &state, double mu)
{
    State2 result;

    result.position = state.velocity;

    result.velocity = gravitationalAcceleration(state.position, mu);

    return result;
}

inline State2 stepEuler(const State2 &state, double dt, double mu)
{
    State2 k = derivatives(state, mu);

    State2 result;

    result.position = state.position + k.position * dt;

    result.velocity = state.velocity + k.velocity * dt;

    return result;
}

inline State2 stepRK4(const State2 &state, double dt, double mu)
{
    const State2 k1 = derivatives(state, mu);

    State2 temp2;
    temp2.position = state.position + k1.position * (dt * 0.5);
    temp2.velocity = state.velocity + k1.velocity * (dt * 0.5);
    const State2 k2 = derivatives(temp2, mu);

    State2 temp3;
    temp3.position = state.position + k2.position * (dt * 0.5);
    temp3.velocity = state.velocity + k2.veloctiy * (dt * 0.5);
    const State2 k3 = derivatives(temp3, mu);

    State2 temp4;
    temp4.position = state.position + k3.position * dt;
    temp4.velocity = state.velocity + k3.velocity * dt;
    const State2 k4 = derivatives(temp4, mu);

    State2 result;
    result.position = state.position + (k1.position + k2.position * 2.0 + k3.position * 2.0 + k4.position) * (dt / 6.0);
    
    result.velocity = state.velocity + (k1.velocity + k2.velocity * 2.0 + k3.velocity * 2.0 + k4.velocity) * (dt / 6.0);

    return result;
}