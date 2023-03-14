#ifndef IMAGECOMPRESSOR_H
#define IMAGECOMPRESSOR_H

#include <QString>
#include <QImage>
#include <QProcess>
#include "Common.h"

class ImageCompressor
{
public:
    ImageCompressor(const QString& pngFileName);
    bool compress();
    bool isTerminated() const { return m_isTerminated; }

private:
    // init
    void initPath();

    // normalized image
    bool createNormalizedImage();
    bool exportNormalizedImage(const QImage &image);
    bool premultiplyNormalizedImage();
    process_flow_ctrl convert_normalized_png_to_premult_bmp();
    bool convert_premult_bmp_to_normalized_png();

    // astcenc
    process_flow_ctrl runAstcEncoder();
    function_flow_ctrl retrieveAstcFromBackup();
    void backupAstcFile();

    // lz4
    bool runLz4Compress();

    // process
    bool waitProcess();
    bool checkProcessExecutable(const QString &processName);

    // post-process
    bool garbageCollect();

private:
    const QString m_pngFileName;
    QProcess m_process;
    bool m_isTerminated;

    // unchanged path
    QString m_normalizedPath;
    QString m_premultBmpPath;
    QString m_astcPath;
    QString m_astcBackupPath;
    QString m_astcHeaderPath;
    QString m_astcLz4Path;
};

#endif // IMAGECOMPRESSOR_H
