#pragma once

#include "../core/Vector2.h"

struct ScreenPoint
{
    double x;
    double y;
};

class ScreenSpaceConverter
{
public:
    ScreenSpaceConverter()
        : worldMinX_(-1.0),
          worldMaxX_(1.0),
          worldMinY_(-1.0),
          worldMaxY_(1.0),
          screenWidth_(800),
          screenHeight_(600)
    {
    }

    void setWorldBounds(double minX, double maxX, double minY, double maxY)
    {
        worldMinX_ = minX;
        worldMaxX_ = maxX;
        worldMinY_ = minY;
        worldMaxY_ = maxY;
    }

    void setScreenSize(int width, int height)
    {
        screenWidth_ = width;
        screenHeight_ = height;
    }

    ScreenPoint toScreen(const Vector2 &world) const
    {
        ScreenPoint result;
        result.x = 0.0;
        result.y = 0.0;

        if (screenWidth_ <= 0 || screenHeight_ <= 0)
        {
            return result;
        }

        const double worldWidth = worldMaxX_ - worldMinX_;
        const double worldHeight = worldMaxY_ - worldMinY_;

        if (worldWidth <= 0.0 || worldHeight <= 0.0)
        {
            result.x = static_cast<double>(screenWidth_) * 0.5;
            result.y = static_cast<double>(screenHeight_) * 0.5;
            return result;
        }

        const double scaleX = static_cast<double>(screenWidth_) / worldWidth;
        const double scaleY = static_cast<double>(screenHeight_) / worldHeight;

        const double scale = (scaleX < scaleY) ? scaleX : scaleY;

        const double worldCenterX = 0.5 * (worldMinX_ + worldMaxX_);
        const double worldCenterY = 0.5 * (worldMinY_ + worldMaxY_);

        const double screenCenterX = static_cast<double>(screenWidth_) * 0.5;
        const double screenCenterY = static_cast<double>(screenHeight_) * 0.5;

        const double dx = world.x - worldCenterX;
        const double dy = world.y - worldCenterY;

        result.x = screenCenterX + dx * scale;
        result.y = screenCenterY - dy * scale;

        return result;
    }

private:
    double worldMinX_;
    double worldMaxX_;
    double worldMinY_;
    double worldMaxY_;

    int screenWidth_;
    int screenHeight_;
};