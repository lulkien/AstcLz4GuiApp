#ifndef COMMON_H
#define COMMON_H

#include <QDebug>
#include <QStringList>

#define DEBUG   qDebug().noquote() << "[DEBUG][" << __FUNCTION__ << "][" << __LINE__ << "]"
#define INFO    qInfo().noquote() << "[INFO][" << __FUNCTION__ << "][" << __LINE__ << "]"
#define WARN    qWarning().noquote() << "[WARN][" << __FUNCTION__ << "][" << __LINE__ << "]"

#define STREF_TO_STR(stref)     *(stref.string())
#define STR_TO_STREF(str)       (QStringRef(&str))
#define STR_LITERAL(str)        QLatin1String(str)
#define FILE_NAME(path)         QFileInfo(path).fileName()

#define IMAGE_MAGICK        QLatin1String("/usr/local/astc_lz4_tools/convert")
#define ASTCENCODER         QLatin1String("/usr/local/astc_lz4_tools/astcenc")

#define ENCODED_HEADER      "\tLZ_ENJOYTOOLS\t"
#define ENCODED_HEADER_LEN  15

#define ASTCENC_HEADER_LENGTH   16

enum function_flow_ctrl {
    f_ignore    = -1,
    f_success   = 0,
    f_failure   = 1,
};

enum process_flow_ctrl {
    p_terminated    = -1,
    p_success       = 0,
    p_failure       = 1,
};

namespace {
QStringList LOG_LEVEL = QStringList() << QLatin1String("DEBUG")
                                      << QLatin1String("INFO")
                                      << QLatin1String("WARN")
                                      << QLatin1String("FATAL");
}

#endif // COMMON_H
