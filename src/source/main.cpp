#include <QGuiApplication>
#include <QLoggingCategory>
#include <QCommandLineParser>
#include <QDir>
#include "ApplicationEngine.h"
#include "ApplicationSettings.h"
#include "Common.h"

int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules("*.debug=false");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QCoreApplication::setApplicationName("astc_lz4");
    QCoreApplication::setApplicationVersion("1.1.2");

    QCommandLineParser parser;
    ApplicationEngine engine;
    parser.setApplicationDescription("Over-engineering ASTC generator");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOptions({
                          {{"g", "use-gui"},    "Use graphical user interface.\nThis option will ignore option -d and -f."},
                          {{"d", "directory"},  "Compress all PNG files in <directory>.", "directory"},
                          {{"f", "file"},       "Compress file <file>.", "file"},
                          {{"n", "now"},        "Compress all PNG file in current directory.\nThis option will ignore option -d, -f and -g."},
                          {"no-flip",           "No flip image in normalize process."},
                          {"no-premult",        "No premultiply image with alpha channel.\nUse carefully."},
                          {"use-temporary",     "Use pregenerated file to increase speed.\nTemp directory won't be remove."},
                          {"veryfast",          "Use veryfast astc compression."},
                          {"log-level",         "Set log level for application.\nDefault is 1.", "level"},
                          {"verbose",           "Set log level to DEBUG. This option will override --log-level."},
                          {{"silent", "quiet"}, "Set log level to FATAL. This option will override --log-level and --verbose."},
                      });

    parser.process(app);

    if (parser.isSet("no-flip"))
        SETTINGS.setNoFlip(true);

    if (parser.isSet("no-premult"))
        SETTINGS.setNoPremult(true);

    if (parser.isSet("use-temporary"))
        SETTINGS.setUseBackup(true);

    if (parser.isSet("veryfast"))
        SETTINGS.setVeryfast(true);

    if (parser.isSet("log-level"))
    {
        int level = parser.value("log-level").toInt();
        if (level < 0 || level > 3)
        {
            STDL << "Available log level is in range [0-3].\n0: DEBUG\n1: INFO\n2: WARNING\n3: FATAL";
            return 1;
        }
        SETTINGS.setLogLevel(level);
    }

    if (parser.isSet("verbose"))
        SETTINGS.setLogLevel(0);

    if (parser.isSet("silent"))
        SETTINGS.setLogLevel(3);

    if (parser.isSet("g"))
    {
        engine.startApplication();
        return app.exec();
    }

    if (parser.isSet("now"))
    {
        if (!engine.cliLoadDirectory(QDir::currentPath()))
            return 1;
        engine.cliGenImage();
        return 0;
    }

    if (parser.isSet("d"))
    {
        if (!engine.cliLoadDirectory(parser.value("d")))
            return 1;
        engine.cliGenImage();
        return 0;
    }

    if (parser.isSet("f"))
    {
        if (!engine.cliLoadImage(parser.value("f")))
            return 1;
        engine.cliGenImage();
        return 0;
    }

    STDL << "No target arguments parsed.\nUse astc_lz4 -n or astc_lz4 --now to process at current directory.";

    return 0;
}
