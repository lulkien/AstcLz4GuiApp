import QtQuick 2.0

Item {
    id: root
    property int icoSize: 35
    property int animInterval: 200
    width: icoSize
    height: icoSize

    QtObject {
        id: privateProperties
        property int rotateIdx: 0
        readonly property int rotateAngle: 36
    }

    Image {
        id: img
        source: "qrc:/images/resources/png/loading.png"
        width: parent.width
        height: parent.height
        sourceSize.width: parent.width
        sourceSize.height: parent.height
        smooth: true
        antialiasing: true
        transform: Rotation {
            origin.x: img.width / 2
            origin.y: img.height / 2
            angle: privateProperties.rotateIdx * privateProperties.rotateAngle
        }
    }

    Timer {
        id: animationTimer
        interval: root.animInterval
        running: true
        repeat: true
        onTriggered: {
            if ((privateProperties.rotateIdx + 1) === 10)
                privateProperties.rotateIdx = 0
            else
                privateProperties.rotateIdx += 1
        }
    }

    onVisibleChanged: {
        if (root.visible) {
            animationTimer.running = true
        }
        else {
            animationTimer.running = false
            privateProperties.rotateIdx = 0
        }
    }

}
