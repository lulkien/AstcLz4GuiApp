#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QObject>
#include <QStringList>

#define SETTINGS Settings::instance()

class Settings : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Settings)
    Q_PROPERTY(bool noFlip          READ noFlip         WRITE setNoFlip         NOTIFY noFlipChanged)
    Q_PROPERTY(bool noPremult       READ noPremult      WRITE setNoPremult      NOTIFY noPremultChanged)
    Q_PROPERTY(bool useBackup       READ useBackup      WRITE setUseBackup      NOTIFY useBackupChanged)
    Q_PROPERTY(bool veryfast        READ veryfast       WRITE setVeryfast       NOTIFY veryfastChanged)
    Q_PROPERTY(int  blockSize       READ blockSize      WRITE setBlockSize      NOTIFY blockSizeChanged)

    // app setting
    Q_PROPERTY(int  logLevel        READ logLevel       WRITE setLogLevel       NOTIFY logLevelChanged)
    Q_PROPERTY(bool showOutput      READ showOutput     WRITE setShowOutput     NOTIFY showOutputChanged)

    // deployment setting
    Q_PROPERTY(bool ipValidated     READ ipValidated    WRITE setIpValidated    NOTIFY ipValidatedChanged)
    Q_PROPERTY(QString user         READ user           WRITE setUser           NOTIFY userChanged)
    Q_PROPERTY(QString host         READ host           WRITE setHost           NOTIFY hostChanged)

public:
    static Settings& instance();
    bool loadConfigFile();
    bool createDefaultConfigFile();
    void saveConfigFile();

    const bool &noFlip() const;
    void setNoFlip(bool newNoFlip);

    const bool &noPremult() const;
    void setNoPremult(bool newNoPremult);

    const bool &useBackup() const;
    void setUseBackup(bool newUseBackup);

    const bool &veryfast() const;
    void setVeryfast(bool newVeryfast);

    const int &blockSize() const;
    void setBlockSize(int newBlockSize);

    const int &logLevel() const;
    void setLogLevel(int newLogLevel);

    const bool &showOutput() const;
    void setShowOutput(bool newShowOutput);

    bool ipValidated() const;
    void setIpValidated(bool newIpValidated);

    const QStringList &scpPaths() const;

    const QString &user() const;
    void setUser(const QString &newUser);

    const QString &host() const;
    void setHost(const QString &newHost);

public slots:
    bool addScpPath(const QString &path);
    void clsScpPaths();

private:
    Settings();

signals:
    void noFlipChanged();
    void noPremultChanged();
    void useBackupChanged();
    void veryfastChanged();
    void logLevelChanged();
    void showOutputChanged();
    void blockSizeChanged();
    void ipValidatedChanged();
    void userChanged();

    void hostChanged();

private:
    bool m_noFlip;
    bool m_noPremult;
    bool m_useBackup;
    bool m_veryfast;
    int  m_logLevel;
    bool m_showOutput;
    int  m_blockSize;
    bool m_ipValidated;
    QStringList m_scpPaths;
    QString m_user;
    QString m_host;
};

#endif // APPLICATIONSETTINGS_H
