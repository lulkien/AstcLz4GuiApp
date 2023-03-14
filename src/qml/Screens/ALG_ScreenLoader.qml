import QtQuick 2.0
import QmlCustomType 1.0

Item {
    id: root

    Rectangle {
        anchors.fill: parent
        color: GUI.lightgray
    }

    Rectangle {
        anchors.left: parent.left
        width: 1
        height: parent.height
        color: GUI.black
    }

    Loader {
        id: mainScreenLoader
        asynchronous: true
        anchors.fill: parent
        visible: AppModel.currentTab === QmlEvents.MAIN_SCREEN
        source: "qrc:/qml/Screens/ALG_MainScreen.qml"
    }

    Loader {
        id: setupScreenLoader
        asynchronous: true
        anchors.fill: parent
        visible: AppModel.currentTab === QmlEvents.SETUP_SCREEN
        source: "qrc:/qml/Screens/ALG_SetupScreen.qml"
    }
}
