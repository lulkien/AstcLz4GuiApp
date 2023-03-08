import QtQuick 2.0
import QtQuick.Controls 2.12
import "./Components"

Item {
    id: root
    property real value: progressBar.value
    property string log: txtLog.text

    Rectangle {
        id: box
        width: parent.width
        height: parent.height
        border.color: "#0C0505"
        radius: 10
        ScrollView {
            id: viewLog
            height: parent.height
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            clip: true
            TextArea {
                id: txtLog
                width: viewLog.width - 20
                wrapMode: Text.Wrap
                text: root.log
                onTextChanged: {
                    viewLog.ScrollBar.vertical.position = 1.0 - viewLog.ScrollBar.vertical.size
                }
            }
        }
//        Timer {
//            running: true
//            interval: 1000
//            repeat: true
//            property int iteration: 0
//            onTriggered: {
//                txtLog.append("Line %1\n".arg(iteration++))
//                txtLog.append("Line %1\n".arg(iteration++))
//            }
//        }
    }

    QProgressBar {
        id: progressBar
        width: box.width
        height: box.height
        anchors {
            top: box.bottom
            topMargin: 10
            horizontalCenter: root.horizontalCenter
        }
        value: 0.9
    }

}
