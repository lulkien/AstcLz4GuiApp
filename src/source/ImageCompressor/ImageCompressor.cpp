#include "ImageCompressor.h"
#include "Utilities.h"
#include "Common.h"
#include "Settings.h"

#include <QFileInfo>
#include <QProcess>
#include <QThread>
#include <lz4.h>

ImageCompressor::ImageCompressor(const QString &pngFileName)
    : m_pngFileName     { pngFileName }
    , m_isTerminated    { false }
{
    INFO << "Construct an ImageProcessor:" << pngFileName;
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

    if (!SETTINGS.noAstc())
    {
        process_flow_ctrl _status = runAstcEncoder(normalizedFileName, astcFilename);
        switch (_status) {
        case p_failure:
            WARN << QString("Cannot run ASTCENC for image: \"%1\"").arg(normalizedFileName);
            return false;
        case p_terminated:
            WARN << "Received termination request at function ImageCompressor::runAstcEncoder";
            return false;
        case p_success: // fall-through
        default:
            break;
        }
    }

    if (!runLz4Compress(astcFilename))
        return false;

    INFO << "Success";
    return true;
}

bool ImageCompressor::createNormalizedImage(const QString &normalizedFileName, const QString &astcFileName)
{
    INFO << normalizedFileName;
    if (SETTINGS.keep() && QFile::exists(normalizedFileName))
        return true;

    QImage image(m_pngFileName);
    if (image.isNull())
    {
        WARN << "Cannot read image file:" << m_pngFileName;
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
        WARN << "Cannot export image >" + normalizedFileName;
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
        INFO << "No alpha channel -> just saving";
        return image.save(exportFileName, "png");
    }

    if (image.format() != QImage::Format_ARGB32)
    {
        WARN << "Wrong format";
        return false;
    }

    if (!Utilities::removeFile(exportFileName))
        return false;

    if (!image.save(exportFileName, "png"))
        return false;

    if (!SETTINGS.noPremult())
    {
        if (!premultiplyImage(exportFileName))
            return false;
    }

    INFO << "-->" << exportFileName;
    return true;
}

bool ImageCompressor::premultiplyImage(const QString &fileName)
{
    INFO << fileName;
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
        WARN << "Error converting to BMP premultiplied";
        return p_failure;
    }

    INFO << "-> Done";
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
        WARN << "Error loading" << bmpFileName;
        return false;
    }

    if (!image.save(pngFileName, "png"))
    {
        WARN << "Error saving" << pngFileName;
        return false;
    }

    if (!QFile::remove(bmpFileName))
    {
        WARN << "Couldn't remove" << bmpFileName;
        return false;
    }
    return true;
}

