#include "ImageCompressor.h"
#include "Utilities.h"
#include "Common.h"
#include "Settings.h"
#include "ApplicationModel.h"

#include <QFileInfo>
#include <QProcess>
#include <QThread>
#include <lz4.h>

ImageCompressor::ImageCompressor(const QString &pngFileName)
    : m_pngFileName     { pngFileName }
    , m_isTerminated    { false }
{
    INFO << "Construct an ImageProcessor:" << pngFileName;
    MODEL.printQmlLog(QLatin1String("---------------------------------"));
    MODEL.printQmlLogWithTime(QString("Start process: <b>%1<\b>").arg(m_pngFileName));
}

bool ImageCompressor::compress()
{
    INFO << m_pngFileName;
    QFileInfo info(m_pngFileName);
    if (!info.exists())
    {
        WARN << "File not found:" << m_pngFileName;
        return false;
    }

    if (!m_pngFileName.endsWith(STR_LITERAL(".png")))
    {
        WARN << "File must be a png:" << m_pngFileName;
        return false;
    }

    const QString normalizedFileName = QString("%1/%2.normalized.png")
            .arg(info.absolutePath(), info.completeBaseName());
    const QString astcFilename = Utilities::astcFileName(m_pngFileName);

    if (!createNormalizedImage(normalizedFileName, astcFilename))
        return false;

    process_flow_ctrl _status = runAstcEncoder(normalizedFileName, astcFilename);
    switch (_status) {
    case p_failure:
        return false;
    case p_terminated:
        WARN << "Received termination request at function ImageCompressor::runAstcEncoder";
        return false;
    case p_success: // fall-through
    default:
        break;
    }

    if (!runLz4Compress(astcFilename))
        return false;

    MODEL.printQmlLogWithTime(QLatin1String("Completed."));
    return true;
}

bool ImageCompressor::createNormalizedImage(const QString &normalizedFileName, const QString &astcFileName)
{
    MODEL.printQmlLogWithTime(QLatin1String("Normalizing..."));
    if (SETTINGS.useBackup() && QFile::exists(normalizedFileName))
    {
        MODEL.printQmlLogWithTime(QLatin1String("Normalized image found, skip normalize process."));
        return true;
    }

    QImage image;
    if (!image.load(m_pngFileName, "png"))
    {
        MODEL.printQmlLogWithTime(QLatin1String("Cannot load image."));
        return false;
    }

    if (image.depth() != 32)
    {
        DEBUG << "Image depth != 32";
        image = image.convertToFormat(image.hasAlphaChannel() ? QImage::Format_ARGB32
                                                              : QImage::Format_RGB32);
    }

    if (!SETTINGS.noFlip())
    {
        DEBUG << "Flip image vertically";
        image = image.mirrored(false, true);
    }

    if (!exportImage(image, normalizedFileName))
    {
        MODEL.printQmlLogWithTime(QLatin1String("Cannot save image."));
        return false;
    }

    // normalized png was regenerated, remove astc and .gz
    const QString headerFileName = Utilities::hdrFileName(astcFileName);
    const QString gzFileName = Utilities::gzFileName(astcFileName);
    const QString lz4FileName = Utilities::lz4FileName(astcFileName);

    bool removeStatus = (Utilities::removeFile(headerFileName)
                         && Utilities::removeFile(gzFileName)
                         && Utilities::removeFile(lz4FileName)
                         && Utilities::removeFile(astcFileName));
    if (!removeStatus)
        return false;

    return true;
}

bool ImageCompressor::exportImage(const QImage &image, const QString &exportFileName)
{
    if (!image.hasAlphaChannel())
    {
        MODEL.printQmlLogWithTime(QLatin1String("No alpha channel, skip premultiply process."));
        return image.save(exportFileName, "png");
    }

    if (image.format() != QImage::Format_ARGB32)
    {
        MODEL.printQmlLogWithTime(QLatin1String("Wrong format."));
        return false;
    }

    if (!Utilities::removeFile(exportFileName))
        return false;

    if (!image.save(exportFileName, "png"))
    {
        MODEL.printQmlLogWithTime(QLatin1String("Cannot save image."));
        return false;
    }

    if (!SETTINGS.noPremult())
    {
        if (!premultiplyImage(exportFileName))
            return false;
    }

    return true;
}

