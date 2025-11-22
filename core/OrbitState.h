#pragma once

#include "Vector2.h"

// Represents the current orbital state of a spacecraft/particle.

struct OrbitState
{
    Vector2 position;      // [m]
    Vector2 velocity;      // [m/s]

    double radius = 0.0;   // |r| [m]
    double speed = 0.0;    // |v| [m/s]

    double angularMomentum = 0.0; // |h| = |r × v| (z-component) [m^2/s]

    double energy = 0.0;   // specific orbital energy [m^2/s^2]

    double semiMajorAxis = 0.0; // a [m], can be negative for hyperbolic orbits
    double eccentricity = 0.0;  // dimensionless
    Vector2 eccentricityVec; // full vector direction of periapsis
    double trueAnomaly = 0.0;   // [rad]

    // This will be expanded later when we add semimajor axis, periapsis distance, etc.
};