#include "ApplicationModel.h"
#include "ApplicationSettings.h"
#include "Events.h"

#include <QDateTime>
#include <QFileInfo>
#include <QTime>

#define MAX_IMAGE_ALLOW 500

ApplicationModel &ApplicationModel::instance()
{
    static ApplicationModel _self;
    return _self;
}

void ApplicationModel::setListFiles(const QStringList &list)
{
    m_listFiles.clear();
    m_listFiles = list;
    setTotalFound(list.count());
}

void ApplicationModel::setFile(const QString &filePath)
{
    m_listFiles.clear();
    m_listFiles.append(filePath);
    setTotalFound(1);
}

void ApplicationModel::printQmlLog(Events::LogLevel level, QString logData)
{
    if (static_cast<int>(level) >= SETTINGS.logLevel())
    {
        QTime now = QTime::currentTime();
        QString formatedTime = now.toString("[hh:mm:ss.zzz]<br>");
        logData = formatedTime + logData;
        emit reqPrintQmlLog(logData);
    }
}

void ApplicationModel::printQmlLogSeparator()
{
    emit reqPrintQmlLog(QLatin1String("---------------------------------"));
}

void ApplicationModel::clearQmlLog()
{
    emit reqClearQmlLog();
}

void ApplicationModel::setBackupDir(const QString &dir)
{
    m_backupDir = dir;
}

void ApplicationModel::setResultDir(const QString &dir)
{
    m_resultDir = dir;
}

QString ApplicationModel::workingDir() const
{
    return !m_isDirectory ? QFileInfo(m_sourcePath).path()
                          : m_sourcePath;
}

void ApplicationModel::setIsDirectory(bool isDirectory)
{
    if (m_isDirectory == isDirectory)
        return;

    m_isDirectory = isDirectory;
    emit isDirectoryChanged(m_isDirectory);
}

void ApplicationModel::setSourcePath(QString sourcePath)
{
    sourcePath = sourcePath.trimmed();
    if (m_sourcePath == sourcePath)
        return;

    m_sourcePath = sourcePath;
    emit sourcePathChanged(m_sourcePath);
}

void ApplicationModel::setSourcePathFound(bool sourcePathFound)
{
    if (m_sourcePathFound == sourcePathFound)
        return;

    m_sourcePathFound = sourcePathFound;
    emit sourcePathFoundChanged(m_sourcePathFound);
}

void ApplicationModel::setIsScanning(bool isScanning)
{
    if (m_isScanning == isScanning)
        return;

    m_isScanning = isScanning;
    emit isScanningChanged(m_isScanning);
}

void ApplicationModel::setIsProcessing(bool isProcessing)
{
    if (m_isProcessing == isProcessing)
        return;

    m_isProcessing = isProcessing;
    emit isProcessingChanged(m_isProcessing);
}

void ApplicationModel::setTotalFound(int totalFound)
{
    if (m_totalFound == totalFound)
        return;

    m_totalFound = totalFound;
    emit totalFoundChanged(m_totalFound);
}

void ApplicationModel::setSuccessCount(int successCount)
{
    if (m_successCount == successCount)
        return;

    m_successCount = successCount;
    emit successCountChanged(m_successCount);
}

void ApplicationModel::setFailureCount(int failureCount)
{
    if (m_failureCount == failureCount)
        return;

    m_failureCount = failureCount;
    emit failureCountChanged(m_failureCount);
}

void ApplicationModel::setIsRunable(bool isRunable)
{
    if (m_isRunable == isRunable)
        return;

    m_isRunable = isRunable;
    emit isRunableChanged(m_isRunable);
}

void ApplicationModel::setCurrentTab(int currentTab)
{
    if (m_currentTab == currentTab)
        return;

    m_currentTab = currentTab;
    emit currentTabChanged(m_currentTab);
}

ApplicationModel::ApplicationModel()
    : m_isDirectory     { true }
    , m_sourcePath      { "" }
    , m_sourcePathFound { false }
    , m_isScanning      { false }
    , m_isProcessing    { false }
    , m_totalFound      { 0 }
    , m_successCount    { 0 }
    , m_failureCount    { 0 }
    , m_isRunable       { true }
    , m_currentTab      { static_cast<int>(Events::MAIN_SCREEN) }
    , m_backupDir       { "" }
    , m_resultDir       { "" }
{
    m_listFiles.clear();
}