bool ImageCompressor::premultiplyImage(const QString &fileName)
{
    QFileInfo info(fileName);
    const QString bmpFilename = QString("%1/%2.bmp")
            .arg(info.absolutePath(), info.completeBaseName());

    process_flow_ctrl _status = convert_png_to_premult_bitmap(fileName, bmpFilename);
    switch (_status) {
    case p_failure:
        return false;
    case p_terminated:
        WARN << "Received termination request at function ImageCompressor::premultiplyImage";
        return false;
    case p_success: // fall-through
    default:
        break;
    }

    if (!convert_premult_bitmap_to_png(bmpFilename, fileName))
        return false;

    return true;
}

process_flow_ctrl ImageCompressor::convert_png_to_premult_bitmap(const QString &pngFileName, const QString &bmpFileName)
{
    MODEL.printQmlLogWithTime(QLatin1String("Premultiplying..."));
    if (!pngFileName.endsWith(".png") || !bmpFileName.endsWith(".bmp"))
    {
        WARN << "Invalid PNG of BMP filename:" << pngFileName << bmpFileName;
        return p_failure;
    }
    Utilities::removeFile(bmpFileName);
    QStringList args = QStringList() << pngFileName
                                     << QString("(")
                                     << QString("+clone")
                                     << QString("-alpha")
                                     << QString("Extract")
                                     << QString(")")
                                     << QString("-channel")
                                     << QString("RGB")
                                     << QString("-compose")
                                     << QString("multiply")
                                     << QString("-composite")
                                     << bmpFileName;
    DEBUG << "Running" << IMAGE_MAGICK << "with" << args;
    m_process.start(IMAGE_MAGICK, args);

    if (!waitProcess())
        return p_terminated;

    if (m_process.exitCode() != 0)
    {
        MODEL.printQmlLogWithTime(QLatin1String("Cannot pre-multiply image."));
        return p_failure;
    }

    return p_success;
}

bool ImageCompressor::convert_premult_bitmap_to_png(const QString &bmpFileName, const QString &pngFileName)
{
    if (!pngFileName.endsWith(".png") || !bmpFileName.endsWith(".bmp"))
    {
        WARN << "Invalid png of bmp filename:" << pngFileName << bmpFileName;
        return false;
    }

    QImage image;
    if (!image.load(bmpFileName, "bmp"))
    {
        MODEL.printQmlLogWithTime(QString("Error loading %1").arg(FILE_NAME(bmpFileName)));
        return false;
    }

    if (!image.save(pngFileName, "png"))
    {
        MODEL.printQmlLogWithTime(QString("Error saving %1").arg(FILE_NAME(bmpFileName)));
        return false;
    }

    if (!QFile::remove(bmpFileName))
    {
        MODEL.printQmlLogWithTime(QString("Can't remove %1").arg(FILE_NAME(bmpFileName)));
        return false;
    }
    return true;
}

