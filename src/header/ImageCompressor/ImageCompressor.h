#ifndef IMAGECOMPRESSOR_H
#define IMAGECOMPRESSOR_H

#include <QString>
#include <QImage>

class ImageCompressor
{
public:
    ImageCompressor(const QString& pngFileName);
    bool startProcess();

private:

    // normalized image
    bool createNormalizedImage(const QString& normalizedFileName,
                               const QString& astcFileName);
    bool exportImage(const QImage &image, const QString &exportFileName);
    bool premultiplyImage(const QString &fileName);
    bool convert_png_to_premult_bitmap(const QString& pngFileName,
                                   const QString& bmpFileName);
    bool convert_premult_bitmap_to_png(const QString& bmpFileName,
                                   const QString& pngFileName);

    // astcenc
    bool runAstcEncoder(const QString& normalizedFileName,
                        const QString& astcFileName);
    int retrieveAstcFromBackup(const QString& backupAstcFileName,
                               const QString& astcFileName);
    void backupAstcFile(const QString& astcFileName,
                        const QString& backupAstcFileName);

    // lz4
    bool runLz4Compress(const QString& astcFileName);

private:
    const QString m_pngFileName;
};

#endif // IMAGECOMPRESSOR_H
