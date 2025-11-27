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