#pragma once

#include "../core/State2.h"
#include "../core/Dynamics.h"
#include <functional>

enum class IntegratorType
{
    RK4,
    Euler
};

class SimulationController
{
public:
    SimulationController(const State2 &initialState, double muValue, double dtValue, IntegratorType type = IntegratorType::RK4) : state_(initialState), mu_(muValue), dt_(dtValue), integrator_(type)
    {
    }

    void step()
    {
        switch (integrator_)
        {
        case IntegratorType::Euler:
            state_ = stepEuler(state_, dt_, mu_);
            break;
        case IntegratorType::RK4:
        default:
            state_ = stepRK4(state_, dt_, mu_);
            break;
        }
    }

    void stepWithAcceleration(const std::function<Vector2(const Vector2&)> &accel)
    {
        switch (integrator_)
        {
        case IntegratorType::Euler:
            state_ = stepEuler(state_, dt_, accel);
            break;
        case IntegratorType::RK4:
        default:
            state_ = stepRK4(state_, dt_, accel);
            break;
        }
    }

    void reset(const State2 &newState)
    {
        state_ = newState;
    }

    const State2& state() const
    {
        return state_;
    }

    void setDt(double newDt)
    {
        dt_ = newDt;
    }

    double dt() const
    {
        return dt_;
    }

    void setMu(double newMu)
    {
        mu_ = newMu;
    }

    void setIntegrator(IntegratorType type)
    {
        integrator_ = type;
    }

    IntegratorType integrator() const
    {
        return integrator_;
    }

private:
    State2 state_;
    double mu_;
    double dt_;
    IntegratorType integrator_;
};