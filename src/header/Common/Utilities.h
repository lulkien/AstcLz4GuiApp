#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>
#include <QStringView>

class Utilities
{
public:
    Utilities();
    static bool removeFile(const QString &fileNameRef);
    static QString modifyPath(QString origName,             // original file name
                              QLatin1String oldSuffix,      // put old suffix here, put an empty string if you only wanna append the new suffix
                              QLatin1String newSuffix,      // put new suffix here
                              QStringView oldPrefix,        // put old prefix here, empty if you dont wanna change anything
                              QStringView newPrefix);       // put new prefix here, empty if you dont wanna change anything
};

#endif // UTILITIES_H
