import QtQuick 2.0
import QtQuick.Controls 2.12

Item {
    id: root
    property alias label: text
    property alias box: cbb
    height: GUI.globalBarHeight
    clip: true
    Row {
        anchors.fill: parent
        spacing: 10
        ALG_Text {
            id: text
            font.pixelSize: 13
            height: parent.height
            verticalAlignment: Text.AlignVCenter
        }

        ComboBox {
            id: cbb
            width: 150
            height: parent.height
            background: Rectangle {
                width: cbb.width
                height: cbb.height
                opacity: enabled ? 1 : 0.3
                color: GUI.whitesmoke
                border.color: GUI.black
            }
            delegate: Rectangle {
                width: cbb.width
                height: cbb.height
                color: GUI.snow
                Rectangle {
                    anchors.bottom: parent.bottom
                    color: GUI.gray
                    width: parent.width
                    height: 1
                }
                Text {
                    text: modelData
                    color: "#404040"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        cbb.activated(index)
                        cbb.popup.close()
                    }
                }
            }
        }
    }
}
