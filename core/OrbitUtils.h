#pragma once

#include "Vector2.h"
#include "OrbitState.h"

// A utility function that will compute an OrbitState from raw inputs.
// Right now it contains only the structure (no physics calculations yet).

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
    state.trueAnomaly = 0.0;

    return state;
}