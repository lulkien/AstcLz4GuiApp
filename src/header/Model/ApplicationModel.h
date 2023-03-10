#ifndef APPLICATIONMODEL_H
#define APPLICATIONMODEL_H

#include <QObject>
#include <QString>
#include "Events.h"

#define MODEL ApplicationModel::instance()

class ApplicationModel : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ApplicationModel)

    // for image process
    Q_PROPERTY(bool     isDirectory         READ isDirectory        WRITE setIsDirectory        NOTIFY isDirectoryChanged)
    Q_PROPERTY(QString  sourcePath          READ sourcePath         WRITE setSourcePath         NOTIFY sourcePathChanged)
    Q_PROPERTY(bool     sourcePathFound     READ sourcePathFound    WRITE setSourcePathFound    NOTIFY sourcePathFoundChanged)
    Q_PROPERTY(bool     isScanning          READ isScanning         WRITE setIsScanning         NOTIFY isScanningChanged)
    Q_PROPERTY(bool     isProcessing        READ isProcessing       WRITE setIsProcessing       NOTIFY isProcessingChanged)
    Q_PROPERTY(int      totalFound          READ totalFound         WRITE setTotalFound         NOTIFY totalFoundChanged)
    Q_PROPERTY(int      successCount        READ successCount       WRITE setSuccessCount       NOTIFY successCountChanged)
    Q_PROPERTY(int      failureCount        READ failureCount       WRITE setFailureCount       NOTIFY failureCountChanged)
    Q_PROPERTY(bool     isRunable           READ isRunable          WRITE setIsRunable          NOTIFY isRunableChanged)

    // for GUI
    Q_PROPERTY(int      currentTab          READ currentTab         WRITE setCurrentTab         NOTIFY currentTabChanged)

public:
    static ApplicationModel &instance();
    bool isDirectory() const        { return m_isDirectory; }
    QString sourcePath() const      { return m_sourcePath; }
    bool sourcePathFound() const    { return m_sourcePathFound; }
    bool isScanning() const         { return m_isScanning; }
    bool isProcessing() const       { return m_isProcessing; }
    int totalFound() const          { return m_totalFound; }
    int successCount() const        { return m_successCount; }
    int failureCount() const        { return m_failureCount; }
    QStringList listFiles() const   { return m_listFiles; }
    bool isRunable() const          { return m_isRunable; }
    int currentTab() const          { return m_currentTab; }

    // general
    void setListFiles(const QStringList &list);
    void setFile(const QString &filePath);

    void printQmlLog(Events::LogLevel level, QString logData);
    void printQmlLogSeparator();
    void clearQmlLog();

    void setBackupDir(const QString &dir);
    QString backupDir() const { return m_backupDir; }
    void setResultDir(const QString &dir);
    QString resultDir() const { return m_resultDir; }
    QString workingDir() const;


public slots:
    void setIsDirectory(bool isDirectory);
    void setSourcePath(QString sourcePath);
    void setSourcePathFound(bool sourcePathFound);
    void setIsScanning(bool isScanning);
    void setIsProcessing(bool isProcessing);
    void setTotalFound(int totalFound);
    void setSuccessCount(int successCount);
    void setFailureCount(int failureCount);
    void setIsRunable(bool isRunable);
    void setCurrentTab(int currentTab);

private:
    ApplicationModel();

signals:
    void isDirectoryChanged(bool isDirectory);
    void sourcePathChanged(QString sourcePath);
    void sourcePathFoundChanged(bool sourcePathFound);
    void isScanningChanged(bool isScanning);
    void isProcessingChanged(bool isProcessing);
    void totalFoundChanged(int totalFound);
    void successCountChanged(int successCount);
    void failureCountChanged(int failureCount);
    void currentTabChanged(int currentTab);
    void isRunableChanged(bool isRunable);

    void reqPrintQmlLog(QString logData);
    void reqClearQmlLog();

private:
    bool m_isDirectory;
    QString m_sourcePath;
    bool m_sourcePathFound;
    bool m_isScanning;
    bool m_isProcessing;
    int m_totalFound;
    int m_successCount;
    int m_failureCount;
    bool m_isRunable;
    int m_currentTab;

    QStringList m_listFiles;
    QString m_backupDir;
    QString m_resultDir;
};

#endif // APPLICATIONMODEL_H
