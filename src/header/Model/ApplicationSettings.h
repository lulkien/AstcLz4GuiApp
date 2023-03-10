#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QObject>

#define SETTINGS Settings::instance()

class Settings : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Settings)
    Q_PROPERTY(bool noFlip          READ noFlip         WRITE setNoFlip         NOTIFY noFlipChanged)
    Q_PROPERTY(bool useBackup       READ useBackup      WRITE setUseBackup      NOTIFY useBackupChanged)
    Q_PROPERTY(bool noPremult       READ noPremult      WRITE setNoPremult      NOTIFY noPremultChanged)
    Q_PROPERTY(bool veryfast        READ veryfast       WRITE setVeryfast       NOTIFY veryfastChanged)
    Q_PROPERTY(int  logLevel        READ logLevel       WRITE setLogLevel       NOTIFY logLevelChanged)

public:
    static Settings& instance();

    bool noFlip() const;
    void setNoFlip(bool newNoFlip);

    bool useBackup() const;
    void setUseBackup(bool newUseBackup);

    bool noPremult() const;
    void setNoPremult(bool newNoPremult);

    bool veryfast() const;
    void setVeryfast(bool newVeryfast);

    int logLevel() const;
    void setLogLevel(int newLogLevel);

private:
    Settings();

public slots:

signals:
    void logLevelChanged();

    void noFlipChanged();

    void useBackupChanged();

    void noPremultChanged();

    void veryfastChanged();

private:
    bool m_noFlip;
    bool m_useBackup;
    bool m_noPremult;
    bool m_veryfast;
    int m_logLevel;
};

#endif // APPLICATIONSETTINGS_H
