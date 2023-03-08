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
    };
    Q_ENUMS(EventID)

    enum ScreenID {
        MAIN_SCREEN,
        SETUP_SCREEN,
    };
    Q_ENUMS(ScreenID)

    explicit Events(QObject *parent = nullptr);

public slots:
    void qmlSendEvent(int eventId, QByteArray data = QByteArray());

signals:
    void sendEvent(int eventId, QByteArray data);
};

#endif // EVENTS_H
