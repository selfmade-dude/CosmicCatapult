#include <iostream>
#include <QApplication>
#include "MainWindow.h"
#include "OrbitUtils.h"
#include "OrbitDebug.h"
#include "Body.h"

int main(int argc, char *argv[])
{
    Vector2 position(7000.0, 0.0);
    Vector2 velocity(0.0, 7.5);
    double mu = 398600.0;

    OrbitState st = makeOrbitState(position, velocity, mu);

    std::cout << orbitStateToString(st) << std::endl;

    QApplication app(argc, argv);
    MainWindow window;

    const QString text = QString::fromStdString(orbitStateToString(st));
    window.setOrbitText(text);
    
    window.show();

    return app.exec();
}