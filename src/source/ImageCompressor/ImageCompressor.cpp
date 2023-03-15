#include "ImageCompressor.h"
#include "Utilities.h"
#include "Common.h"
#include "ApplicationSettings.h"
#include "ApplicationModel.h"

#include <QFileInfo>
#include <QProcess>
#include <QThread>
#include <lz4.h>

ImageCompressor::ImageCompressor(const QString &pngFileName)
    : m_pngFileName     { pngFileName }
    , m_isTerminated    { false }
{
    DEBUG << "Construct an ImageProcessor:" << pngFileName;
    initPath();
//    MODEL.printQmlLogSeparator();
    MODEL.printQmlLog(Events::QML_WARN, QString("Start process: <b>%1<\b>").arg(m_pngFileName));
}

bool ImageCompressor::compress()
{
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

    if (!createNormalizedImage())
        return false;

    process_flow_ctrl _status = runAstcEncoder();
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

    if (!runLz4Compress())
        return false;

    MODEL.printQmlLog(Events::QML_WARN, QLatin1String("<b>Completed.</b>"));
    garbageCollect();
    return true;
}

void ImageCompressor::initPath()
{
    //                                         <original file>      <old suffix>                <new suffix>                    <old prefix>            <new prefix>
    m_normalizedPath    = Utilities::modifyPath(m_pngFileName, QLatin1String(".png"),   QLatin1String("_normalized.png"),   MODEL.workingDir(),     MODEL.backupDir());
    m_premultBmpPath    = Utilities::modifyPath(m_pngFileName, QLatin1String(".png"),   QLatin1String("_normalized.bmp"),   MODEL.workingDir(),     MODEL.backupDir());
    m_astcPath          = Utilities::modifyPath(m_pngFileName, QLatin1String(".png"),   QLatin1String(".astc"),             MODEL.workingDir(),     MODEL.backupDir());
    m_astcBackupPath    = Utilities::modifyPath(m_pngFileName, QLatin1String(".png"),   QLatin1String("_saved.astc"),       MODEL.workingDir(),     MODEL.backupDir());
    m_astcHeaderPath    = Utilities::modifyPath(m_pngFileName, QLatin1String(""),       QLatin1String(".astc.header"),      MODEL.workingDir(),     MODEL.resultDir());
    m_astcLz4Path       = Utilities::modifyPath(m_pngFileName, QLatin1String(""),       QLatin1String(".astc.lz4"),         MODEL.workingDir(),     MODEL.resultDir());
//    INFO << m_normalizedPath;
//    INFO << m_premultBmpPath;
//    INFO << m_astcPath;
//    INFO << m_astcBackupPath;
//    INFO << m_astcHeaderPath;
//    INFO << m_astcLz4Path;
}

