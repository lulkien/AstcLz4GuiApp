#include <QGuiApplication>
#include <QLoggingCategory>
#include "ApplicationEngine.h"

int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules("*.debug=false");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    ApplicationEngine engine;
    engine.startApplication();

    return app.exec();
}
