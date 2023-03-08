#include "Settings.h"

Settings &Settings::instance()
{
    static Settings _self;
    return _self;
}

Settings::Settings()
    : m_noFlip      { false }
    , m_keep        { false }
    , m_noPremult   { false }
    , m_noAstc      { false }
    , m_veryfast    { false }
    , m_backupAstc  { false }
{

}

void Settings::setNoFlip(bool noFlip)
{
    if (m_noFlip == noFlip)
        return;

    m_noFlip = noFlip;
    emit noFlipChanged(m_noFlip);
}

void Settings::setKeep(bool keep)
{
    if (m_keep == keep)
        return;

    m_keep = keep;
    emit keepChanged(m_keep);
}

void Settings::setNoPremult(bool noPremult)
{
    if (m_noPremult == noPremult)
        return;

    m_noPremult = noPremult;
    emit noPremultChanged(m_noPremult);
}

void Settings::setNoAstc(bool noAstc)
{
    if (m_noAstc == noAstc)
        return;

    m_noAstc = noAstc;
    emit noAstcChanged(m_noAstc);
}

void Settings::setveryfast(bool veryfast)
{
    if (m_veryfast == veryfast)
        return;

    m_veryfast = veryfast;
    emit veryfastChanged(m_veryfast);
}

void Settings::setBackupAstc(bool backupAstc)
{
    if (m_backupAstc == backupAstc)
        return;

    m_backupAstc = backupAstc;
    emit backupAstcChanged(m_backupAstc);
}
