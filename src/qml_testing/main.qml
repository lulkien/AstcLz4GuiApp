import QtQuick 2.12
import QtQuick.Window 2.12
import "Components"
import "Screens"

Window {
    id: root
    visible: true
    minimumWidth: 800
    minimumHeight: 600
    maximumWidth: 800
    maximumHeight: 600
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
