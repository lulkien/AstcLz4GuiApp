#include <QGuiApplication>
#include "ApplicationEngine.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    ApplicationEngine engine;
    engine.startApplication();

    return app.exec();
}
