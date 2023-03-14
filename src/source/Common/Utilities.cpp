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

QString Utilities::modifyPath(QString origName, QLatin1String oldSuffix, QLatin1String newSuffix, QStringView oldPrefix, QStringView newPrefix)
{
    if (!oldSuffix.isEmpty() && origName.endsWith(oldSuffix))
    {
        origName.chop(oldSuffix.size());
    }
    origName.append(newSuffix);

    if (oldPrefix.isEmpty() || newPrefix.isEmpty())
        return origName;
    if (!origName.startsWith(newPrefix)
            && oldPrefix.compare(newPrefix) != 0)
    {
        origName.remove(0, oldPrefix.length());
        origName = newPrefix.toString() + origName;
    }
    return origName;
}
