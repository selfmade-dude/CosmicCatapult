#include "MainWindow.h"
#include <cmath>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QWidget>

double MainWindow::timeScaleForSpeed(MainWindow::SimulationSpeed speed) const
{
    switch (speed)
    {
    case MainWindow::SimulationSpeed::VerySlow:
        return 3600.0;
    case MainWindow::SimulationSpeed::Slow:
        return 86400.0;
    case MainWindow::SimulationSpeed::Normal:
        return 604800.0;
    case MainWindow::SimulationSpeed::Fast:
        return 3024000.0;
    case MainWindow::SimulationSpeed::VeryFast:
        return 6048000.0;
    default:
        return 604800.0;
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Cosmic Catapult"));
    resize(800, 600);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    m_pauseButton = new QPushButton(tr("Pause"), this);

    speedComboBox_ = new QComboBox(this);
    speedComboBox_->addItem(tr("Very slow"), static_cast<int>(SimulationSpeed::VerySlow));
    speedComboBox_->addItem(tr("Slow"), static_cast<int>(SimulationSpeed::Slow));
    speedComboBox_->addItem(tr("Normal"), static_cast<int>(SimulationSpeed::Normal));
    speedComboBox_->addItem(tr("Fast"), static_cast<int>(SimulationSpeed::Fast));
    speedComboBox_->addItem(tr("Very fast"), static_cast<int>(SimulationSpeed::VeryFast));
    speedComboBox_->setCurrentIndex(2);

    x0Spin_ = new QDoubleSpinBox(this);
    x0Spin_->setRange(-1e9, 1e9);
    x0Spin_->setDecimals(2);
    x0Spin_->setValue(150000000.0);

    y0Spin_ = new QDoubleSpinBox(this);
    y0Spin_->setRange(-1e9, 1e9);
    y0Spin_->setDecimals(2);
    y0Spin_->setValue(0.0);

    v0Spin_ = new QDoubleSpinBox(this);
    v0Spin_->setRange(0.0, 1e6);
    v0Spin_->setDecimals(4);
    v0Spin_->setValue(40);

    fi0Spin_ = new QDoubleSpinBox(this);
    fi0Spin_->setRange(-360.0, 360.0);
    fi0Spin_->setDecimals(2);
    fi0Spin_->setValue(90.0);

    dtSpin_ = new QDoubleSpinBox(this);
    dtSpin_->setRange(1e-6, 1000.0);
    dtSpin_->setDecimals(6);
    dtSpin_->setValue(0.1);

    clearTrailsCheck_ = new QCheckBox(tr("Clear trails on init"), this);
    clearTrailsCheck_->setChecked(true);

    autoAlignPlanetCheck_ = new QCheckBox(tr("Auto-align planet (assist)"), this);
    autoAlignPlanetCheck_->setChecked(false);

    initButton_ = new QPushButton(tr("Initialize"), this);

    timeLabel_ = new QLabel(tr("Time: 0.00 yr"), this);
    timeLabel_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    speedLabel_ = new QLabel(tr("Speed: 0.00 km/s"), this);
    speedLabel_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    orbitView_ = new OrbitViewWidget(this);
    orbitView_->setMinimumHeight(400);
    orbitView_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //Layout

    //Left
    QWidget *leftPanel = new QWidget(central);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);

    leftLayout->addWidget(orbitView_, /*stretch*/ 1);

    leftLayout->setContentsMargins(0, 0, 0, 0);

    //Right
    QWidget *rightPanel = new QWidget(central);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(8);

    rightLayout->addWidget(new QLabel(tr("Speed:"), this));
    rightLayout->addWidget(speedComboBox_);

    rightLayout->addWidget(new QLabel(tr("x0"), this));
    rightLayout->addWidget(x0Spin_);

    rightLayout->addWidget(new QLabel(tr("y0"), this));
    rightLayout->addWidget(y0Spin_);

    rightLayout->addWidget(new QLabel(tr("V0"), this));
    rightLayout->addWidget(v0Spin_);

    rightLayout->addWidget(new QLabel(tr("Fi0 (deg)"), this));
    rightLayout->addWidget(fi0Spin_);

    rightLayout->addWidget(new QLabel(tr("dt (advanced)"), this));
    rightLayout->addWidget(dtSpin_);

    rightLayout->addWidget(clearTrailsCheck_);
    rightLayout->addWidget(autoAlignPlanetCheck_);
    rightLayout->addWidget(initButton_);

    rightLayout->addWidget(m_pauseButton);

    rightLayout->addWidget(timeLabel_);
    rightLayout->addWidget(speedLabel_);

    rightLayout->addStretch(1);

    QScrollArea *rightScroll = new QScrollArea(central);
    rightScroll->setWidgetResizable(true);
    rightScroll->setWidget(rightPanel);
    rightScroll->setFixedWidth(320);
    rightScroll->setMinimumWidth(320);
    rightScroll->setMaximumWidth(380);

    QHBoxLayout *rootLayout = new QHBoxLayout();
    rootLayout->addWidget(leftPanel, 3);
    rootLayout->addWidget(rightScroll, 1);
    rootLayout->setContentsMargins(4, 4, 4, 4);
    central->setLayout(rootLayout);

    State2 initialState;

    initialState.position = Vector2(150000000.0, 0.0);
    initialState.velocity = Vector2(0.0, 40);

    constexpr double MU_SUN = 1.32712440018e11;
    double mu = MU_SUN;
    double dt = 0.1;

    appModel_ = new AppModel(initialState, mu, dt);
    orbitView_->setAppModel(appModel_);
    orbitView_->setWorldBounds(-15000.0, 15000.0, -15000.0, 15000.0);

    m_timer = new QTimer(this);
    m_timer->setInterval(20);

    //Connecting widgets
    connect(m_timer, &QTimer::timeout, this, &MainWindow::onSimulationTick);

    m_timer->start();

    connect(speedComboBox_, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
    [this](int index)
    {
        const int value = speedComboBox_->itemData(index).toInt();
        simulationSpeed_ = static_cast<SimulationSpeed>(value);

        if (appModel_)
        {
            const double ts = timeScaleForSpeed(simulationSpeed_);
            appModel_->setTimeScale(ts);
        }
    });

    connect(initButton_, &QPushButton::clicked, 
            this,
            [this]()
            {
                ScenarioParams params;

                params.shipPosition = Vector2(x0Spin_->value(), y0Spin_->value());

                const double v0 = v0Spin_->value();
                const double fi0Deg = fi0Spin_->value();
                const double fi0Rad = math::deg2rad(fi0Deg);

                params.shipVelocity = Vector2(v0 * std::cos(fi0Rad), v0 * std::sin(fi0Rad));

                params.dt = dtSpin_->value();
                params.clearTrajectoriesOnReset = clearTrailsCheck_->isChecked();
                params.autoAlignPlanetForAssist = autoAlignPlanetCheck_->isChecked();
                params.assistPlanetIndex = 0;

                if (appModel_)
                {
                    appModel_->reset(params);
                }
            });

    connect(m_pauseButton, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
}

MainWindow::~MainWindow()
{
    delete appModel_;
}

void MainWindow::onSimulationTick()
{
    if (!appModel_)
    {
        return;
    }

    if (!isPaused_)
    {
        appModel_->update();
    }

    orbitView_->update();

    if (timeLabel_ && speedLabel_ && appModel_)
    {
        const double secondsPerYear = 365.0 * 24.0 * 3600.0;

        const double tSeconds = appModel_->time();
        const double tYears = tSeconds / secondsPerYear;

        const State2 &st = appModel_->state();
        const double v = std::sqrt(st.velocity.x * st.velocity.x + st.velocity.y * st.velocity.y);

        timeLabel_->setText(tr("Time: %1 yr").arg(tYears, 0, 'f', 2));
        speedLabel_->setText(tr("Speed: %2 km/s").arg(v, 0, 'f', 2));
    }
}

void MainWindow::onPauseClicked()
{
    if (!m_timer)
    {
        return;
    }

    isPaused_ = !isPaused_;

    if (isPaused_)
    {
        m_pauseButton->setText(tr("Resume"));
    }
    else
    {
        m_pauseButton->setText(tr("Pause"));
    }
}