#include "Utilities.h"
#include "Common.h"
#include <QFile>

Utilities::Utilities()
{

}

bool Utilities::removeFile(QStringRef fileNameRef)
{
    INFO << "Try to remove file:" << fileNameRef;

    if (QFile::exists(STREF_TO_STR(fileNameRef)))
        return QFile::remove(STREF_TO_STR(fileNameRef));

    INFO << fileNameRef << "not existed";
    return true;
}

QString Utilities::hdrFileName(QStringRef astcFilename)
{
    return QString("%1.header").arg(STREF_TO_STR(astcFilename));
}

QString Utilities::gzFileName(QStringRef astcFilename)
{
    return QString("%1.gz").arg(STREF_TO_STR(astcFilename));
}

QString Utilities::lz4FileName(QStringRef astcFilename)
{
    return QString("%1.lz4").arg(STREF_TO_STR(astcFilename));
}

QString Utilities::astcFileName(QStringRef pngFilename)
{
    return QString("%1.astc").arg(STREF_TO_STR(pngFilename));
}

QString Utilities::backupAstcFileName(QStringRef astcFilename)
{
    return QString("%1_").arg(STREF_TO_STR(astcFilename));
}
