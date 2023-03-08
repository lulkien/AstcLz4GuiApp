#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>

class Utilities
{
public:
    Utilities();

    static bool removeFile(QStringRef fileNameRef);
    static QString hdrFileName(QStringRef astcFilename);
    static QString gzFileName(QStringRef astcFilename);
    static QString lz4FileName(QStringRef astcFilename);
    static QString astcFileName(QStringRef pngFilename);
    static QString backupAstcFileName(QStringRef astcFilename);
};

#endif // UTILITIES_H
