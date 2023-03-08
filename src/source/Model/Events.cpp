#include "Events.h"
#include "Common.h"

Events::Events(QObject *parent)
    : QObject(parent)
{

}

void Events::qmlSendEvent(int eventId, QByteArray data)
{
    INFO << "Send event:" << static_cast<EventID>(eventId)
         << ", Data:" << data;
    emit sendEvent(eventId, data);
}
