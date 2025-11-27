#pragma once

#include <QMainWindow>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setOrbitText(const QString &text);

private:
    QLabel *m_stateLabel = nullptr;
};