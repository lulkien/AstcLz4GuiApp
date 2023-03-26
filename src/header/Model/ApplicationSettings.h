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
    Q_PROPERTY(int  blockSize       READ blockSize      WRITE setBlockSize      NOTIFY blockSizeChanged)

    // app setting
    Q_PROPERTY(int  logLevel        READ logLevel       WRITE setLogLevel       NOTIFY logLevelChanged)
    Q_PROPERTY(bool showOutput      READ showOutput     WRITE setShowOutput     NOTIFY showOutputChanged)

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

private:
    bool m_noFlip;
    bool m_noPremult;
    bool m_useBackup;
    bool m_veryfast;
    int  m_logLevel;
    bool m_showOutput;
    int  m_blockSize;
};

#endif // APPLICATIONSETTINGS_H
