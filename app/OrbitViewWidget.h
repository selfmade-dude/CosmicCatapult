#pragma once

#include <QWidget>
#include <QPainter>
#include <vector>
#include "AppModel.h"
#include "ScreenSpaceConverter.h"

class OrbitViewWidget : public QWidget
{
public:
    explicit OrbitViewWidget(QWidget *parent = nullptr) : QWidget(parent), appModel_(nullptr)
    {

    }

    void setAppModel(AppModel *model)
    {
        appModel_ = model;
    }

    void setWorldBounds(double minX, double maxX, double minY, double maxY)
    {
        converter_.setWorldBounds(minX, maxX, minY, maxY);
        update();
    }

protected:
    void resizeEvent(QResizeEvent *event) override
    {
        converter_.setScreenSize(width(), height());
        QWidget::resizeEvent(event);
    }

    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

        painter.fillRect(rect(), QColor(10, 10, 30));

        if (!appModel_)
        {
            return;
        }

        const std::vector<Vector2> &traj = appModel_->trajectory();
        if (traj.empty())
        {
            return;
        }

        QPen pen(Qt::cyan);
        pen.setWidth(2);
        painter.setPen(pen);

        QPointF prevPoint;
        bool first = true;

        for (const Vector2 &p : traj)
        {
            ScreenPoint sp = converter_.toScreen(p);
            QPointF screenPt(sp.x, sp.y);

            if (!first)
            {
                painter.drawLine(prevPoint, screenPt);
            }

            prevPoint = screenPt;
            first = false;
        }

        const State2 &st = appModel_->state();
        ScreenPoint sp = converter_.toScreen(st.position);

        painter.setBrush(Qt::yellow);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPointF(sp.x, sp.y), 4.0, 4.0);
    }

private:
    AppModel *appModel_;
    ScreenSpaceConverter converter_;
};