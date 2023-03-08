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
    bool startProcess();
    bool isTerminated() const { return m_isTerminated; }

private:

    // normalized image
    bool createNormalizedImage(const QString& normalizedFileName,
                               const QString& astcFileName);
    bool exportImage(const QImage &image, const QString &exportFileName);
    bool premultiplyImage(const QString &fileName);
    process_flow_ctrl convert_png_to_premult_bitmap(const QString& pngFileName,
                                                    const QString& bmpFileName);
    bool convert_premult_bitmap_to_png(const QString& bmpFileName,
                                   const QString& pngFileName);

    // astcenc
    process_flow_ctrl runAstcEncoder(const QString& normalizedFileName,
                                     const QString& astcFileName);
    function_flow_ctrl retrieveAstcFromBackup(const QString& backupAstcFileName,
                                              const QString& astcFileName);
    void backupAstcFile(const QString& astcFileName,
                        const QString& backupAstcFileName);

    // lz4
    bool runLz4Compress(const QString& astcFileName);

private:
    const QString m_pngFileName;
    QProcess m_process;
    bool m_isTerminated;
};

#endif // IMAGECOMPRESSOR_H
