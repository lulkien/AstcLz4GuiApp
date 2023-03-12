import QtQuick 2.12
import QtQuick.Window 2.12
import "Components"
import "Screens"

Window {
    id: root
    visible: true
    width: GUI.globalWidth
    height: GUI.globalHeight
//    minimumWidth: GUI.globalWidth
//    maximumWidth: GUI.globalWidth
//    minimumHeight: GUI.globalHeight
//    maximumHeight: GUI.globalHeight
    title: qsTr("Over-engineering ASTC generator")

    ALG_ScreenLoader {
        id: loader
        width: root.width - screenBar.width * 1.35
        height: root.height
        anchors.right: parent.right
    }

    ALG_TabBar {
        id: screenBar
        width: 100
        height: root.height
    }
}
