#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>

class Utilities
{
public:
    Utilities();

    static bool removeFile(const QString &fileNameRef);
    static QString astcFileName(const QString &pngFilename);
    static QString normalizedFileName(const QString &pngFileName);
    static QString hdrFileName(const QString &astcFilename);
    static QString lz4FileName(const QString &astcFilename);
    static QString backupAstcFileName(const QString &astcFilename);
};

#endif // UTILITIES_H
