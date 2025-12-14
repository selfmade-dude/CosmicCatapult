#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
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
    QComboBox *speedComboBox_ = nullptr;

    QDoubleSpinBox *x0Spin_ = nullptr;
    QDoubleSpinBox *y0Spin_ = nullptr;
    QDoubleSpinBox *v0Spin_ = nullptr;
    QDoubleSpinBox *fi0Spin_ = nullptr;
    QDoubleSpinBox *dtSpin_ = nullptr;

    QCheckBox *clearTrailsCheck_ = nullptr;
    QPushButton *initButton_ = nullptr;

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