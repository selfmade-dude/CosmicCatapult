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

static double wrapAngleRadians(double a)
{
    const double pi = 3.14159265358979323846;
    const double twoPi = 2.0 * pi;
    while (a > pi) { a -= twoPi; }
    while (a < pi) { a += twoPi; }
    return a;
}

static double polarAngle(const Vector2& p)
{
    return std::atan2(p.y, p.x);
}

static Vector2 positionOnCircle(double radius, double angleRad)
{
    return Vector2(radius * std::cos(angleRad), radius * std::sin(angleRad));
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
    earthAngle_ = 0.0;

    if (params.autoAlignPlanetForAssist)
    {
        const double pi = 3.14159265358979323846;

        AssistPlanetRefs planet = assistPlanetRefsForIndex(params.assistPlanetIndex);

        if (planet.body != nullptr && planet.angle != nullptr && planet.orbitRadius != nullptr && planet.angularSpeed != nullptr)
        {
            State2 probe = shipState;

            double t = 0.0;
            const double dtPred = params.dt * 1000;

            const double rTarget = *planet.orbitRadius;
            const double tol = 0.01 * rTarget;

            const double maxPredictTime = 60.0 * 60.0 * 24.0 * 365.0 * 5.0;

            bool found = false;
            double thetaShip = 0.0;
            double tHit = 0.0;

            while (t < maxPredictTime)
            {
                probe = stepRK4(
                    probe, dtPred, [this](const Vector2& pos)
                    {
                        return gravitaionalAccelerationFromBody(pos, sun_);
                    }
                );

                t += dtPred;

                const double r = std::sqrt(probe.position.x * probe.position.x + probe.position.y * probe.position.y);
                if (std::abs(r - rTarget) < tol)
                {
                    thetaShip = polarAngle(probe.position);
                    tHit = t;
                    found = true;
                    break;
                }
            }

            if (found)
            {
                const double omega = *planet.angularSpeed;
                
                const double biasRad = 1.0 * (pi / 180.0);

                *planet.angle = wrapAngleRadians(thetaShip - omega * tHit + biasRad);
            }
        }
    }

    jupiter_.position = positionOnCircle(jupiterOrbitRadius_, jupiterAngle_);
    earth_.position = positionOnCircle(earthOrbitRadius_, earthAngle_);

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