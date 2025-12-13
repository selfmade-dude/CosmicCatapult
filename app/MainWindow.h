#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include "AppModel.h"
#include "OrbitViewWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setOrbitText(const QString &text);

private slots:
    void onSimulationTick();
    void onPauseClicked();

private:
    QLabel *m_stateLabel = nullptr;
    QPushButton *m_pauseButton = nullptr;
    AppModel *appModel_ = nullptr;
    QTimer *m_timer = nullptr;
    OrbitViewWidget *orbitView_ = nullptr;

    enum class SimulationSpeed
    {
        VerySlow,
        Slow,
        Normal,
        Fast,
        VeryFast
    };

    SimulationSpeed simulationSpeed_ = SimulationSpeed::Normal;

    int stepsPerTickForSpeed(SimulationSpeed speed) const;

    bool isPaused_ = false;
};