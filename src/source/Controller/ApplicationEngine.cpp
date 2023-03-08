#include "ApplicationEngine.h"
#include "ImageCompressor.h"
#include "Common.h"

#include <QQmlContext>
#include <QDir>
#include <QFile>
#include <QDirIterator>

ApplicationEngine::ApplicationEngine()
    : m_processThread { new QThread }
{
    INFO;
    m_username = getCurrentUser();
    registerQmlType();
    initQmlcontext();
    initConnection();

    m_processWorker.moveToThread(m_processThread);
    m_processThread->start();
}

ApplicationEngine::~ApplicationEngine()
{
    INFO;
    m_processThread->quit();
    m_processThread->wait();
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
}

void ApplicationEngine::initConnection()
{
    INFO;
    connect(&m_eventHandler, &Events::sendEvent,
            this, &ApplicationEngine::onReceivedEvent);
    connect(&MODEL, &ApplicationModel::sourcePathChanged,
            this, &ApplicationEngine::onSourcePathChanged);
    connect(this, &ApplicationEngine::requestStartProcessImages,
            &m_processWorker, &ProcessWorker::onStartProcessImages, Qt::QueuedConnection);
    connect(m_processThread, &QThread::finished,
            m_processThread, &QThread::deleteLater);
}

void ApplicationEngine::loadImages()
{
    INFO;
    MODEL.setSuccessCount(0);
    MODEL.setFailureCount(0);
    MODEL.setIsScanning(true);
    MODEL.setListFiles(scanAllPngFromDirectory(MODEL.sourcePath()));
    MODEL.setIsScanning(false);
}

void ApplicationEngine::genImages()
{
    INFO;
    emit requestStartProcessImages();
}

QStringList ApplicationEngine::scanAllPngFromDirectory(const QString &dir)
{
    QStringList list;
    QDirIterator iter(dir, QStringList() << "*.png" << "*.PNG", QDir::Files, QDirIterator::Subdirectories);
    while (iter.hasNext())
    {
        QString found = iter.next();
        if (!found.endsWith(".normalized.png"))
        {
            DEBUG << "Found:" << found;
            list << found;
        }
    }
    INFO << "Total:" << list.count();
    return list;
}

QString ApplicationEngine::getCurrentUser()
{
    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");
    return name;
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
    default:
        break;
    }
}

void ApplicationEngine::onSourcePathChanged()
{
    INFO << "Source path changed to" << MODEL.sourcePath();
    if (MODEL.sourcePath().isEmpty())
    {
        WARN << "Source path empty";
        MODEL.setSourcePathFound(false);
        return;
    }

    if (MODEL.isDirectory())
    {
        // handle a whole directory
        QDir dir(MODEL.sourcePath());
        if (!dir.exists())
        {
            WARN << "Directory not found:" << MODEL.sourcePath();
            MODEL.setSourcePathFound(false);
            return;
        }
        QFileInfo fileInfo(MODEL.sourcePath());
        if (fileInfo.owner() != m_username || !fileInfo.isWritable())
        {
            WARN << "Permission denied:" << MODEL.sourcePath();
            MODEL.setSourcePathFound(false);
            return;
        }

        INFO << "Directory found" << MODEL.sourcePath();
        MODEL.setSourcePathFound(true);
    }
    else
    {
        // handle one file only
    }
}
