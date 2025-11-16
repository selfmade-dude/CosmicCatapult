#include "MainWindow.h"
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("Cosmic Catapult"));
    resize(800, 600);
}

MainWindow::~MainWindow()
{
}