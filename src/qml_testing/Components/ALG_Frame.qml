import QtQuick 2.0

Item {
    id: root

    property alias frameText:   frameID
    property int spacer:        10
    property color borderColor: "black"
    property int borderSize:    1
    property bool border:       true

    Text {
        id: frameID
        anchors {
            verticalCenter: root.top
            horizontalCenter: root.horizontalCenter
        }

        text: ""
    }

    Rectangle {
        id: topLeftBorder
        width: (root.width - frameID.width) / 2 - root.spacer
        height: root.borderSize
        color: root.borderColor
        visible: root.border
    }

    Rectangle {
        id: topRightBorder
        anchors.right: root.right
        width: topLeftBorder.width
        height: root.borderSize
        color: root.borderColor
        visible: root.border
    }

    Rectangle {
        id: leftBorder
        height: root.height
        width: root.borderSize
        color: root.borderColor
        visible: root.border
    }

    Rectangle {
        id: rightBorder
        anchors.right: root.right
        height: root.height
        width: root.borderSize
        color: root.borderColor
        visible: root.border
    }

    Rectangle {
        id: bottomBorder
        anchors.bottom: root.bottom
        height: root.borderSize
        width: root.width
        color: root.borderColor
        visible: root.border
    }
}
