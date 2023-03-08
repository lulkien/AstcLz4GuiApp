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
    INFO << "Construct an ImageProcessor <" << pngFileName;
}

bool ImageCompressor::startProcess()
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

    const QString normalizedFileName = QString("%1/%2.normalized.png")
            .arg(info.absolutePath(), info.completeBaseName());
    const QString astcFilename = Utilities::astcFileName(STR_TO_STREF(m_pngFileName));

    if (!createNormalizedImage(normalizedFileName, astcFilename))
        return false;

    if (!SETTINGS.noAstc())
    {
        if (!runAstcEncoder(normalizedFileName, astcFilename))
        {
            WARN << QString("Cannot run ASTCENC for image: \"%1\" or process is terminated").arg(normalizedFileName);
            return false;
        }
    }

    if (!runLz4Compress(astcFilename))
        return false;

    INFO << "Success";
    return true;
}

bool ImageCompressor::createNormalizedImage(const QString &normalizedFileName, const QString &astcFileName)
{
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
    const QString headerFileName = Utilities::hdrFileName(STR_TO_STREF(astcFileName));
    const QString gzFileName = Utilities::gzFileName(STR_TO_STREF(astcFileName));
    const QString lz4FileName = Utilities::lz4FileName(STR_TO_STREF(astcFileName));

    bool removeStatus = (Utilities::removeFile(STR_TO_STREF(headerFileName))
                         && Utilities::removeFile(STR_TO_STREF(gzFileName))
                         && Utilities::removeFile(STR_TO_STREF(lz4FileName))
                         && Utilities::removeFile(STR_TO_STREF(astcFileName)));
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
        Q_ASSERT(false);
        WARN << "Wrong format";
        return false;
    }

    if (!Utilities::removeFile(STR_TO_STREF(exportFileName)))
        return false;

    if (!image.save(exportFileName, "png"))
        return false;

    if (!SETTINGS.noPremult())
    {
        if (!premultiplyImage(exportFileName))
            return false;
    }

    INFO << "Exported image" << exportFileName;
    return true;
}

bool ImageCompressor::premultiplyImage(const QString &fileName)
{
    const QString bmpFilename = fileName + STR_LITERAL(".bmp");

    if (!convert_png_to_premult_bitmap(fileName, bmpFilename))
        return false;

    if (!convert_premult_bitmap_to_png(bmpFilename, fileName))
        return false;

    return true;
}

bool ImageCompressor::convert_png_to_premult_bitmap(const QString &pngFileName, const QString &bmpFileName)
{
    if (!pngFileName.endsWith(".png") || !bmpFileName.endsWith(".bmp"))
    {
        WARN << "Invalid PNG of BMP filename:" << pngFileName << bmpFileName;
        return false;
    }

    const QStringList args = QStringList() << pngFileName
                                           << STR_LITERAL("( +clone -alpha Extract ) -channel RGB -compose Multiply -composite")
                                           << bmpFileName;

    INFO << "Running " << IMAGE_MAGICK << "with arguments" << args;
    m_process.start(IMAGE_MAGICK, args);

    //Wake up every 100ms and check if we must exit
    while(!m_process.waitForFinished(100))
    {
        if (QThread::currentThread()->isInterruptionRequested())
        {
            WARN << "TERMINATE" << ASTCENCODER;
            m_process.terminate();
            if (!m_process.waitForFinished(1000))
            {
                WARN << "KILL" << ASTCENCODER;
                m_process.kill();
            }
            m_isTerminated = true;
            return false;
        }
    }

    if (m_process.exitCode() != 0)
    {
        WARN << "Error converting to BMP premultiplied";
        return false;
    }

    return true;
}