process_flow_ctrl ImageCompressor::runAstcEncoder(const QString &normalizedFileName, const QString &astcFileName)
{
    MODEL.printQmlLogWithTime(QLatin1String("ASTC encoding..."));
    const QString backupAstcFileName = Utilities::backupAstcFileName(astcFileName);
    bool useBackupAstc = false;
    if (SETTINGS.useBackup())
    {
        if (QFile::exists(astcFileName))
        {
            MODEL.printQmlLogWithTime(QLatin1String("Old ASTC file found, skip ASTC encoder."));
            return p_success;
        }

        if (QFile::exists(backupAstcFileName))
        {
            function_flow_ctrl flag = retrieveAstcFromBackup(backupAstcFileName, astcFileName);
            switch (flag) {
            case f_success:
                MODEL.printQmlLogWithTime(QLatin1String("Backup ASTC file found, skip ASTC encoder."));
                useBackupAstc = true;
                break;
            case f_failure:
                return p_failure;
            case f_ignore:      // fall-through
            default:
                DEBUG << "Continue";
                useBackupAstc = false;
                break;
            }
        }
    }

    // old gz/lz4/header file name
    const QString headerFileName = Utilities::hdrFileName(astcFileName);
    const QString gzFileName = Utilities::gzFileName(astcFileName);
    const QString lz4FileName = Utilities::lz4FileName(astcFileName);

    // If not use backup file -> gen astc file from png
    if (!useBackupAstc)
    {
        const QString speed = (SETTINGS.veryfast() ? QString("-veryfast")
                                                   : QString("-medium"));

        const QStringList args = QStringList() << QString("-c")
                                               << normalizedFileName
                                               << astcFileName
                                               << QString("4x4")
                                               << speed
                                               << QString("-silentmode");

        DEBUG << "Running" << ASTCENCODER << "with arguments" << args;
        m_process.start(ASTCENCODER, args);

        if (!waitProcess())
            return p_terminated;

        if (m_process.exitCode() != 0)
        {
            MODEL.printQmlLogWithTime(QLatin1String("ASTC encoder fail to start."));
            return p_failure;
        }

        // backup after gen astc file
        if (SETTINGS.useBackup())
            backupAstcFile(astcFileName, backupAstcFileName);

        bool removeStatus = (Utilities::removeFile(headerFileName)
                             && Utilities::removeFile(gzFileName)
                             && Utilities::removeFile(lz4FileName));
        if (!removeStatus)
            return p_failure;
    }

    QFile astcFile(astcFileName);
    if (!astcFile.open(QFile::ReadOnly))
    {
        MODEL.printQmlLogWithTime(QLatin1String("Cannot read ASTC file."));
        WARN << "Cannot read ASTC file:" << astcFileName;
        return p_failure;
    }

    const QByteArray astcRawData = astcFile.readAll();
    astcFile.close();

    // Write ASTC header
    QFile headerFile(headerFileName);
    if (!headerFile.open(QFile::WriteOnly))
    {
        MODEL.printQmlLogWithTime(QLatin1String("Cannot write ASTC file."));
        WARN << "Cannot write ASTC file:" << astcFileName;
        return p_failure;
    }
    if (headerFile.write(astcRawData.constData(), ASTCENC_HEADER_LENGTH) != ASTCENC_HEADER_LENGTH)
    {
        MODEL.printQmlLogWithTime(QLatin1String("Cannot write ASTC header file."));
        WARN << "Cannot write ASTC header file:" << headerFileName;
        return p_failure;
    }
    headerFile.close();

    // Overwrite ASTC file
    if (!astcFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        MODEL.printQmlLogWithTime(QLatin1String("Cannot overwrite ASTC file."));
        WARN << "Cannot overwrite ASTC file:" << astcFileName;
        return p_failure;
    }
    int dataSize = astcRawData.size() - ASTCENC_HEADER_LENGTH;
    if (astcFile.write(astcRawData.constData() + ASTCENC_HEADER_LENGTH, dataSize) != dataSize)
    {
        MODEL.printQmlLogWithTime(QLatin1String("Cannot write ASTC file."));
        WARN << "Cannot write ASTC file:" << astcFileName;
        return p_failure;
    }
    astcFile.close();

    return p_success;

}

