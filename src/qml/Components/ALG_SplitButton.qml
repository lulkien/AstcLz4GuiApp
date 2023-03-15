import QtQuick 2.0

ListView {
    id: root
    property int timeOutInterval: 2000

    width: GUI.globalButtonSize
    height: GUI.globalButtonSize
    interactive: false
    onVisibleChanged: {
        if (visible)
            timeOut.restart()
        else
            timeOut.stop()
    }

    Timer {
        id: timeOut
        interval: root.timeOutInterval
        running: false
        repeat: false
        onTriggered: root.visible = false
    }
}
