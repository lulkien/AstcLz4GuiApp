#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QObject>

#define SETTINGS Settings::instance()

class Settings : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Settings)
    Q_PROPERTY(bool noFlip          READ noFlip         WRITE setNoFlip         NOTIFY noFlipChanged)
    Q_PROPERTY(bool noPremult       READ noPremult      WRITE setNoPremult      NOTIFY noPremultChanged)
    Q_PROPERTY(bool useBackup       READ useBackup      WRITE setUseBackup      NOTIFY useBackupChanged)
    Q_PROPERTY(bool veryfast        READ veryfast       WRITE setVeryfast       NOTIFY veryfastChanged)

    // app setting
    Q_PROPERTY(int  logLevel        READ logLevel       WRITE setLogLevel       NOTIFY logLevelChanged)
    Q_PROPERTY(bool clearBackup     READ clearBackup    WRITE setClearBackup    NOTIFY clearBackupChanged)

public:
    static Settings& instance();

    bool noFlip() const;
    void setNoFlip(bool newNoFlip);

    bool noPremult() const;
    void setNoPremult(bool newNoPremult);

    bool useBackup() const;
    void setUseBackup(bool newUseBackup);

    bool veryfast() const;
    void setVeryfast(bool newVeryfast);

    int logLevel() const;
    void setLogLevel(int newLogLevel);

    bool clearBackup() const;
    void setClearBackup(bool newClearBackup);

private:
    Settings();

public slots:

signals:
    void noFlipChanged();
    void noPremultChanged();
    void useBackupChanged();
    void veryfastChanged();
    void logLevelChanged();
    void clearBackupChanged();

private:
    bool m_noFlip;
    bool m_noPremult;
    bool m_useBackup;
    bool m_veryfast;
    int m_logLevel;
    bool m_clearBackup;
};

#endif // APPLICATIONSETTINGS_H
