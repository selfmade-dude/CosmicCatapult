#include "OrbitViewWidget.h"

#include <QPainter>
#include <QResizeEvent>
#include <QColor>

OrbitViewWidget::OrbitViewWidget(QWidget *parent) : QWidget(parent), appModel_(nullptr)
{
}

void OrbitViewWidget::setAppModel(AppModel *model)
{
    appModel_ = model;
}

void OrbitViewWidget::setWorldBounds(double minX, double maxX, double minY, double maxY)
{
    converter_.setWorldBounds(minX, maxX, minY, maxY);
    update();
}

void OrbitViewWidget::autoFitBounds(const std::vector<Vector2> &trajectory)
{
    if (trajectory.empty())
    {
        return;
    }

    double minX = trajectory[0].x;
    double maxX = trajectory[0].x;
    double minY = trajectory[0].y;
    double maxY = trajectory[0].y;

    for (const Vector2 &p : trajectory)
    {
        if (p.x < minX) minX = p.x;
        if (p.x > maxX) maxX = p.x;
        if (p.y < minY) minY = p.y;
        if (p.y > maxY) maxY = p.y;
    }

    double dx = (maxX - minX) * 0.10;
    double dy = (maxY - minY) * 0.10;

    minX -= dx;
    maxX += dx;
    minY -= dy;
    maxY += dy;

    setWorldBounds(minX, maxX, minY, maxY);
}

void OrbitViewWidget::resizeEvent(QResizeEvent *event)
{
    converter_.setScreenSize(width(), height());
    QWidget::resizeEvent(event);
}

void OrbitViewWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.fillRect(rect(), QColor(10, 10, 30));

    if (!appModel_)
    {
        return;
    }

    painter.setPen(QPen(QColor(80, 80, 80), 1));

    const ScreenPoint originScreen = converter_.toScreen(Vector2(0.0, 0.0));
    painter.drawLine(QPointF(0.0, originScreen.y), QPointF(width(), originScreen.y));
    painter.drawLine(QPointF(originScreen.x, 0.0), QPointF(originScreen.x, height()));

    //Sun
    const Vector2 sunWorld = appModel_->sunPosition();
    const ScreenPoint sunScreen = converter_.toScreen(sunWorld);

    painter.setBrush(QBrush(Qt::yellow));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(sunScreen.x, sunScreen.y), 6.0, 6.0);

    //Jupiter
    const Vector2 jupiterWorld = appModel_->jupiterPosition();
    const ScreenPoint jupiterScreen = converter_.toScreen(jupiterWorld);

    painter.setBrush(QBrush(QColor(255, 165, 0))); 
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(jupiterScreen.x, jupiterScreen.y), 5.0, 5.0);

    const std::vector<Vector2> &jupiterTraj = appModel_->jupiterTrajectory();
    if (!jupiterTraj.empty())
    {
        QPen jupiterPen(QColor(255, 165, 0));
        jupiterPen.setWidth(2);
        painter.setPen(jupiterPen);

        QPointF prev;
        bool first = true;

        for (const Vector2 &p : jupiterTraj)
        {
            if (TrajectoryBuffer::isBreakPoint(p))
            {
                first = true;
                continue;
            }
            
            const ScreenPoint sp = converter_.toScreen(p);
            const QPointF pt(sp.x, sp.y);

            if (!first)
            {
                painter.drawLine(prev, pt);
            }

            prev = pt;
            first = false;
        }
    }

    //Earth
    const Vector2 earthWorld = appModel_->earthPosition();
    const ScreenPoint earthScreen = converter_.toScreen(earthWorld);

    painter.setBrush(QBrush(QColor(100, 170, 255))); 
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(earthScreen.x, earthScreen.y), 4.0, 4.0);

    const std::vector<Vector2> &earthTraj = appModel_->earthTrajectory();
    if (!earthTraj.empty())
    {
        QPen earthPen(QColor(100, 170, 255));
        earthPen.setWidth(2);
        painter.setPen(earthPen);

        QPointF prev;
        bool first = true;

        for (const Vector2 &p : earthTraj)
        {
            if (TrajectoryBuffer::isBreakPoint(p))
            {
                first = true;
                continue;
            }

            const ScreenPoint sp = converter_.toScreen(p);
            const QPointF pt(sp.x, sp.y);

            if (!first)
            {
                painter.drawLine(prev, pt);
            }

            prev = pt;
            first = false;
        }
    }

    //Ship
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
        if (TrajectoryBuffer::isBreakPoint(p))
        {
            first = true;
            continue;
        }

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

