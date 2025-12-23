#pragma once

#include <vector>
#include "SimulationController.h"
#include "SimulationClock.h"
#include "TrajectoryBuffer.h"
#include "ScenarioParams.h"
#include "../core/Body.h"
#include "../core/MathUtils.h"

constexpr double AU_KM = 149597870.7;
constexpr double MU_SUN = 1.32712440018e11;

class SimulationModel
{
public:
    explicit SimulationModel(
        const State2 &initialState, 
        double muValue, 
        double dtValue, 
        IntegratorType integratorType = IntegratorType::RK4, 
        std::size_t trajectoryMaxSize = 5000);

    void update();

    void reset(const ScenarioParams &params);

    const State2& state() const;

    double time() const;

    const std::vector<Vector2>& trajectory() const;

    const Body& sun() const;
    const Vector2& sunPosition() const;

    const Body& jupiter() const;
    const Vector2& jupiterPosition() const;
    const std::vector<Vector2>& jupiterTrajectory() const;

    const Vector2& earthPosition() const;
    const std::vector<Vector2>& earthTrajectory() const;

    double dt() const;
    void setDt(double newDt);

    double timeScale() const;
    void setTimeScale(double newTimeScale);

    void setMu(double newMu);

    void setIntegrator(IntegratorType type);

private:
    SimulationController controller_;
    SimulationClock clock_;
    TrajectoryBuffer trajectory_;
    double timeScale_ = 360000.0;

    Body sun_;
    Body jupiter_;

    double jupiterAngle_ = 0.0;
    double jupiterOrbitRadius_ = 5.204* AU_KM;
    double jupiterAngularSpeed_ = 0.0;
    TrajectoryBuffer jupiterTrajectory_;

    Body earth_;

    double earthAngle_ = 0.0;
    double earthOrbitRadius_ = 1.0 * AU_KM;
    double earthAngularSpeed_ = 0.0;
    TrajectoryBuffer earthTrajectory_;

    struct AssistPlanetRefs
    {
        Body* body = nullptr;
        double* angle = nullptr;
        double* orbitRadius = nullptr;
        double* angularSpeed = nullptr;
    };

    AssistPlanetRefs assistPlanetRefsForIndex(int index);
};