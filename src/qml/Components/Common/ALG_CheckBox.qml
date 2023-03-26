import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root
    property alias box: checkBox
    property alias label: text
    clip: true
    height: GUI.globalBarHeight
    Row {
        anchors.fill: parent
        spacing: 10
        CheckBox {
            id: checkBox
            width: GUI.globalBarHeight
            height: GUI.globalBarHeight
            indicator: Rectangle {
                implicitWidth: checkBox.width
                implicitHeight: checkBox.height
                color: GUI.whitesmoke
                border.color: GUI.black

                Image {
                    source: "qrc:/images/resources/png/check.png"
                    anchors.centerIn: parent
                    sourceSize.width: parent.width * 0.6
                    sourceSize.height: parent.height * 0.6
                    visible: checkBox.checked
                    antialiasing: true
                    smooth: true
                }
            }
        }
        ALG_Text {
            id: text
            height: GUI.globalBarHeight
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 13
        }
    }
}

