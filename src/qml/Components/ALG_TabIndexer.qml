import QtQuick 2.0
import "Common"

Item {
    id: root

    width: GUI.globalButtonSize
    height: GUI.globalButtonSize

    property color bgColor: GUI.whitesmoke
    property color idxerColor: GUI.lightgray

    Rectangle {
        id: bg_idxer
        readonly property double widthFactor: 1.7 // modify this offset for better looking
        readonly property double heightFactor: .7 // modify this offset for better looking
        width: root.width * bg_idxer.widthFactor
        height: root.height * bg_idxer.heightFactor
        radius: height / 2
        anchors {
            horizontalCenter: root.horizontalCenter
            verticalCenter: root.verticalCenter
        }
        color: root.bgColor
    }

    Rectangle {
        anchors.top: bg_idxer.top
        width: root.width ; height: 1
        color: GUI.black
    }
    Rectangle {
        anchors.bottom: bg_idxer.bottom
        width: root.width ; height: 1
        color: GUI.black
    }

    Rectangle {
        id: idxer
        readonly property double widthFactor: .93 // modify this offset for better looking
        readonly property double heightFactor: .85 // modify this offset for better looking
        width: bg_idxer.width * idxer.widthFactor
        height: bg_idxer.height * idxer.heightFactor
        radius: height / 2
        anchors {
            horizontalCenter: bg_idxer.horizontalCenter
            verticalCenter: bg_idxer.verticalCenter
        }
        color: root.idxerColor
    }
}
