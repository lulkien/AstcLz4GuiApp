import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import "Screens" as SCR
import "./Components"

Window {
    id: root
    visible: true
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("GEN IMAGES")

    property var tabs: [
        "qrc:/Gui/HomeScreen.qml"   ,
        "qrc:/Gui/SettingScreen.qml",
        "qrc:/Gui/AboutScreen.qml"
    ]

    QHMI_Tabbar {
        id: tabBar
        width: parent.width/12
        height: parent.height

    }

    Item {
        id: content
        width: parent.width - tabBar.width
        height: parent.height
        anchors {
            top: parent.top
            left: tabBar.right
        }
        Loader {
            anchors.fill: parent
            source: tabs[tabBar.idx]
        }
    }
}
