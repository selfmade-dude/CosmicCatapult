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
}

MainWindow::~MainWindow()
{
}

void MainWindow::setOrbitText(const QString &text)
{
    if (m_stateLabel)
    {
        m_stateLabel->setText(text);
    }
}