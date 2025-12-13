#pragma once

#include <QWidget>
#include <vector>
#include "AppModel.h"
#include "ScreenSpaceConverter.h"

class OrbitViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OrbitViewWidget(QWidget *parent = nullptr);

    void setAppModel(AppModel *model);
    void setWorldBounds(double minX, double maxX, double minY, double maxY);
    void autoFitBounds(const std::vector<Vector2> &trajectory);


protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    AppModel *appModel_;
    ScreenSpaceConverter converter_;
};