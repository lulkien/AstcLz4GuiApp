import QtQuick 2.0
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

Item {
    id: root

    property alias value: progBar.value

    ProgressBar {
        id: progBar
        width: parent.width
        height: parent.height*0.1

        from: 0
        to: 1
        value: 0.5

        background: Rectangle {
            width: progBar.width
            height: progBar.height
            border.color: "#999999"
            radius: 10
        }
        contentItem: Item {
            width: progBar.width
            height: progBar.height
            Rectangle {
                id: bar
                width: progBar.visualPosition * parent.width
                height: progBar.height
                radius: 10
            }
            LinearGradient {
                anchors.fill: bar
                start: Qt.point(0,0)
                end: Qt.point(bar.width, 0)
                source: bar
                gradient: Gradient {
                    GradientStop {position: 0.0; color: "#17A81A"}
                    GradientStop {id: grad; position: 0.5; color: Qt.lighter("#17A81A", 2)}
                    GradientStop {position: 1.0; color: "#17A81A"}
                }
                PropertyAnimation {
                    target: grad
                    property: "position"
                    from: 0.1
                    to: 0.9
                    duration: 1000
                    running: true
                    loops: Animation.Infinite
                }
            }
            LinearGradient {
                anchors.fill: bar
                start: Qt.point(0,0)
                end: Qt.point(0, bar.height)
                source: bar
                gradient: Gradient {
                    GradientStop {position: 0.0; color: Qt.rgba(0,0,0,0)}
                    GradientStop {position: 0.5; color: Qt.rgba(1,1,1,0.3)}
                    GradientStop {position: 1.0; color: Qt.rgba(0,0,0,0.05)}
                }
            }
        }
//        PropertyAnimation {
//            target: progBar
//            property: "value"
//            from: 0
//            to: 1
//            duration: 2000
//            running: true
//            loops: Animation.Infinite
//        }

    }

}
