import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root
    property alias label: label.text
    property alias validdata: input.validator
    Row {
        spacing: 10
        Rectangle {
            id: rect
            width: root.width - 50
            height: 30
            border.color: "#6C6362"
            radius: 10
            TextInput{
                id: input
                anchors.centerIn: parent
                width: parent.width-10
                height: parent.height-10
                selectByMouse: true
                selectionColor: "#64B7F7"
                selectedTextColor: "#FFFFFF"
                clip: true
                font.pointSize: 11
//                Rectangle {
//                    anchors.fill: parent
//                    color: "red"
//                    opacity: 0.5
//                }
            }
        }
        Text {
            id: label
            text: ""
            anchors.verticalCenter: rect.verticalCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
