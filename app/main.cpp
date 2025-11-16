#include <QApplication>
#include "MainWindow.h"

// Include the core header (test!)
#include "Body.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}