bool ImageCompressor::createNormalizedImage()
{
    MODEL.printQmlLog(Events::QML_INFO, QLatin1String("Normalizing..."));
    if (SETTINGS.useBackup() && QFileInfo(m_normalizedPath).isFile())
    {
        MODEL.printQmlLog(Events::QML_DEBUG, QLatin1String("Normalized image found, skip normalize process."));
        return true;
    }

    QImage image;
    if (!image.load(m_pngFileName, "png"))
    {
        MODEL.printQmlLog(Events::QML_FATAL, QLatin1String("Cannot load image."));
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

    if (!exportNormalizedImage(image))
    {
        MODEL.printQmlLog(Events::QML_FATAL, QLatin1String("Cannot save image."));
        return false;
    }

    // normalized png was regenerated, remove astc and .gz
    bool removeStatus = (Utilities::removeFile(m_astcHeaderPath)
                         && Utilities::removeFile(m_astcLz4Path)
                         && Utilities::removeFile(m_astcPath));
    if (!removeStatus)
        return false;

    return true;
}

bool ImageCompressor::exportNormalizedImage(const QImage &image)
{
    if (!image.hasAlphaChannel())
    {
        MODEL.printQmlLog(Events::QML_DEBUG, QLatin1String("No alpha channel, skip premultiply process."));
        return image.save(m_normalizedPath, "png");
    }

    if (image.format() != QImage::Format_ARGB32)
    {
        MODEL.printQmlLog(Events::QML_FATAL, QLatin1String("Wrong format."));
        return false;
    }

    if (!Utilities::removeFile(m_normalizedPath))
        return false;

    if (!image.save(m_normalizedPath, "png"))
    {
        MODEL.printQmlLog(Events::QML_FATAL, QLatin1String("Cannot save image."));
        return false;
    }

    if (!SETTINGS.noPremult())
    {
        if (!premultiplyNormalizedImage())
            return false;
    }

    return true;
}

bool ImageCompressor::premultiplyNormalizedImage()
{
    process_flow_ctrl _status = convert_normalized_png_to_premult_bmp();
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

    if (!convert_premult_bmp_to_normalized_png())
        return false;

    return true;
}

process_flow_ctrl ImageCompressor::convert_normalized_png_to_premult_bmp()
{
    MODEL.printQmlLog(Events::QML_INFO, QLatin1String("Premultiplying..."));
    Utilities::removeFile(m_premultBmpPath);
    QStringList args = QStringList() << m_normalizedPath
                                     << QLatin1String("(")
                                     << QLatin1String("-clone")
                                     << QLatin1String("-1")
                                     << QLatin1String("-alpha")
                                     << QLatin1String("Extract")
                                     << QLatin1String(")")
                                     << QLatin1String("-channel")
                                     << QLatin1String("RGB")
                                     << QLatin1String("-compose")
                                     << QLatin1String("multiply")
                                     << QLatin1String("-composite")
                                     << m_premultBmpPath;
    DEBUG << "Running" << IMAGE_MAGICK << "with" << args;
    if (!checkProcessExecutable(IMAGE_MAGICK))
        return p_failure;

    m_process.start(IMAGE_MAGICK, args);
    if (!waitProcess())
        return p_terminated;
    if (m_process.exitCode() != 0)
    {
        MODEL.printQmlLog(Events::QML_FATAL, QLatin1String("Cannot premultiply image."));
        return p_failure;
    }
    return p_success;
}

bool ImageCompressor::convert_premult_bmp_to_normalized_png()
{
    QImage image;
    if (!image.load(m_premultBmpPath, "bmp"))
    {
        MODEL.printQmlLog(Events::QML_FATAL, QString("Error loading %1").arg(FILE_NAME(m_premultBmpPath)));
        return false;
    }
    if (!image.save(m_normalizedPath, "png"))
    {
        MODEL.printQmlLog(Events::QML_FATAL, QString("Error saving %1").arg(FILE_NAME(m_normalizedPath)));
        return false;
    }
//    Utilities::removeFile(m_premultBmpPath);
//    if (!QFile::remove(m_premultBmpPath))
//    {
//        MODEL.printQmlLog(Events::QML_FATAL, QString("Can't remove %1").arg(FILE_NAME(m_premultBmpPath)));
//        return false;
//    }
    return true;
}

process_flow_ctrl ImageCompressor::runAstcEncoder()
{
    MODEL.printQmlLog(Events::QML_INFO, QLatin1String("ASTC encoding..."));
    bool useBackupAstc = false;
    if (SETTINGS.useBackup())
    {
        if (QFile::exists(m_astcPath))
        {
            MODEL.printQmlLog(Events::QML_DEBUG, QLatin1String("Old ASTC file found, skip ASTC encoder."));
            return p_success;
        }

        if (QFile::exists(m_astcBackupPath))
        {
            function_flow_ctrl flag = retrieveAstcFromBackup();
            switch (flag) {
            case f_success:
                MODEL.printQmlLog(Events::QML_DEBUG, QLatin1String("Backup ASTC file found, skip ASTC encoder."));
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

    // If not use backup file -> gen astc file from png
    if (!useBackupAstc)
    {
        const QString speed = (SETTINGS.veryfast() ? QString("-veryfast")
                                                   : QString("-medium"));

        const QStringList args = QStringList() << QString("-c")
                                               << m_normalizedPath
                                               << m_astcPath
                                               << QString("4x4")
                                               << speed
                                               << QString("-silentmode");

        if (!checkProcessExecutable(ASTCENCODER))
            return p_failure;

        DEBUG << "Running" << ASTCENCODER << "with arguments" << args;
        m_process.start(ASTCENCODER, args);

        if (!waitProcess())
            return p_terminated;

        if (m_process.exitCode() != 0)
        {
            MODEL.printQmlLog(Events::QML_FATAL, QLatin1String("ASTC encoder fail to start."));
            return p_failure;
        }

        // backup after gen astc file
        if (SETTINGS.useBackup())
            backupAstcFile();

        bool removeStatus = (Utilities::removeFile(m_astcHeaderPath)
                             && Utilities::removeFile(m_astcLz4Path));
        if (!removeStatus)
            return p_failure;
    }

    QFile astcFile(m_astcPath);
    if (!astcFile.open(QFile::ReadOnly))
    {
        MODEL.printQmlLog(Events::QML_FATAL, QLatin1String("Cannot read ASTC file."));
        WARN << "Cannot read ASTC file:" << m_astcPath;
        return p_failure;
    }

    const QByteArray astcRawData = astcFile.readAll();
    astcFile.close();

    // Write ASTC header
    QFile headerFile(m_astcHeaderPath);
    if (!headerFile.open(QFile::WriteOnly))
    {
        MODEL.printQmlLog(Events::QML_FATAL, QLatin1String("Cannot write ASTC file."));
        WARN << "Cannot write ASTC file:" << m_astcPath;
        return p_failure;
    }
    if (headerFile.write(astcRawData.constData(), ASTCENC_HEADER_LENGTH) != ASTCENC_HEADER_LENGTH)
    {
        MODEL.printQmlLog(Events::QML_FATAL, QLatin1String("Cannot write ASTC header file."));
        WARN << "Cannot write ASTC header file:" << m_astcHeaderPath;
        return p_failure;
    }
    headerFile.close();

    // Overwrite ASTC file
    if (!astcFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        MODEL.printQmlLog(Events::QML_FATAL, QLatin1String("Cannot overwrite ASTC file."));
        WARN << "Cannot overwrite ASTC file:" << m_astcPath;
        return p_failure;
    }
    int dataSize = astcRawData.size() - ASTCENC_HEADER_LENGTH;
    if (astcFile.write(astcRawData.constData() + ASTCENC_HEADER_LENGTH, dataSize) != dataSize)
    {
        MODEL.printQmlLog(Events::QML_FATAL, QLatin1String("Cannot write ASTC file."));
        WARN << "Cannot write ASTC file:" << m_astcPath;
        return p_failure;
    }

    astcFile.close();
    return p_success;
}

function_flow_ctrl ImageCompressor::retrieveAstcFromBackup()
{
    QFile backupAstcFile(m_astcBackupPath);
    if (backupAstcFile.copy(m_astcPath))
    {
        DEBUG << "Retrieve ASTC from backup ASTC file successfully";
        return f_success;
    }

    DEBUG << "Cannot retrieve ASTC file from backup ASTC file. Remove the backup file and continue...";
    if (!backupAstcFile.remove())
    {
        WARN << "Cannot remove backup ASTC file. Stop process!!!";
        return f_failure;
    }

    return f_ignore;
}

void ImageCompressor::backupAstcFile()
{
    MODEL.printQmlLog(Events::QML_INFO, QLatin1String("Backupping ASTC file..."));
    QFile astcFile(m_astcPath);
    Utilities::removeFile(m_astcBackupPath);
    if (!astcFile.copy(m_astcBackupPath))
    {
        MODEL.printQmlLog(Events::QML_WARN, QLatin1String("<b>Cannot backup ASTC file</b>"));
        WARN << "Cannot backup ASTC file:" << m_astcPath;
    }
    else
    {
        DEBUG << "ASTC file is saved to" << m_astcBackupPath;
    }
    return;
}

bool ImageCompressor::runLz4Compress()
{
    MODEL.printQmlLog(Events::QML_INFO, QLatin1String("LZ4 compressing..."));
    if (!QFile::exists(m_astcPath))
    {
        MODEL.printQmlLog(Events::QML_FATAL, QLatin1String("ASTC file not found."));
        WARN << m_astcPath << " not found";
        return false;
    }

    Utilities::removeFile(m_astcLz4Path);

    // init
    QFile inputFile(m_astcPath);
    QByteArray inputByteArray;
    char* inputBuffer = nullptr;
    size_t inputBufferSize = 0;

    QFile outputFile(m_astcLz4Path);
    char* outputBuffer = nullptr;
    int outputBufferSize = 0;

    // read input data
    if (!inputFile.open(QFile::ReadOnly))
    {
        MODEL.printQmlLog(Events::QML_FATAL, QLatin1String("Cannot read ASTC file."));
        WARN << "Cannot open file" << m_astcPath << "to read";
        return false;
    }
    inputByteArray = inputFile.readAll();
    inputBuffer = inputByteArray.data();
    inputFile.close();

    inputBufferSize = static_cast<size_t>(inputByteArray.size());
    DEBUG << "Input buffer size:" << inputBufferSize;

    if (inputBufferSize < 20)
    {
        MODEL.printQmlLog(Events::QML_FATAL, QLatin1String("Input data is too small."));
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
        MODEL.printQmlLog(Events::QML_FATAL, QLatin1String("Cannot compress ASTC file."));
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

bool ImageCompressor::checkProcessExecutable(const QString &processName)
{
    QFileInfo info(processName);
    if (!info.isExecutable())
        return false;
    return true;
}

bool ImageCompressor::garbageCollect()
{
    DEBUG << "Remove headless ASTC file";
    return Utilities::removeFile(m_astcPath);
}

