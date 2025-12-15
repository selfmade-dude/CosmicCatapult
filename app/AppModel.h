#pragma once

#include "../sim/SimulationModel.h"
#include "../sim/ScenarioParams.h"

class AppModel
{
public: 
    AppModel(const State2 &initialState, double muValue, double dtValue, IntegratorType integratorType = IntegratorType::RK4, std::size_t trajectoryMaxSize = 100000) : sim_(initialState, muValue, dtValue, integratorType, trajectoryMaxSize)
    {
    }

    void update()
    {
        sim_.update();
    }

    void reset(const ScenarioParams &params)
    {
        sim_.reset(params);
    }

    void setDt(double newDt)
    {
        sim_.setDt(newDt);
    }

    void setMu(double newMu)
    {
        sim_.setMu(newMu);
    }

    void setIntegrator(IntegratorType type)
    {
        sim_.setIntegrator(type);
    }

    double timeScale() const
    {
        return sim_.timeScale();
    }

    void setTimeScale(double newTimeScale)
    {
        sim_.setTimeScale(newTimeScale);
    }

    const State2& state() const
    {
        return sim_.state();
    }

    double time() const
    {
        return sim_.time();
    }

    const std::vector<Vector2>& trajectory() const
    {
        return sim_.trajectory();
    }

    const Vector2& sunPosition() const
    {
        return sim_.sunPosition();
    }

    const Vector2& jupiterPosition() const
    {
        return sim_.jupiterPosition();
    }

    const std::vector<Vector2>& jupiterTrajectory() const
    {
        return sim_.jupiterTrajectory();
    }

    const Vector2& earthPosition() const
    {
        return sim_.earthPosition();
    }

    const std::vector<Vector2>& earthTrajectory() const
    {
        return sim_.earthTrajectory();
    }

private:
    SimulationModel sim_;
};