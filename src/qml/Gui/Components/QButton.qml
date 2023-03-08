import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: root
    property alias txtBtn: txtBtn.text
    property alias iconSrc: icon.source

    property bool isFocus: false
    property bool isPress: false
    property bool isEnter: false

    signal clickedBtn()

    width: 100
    opacity: isPress ? 0.7 : 1
    border.color: !isPress ? "#0C0505" : "#494545"
    color: !isPress ? "#D6C8C8" : "#EFE8E8"
    Text {
        id: txtBtn
        anchors.centerIn: parent
        color: !isPress ? "#0C0505" : "#494545"
    }
    Image {
        id: icon
        width: parent.width/2
        height: parent.height/2
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectFit
        scale: isFocus ? 1.3 : 0.9
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            clickedBtn()
        }
        onPressed: {
            isPress = true
        }
        onReleased: {
            isPress = false
        }
        onEntered: {
            isEnter = true
        }
        onExited: {
            isEnter = false
        }
    }
}
