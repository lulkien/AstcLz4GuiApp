#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

#define SETTINGS Settings::instance()

class Settings : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Settings)
    Q_PROPERTY(bool noFlip      READ noFlip     WRITE setNoFlip     NOTIFY noFlipChanged)
    Q_PROPERTY(bool keep        READ keep       WRITE setKeep       NOTIFY keepChanged)
    Q_PROPERTY(bool noPremult   READ noPremult  WRITE setNoPremult  NOTIFY noPremultChanged)
    Q_PROPERTY(bool noAstc      READ noAstc     WRITE setNoAstc     NOTIFY noAstcChanged)
    Q_PROPERTY(bool veryfast    READ veryfast   WRITE setveryfast   NOTIFY veryfastChanged)
    Q_PROPERTY(bool backupAstc  READ backupAstc WRITE setBackupAstc NOTIFY backupAstcChanged)

public:
    static Settings& instance();
    bool noFlip() const     { return m_noFlip; }
    bool keep() const       { return m_keep; }
    bool noPremult() const  { return m_noPremult; }
    bool noAstc() const     { return m_noAstc; }
    bool veryfast() const   { return m_veryfast; }
    bool backupAstc() const { return m_backupAstc; }

private:
    Settings();

public slots:
    void setNoFlip(bool noFlip);
    void setKeep(bool keep);
    void setNoPremult(bool noPremult);
    void setNoAstc(bool noAstc);
    void setveryfast(bool veryfast);
    void setBackupAstc(bool backupAstc);

signals:
    void noFlipChanged(bool noFlip);
    void keepChanged(bool keep);
    void noPremultChanged(bool noPremult);
    void noAstcChanged(bool noAstc);
    void veryfastChanged(bool veryfast);

    void backupAstcChanged(bool backupAstc);

private:
    bool m_noFlip;
    bool m_keep;
    bool m_noPremult;
    bool m_noAstc;
    bool m_veryfast;
    bool m_backupAstc;
};

#endif // SETTINGS_H
