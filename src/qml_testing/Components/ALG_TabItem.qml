import QtQuick 2.0
import "Common"

Item {
    id: root

    width: 100
    height: width

    property string name: ""
    property string icoSource: ""
    property int screenID: -1
    readonly property bool isSelected: AppModel.currentTab === screenID

    readonly property int translationInterval: 200

    signal clicked()

    Image {
        id: ico
        source: root.icoSource
        readonly property int selectedHOffset: root.width / 2

        anchors {
            verticalCenter: root.verticalCenter
            horizontalCenter: root.horizontalCenter
            horizontalCenterOffset: root.isSelected ? ico.selectedHOffset : 0
        }
        antialiasing:  true

        sourceSize.width: Math.floor(root.width / 2.5)
        sourceSize.height: Math.floor(root.width / 2.5)
        width: sourceSize.width
        height: sourceSize.height

        Behavior on anchors.horizontalCenterOffset {
            NumberAnimation {
                duration: root.translationInterval
            }
        }
    }

    ALG_Text {
        id: label
        anchors {
            left: parent.left
            leftMargin: (parent.width - ico.width / 2 - label.width) / 2
            verticalCenter: parent.verticalCenter
        }
        opacity: root.isSelected ? 1 : 0
        text: root.name
        font.pixelSize: root.width / 6
        font.bold: true
        Behavior on opacity {
            NumberAnimation {
                duration: root.translationInterval
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.clicked()
            AppModel.currentTab = root.screenID
        }
    }
}
