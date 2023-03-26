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

    enum ButtonState {
        BROWSE = 0,
        LOAD,
        EXECUTE,
        TERMINATE,
    };
    Q_ENUM(ButtonState)

    enum BlockSize {
        BLK_4x4 = 0,
        BLK_5x5,
        BLK_6x6,
        BLK_8x6,
        BLK_8x8,
        BLK_10x8,
        BLK_10x10,
        BLK_10x12,
        BLK_12x12,
    };
    Q_ENUM(BlockSize)

    explicit Events(QObject *parent = nullptr);

public slots:
    void qmlSendEvent(int eventId, QByteArray data = QByteArray());

signals:
    void sendEvent(int eventId, QByteArray data);
};

#endif // EVENTS_H
