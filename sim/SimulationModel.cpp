#include "SimulationModel.h"

#include <cmath>

SimulationModel::SimulationModel(const State2 &initialState, 
                                 double muValue, 
                                 double dtValue, 
                                 IntegratorType integratorType,
                                 std::size_t trajectoryMaxSize) 
    : controller_(initialState, muValue, dtValue, integratorType), 
      clock_(0.0), 
      trajectory_(trajectoryMaxSize)
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

void SimulationModel::update()
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

void SimulationModel::reset(const ScenarioParams &params)
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

const State2& SimulationModel::state() const
{
    return controller_.state();
}

double SimulationModel::time() const
{
    return clock_.time();
}

const std::vector<Vector2>& SimulationModel::trajectory() const
{
    return trajectory_.points();
}

const Body& SimulationModel::sun() const
{
    return sun_;
}

const Vector2& SimulationModel::sunPosition() const
{
    return sun_.position;
}

const Body& SimulationModel::jupiter() const
{
    return jupiter_;
}

const Vector2& SimulationModel::jupiterPosition() const
{
    return jupiter_.position;
}

    
const std::vector<Vector2>& SimulationModel::jupiterTrajectory() const
{
    return jupiterTrajectory_.points();
}

const Vector2& SimulationModel::earthPosition() const
{
    return earth_.position;
}

const std::vector<Vector2>& SimulationModel::earthTrajectory() const
{
    return earthTrajectory_.points();
}

double SimulationModel::dt() const
{
    return controller_.dt();
}

void SimulationModel::setDt(double newDt)
{
    controller_.setDt(newDt);
}

void SimulationModel::setMu(double newMu)
{
    controller_.setMu(newMu);
}

void SimulationModel::setIntegrator(IntegratorType type)
{
    controller_.setIntegrator(type);
}

double SimulationModel::timeScale() const
{
    return timeScale_;
}

void SimulationModel::setTimeScale(double newTimeScale)
{
    if (newTimeScale <= 0.0)
    {
        return;
    }

    timeScale_ = newTimeScale;
}

SimulationModel::AssistPlanetRefs SimulationModel::assistPlanetRefsForIndex(int index)
{
    if (index == 1)
    {
        AssistPlanetRefs refs;
        refs.body = &earth_;
        refs.angle = &earthAngle_;
        refs.orbitRadius = &earthOrbitRadius_;
        refs.angularSpeed = &earthAngularSpeed_;
        return refs;
    }

    AssistPlanetRefs refs;
    refs.body = &jupiter_;
    refs.angle = &jupiterAngle_;
    refs.orbitRadius = &jupiterOrbitRadius_;
    refs.angularSpeed = &jupiterAngularSpeed_;
    return refs;
}