#ifndef COMMON_H
#define COMMON_H

#include <QDebug>

#define DEBUG   qDebug().noquote() << "[DEBUG][" << __FUNCTION__ << "][" << __LINE__ << "]"
#define INFO    qInfo().noquote() << "[INFO][" << __FUNCTION__ << "][" << __LINE__ << "]"
#define WARN    qWarning().noquote() << "[WARN][" << __FUNCTION__ << "][" << __LINE__ << "]"

#define STREF_TO_STR(stref)     *(stref.string())
#define STR_TO_STREF(str)       (QStringRef(&str))
#define STR_LITERAL(str)        QLatin1String(str)

#define IMAGE_MAGICK_PATH   QLatin1String("/usr/local/astc_lz4_tools/convert")
#define ASTCENC_PATH        QLatin1String("/usr/local/astc_lz4_tools/astcenc")

#define ENCODED_HEADER      "\tLZ_ENJOYTOOLS\t"
#define ENCODED_HEADER_LEN  15

#define ASTCENC_HEADER_LENGTH   16

// process direction
#define RETURN_SUCCESS   1
#define RETURN_FAILURE   -1
#define RETURN_IGNORE 0

#endif // COMMON_H
