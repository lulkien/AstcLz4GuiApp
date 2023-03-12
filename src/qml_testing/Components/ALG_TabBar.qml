import QtQuick 2.0
import QmlCustomType 1.0
import "Common"

Rectangle {
    id: root

    width: 100
    height: 800

    color: "lightgray"

    ALG_TabIndexer {
        id: indexer

        width: root.width
        idxerColor: root.color
        bgColor: "white"

        anchors {
            top: parent.top
            topMargin: ((AppModel.currentTab + 0.5) * root.width) - (indexer.height / 2)
            horizontalCenter: root.horizontalCenter
        }

        Behavior on anchors.topMargin {
            NumberAnimation {
                duration: 200
            }
        }
    }

    ALG_TabItem {
        id: main
        anchors {
            top: parent.top
            left: parent.left
        }
        width: root.width
        name: "Main"
        icoSource: "qrc:/images/resources/png/home.png"
        screenID: QmlEvents.MAIN_SCREEN
    }

    ALG_TabItem {
        id: settings
        anchors {
            top: main.bottom
            left: parent.left
        }
        width: root.width
        name: "Setup"
        icoSource: "qrc:/images/resources/png/setup.png"
        screenID: QmlEvents.SETUP_SCREEN
    }
}
