import QtQuick 2.0

ListView {
    id: root
    property int timeOutInterval: 1500

    width: GUI.globalButtonSize
    height: GUI.globalButtonSize
    interactive: false
    onVisibleChanged: {
        if (visible) {
            timeOut.restart()
        }
    }

    Timer {
        id: timeOut
        interval: root.timeOutInterval
        running: false
        repeat: false
        onTriggered: root.visible = false
    }
}
