#pragma once

class SimulationClock
{
public:
    explicit SimulationClock(double startTime = 0.0) : t_(startTime)
    {
    }

    void advance(double dt)
    {
        t_ += dt;
    }

    void reset(double newTime = 0.0)
    {
        t_ = newTime;
    }

    double time() const
    {
        return t_;
    }

private:
    double t_;
}