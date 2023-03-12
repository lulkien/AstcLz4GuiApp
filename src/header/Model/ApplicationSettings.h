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
    Q_PROPERTY(int  resolutionID    READ resolutionID   WRITE setResolutionID   NOTIFY resolutionIDChanged)

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

    int resolutionID() const;
    void setResolutionID(int newResolutionID);

private:
    Settings();

public slots:

signals:
    void logLevelChanged();
    void noFlipChanged();
    void noPremultChanged();
    void useBackupChanged();
    void veryfastChanged();
    void resolutionIDChanged();

private:
    bool m_noFlip;
    bool m_noPremult;
    bool m_useBackup;
    bool m_veryfast;
    int m_logLevel;
    int m_resolutionID;
};

#endif // APPLICATIONSETTINGS_H
