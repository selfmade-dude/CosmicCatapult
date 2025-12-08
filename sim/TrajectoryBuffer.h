#pragma once

#include <vector>
#include "../core/Vector2.h"

class TrajectoryBuffer
{
public:
    explicit TrajectoryBuffer(std::size_t maxSize = 5000) : maxSize_(maxSize)
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