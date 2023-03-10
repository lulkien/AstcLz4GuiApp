#include "Settings.h"

Settings &Settings::instance()
{
    static Settings _self;
    return _self;
}

Settings::Settings()
    : m_noFlip          { false }
    , m_useBackup       { true }
    , m_noPremult       { false }
    , m_veryfast        { true }
{

}

void Settings::setNoFlip(bool noFlip)
{
    if (m_noFlip == noFlip)
        return;

    m_noFlip = noFlip;
    emit noFlipChanged(m_noFlip);
}

void Settings::setUseBackup(bool useBackup)
{
    if (m_useBackup == useBackup)
        return;

    m_useBackup = useBackup;
    emit useBackupChanged(m_useBackup);
}

void Settings::setNoPremult(bool noPremult)
{
    if (m_noPremult == noPremult)
        return;

    m_noPremult = noPremult;
    emit noPremultChanged(m_noPremult);
}

void Settings::setveryfast(bool veryfast)
{
    if (m_veryfast == veryfast)
        return;

    m_veryfast = veryfast;
    emit veryfastChanged(m_veryfast);
}
