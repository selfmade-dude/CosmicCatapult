#include <iostream>
#include <QApplication>
#include "MainWindow.h"
#include "OrbitUtils.h"
#include "OrbitDebug.h"

// Include the core header (test!)
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
    window.show();

    return app.exec();
}