#ifndef EVENTS_H
#define EVENTS_H

#include <QObject>
#include <QByteArray>

class Events : public QObject
{
    Q_OBJECT
public:
    enum EventID {
        REQ_GEN_IMAGE,
        REQ_LOAD_IMAGES,
        REQ_VERIFY_SOURCE,
    };
    Q_ENUM(EventID)

    enum ScreenID {
        MAIN_SCREEN,
        SETUP_SCREEN,
    };
    Q_ENUM(ScreenID)

    enum LogLevel {
        QML_DEBUG = 0,
        QML_INFO,
        QML_WARN,
        QML_FATAL,
    };
    Q_ENUM(LogLevel)

    enum Resolutions {
        RES_640X480,
        RES_800X600,
        RES_960X720,
        RES_1024X768,
        RES_1280X960,
    };
    Q_ENUM(Resolutions)

    explicit Events(QObject *parent = nullptr);

public slots:
    void qmlSendEvent(int eventId, QByteArray data = QByteArray());

signals:
    void sendEvent(int eventId, QByteArray data);
};

#endif // EVENTS_H
