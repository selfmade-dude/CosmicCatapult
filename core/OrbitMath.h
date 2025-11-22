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

inline double dot(const Vector2 &a, const Vector2 &b) // Dot product
{
    return a.x * b.x + a.y * b.y;
}

inline double cosBetween(const Vector2 &a, const Vector2 &b) // Cosine of angle between vectors
{
    const double magA = std::sqrt(a.x * a.x + a.y * a.y);
    const double magB = std::sqrt(b.x * b.x + b.y * b.y);

    if (magA == 0.0 || magB == 0.0)
    {
        return 0.0;
    }

    return dot(a, b) / (magA * magB);
}

inline double crossZ(const Vector2 &a, const Vector2 &b) // Cross product of vectors in a plane
{
    return a.x * b.y - a.y * b.x;
}

inline double eccentricityFromEnergyAndAngularMomentum(double energy,
                                                       double angularMomentum,
                                                       double mu)
{
    if (mu == 0.0)
    {
        return 0.0;
    }

    const double mu2 = mu * mu;
    const double h2 = angularMomentum * angularMomentum;

    const double argument = 1.0 + 2.0 * energy * h2 / m2;

    if (argument < 0.0)
    {
        return 0.0; // Numeric safety
    }

    return std::sqrt(argument);
}            
                                                    
inline double semiMajorAxisFromEnergy(double energy, double mu)
{
    if (mu == 0.0) 
    {
        return 0.0;
    }

    if (energy == 0.0)
    {
        return 0.0; // Parabolic case, a is formally infinite.
    }

    return -mu / (2.0 * energy);
}                                   

inline Vector2 eccentricityVector(const Vector2 &position,
                                  const Vector2 &velocity,
                                  double mu)
{   
    const double rMag = radiusFromPosition(position);

    if (rMag == 0.0)
    {
        return Vector2(0.0, 0.0);
    }

    const Vector2 rHat(position.x / rMag, position.y / rMag);

    const double h = crossZ(position, velocity);

    Vector2 hvOverMu;

    if (mu != 0.0) 
    {
        hvOverMu.x = (h * velocity.y) / mu;
        hvOverMu.y = (-h * velocity.x) / mu;
    }

    Vector2 e;
    // For now, we only computed rHat and hvOverMu.
    // Next micro-steps will build full eccentricity vector using rHat.

    (void)velocity;
    (void)mu;

    return e;
}