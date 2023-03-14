import QtQuick 2.12
import QtQuick.Window 2.12
import "Components"
import "Screens"

Window {
    id: root
    visible: true
    width: GUI.defaultWidth
    height: GUI.defaultHeight
    minimumWidth: GUI.defaultWidth
    minimumHeight: GUI.defaultHeight
    title: qsTr("Over-engineering ASTC generator")

    Rectangle {
        anchors.fill: parent
        color: GUI.whitesmoke
    }

    ALG_ScreenLoader {
        id: loader
        width: root.width - screenBar.width * 1.35
        height: root.height
        anchors.right: parent.right
    }

    ALG_TabBar {
        id: screenBar
        width: GUI.globalButtonSize
        height: root.height
    }
}