bool ImageCompressor::convert_premult_bitmap_to_png(const QString &bmpFileName, const QString &pngFileName)
{
    if (!pngFileName.endsWith(".png") || !bmpFileName.endsWith(".bmp"))
    {
        WARN << "Invalid png of bmp filename:" << pngFileName << bmpFileName;
        return false;
    }

    QImage image(bmpFileName);
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

bool ImageCompressor::runAstcEncoder(const QString &normalizedFileName, const QString &astcFileName)
{
    const QString backupAstcFileName = Utilities::backupAstcFileName(STR_TO_STREF(astcFileName));
    bool useBackupAstc = false;
    if (SETTINGS.keep())
    {
        if (QFile::exists(astcFileName))
            return true;

        if (QFile::exists(backupAstcFileName))
        {
            int flag = retrieveAstcFromBackup(backupAstcFileName, astcFileName);
            switch (flag) {
            case RETURN_SUCCESS:
                INFO << "Use backup file" << backupAstcFileName;
                useBackupAstc = true;
                break;
            case RETURN_FAILURE:
                return false;
            default:
                DEBUG << "Continue";
                useBackupAstc = false;
                break;
            }
        }
    }

    // old gz/lz4/header file name
    const QString headerFileName = Utilities::hdrFileName(STR_TO_STREF(astcFileName));
    const QString gzFileName = Utilities::gzFileName(STR_TO_STREF(astcFileName));
    const QString lz4FileName = Utilities::lz4FileName(STR_TO_STREF(astcFileName));

    // If not use backup file -> gen astc file from png
    if (!useBackupAstc)
    {
        const QString speed = (SETTINGS.veryfast() ? STR_LITERAL("-veryfast")
                                                   : STR_LITERAL("-medium"));

        const QStringList args = QStringList() << STR_LITERAL("-c")
                                               << normalizedFileName
                                               << astcFileName
                                               << STR_LITERAL("4x4")
                                               << speed
                                               << STR_LITERAL("-silentmode");

        INFO << "Running" << ASTCENCODER << "with arguments" << args;
        m_process.start(ASTCENCODER, args);

        while(!m_process.waitForFinished(100)) //Wake up every 100ms and check if we must exit
        {
            if (QThread::currentThread()->isInterruptionRequested())
            {
                WARN << "TERMINATE" << ASTCENCODER;
                m_process.terminate();
                if (!m_process.waitForFinished(1000))
                {
                    WARN << "KILL" << ASTCENCODER;
                    m_process.kill();
                }
                m_isTerminated = true;
                return false;
            }
        }

        if (m_process.exitCode() != 0)
        {
            WARN << "Cannot start " << ASTCENCODER;
            return false;
        }

        // backup after gen astc file
        if (SETTINGS.backupAstc())
            backupAstcFile(astcFileName, backupAstcFileName);

        bool removeStatus = (Utilities::removeFile(STR_TO_STREF(headerFileName))
                             && Utilities::removeFile(STR_TO_STREF(gzFileName))
                             && Utilities::removeFile(STR_TO_STREF(lz4FileName)));
        if (!removeStatus)
            return false;
    }

    QFile astcFile(astcFileName);
    if (!astcFile.open(QFile::ReadOnly))
    {
        WARN << "Cannot open ASTC file to read:" << astcFileName;
        return false;
    }

    const QByteArray astcRawData = astcFile.readAll();
    astcFile.close();

    QFile headerFile(headerFileName);
    if (!headerFile.open(QFile::WriteOnly))
    {
        WARN << "Cannot open ASTC header to write:" << headerFileName;
        return false;
    }
    if (headerFile.write(astcRawData.constData(), ASTCENC_HEADER_LENGTH) != ASTCENC_HEADER_LENGTH)
    {
        WARN << "Cannot write file" << headerFileName;
        return false;
    }
    headerFile.close();

    if (!astcFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        WARN << "Cannot open ASTC file to overwrite" << astcFileName;
        return false;
    }
    int dataSize = astcRawData.size() - ASTCENC_HEADER_LENGTH;
    if (astcFile.write(astcRawData.constData() + ASTCENC_HEADER_LENGTH, dataSize) != dataSize)
    {
        WARN << "Cannot write file" << astcFileName;
        return false;
    }
    astcFile.close();

    INFO << "Success";
    return true;

}

int ImageCompressor::retrieveAstcFromBackup(const QString &backupAstcFileName, const QString &astcFileName)
{
    QFile backupAstcFile(backupAstcFileName);
    if (backupAstcFile.copy(astcFileName))
    {
        INFO << "Retrieve ASTC from backup ASTC file successfully";
        return RETURN_SUCCESS;
    }

    INFO << "Cannot retrieve ASTC file from backup ASTC file. Remove the backup file and continue...";
    if (!backupAstcFile.remove())
    {
        WARN << "Cannot remove backup ASTC file. Stop process!!!";
        return RETURN_FAILURE;
    }

    return RETURN_IGNORE;
}

void ImageCompressor::backupAstcFile(const QString &astcFileName, const QString &backupAstcFileName)
{
    QFile astcFile(astcFileName);
    Utilities::removeFile(STR_TO_STREF(backupAstcFileName));
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

    const QString lz4FileName = Utilities::lz4FileName(STR_TO_STREF(astcFileName));
    Utilities::removeFile(STR_TO_STREF(lz4FileName));

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
    INFO << "Input buffer size:" << inputBufferSize;

    if (inputBufferSize < 20)
    {
        WARN << "Input data is small.(must be > 20)";
        return false;
    }

    // make output data
    outputBufferSize = LZ4_compressBound(static_cast<int>(inputBufferSize));
    INFO << "Maximum output buffer size:" << outputBufferSize;

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
    INFO << "Input file size:" << input_file_size;
    INFO << "Output LZ4_compressBound:" << output_file_size;

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
    INFO << "Read successfully" << byte_read << " byte.";

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

