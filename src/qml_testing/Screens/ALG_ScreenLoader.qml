import QtQuick 2.0
import QmlCustomType 1.0

Item {
    id: root

    Rectangle {
        anchors.fill: parent
        color: "lightgray"
    }

    Loader {
        anchors.fill: parent
        source: getSource(AppModel.currentTab)
    }

    function getSource(screenID) {
        switch (screenID) {
        case QmlEvents.MAIN_SCREEN:
            return "qrc:/qml/qml_testing/Screens/ALG_MainScreen.qml"
        case QmlEvents.SETUP_SCREEN:
            return "qrc:/qml/qml_testing/Screens/ALG_SetupScreen.qml"
        default:
            return ""
        }
    }
}
