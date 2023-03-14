#ifndef PROCESSWORKER_H
#define PROCESSWORKER_H

#include <QObject>

class ProcessWorker : public QObject
{
    Q_OBJECT
public:
    ProcessWorker();
    void startProcessImages();

public slots:
    void onStartProcessImages();

private:
    bool createOutputDirectory();
    bool makeTempDir(const QString &tmpDir);
    bool makeResultDir(const QString &resultDir);

    void removeTempFiles();

signals:


};

#endif // PROCESSWORKER_H
