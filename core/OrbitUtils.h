#pragma once

#include <cmath>
#include "Vector2.h"
#include "OrbitMath.h"
#include "OrbitState.h"

// A utility function that will compute an OrbitState from raw inputs.

inline OrbitState makeOrbitState(const Vector2 &position,
                                 const Vector2 &velocity,
                                double mu)
{
    OrbitState state;

    // Fill raw values
    state.position = position;
    state.velocity = velocity;

    // These will be computed later using physics formulas
    state.radius = radiusFromPosition(position);
    state.speed = speedFromVelocity(velocity);

    state.energy = (state.speed * state.speed) / 2.0 - mu / state.radius; // Compute specific orbital energy: ε = v²/2 − μ/r
    state.angularMomentum = std::abs(crossZ(position, velocity));

    state.semiMajorAxis = semiMajorAxisFromEnergy(state.energy, mu);

    state.eccentricity = eccentricityFromEnergyAndAngularMomentum(
        state.energy,
        state.angularMomentum,
        mu
    );
    state.eccentricityVec = eccentricityVector(position, velocity, mu);

    state.periapsis = state.semiMajorAxis * (1.0 - state.eccentricity);
    state.apoapsis = state.semiMajorAxis * (1.0 + state.eccentricity);

    double cosTrueAnomaly = 0.0;
    const double denom = state.eccentricity * state.radius;
    if (denom != 0.0)
    {
        cosTrueAnomaly = dot(state.eccentricityVec, position) / denom;
    }
    if (cosTrueAnomaly > 1.0) cosTrueAnomaly = 1.0;
    if (cosTrueAnomaly < -1.0) cosTrueAnomaly = -1.0;
    state.trueAnomaly = std::acos(cosTrueAnomaly);

    return state;
}