#pragma once

#include "../sim/SimulationModel.h"

class AppModel
{
public: 
    AppModel(const State2 &initialState, double muValue, double dtValue, IntegratorType integratorType = IntegratorType::RK4, std::size_t trajectoryMaxSize = 5000) : sim_(initialState, muValue, dtValue, integratorType, trajectoryMaxSize)
    {
    }

    void update()
    {
        sim_.update();
    }

    void reset(const State2 &newState)
    {
        sim_.reset(newState);
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

private:
    SimulationModel sim_;
};