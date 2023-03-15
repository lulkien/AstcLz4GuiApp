#include "ProcessWorker.h"
#include "Common.h"
#include "ApplicationModel.h"
#include "ApplicationSettings.h"
#include "ImageCompressor.h"
#include <QDir>
#include <QDirIterator>

ProcessWorker::ProcessWorker()
{}

void ProcessWorker::startProcessImages()
{
    DEBUG << "Use another thread to compress";
    MODEL.setSuccessCount(0);
    MODEL.setFailureCount(0);
    MODEL.setIsProcessing(true);

    if (MODEL.totalFound() != 0)
    {
        if (MODEL.isDirectory() && !createOutputDirectory())
        {
            MODEL.printQmlLog(Events::QML_FATAL, QLatin1String("Cannot make destination directory."));
            MODEL.setIsProcessing(false);
            return;
        }

        DEBUG << "Start process" << MODEL.totalFound() << "images...";
        for (QString file : MODEL.listFiles())
        {
            ImageCompressor process(file);
            bool result = process.compress();
            if (result)
                MODEL.setSuccessCount(MODEL.successCount() + 1);
            else
                MODEL.setFailureCount(MODEL.failureCount() + 1);
            if (process.isTerminated())
            {
                STDL << "Worker has been terminated";
                removeTempFiles();
                MODEL.setIsProcessing(false);
                return;
            }
        }

        removeTempFiles();
        MODEL.printQmlLog(Events::QML_WARN, QString("Output files directory: <b>%1</b>").arg(MODEL.resultDir()));
    }
    MODEL.setIsProcessing(false);
}

void ProcessWorker::onStartProcessImages()
{
    startProcessImages();
}

bool ProcessWorker::createOutputDirectory()
{
    QString inputDir = MODEL.sourcePath();
    // Create parent backup and result dir
    QString backupDir = inputDir + TEMP_DIR_SUFFIX;
    QString resultDir = inputDir + OUTPUT_DIR_SUFFIX;

    if (!makeTempDir(backupDir))
        MODEL.setBackupDir(inputDir);
    else
        MODEL.setBackupDir(backupDir);

    if (!makeResultDir(resultDir))
        MODEL.setResultDir(inputDir);
    else
        MODEL.setResultDir(resultDir);

    // Create child dir
    QDirIterator iterDir(inputDir, QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (iterDir.hasNext())
    {
        QString found = iterDir.next();
        DEBUG << "Found dir:" << found;

        QString backup = QString(found).replace(inputDir, backupDir);
        if (!QDir().mkpath(backup))
        {
            MODEL.printQmlLog(Events::QML_FATAL, QString("Cannot make path %1").arg(backup));
            return false;
        }
        DEBUG << "Make backup directory:" << backup;

        QString result = QString(found).replace(inputDir, resultDir);
        if (!QDir().mkpath(result))
        {
            MODEL.printQmlLog(Events::QML_FATAL, QString("Cannot make path %1").arg(result));
            return false;
        }
        DEBUG << "Make result directory:" << result;
    }
    return true;
}

bool ProcessWorker::makeTempDir(const QString &tmpDir)
{
    QFileInfo info(tmpDir);
    if (info.isFile() && !QFile::remove(tmpDir))
    {
        WARN << tmpDir << "is an existed file and cannot remove";
        return false;
    }
    if (info.isDir())
    {
        if (!info.isWritable() || !info.isReadable())
        {
            WARN << "Permission denied";
            return false;
        }
        return true;
    }
    if (!info.exists())
    {
        DEBUG << "Make path" << tmpDir;
        return QDir().mkpath(tmpDir);
    }

    WARN << "Cannot regconize" << tmpDir << ", write backup in to input directory";
    return false;
}

bool ProcessWorker::makeResultDir(const QString &resultDir)
{
    QFileInfo info(resultDir);
    if (info.isDir() && !QDir(resultDir).removeRecursively())
    {
        WARN << resultDir << "is an existed dir and cannot remove";
        return false;
    }
    if (info.isFile() && QFile::remove(resultDir))
    {
        WARN << resultDir << "is an existed file and cannot remove";
        return false;
    }
    if (!QDir().mkpath(resultDir))
    {
        WARN << resultDir << "cannot be created";
        return false;
    }
    return true;
}

void ProcessWorker::removeTempFiles()
{
    if (MODEL.isDirectory()                                 // loaded directory
            && MODEL.backupDir() != MODEL.sourcePath()      // backup dir is not same with source path
            && !SETTINGS.useBackup())                       // use backup option is diable
    {
        MODEL.printQmlLog(Events::QML_WARN, QString("Remove temporary files directory: <b>%1</b>").arg(MODEL.backupDir()));
        QDir(MODEL.backupDir()).removeRecursively();
    }
}
