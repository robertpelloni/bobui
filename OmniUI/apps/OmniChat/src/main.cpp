#include <QApplication>
#include <QDebug>
#include "OmniApp.h"

int main(int argc, char *argv[])
{
    OmniApplication app(argc, argv);
    app.initializeJuce();

    // In QML mode, we don't need to manually create QWidgets here.
    // The QML engine will load the UI definition.

    // Path relative to the build directory or embedded resource
    app.loadMainSource("assets/main.qml");

    return app.exec();
}
