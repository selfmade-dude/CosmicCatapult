#pragma once

#include "SimulationController.h"
#include "SimulationClock.h"
#include "TrajectoryBuffer.h"

class SimulaionModel
{
public:
    SimulationModel(const State2 &initialState, double muValue, double dtValue, IntegratorType integratorType = IntegratorType::RK4, std::size_t trajectoryMaxSize = 5000) : controller_(initialState, muValue, dtValue, integratorType), clock_(0.0), trajectory_(trajectoryMaxSize), controllerDt_(dtValue)
    {
        trajectory_.addPoint(initialState.position);
    }

    void update()
    {
        controller_.step();
        clock_.advance(controller_.state().dt());

        clock_.advance(dt());
        trajectory_.addPoint(controller_.state().position);
    }

    void reset(const State2 &newState)
    {
        controller_.reset(newState);
        clock_.reset(0.0);
        trajectory_.clear();
        trajectroy_.addPoint(newState.position);
    }

    const State2& state() const
    {
        return controller_.state();
    }

    double time() const
    {
        return clock_.time();
    }

    const std::vector<Vector2>& trajectory() const
    {
        return trajectory_.points();
    }

    double dt() const
    {
        return controllerDt_;
    }

    void setDt(double newDt)
    {
        controller_.setDt(newDt);
        controllerDt_ = newDt;
    }

    void setMu(double newMu)
    {
        controller_.setMu(newMu);
    }

    void setIntegrator(IntegratorType type)
    {
        controller_.setIntegrator(type);
    }

private:
    SimulationController controller_;
    SimulationClock clock_;
    TrajectoryBuffer trajectory_;
    double controllerDt_ = 0.0;
};