#pragma once

// Represents a celestial body (planet, spacecraft, etc.)
struct Body
{
    double mass = 0.0;      // [kg]
    double radius = 0.0;    // [m]
    double mu = 0.0;        // gravitational parameter GM [m^3/s^2]
};