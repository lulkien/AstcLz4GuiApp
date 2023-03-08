#include "ApplicationModel.h"

#define MAX_IMAGE_ALLOW 500

ApplicationModel &ApplicationModel::instance()
{
    static ApplicationModel _self;
    return _self;
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

void ApplicationModel::setListFiles(QStringList list)
{
    m_listFiles.clear();
    m_listFiles = list;
    setTotalFound(list.count());
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
    , m_currentTab      { 0 }
{
    m_listFiles.clear();
}
