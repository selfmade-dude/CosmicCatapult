#pragma once

#include <vector>
#include <cmath>
#include "SimulationController.h"
#include "SimulationClock.h"
#include "TrajectoryBuffer.h"
#include "../core/Body.h"

class SimulationModel
{
public:
    SimulationModel(const State2 &initialState, double muValue, double dtValue, IntegratorType integratorType = IntegratorType::RK4, std::size_t trajectoryMaxSize = 5000) : controller_(initialState, muValue, dtValue, integratorType), clock_(0.0), trajectory_(trajectoryMaxSize), controllerDt_(dtValue)
    {
        trajectory_.addPoint(initialState.position);

        sun_.position = Vector2(0.0, 0.0);
        sun_.mu = muValue;

        jupiter_.mu = muValue * 0.001;
        jupiter_.position = Vector2(jupiterOrbitRadius_, 0.0);
    }

    void update()
    {
        jupiterAngle_ += jupiterAngularSpeed_ * dt();

        jupiter_.position.x = jupiterOrbitRadius_ * std::cos(jupiterAngle_);
        jupiter_.position.y = jupiterOrbitRadius_ * std::sin(jupiterAngle_);

        controller_.stepWithAcceleration([this](const Vector2 &pos)
        {
            const Vector2 aSun = gravitaionalAccelerationFromBody(pos, sun_);
            const Vector2 aJupiter = gravitaionalAccelerationFromBody(pos, jupiter_);
            return aSun + aJupiter;
        });

        clock_.advance(dt());
        trajectory_.addPoint(controller_.state().position);
    }

    void reset(const State2 &newState)
    {
        controller_.reset(newState);
        clock_.reset(0.0);
        trajectory_.clear();
        trajectory_.addPoint(newState.position);

        jupiterAngle_ = 0.0;
        jupiter_.position = Vector2(jupiterOrbitRadius_, 0.0);
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

    const Body& sun() const
    {
        return sun_;
    }

    const Vector2& sunPosition() const
    {
        return sun_.position;
    }

    const Body& jupiter() const
    {
        return jupiter_;
    }

    const Vector2& jupiterPosition() const
    {
        return jupiter_.position;
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

    Body sun_;
    Body jupiter_;

    double jupiterAngle_ = 0.0;
    double jupiterOrbitRadius_ = 12000.0;
    double jupiterAngularSpeed_ = 0.00005;
};