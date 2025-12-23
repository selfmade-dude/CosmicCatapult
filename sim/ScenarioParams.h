#pragma once

#include "Vector2.h"

struct ScenarioParams
{
    Vector2 shipPosition = Vector2(0.0, 0.0);
    Vector2 shipVelocity = Vector2(0.0, 0.0);

    double dt = 0.1;

    bool clearTrajectoriesOnReset = true;

    bool autoAllignPlanetForAssist = false;
    int assistPlanetIndex = 0;
};
