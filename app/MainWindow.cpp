#include "MainWindow.h"
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("Cosmic Catapult"));
    resize(800, 600);

    m_stateLabel = new QLabel(this);

    m_stateLabel->setWordWrap(true);
    m_stateLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    setCentralWidget(m_stateLabel);

    State2 initialState;

    initialState.position = Vector2(7000.0, 0.0);
    initialState.velocity = Vector2(0.0, 7.5);

    double mu = 398600.4418;
    double dt = 0.1;

    appModel_ = new AppModel(initialState, mu, dt);

    const State2 &st = appModel_->state();
    
    m_stateLabel->setText(
        QString("Initial position: (%1, %2)\nVelocityi: (%3, %4)")
            .arg(st.position.x)
            .arg(st.position.y)
            .arg(st.velocity.x)
            .arg(st.velocity.y)
    );

    m_timer = new QTimer(this);
    m_timer->setInterval(50);

    connect(m_timer, &QTimer::timeout, this, &MainWindow::onSimulationTick);

    m_timer->start();
}

MainWindow::~MainWindow()
{
    delete appModel_;
}

void MainWindow::onSimulationTick()
{
    if (!appModel_ || !m_stateLabel)
    {
        return;
    }

    appModel_->update();

    const State2 &st = appModel_->state();
    double t = appModel_->time();

    m_stateLabel->setText(
        QString("t = %1 s\n"
                "position: (%2, %3)\n"
                "velocity: (%4, %5)")
            .arg(t, 0, 'f',2)
            .arg(st.position.x, 0, 'f', 2)
            .arg(st.position.y, 0, 'f', 2)
            .arg(st.velocity.x, 0, 'f', 4)
            .arg(st.velocity.y, 0, 'f', 4)
    );
}

void MainWindow::setOrbitText(const QString &text)
{
    if (m_stateLabel)
    {
        m_stateLabel->setText(text);
    }
}