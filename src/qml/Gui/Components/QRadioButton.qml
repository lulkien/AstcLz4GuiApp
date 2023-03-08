import QtQuick 2.12
import QtQuick.Controls 2.12

RadioButton {
    id: root
    indicator: Rectangle {
        implicitWidth: 20
        implicitHeight: 20
        x: root.leftPadding
        y: parent.height/2 - height/2
        radius: 10
        border.color: !root.checked ? "#566573" : "#21BE2B"

        Rectangle {
            width: 10
            height: 10
            x: 5
            y: 5
            radius: 6
            color: !root.checked ? "#999999" : "#21BE2B"
            visible: root.checked
        }
    }
    contentItem: Text {
        text: root.text
        font: root.font
        color: !root.checked ? "#566573" : "#21BE2B"
        verticalAlignment: Text.AlignVCenter
        leftPadding: root.indicator.width + root.spacing
    }
}
