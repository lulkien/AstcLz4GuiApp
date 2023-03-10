#include "Utilities.h"
#include "Common.h"
#include "ApplicationModel.h"

#include <QFile>
#include <QFileInfo>

Utilities::Utilities()
{

}

bool Utilities::removeFile(const QString &fileNameRef)
{
    DEBUG << "Try to remove file:" << fileNameRef;
    if (QFile::exists(fileNameRef))
        return QFile::remove(fileNameRef);
    return true;
}

QString Utilities::astcFileName(const QString &pngFilename)
{
    QString newString = pngFilename;
    if (MODEL.sourcePath() != MODEL.backupDir())
        newString.replace(MODEL.sourcePath(), MODEL.backupDir());

    return QString("%1.astc").arg(newString);
}

QString Utilities::normalizedFileName(const QString &pngFileName)
{
    QFileInfo info(pngFileName);
    QString newString = QString("%1/%2.normalized.png")
            .arg(info.absolutePath(), info.completeBaseName());
    if (MODEL.sourcePath() != MODEL.backupDir())
        newString.replace(MODEL.sourcePath(), MODEL.backupDir());

    return newString;
}

QString Utilities::hdrFileName(const QString &astcFilename)
{
    QString newString = astcFilename + QLatin1String(".header");
    newString.replace(MODEL.backupDir(), MODEL.resultDir());
    return newString;
}

QString Utilities::lz4FileName(const QString &astcFilename)
{
    QString newString = astcFilename + QLatin1String(".lz4");
    newString.replace(MODEL.backupDir(), MODEL.resultDir());
    return newString;
}

QString Utilities::backupAstcFileName(const QString &astcFilename)
{
    QString newString = astcFilename + QLatin1String("_");
    return newString;
}
