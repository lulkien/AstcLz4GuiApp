#include "ApplicationEngine.h"
#include "ApplicationModel.h"
#include "ApplicationSettings.h"
#include "GUIModel.h"
#include "Common.h"

#include <QQmlContext>
#include <QDir>
#include <QFile>
#include <QDirIterator>
#include <QDesktopServices>

ApplicationEngine::ApplicationEngine()
{
    DEBUG;
    m_username = getCurrentUser();
    registerQmlType();
    initQmlcontext();
    initApplicationConfig();
    initConnection();

    m_processWorker.moveToThread(&m_processThread);
    m_processThread.start();
}

ApplicationEngine::~ApplicationEngine()
{
    DEBUG;
    terminateGenerator();
    m_processThread.quit();
    m_processThread.wait();
}

void ApplicationEngine::startApplication()
{
    load(QStringLiteral("qrc:/qml/main.qml"));
}

bool ApplicationEngine::cliLoadDirectory(const QString &dir)
{
    QFileInfo info(dir);
    if (!info.isDir())
    {
        STDL << dir << "is not a directory.";
        return false;
    }

    if (info.owner() != m_username)
    {
        STDL << "Permission denied.";
        return false;
    }

    if (!info.isReadable())
    {
        STDL << dir << "is not readable.";
        return false;
    }

    if (!QFileInfo(info.path()).isWritable())
    {
        STDL << info.path() << "is not writable.";
        return false;
    }

    MODEL.setSourcePath(dir);
    MODEL.setIsDirectory(true);
    MODEL.setSourcePathFound(true);
    loadImages();
    return true;
}

bool ApplicationEngine::cliLoadImage(const QString &path)
{
    QFileInfo info(path);
    if (!info.isFile())
    {
        STDL << path << "is not a file.";
        return false;
    }

    if (info.owner() != m_username)
    {
        STDL << "Permission denied.";
        return false;
    }

    if (!info.isReadable())
    {
        STDL << path << "is not readable.";
        return false;
    }

    if (!QFileInfo(info.path()).isWritable())
    {
        STDL << info.path() << "is not writable.";
        return false;
    }

    MODEL.setSourcePath(path);
    MODEL.setIsDirectory(false);
    MODEL.setSourcePathFound(true);
    loadImages();
    return true;
}

void ApplicationEngine::cliGenImage()
{
    m_processWorker.startProcessImages();
    if (MODEL.isDirectory())
    {
        STDL << "Output directory:" << MODEL.resultDir();
        QDesktopServices::openUrl(QUrl::fromLocalFile(MODEL.resultDir()));
    }
    else
    {
        QFileInfo info(MODEL.sourcePath());
        STDL << "Output directory:" << info.absolutePath();
        QDesktopServices::openUrl(QUrl::fromLocalFile(info.absolutePath()));
    }
}

void ApplicationEngine::registerQmlType()
{
    qmlRegisterUncreatableType<Events>("QmlCustomType", 1, 0, "QmlEvents", "Cannot create QML Object from QmlEvents");
}

void ApplicationEngine::initQmlcontext()
{
    rootContext()->setContextProperty("GUI", &GUI_MODEL);
    rootContext()->setContextProperty("EventHandler", &m_eventHandler);
    rootContext()->setContextProperty("AppModel", &MODEL);
    rootContext()->setContextProperty("AppSettings", &SETTINGS);
}

void ApplicationEngine::initApplicationConfig()
{
    DEBUG;
}

void ApplicationEngine::initConnection()
{
    DEBUG;
    connect(&m_eventHandler, &Events::sendEvent,
            this, &ApplicationEngine::onReceivedEvent);
    connect(this, &ApplicationEngine::requestStartProcessImages,
            &m_processWorker, &ProcessWorker::onStartProcessImages, Qt::QueuedConnection);
    connect(&m_processThread, &QThread::finished,
            &m_processThread, &QThread::deleteLater);
}

void ApplicationEngine::loadImages()
{
    DEBUG;
    if (!MODEL.sourcePathFound())
        return;
    MODEL.reqClearQmlLog();
    MODEL.setSuccessCount(0);
    MODEL.setFailureCount(0);
    if (MODEL.isDirectory())
    {
        MODEL.setIsScanning(true);
        MODEL.printQmlLog(Events::QML_WARN, QString("Loaded directory: <b>%1</b>").arg(MODEL.sourcePath()));
        MODEL.setListFiles(scanAllPngFromDirectory(MODEL.sourcePath()));
        MODEL.setIsScanning(false);
    }
    else
    {
        MODEL.setIsScanning(true);
        MODEL.printQmlLog(Events::QML_WARN, QString("Loaded file: <b>%1</b>").arg(MODEL.sourcePath()));
        MODEL.setFile(MODEL.sourcePath());
        MODEL.setIsScanning(false);
    }
}

void ApplicationEngine::genImages()
{
    DEBUG;
    if (MODEL.isProcessing())
        return;
    emit requestStartProcessImages();
}

QStringList ApplicationEngine::scanAllPngFromDirectory(const QString &dir)
{
    QStringList list;

    // scan all images file after confirm destination directory is made
    QDirIterator iter(dir, QStringList() << "*.png" << "*.PNG", QDir::Files, QDirIterator::Subdirectories);
    while (iter.hasNext())
    {
        QString found = iter.next();
        if (!found.endsWith(".normalized.png"))
        {
            MODEL.printQmlLog(Events::QML_INFO, QString("Found: %1").arg(found));
            list << found;
        }
    }

    MODEL.printQmlLog(Events::QML_WARN, QString("Total found: <b>%1</b> image(s)").arg(list.count()));
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

void ApplicationEngine::verifyInputPath(QString path)
{
    if (path.isEmpty())
    {
        WARN << "Empty";
        return;
    }

    if (path.length() > 1 && path.endsWith(QLatin1Char('/')))
        path.chop(1);

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
        DEBUG << "Directory found:" << path;
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
        DEBUG << "File found:" << path;
        MODEL.setSourcePath(path);
        MODEL.setIsDirectory(false);
        MODEL.setBackupDir(info.path());
        MODEL.setResultDir(info.path());
        MODEL.setSourcePathFound(true);
    }
    else
    {
        // do nothing, ignore some symlink sh*t
    }
}

void ApplicationEngine::terminateGenerator()
{
    if (MODEL.isProcessing())
    {
        WARN << "Request terminate ProcessWorker";
        m_processThread.requestInterruption();
    }
}

void ApplicationEngine::onReceivedEvent(int eventId, QByteArray data)
{
    DEBUG << "Send event:" << static_cast<Events::EventID>(eventId)
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
