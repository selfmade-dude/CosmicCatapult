#pragma once

#include <string>
#include <sstream>
#include "OrbitState.h"

inline std::string orbitStateToString(const OrbitState &state)
{
    std::ostringstream out;

    out << "OrbitState:\n";
    out << " position:        (" << state.position.x << ", " << state.position.y << ")\n";
    out << " velocity:        (" << state.velocity.x << ", " << state.velocity.y << ")\n";
    out << " radius:          " << state.radius << "\n";
    out << " speed:           " << state.speed << "\n";
    out << " energy:          " << state.energy << "\n";
    out << " angularMomentum: " << state.angularMomentum << "\n";
    out << " eccentricity:    " << state.eccentricity << "\n";
    out << " eccentricityVec: (" << state.eccentricityVec.x << ", " << state.eccentricityVec.y << ")\n";
    out << " semiMajorAxis:   " << state.semiMajorAxis << "\n";
    out << " trueAnomaly:     " << state.trueAnomaly << "\n";

    return out.str();
}