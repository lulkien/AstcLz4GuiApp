#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

#define SETTINGS Settings::instance()

class Settings : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Settings)
    Q_PROPERTY(bool noFlip          READ noFlip         WRITE setNoFlip         NOTIFY noFlipChanged)
    Q_PROPERTY(bool useBackup       READ useBackup      WRITE setUseBackup      NOTIFY useBackupChanged)
    Q_PROPERTY(bool noPremult       READ noPremult      WRITE setNoPremult      NOTIFY noPremultChanged)
    Q_PROPERTY(bool veryfast        READ veryfast       WRITE setveryfast       NOTIFY veryfastChanged)

public:
    static Settings& instance();
    bool noFlip() const             { return m_noFlip; }
    bool useBackup() const      { return m_useBackup; }
    bool noPremult() const          { return m_noPremult; }
    bool veryfast() const           { return m_veryfast; }

private:
    Settings();

public slots:
    void setNoFlip(bool noFlip);
    void setUseBackup(bool useBackup);
    void setNoPremult(bool noPremult);
    void setveryfast(bool veryfast);

signals:
    void noFlipChanged(bool noFlip);
    void useBackupChanged(bool useBackup);
    void noPremultChanged(bool noPremult);
    void veryfastChanged(bool veryfast);

private:
    bool m_noFlip;
    bool m_useBackup;
    bool m_noPremult;
    bool m_veryfast;
};

#endif // SETTINGS_H
