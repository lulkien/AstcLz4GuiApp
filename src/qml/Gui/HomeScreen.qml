import QtQuick 2.0

Item {
    implicitWidth: 750
    implicitHeight: 400
    QHMI_InputFiles {
        id: inputFolder
        width: parent.width
        height: 70
        anchors {
            top: parent.top
            topMargin: parent.height/8
            horizontalCenter: parent.horizontalCenter
        }
    }

    QHMI_OptionBox {
        id: optionBox
        width: parent.width/3 - 20
        height: parent.height/3
        anchors {
            top: parent.top
            topMargin: parent.height/4
            left: parent.left
            leftMargin: parent.width/10
        }
    }

    QHMI_LogOutput {
        id: logOutput
        width: parent.width/2 - 50
        height: parent.height/3
        anchors {
            top: parent.top
            topMargin: parent.height/4 + 22
            right: parent.right
            rightMargin: parent.width/10
        }
        log: ""
        value: 0.5
    }
}