function_flow_ctrl ImageCompressor::retrieveAstcFromBackup(const QString &backupAstcFileName, const QString &astcFileName)
{
    QFile backupAstcFile(backupAstcFileName);
    if (backupAstcFile.copy(astcFileName))
    {
        INFO << "Retrieve ASTC from backup ASTC file successfully";
        return f_success;
    }

    INFO << "Cannot retrieve ASTC file from backup ASTC file. Remove the backup file and continue...";
    if (!backupAstcFile.remove())
    {
        WARN << "Cannot remove backup ASTC file. Stop process!!!";
        return f_failure;
    }

    return f_ignore;
}

void ImageCompressor::backupAstcFile(const QString &astcFileName, const QString &backupAstcFileName)
{
    MODEL.printQmlLogWithTime(QLatin1String("Backupping ASTC file..."));
    QFile astcFile(astcFileName);
    Utilities::removeFile(backupAstcFileName);
    if (!astcFile.copy(backupAstcFileName))
    {
        MODEL.printQmlLogWithTime(QLatin1String("Cannot backup ASTC file"));
        WARN << "Cannot backup ASTC file:" << astcFileName;
    }
    else
    {
        DEBUG << "ASTC file is saved to" << backupAstcFileName;
    }
    return;
}

bool ImageCompressor::runLz4Compress(const QString &astcFileName)
{
    MODEL.printQmlLogWithTime(QLatin1String("LZ4 compressing..."));
    if (!QFile::exists(astcFileName))
    {
        MODEL.printQmlLogWithTime(QLatin1String("ASTC file not found."));
        WARN << astcFileName << " not found";
        return false;
    }

    const QString lz4FileName = Utilities::lz4FileName(astcFileName);
    Utilities::removeFile(lz4FileName);

    // init
    QFile inputFile(astcFileName);
    QByteArray inputByteArray;
    char* inputBuffer = nullptr;
    size_t inputBufferSize = 0;

    QFile outputFile(lz4FileName);
    char* outputBuffer = nullptr;
    int outputBufferSize = 0;

    // read input data
    if (!inputFile.open(QFile::ReadOnly))
    {
        MODEL.printQmlLogWithTime(QLatin1String("Cannot read ASTC file."));
        WARN << "Cannot open file" << astcFileName << "to read";
        return false;
    }
    inputByteArray = inputFile.readAll();
    inputBuffer = inputByteArray.data();
    inputFile.close();

    inputBufferSize = static_cast<size_t>(inputByteArray.size());
    DEBUG << "Input buffer size:" << inputBufferSize;

    if (inputBufferSize < 20)
    {
        MODEL.printQmlLogWithTime(QLatin1String("Input data is too small."));
        WARN << "Input data is small.(must be > 20 bytes)";
        return false;
    }

    // make output data
    outputBufferSize = LZ4_compressBound(static_cast<int>(inputBufferSize));
    DEBUG << "Maximum output buffer size:" << outputBufferSize;

    outputBuffer = new char[outputBufferSize];
    outputBufferSize = LZ4_compress(inputBuffer,
                                    outputBuffer,
                                    static_cast<int>(inputBufferSize));

    // write to file
    if (!outputFile.open(QFile::WriteOnly))
    {
        MODEL.printQmlLogWithTime(QLatin1String("Cannot compress ASTC file."));
        delete[] outputBuffer;
        return false;
    }
    outputFile.write(ENCODED_HEADER, ENCODED_HEADER_LEN);
    outputFile.write((char*)&inputBufferSize, sizeof(size_t));
    outputFile.write(outputBuffer, outputBufferSize);
    outputFile.close();

    delete[] outputBuffer;
    return true;
}

bool ImageCompressor::waitProcess()
{
    DEBUG << m_process.program();
    while(!m_process.waitForFinished(100)) //Wake up every 100ms and check if we must exit
    {
        if (QThread::currentThread()->isInterruptionRequested())
        {
            WARN << "TERMINATE" << m_process.program();
            m_process.terminate();
            if (!m_process.waitForFinished(1000))
            {
                WARN << "KILL" << m_process.program();
                m_process.kill();
            }
            m_isTerminated = true;
            return false;
        }
    }
    return true;
}

