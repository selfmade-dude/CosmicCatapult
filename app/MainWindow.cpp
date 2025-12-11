#include "MainWindow.h"
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Cosmic Catapult"));
    resize(800, 600);

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    m_stateLabel = new QLabel(central);
    m_stateLabel->setWordWrap(true);
    m_stateLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_pauseButton = new QPushButton(tr("Pause"), central);

    layout->addWidget(m_stateLabel);
    layout->addWidget(m_pauseButton);

    setCentralWidget(central);

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

    connect(m_pauseButton, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
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

    if (!isPaused_)
    {
        appModel_->update();
    }

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

void MainWindow::setOrbitText(const QString &text)
{
    if (m_stateLabel)
    {
        m_stateLabel->setText(text);
    }
}