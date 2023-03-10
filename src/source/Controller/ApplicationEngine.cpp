#include "ApplicationEngine.h"
#include "ApplicationModel.h"
#include "Settings.h"
#include "Common.h"

#include <QQmlContext>
#include <QDir>
#include <QFile>
#include <QDirIterator>

ApplicationEngine::ApplicationEngine()
{
    INFO;
    m_username = getCurrentUser();
    registerQmlType();
    initQmlcontext();
    initConnection();

    m_processWorker.moveToThread(&m_processThread);
    m_processThread.start();
}

ApplicationEngine::~ApplicationEngine()
{
    INFO;
    if (MODEL.isProcessing())
    {
        WARN << "Request terminate ProcessWorker";
        m_processThread.requestInterruption();
    }
    m_processThread.quit();
    m_processThread.wait();
}

void ApplicationEngine::startApplication()
{
    load(QStringLiteral("qrc:/qml/qml_testing/main.qml"));
}

void ApplicationEngine::registerQmlType()
{
    qmlRegisterUncreatableType<Events>("QmlCustomType", 1, 0, "QmlEvents", "Cannot create QML Object from QmlEvents");
}

void ApplicationEngine::initQmlcontext()
{
    rootContext()->setContextProperty("EventHandler", &m_eventHandler);
    rootContext()->setContextProperty("AppModel", &MODEL);
    rootContext()->setContextProperty("AstcSettings", &SETTINGS);
}

void ApplicationEngine::initConnection()
{
    INFO;
    connect(&m_eventHandler, &Events::sendEvent,
            this, &ApplicationEngine::onReceivedEvent);
    connect(this, &ApplicationEngine::requestStartProcessImages,
            &m_processWorker, &ProcessWorker::onStartProcessImages, Qt::QueuedConnection);
    connect(&m_processThread, &QThread::finished,
            &m_processThread, &QThread::deleteLater);
}

void ApplicationEngine::loadImages()
{
    INFO;
    if (!MODEL.sourcePathFound())
        return;
    MODEL.reqClearQmlLog();
    MODEL.setSuccessCount(0);
    MODEL.setFailureCount(0);
    if (MODEL.isDirectory())
    {
        MODEL.setIsScanning(true);
        MODEL.setListFiles(scanAllPngFromDirectory(MODEL.sourcePath()));
        MODEL.setIsScanning(false);
    }
    else
    {
        MODEL.setIsScanning(true);
        MODEL.printQmlLogWithTime("Loaded file: " + MODEL.sourcePath());
        MODEL.setFile(MODEL.sourcePath());
        MODEL.setIsScanning(false);
    }
}

void ApplicationEngine::genImages()
{
    INFO;
    if (MODEL.backupDir().isEmpty() || MODEL.resultDir().isEmpty())
    {
        MODEL.printQmlLogWithTime(QLatin1String("Backup directory or result directory not found"));
        return;
    }
    emit requestStartProcessImages();
}

QStringList ApplicationEngine::scanAllPngFromDirectory(const QString &dir)
{
    QStringList list;

    if (!makeDestDirectory(dir))
    {
        MODEL.printQmlLogWithTime(QLatin1String("Cannot make destination directory."));
        MODEL.setIsRunable(false);
        return list;
    }

    // scan all images file after confirm destination directory is made
    QDirIterator iter(dir, QStringList() << "*.png" << "*.PNG", QDir::Files, QDirIterator::Subdirectories);
    MODEL.printQmlLogWithTime(QString("Scan directory: <b>%1</b>").arg(dir));
    while (iter.hasNext())
    {
        QString found = iter.next();
        if (!found.endsWith(".normalized.png"))
        {
            MODEL.printQmlLogWithTime("Found: " + found);
            list << found;
        }
    }

    MODEL.printQmlLogWithTime(QString("Total found: %1 image(s)").arg(list.count()));
    MODEL.setIsRunable(true);
    return list;
}

QString ApplicationEngine::getCurrentUser()
{
    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");
    return name;
}

void ApplicationEngine::verifyInputPath(const QString &path)
{
    QFileInfo info(path);
    if (!info.exists())
    {
        WARN << "Path not found:" << path;
        MODEL.setSourcePathFound(false);
        return;
    }

    if (info.isDir())
    {
        if (!info.isWritable())
        {
            WARN << "Permission denied:" << path;
            MODEL.setSourcePathFound(false);
            return;
        }
        INFO << "Directory found:" << path;
        MODEL.setSourcePath(path);
        MODEL.setIsDirectory(true);
        MODEL.setSourcePathFound(true);
    }
    else if (info.isFile())
    {
        if (!info.isReadable())
        {
            WARN << "Cannot read file:" << path;
            MODEL.setSourcePathFound(false);
            return;
        }

        QFileInfo dirInfo(info.path());
        if (!dirInfo.isWritable())
        {
            WARN << "Permission denied:" << info.path();
            MODEL.setSourcePathFound(false);
            return;
        }
        INFO << "File found:" << path;
        MODEL.setSourcePath(path);
        MODEL.setIsDirectory(false);
        MODEL.setSourcePathFound(true);
    }
    else
    {
        // do nothing, ignore some symlink sh*t
    }
}

bool ApplicationEngine::makeDestDirectory(const QString &srcDir)
{
    // Create parent backup and result dir
    QString backupDir = srcDir + QLatin1String("_backup");
    QString resultDir = srcDir + QLatin1String("_result");
    if (!createDir(backupDir))
        return false;
    if (!createDir(resultDir))
        return false;
    MODEL.setBackupDir(backupDir);
    MODEL.setResultDir(resultDir);

    // Create child dir
    QDirIterator iterDir(srcDir, QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (iterDir.hasNext())
    {
        QString found = iterDir.next();
        DEBUG << "Found dir:" << found;

        QString backup = QString(found).insert(srcDir.length(), "_backup");
        if (!QDir().mkpath(backup))
        {
            MODEL.printQmlLogWithTime("Cannot make path" + backup);
            return false;
        }
        DEBUG << "Make backup directory:" << backup;

        QString result = QString(found).insert(srcDir.length(), "_result");
        if (!QDir().mkpath(result))
        {
            MODEL.printQmlLogWithTime("Cannot make path" + backup);
            return false;
        }
        DEBUG << "Make result directory:" << result;
    }
    return true;
}

bool ApplicationEngine::createDir(const QString &path)
{
    QFileInfo info(path);
    if (info.isDir() && !QDir(path).removeRecursively())
    {
        WARN << path << "is an existed dir and cannot remove";
        return false;
    }
    if (info.isFile() && QFile::remove(path))
    {
        WARN << path << "is an existed file and cannot remove";
        return false;
    }
    if (!QDir().mkpath(path))
    {
        WARN << path << "cannot be created";
        return false;
    }
    return true;
}

void ApplicationEngine::onReceivedEvent(int eventId, QByteArray data)
{
    INFO << "Send event:" << static_cast<Events::EventID>(eventId)
         << ", Data:" << data;
    switch (eventId) {
    case static_cast<int>(Events::REQ_LOAD_IMAGES):
        loadImages();
        break;
    case static_cast<int>(Events::REQ_GEN_IMAGE):
        genImages();
        break;
    case static_cast<int>(Events::REQ_VERIFY_SOURCE):
        verifyInputPath(QString(data));
        break;
    default:
        break;
    }
}
