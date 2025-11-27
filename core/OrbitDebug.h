#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include "MathUtils.h"
#include "OrbitState.h"

inline std::string orbitStateToString(const OrbitState &state)
{
    std::ostringstream out;

    // Configure numeric formatting
    out << std::fixed << std::setprecision(3);

    // Orbit type as text
    const char *typeText = "Elliptic";
    if (state.orbitType == OrbitType::Hyperbolic)
    {
        typeText = "Hyperbolic";
    }
    else if (state.orbitType == OrbitType::Parabolic)
    {
        typeText = "Parabolic";
    }

    // Convert true anomaly to degrees for easier reading
    const double trueAnomalyDeg = math::rad2deg(state.trueAnomaly);

    out << "Orbit State\n";
    out << "===========\n";
    out << "Type:             " << typeText << "\n\n";

    out << "Position  (x,y): (" << state.position.x << ", " << state.position.y << ")\n";
    out << "Velocity  (x,y): (" << state.velocity.x << ", " << state.velocity.y << ")\n";

    out << "Radius:          " << state.radius << " km\n";
    out << "Speed:           " << state.speed << " km/s\n";
    out << "Energy:          " << state.energy << " km^2/s^2\n";
    out << "Ang. momentum:   " << state.angularMomentum << "\n";

    out << "Semi-major axis: " << state.semiMajorAxis << "\n";
    out << "Eccentricity:    " << state.eccentricity << "\n";
    out << "Periapsis:       " << state.periapsis << "\n";
    out << "Apoapsis:        " << state.apoapsis << "\n";
    
    out << "Ecc. vector (x,y): (" 
        << state.eccentricityVec.x << ", " << state.eccentricityVec.y << ")\n";
    out << "True anomaly:    " << trueAnomalyDeg << " deg\n";

    return out.str();
}