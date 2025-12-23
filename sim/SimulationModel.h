#pragma once

#include <vector>
#include <cmath>
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
    SimulationModel(const State2 &initialState, double muValue, double dtValue, IntegratorType integratorType = IntegratorType::RK4, std::size_t trajectoryMaxSize = 5000) : controller_(initialState, muValue, dtValue, integratorType), clock_(0.0), trajectory_(trajectoryMaxSize)
    {
        trajectory_.addPoint(initialState.position);

        sun_.position = Vector2(0.0, 0.0);
        sun_.mu = MU_SUN;

        jupiter_.mu = 1.26686534e8;
        jupiter_.position = Vector2(jupiterOrbitRadius_, 0.0);
        jupiterAngularSpeed_ = std::sqrt(sun_.mu / (jupiterOrbitRadius_ * jupiterOrbitRadius_ * jupiterOrbitRadius_));

        earth_.position = Vector2(earthOrbitRadius_, 0.0);
        earthAngularSpeed_ = std::sqrt(sun_.mu / (earthOrbitRadius_ * earthOrbitRadius_ * earthOrbitRadius_));

        earthTrajectory_.addPoint(earth_.position);
        jupiterTrajectory_.addPoint(jupiter_.position);
    }

    void update()
    {
        const double dtEff = dt() * timeScale_;

        jupiterAngle_ += jupiterAngularSpeed_ * dtEff;

        jupiter_.position.x = jupiterOrbitRadius_ * std::cos(jupiterAngle_);
        jupiter_.position.y = jupiterOrbitRadius_ * std::sin(jupiterAngle_);

        earthAngle_ += earthAngularSpeed_ * dtEff;

        earth_.position.x = earthOrbitRadius_ * std::cos(earthAngle_);
        earth_.position.y = earthOrbitRadius_ * std::sin(earthAngle_);


        const double originalDt = controller_.dt();
        controller_.setDt(dtEff);
        controller_.stepWithAcceleration([this](const Vector2 &pos)
        {
            const Vector2 aSun = gravitaionalAccelerationFromBody(pos, sun_);
            const Vector2 aJupiter = gravitaionalAccelerationFromBody(pos, jupiter_);
            return aSun + aJupiter;
        });
        controller_.setDt(originalDt);

        clock_.advance(dtEff);
        trajectory_.addPoint(controller_.state().position);
        earthTrajectory_.addPoint(earth_.position);
        jupiterTrajectory_.addPoint(jupiter_.position);
    }

    void reset(const ScenarioParams &params)
    {
        State2 shipState;
        shipState.position = params.shipPosition;
        shipState.velocity = params.shipVelocity;

        controller_.setDt(params.dt);

        controller_.reset(shipState);
        clock_.reset(0.0);

        jupiterAngle_ = 0.0;
        jupiter_.position = Vector2(jupiterOrbitRadius_, 0.0);

        earthAngle_ = 0.0;
        earth_.position = Vector2(earthOrbitRadius_, 0.0);

        if (params.clearTrajectoriesOnReset)
        {
            trajectory_.clear();
            earthTrajectory_.clear();
            jupiterTrajectory_.clear();
        }
        else 
        {
            trajectory_.addBreak();
            earthTrajectory_.addBreak();
            jupiterTrajectory_.addBreak();
        }

        trajectory_.addPoint(shipState.position);
        earthTrajectory_.addPoint(earth_.position);
        jupiterTrajectory_.addPoint(jupiter_.position);
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

    
    const std::vector<Vector2>& jupiterTrajectory() const
    {
        return jupiterTrajectory_.points();
    }

    const Vector2& earthPosition() const
    {
        return earth_.position;
    }

    const std::vector<Vector2>& earthTrajectory() const
    {
        return earthTrajectory_.points();
    }

    double dt() const
    {
        return controller_.dt();
    }

    void setDt(double newDt)
    {
        controller_.setDt(newDt);
    }

    void setMu(double newMu)
    {
        controller_.setMu(newMu);
    }

    void setIntegrator(IntegratorType type)
    {
        controller_.setIntegrator(type);
    }

    double timeScale() const
    {
        return timeScale_;
    }

    void setTimeScale(double newTimeScale)
    {
        if (newTimeScale <= 0.0)
        {
            return;
        }

        timeScale_ = newTimeScale;
    }

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
        double* andgularSpeed = nullptr;
    };

    AssistPlanetRefs assistPlanetRefsForIndex(int index)
    {
        if (index == 1)
        {
            AssistPlanetRefs refs;
            refs.body = &earth_;
            refs.angle = &earthAngle_;
            refs.orbitRadius = &earthOrbitRadius_;
            refs.andgularSpeed = &earthAngularSpeed_;
            return refs;
        }

        AssistPlanetRefs refs;
        refs.body = &jupiter_;
        refs.angle = &jupiterAngle_;
        refs.orbitRadius = &jupiterOrbitRadius_;
        refs.andgularSpeed = &jupiterAngularSpeed_;
        return refs;
    }
};