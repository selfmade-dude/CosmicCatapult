#pragma once

#include <vector>
#include <limits>
#include <cmath>
#include "../core/Vector2.h"

class TrajectoryBuffer
{
public:
    explicit TrajectoryBuffer(std::size_t maxSize = 1000000) : maxSize_(maxSize)
    {
    }

    void addPoint(const Vector2 &p)
    {
        if (maxSize_ > 0 && points_.size() >= maxSize_)
        {
            points_.erase(points_.begin());
        }

        points_.push_back(p);
    }

    void clear()
    {
        points_.clear();
    }

    void addBreak()
    {
        const double nanValue = std::numeric_limits<double>::quiet_NaN();
        const Vector2 breakPoint(nanValue, nanValue);
        addPoint(breakPoint);
    }

    static bool isBreakPoint(const Vector2 &p)
    {
        return std::isnan(p.x) || std::isnan(p.y);
    }

    const std::vector<Vector2>& points() const
    {
        return points_;
    }

    void setMaxSize(std::size_t newMaxSize)
    {
        maxSize_ = newMaxSize;

        if (maxSize_ > 0 && points_.size() > maxSize_)
        {
            std::size_t excess = points_.size() - maxSize_;
            points_.erase(points_.begin(), points_.begin() + excess);
        }
    }

private:
    std::vector<Vector2> points_;
    std::size_t maxSize_;
};