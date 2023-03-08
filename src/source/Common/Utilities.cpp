#include "Utilities.h"
#include "Common.h"
#include <QFile>

Utilities::Utilities()
{

}

bool Utilities::removeFile(const QString &fileNameRef)
{
    DEBUG << "Try to remove file:" << fileNameRef;

    if (QFile::exists(fileNameRef))
        return QFile::remove(fileNameRef);

    DEBUG << fileNameRef << "not existed";
    return true;
}

QString Utilities::hdrFileName(const QString &astcFilename)
{
    return QString("%1.header").arg(astcFilename);
}

QString Utilities::gzFileName(const QString &astcFilename)
{
    return QString("%1.gz").arg(astcFilename);
}

QString Utilities::lz4FileName(const QString &astcFilename)
{
    return QString("%1.lz4").arg(astcFilename);
}

QString Utilities::astcFileName(const QString &pngFilename)
{
    return QString("%1.astc").arg(pngFilename);
}

QString Utilities::backupAstcFileName(const QString &astcFilename)
{
    return QString("%1_").arg(astcFilename);
}
