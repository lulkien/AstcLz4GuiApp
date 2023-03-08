#ifndef APPLICATIONENGINE_H
#define APPLICATIONENGINE_H

#include <QDir>
#include <QObject>
#include <QThread>
#include <QQmlApplicationEngine>
#include "ProcessWorker.h"
#include "Events.h"

class ApplicationEngine : public QQmlApplicationEngine
{
    Q_OBJECT
public:
    ApplicationEngine();
    ~ApplicationEngine();
    void startApplication();

private:
    // init
    void registerQmlType();
    void initQmlcontext();
    void initConnection();

    // event handler
    void loadImages();
    void genImages();

    // general
    QStringList scanAllPngFromDirectory(const QString &dir);
    QString getCurrentUser();


private slots:
    void onReceivedEvent(int eventId, QByteArray data);
    void onSourcePathChanged();

signals:
    void requestStartProcessImages();

private:
    Events m_eventHandler;
    QString m_username;
    QThread m_processThread;
    ProcessWorker m_processWorker;
};

#endif // APPLICATIONENGINE_H
