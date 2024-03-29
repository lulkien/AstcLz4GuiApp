#include "ApplicationSettings.h"
#include "Events.h"
#include "Common.h"
#include <QFileInfo>
#include <QSettings>
#include <QHash>
#include <QDir>
#include <QQmlEngine>

#define getBoolStringValue(table, key)  (table.value(key, "no") == "yes")
#define getIntStringValue(table, key)   (table.value(key, "0").toInt())
#define getStringValue(table, key)      table.value(key, "")

Settings &Settings::instance()
{
    static Settings _self;
    return _self;
}

bool Settings::loadConfigFile()
{
    QString configFilePath = QDir::homePath() + QLatin1String("/.config/astc_lz4/default.cfg");
    if (!QFileInfo(configFilePath).exists())
        return createDefaultConfigFile();

    STDL << configFilePath;
    QSettings setting(configFilePath, QSettings::IniFormat);
    QHash<QString, QString> values;

    QStringList keys;
    setting.beginGroup("CompressionSettings");
    keys = setting.childKeys();
    for (const QString& key : keys)
    {
        values.insert(key, setting.value(key).toString());
    }
    setting.endGroup();

    setting.beginGroup("ApplicationSettings");
    keys = setting.childKeys();
    for (const QString& key : keys)
    {
        values.insert(key, setting.value(key).toString());
    }
    setting.endGroup();

    DEBUG << values;

    // setup encoder settings
    setNoFlip(getBoolStringValue(values, "NoFlip"));
    setNoPremult(getBoolStringValue(values, "NoPremult"));
    setUseBackup(getBoolStringValue(values, "UseBackup"));
//    setBlockSize(getStringValue(values, "BlockSize"));
    setVeryfast(getBoolStringValue(values, "VeryfastCompression"));

    // setup application settings
    setLogLevel(getIntStringValue(values, "LogLevel"));
    setShowOutput(getBoolStringValue(values, "OpenOutputDirectory"));

    return true;
}

bool Settings::createDefaultConfigFile()
{
    QString configDir = QDir::homePath() + QLatin1String("/.config/astc_lz4");
    QString configFilePath = QDir::homePath() + QLatin1String("/.config/astc_lz4/default.cfg");
    QFileInfo infoDir(configDir);
    if (infoDir.exists() && !QDir(configDir).removeRecursively())
    {
        STDL << configDir << "cannot be removed";
        return false;
    }
    if (!QDir().mkdir(configDir))
    {
        STDL << configDir << "cannot be created";
        return false;
    }

    // make new config file
    QSettings setting(configFilePath, QSettings::IniFormat);
    setting.beginGroup(STR_LITERAL("CompressionSettings"));
    setting.setValue(STR_LITERAL("NoFlip")                  ,   STR_LITERAL("no"));
    setting.setValue(STR_LITERAL("NoPremult")               ,   STR_LITERAL("no"));
    setting.setValue(STR_LITERAL("UseBackup")               ,   STR_LITERAL("no"));
    setting.setValue(STR_LITERAL("BlockSize")               ,   STR_LITERAL("4x4"));
    setting.setValue(STR_LITERAL("VeryfastCompression")     ,   STR_LITERAL("no"));
    setting.endGroup();

    setting.beginGroup(STR_LITERAL("ApplicationSettings"));
    setting.setValue(STR_LITERAL("LogLevel")                 ,   STR_LITERAL("2"));
    setting.setValue(STR_LITERAL("OpenOutputDirectory")      ,   STR_LITERAL("yes"));
    setting.endGroup();

    setting.sync();
    return true;
}

