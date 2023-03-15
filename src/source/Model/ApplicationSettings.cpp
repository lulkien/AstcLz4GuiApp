#include "ApplicationSettings.h"
#include "Events.h"

Settings &Settings::instance()
{
    static Settings _self;
    return _self;
}

Settings::Settings()
    : m_noFlip          { false }
    , m_noPremult       { false }
    , m_useBackup       {  true }
    , m_veryfast        { false }
    , m_logLevel        { static_cast<int>(Events::QML_WARN) }
{

}

bool Settings::noFlip() const
{
    return m_noFlip;
}

void Settings::setNoFlip(bool newNoFlip)
{
    if (m_noFlip == newNoFlip)
        return;
    m_noFlip = newNoFlip;
    emit noFlipChanged();
}

bool Settings::noPremult() const
{
    return m_noPremult;
}

void Settings::setNoPremult(bool newNoPremult)
{
    if (m_noPremult == newNoPremult)
        return;
    m_noPremult = newNoPremult;
    emit noPremultChanged();
}

bool Settings::useBackup() const
{
    return m_useBackup;
}

void Settings::setUseBackup(bool newUseBackup)
{
    if (m_useBackup == newUseBackup)
        return;
    m_useBackup = newUseBackup;
    emit useBackupChanged();
}

bool Settings::veryfast() const
{
    return m_veryfast;
}

void Settings::setVeryfast(bool newVeryfast)
{
    if (m_veryfast == newVeryfast)
        return;
    m_veryfast = newVeryfast;
    emit veryfastChanged();
}

int Settings::logLevel() const
{
    return m_logLevel;
}

void Settings::setLogLevel(int newLogLevel)
{
    if (m_logLevel == newLogLevel)
        return;
    m_logLevel = newLogLevel;
    emit logLevelChanged();
}

bool Settings::clearBackup() const
{
    return m_clearBackup;
}

void Settings::setClearBackup(bool newClearBackup)
{
    if (m_clearBackup == newClearBackup)
        return;
    m_clearBackup = newClearBackup;
    emit clearBackupChanged();
}
