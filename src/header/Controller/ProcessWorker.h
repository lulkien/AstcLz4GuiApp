#ifndef PROCESSWORKER_H
#define PROCESSWORKER_H

#include <QObject>

class ProcessWorker : public QObject
{
    Q_OBJECT
public:
    ProcessWorker();

public slots:
    void onStartProcessImages();

signals:

};

#endif // PROCESSWORKER_H