process_flow_ctrl ImageCompressor::runAstcEncoder(const QString &normalizedFileName, const QString &astcFileName)
{
    const QString backupAstcFileName = Utilities::backupAstcFileName(astcFileName);
    bool useBackupAstc = false;
    if (SETTINGS.keep())
    {
        if (QFile::exists(astcFileName))
            return p_success;

        if (QFile::exists(backupAstcFileName))
        {
            function_flow_ctrl flag = retrieveAstcFromBackup(backupAstcFileName, astcFileName);
            switch (flag) {
            case f_success:
                INFO << "Use backup file" << backupAstcFileName;
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
            WARN << "Cannot start " << ASTCENCODER;
            return p_failure;
        }

        // backup after gen astc file
        if (SETTINGS.backupAstc())
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
        WARN << "Cannot open ASTC file to read:" << astcFileName;
        return p_failure;
    }

    const QByteArray astcRawData = astcFile.readAll();
    astcFile.close();

    QFile headerFile(headerFileName);
    if (!headerFile.open(QFile::WriteOnly))
    {
        WARN << "Cannot open ASTC header to write:" << headerFileName;
        return p_failure;
    }
    if (headerFile.write(astcRawData.constData(), ASTCENC_HEADER_LENGTH) != ASTCENC_HEADER_LENGTH)
    {
        WARN << "Cannot write file" << headerFileName;
        return p_failure;
    }
    headerFile.close();

    if (!astcFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        WARN << "Cannot open ASTC file to overwrite" << astcFileName;
        return p_failure;
    }
    int dataSize = astcRawData.size() - ASTCENC_HEADER_LENGTH;
    if (astcFile.write(astcRawData.constData() + ASTCENC_HEADER_LENGTH, dataSize) != dataSize)
    {
        WARN << "Cannot write file" << astcFileName;
        return p_failure;
    }
    astcFile.close();

    INFO << "---> Done";
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
    QFile astcFile(astcFileName);
    Utilities::removeFile(backupAstcFileName);
    if (!astcFile.copy(backupAstcFileName))
        WARN << "Cannot backup ASTC file." << astcFileName;
    else
        INFO << "ASTC file is saved to" << backupAstcFileName;
    return;
}

bool ImageCompressor::runLz4Compress(const QString &astcFileName)
{
    if (!QFile::exists(astcFileName))
    {
        WARN << astcFileName << " not found";
        return false;
    }

    const QString lz4FileName = Utilities::lz4FileName(astcFileName);
    Utilities::removeFile(lz4FileName);

#ifdef USE_QT_LZ4
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
        WARN << "Input data is small.(must be > 20)";
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
        WARN << "Cannot open file" << lz4FileName << "to write";
        delete[] outputBuffer;
        return false;
    }
    outputFile.write(ENCODED_HEADER, ENCODED_HEADER_LEN);
    outputFile.write((char*)&inputBufferSize, sizeof(size_t));
    outputFile.write(outputBuffer, outputBufferSize);
    outputFile.close();

    delete[] outputBuffer;
#else
    FILE *fp_in = NULL;
    FILE *fp_out = NULL;
    char *input_file_data;
    char *output_file_data;
    size_t input_file_size;
    uint32_t output_file_size;
    size_t byte_read = 0;

    fp_in = fopen(astcFileName.toLocal8Bit().data(), "rb");
    if (fp_in == NULL)
    {
        WARN << "Failed to open file" << astcFileName;
        return false;
    }

    fseek(fp_in, 0, SEEK_END);
    input_file_size = ftell(fp_in);
    rewind(fp_in);
    output_file_size = LZ4_compressBound(input_file_size);
    DEBUG << "Input file size:" << input_file_size;
    DEBUG << "Output LZ4_compressBound:" << output_file_size;

    if (input_file_size < 20)
    {
        fclose(fp_in);
        WARN << "Input data is small.(must be > 20)";
        return false;
    }

    fp_out = fopen(lz4FileName.toLocal8Bit().data(), "wb");
    if (fp_out == NULL)
    {
        WARN << "Failed to open file" << lz4FileName;
        return false;
    }

    // Allocate memory for input/output file data
    input_file_data = (char*)malloc(input_file_size);
    memset(input_file_data, 0, sizeof(char) * input_file_size);

    output_file_data = (char*)malloc(output_file_size);
    memset(output_file_data, 0, sizeof(char) * output_file_size);

    // read all data from input file
    byte_read = fread(input_file_data, sizeof(char), input_file_size, fp_in);
    DEBUG << "Read successfully" << byte_read << " byte.";

    // do a LZ4 compression
    output_file_size = LZ4_compress(input_file_data, output_file_data, (int)input_file_size);

    // write output file
    fwrite(ENCODED_HEADER, ENCODED_HEADER_LEN, 1, fp_out);
    fwrite(&input_file_size, sizeof(size_t), 1, fp_out);
    fwrite(output_file_data, output_file_size, 1, fp_out);

    fclose(fp_in);
    fclose(fp_out);

    free(output_file_data);
    free(input_file_data);
#endif
    return true;
}

bool ImageCompressor::waitProcess()
{
    INFO << m_process.program();
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