void Settings::saveConfigFile()
{
    STDL << "Overwrite config file";
    QString configDir = QDir::homePath() + QLatin1String("/.config/astc_lz4");
    QString configFilePath = QDir::homePath() + QLatin1String("/.config/astc_lz4/default.cfg");
    QFileInfo infoDir(configDir);
    if (infoDir.exists() && !QDir(configDir).removeRecursively())
    {
        STDL << configDir << "cannot be removed";
        return;
    }
    if (!QDir().mkdir(configDir))
    {
        STDL << configDir << "cannot be created";
        return;
    }

    QSettings setting(configFilePath, QSettings::IniFormat);
    setting.beginGroup(STR_LITERAL("CompressionSettings"));
    setting.setValue(STR_LITERAL("NoFlip")                  ,   STR_LITERAL(m_noFlip    ? "yes" : "no"));
    setting.setValue(STR_LITERAL("NoPremult")               ,   STR_LITERAL(m_noPremult ? "yes" : "no"));
    setting.setValue(STR_LITERAL("UseBackup")               ,   STR_LITERAL(m_useBackup ? "yes" : "no"));
    setting.setValue(STR_LITERAL("BlockSize")               ,   m_blockSize);
    setting.setValue(STR_LITERAL("VeryfastCompression")     ,   STR_LITERAL(m_veryfast  ? "yes" : "no"));
    setting.endGroup();

    setting.beginGroup(STR_LITERAL("ApplicationSettings"));
    setting.setValue(STR_LITERAL("LogLevel")                 ,   QString::number(m_logLevel));
    setting.setValue(STR_LITERAL("OpenOutputDirectory")      ,   STR_LITERAL(m_showOutput ? "yes" : "no"));
    setting.endGroup();

    setting.sync();
}

Settings::Settings()
    : m_noFlip          { false }
    , m_noPremult       { false }
    , m_useBackup       { false }
    , m_veryfast        { false }
    , m_logLevel        { static_cast<int>(Events::QML_WARN) }
    , m_showOutput      {  true }
    , m_ipValidated     { false }
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

const bool &Settings::noFlip() const
{
    return m_noFlip;
}

void Settings::setNoFlip(bool newNoFlip)
{
    m_noFlip = newNoFlip;
    emit noFlipChanged();
}

const bool &Settings::noPremult() const
{
    return m_noPremult;
}

void Settings::setNoPremult(bool newNoPremult)
{
    m_noPremult = newNoPremult;
    emit noPremultChanged();
}

const bool &Settings::useBackup() const
{
    return m_useBackup;
}

void Settings::setUseBackup(bool newUseBackup)
{
    m_useBackup = newUseBackup;
    emit useBackupChanged();
}

const bool &Settings::veryfast() const
{
    return m_veryfast;
}

void Settings::setVeryfast(bool newVeryfast)
{
    m_veryfast = newVeryfast;
    emit veryfastChanged();
}

const int &Settings::logLevel() const
{
    return m_logLevel;
}

void Settings::setLogLevel(int newLogLevel)
{
    m_logLevel = newLogLevel;
    emit logLevelChanged();
}

const bool &Settings::showOutput() const
{
    return m_showOutput;
}

void Settings::setShowOutput(bool newShowOutput)
{
    m_showOutput = newShowOutput;
    emit showOutputChanged();
}


const int &Settings::blockSize() const
{
    return m_blockSize;
}

void Settings::setBlockSize(int newBlockSize)
{
    if (m_blockSize == newBlockSize)
        return;
    m_blockSize = newBlockSize;
    emit blockSizeChanged();
}

bool Settings::ipValidated() const
{
    return m_ipValidated;
}

void Settings::setIpValidated(bool newIpValidated)
{
    m_ipValidated = newIpValidated;
    emit ipValidatedChanged();
}

const QStringList &Settings::scpPaths() const
{
    return m_scpPaths;
}

bool Settings::addScpPath(const QString &path)
{
    if (m_scpPaths.contains(path))
        return false;
    m_scpPaths.append(path);
    return true;
}

void Settings::clsScpPaths()
{
    m_scpPaths.clear();
}

const QString &Settings::user() const
{
    return m_user;
}

void Settings::setUser(const QString &newUser)
{
    if (m_user == newUser)
        return;
    m_user = newUser;
    emit userChanged();
}

const QString &Settings::host() const
{
    return m_host;
}

void Settings::setHost(const QString &newHost)
{
    if (m_host == newHost)
        return;
    m_host = newHost;
    emit hostChanged();
}
