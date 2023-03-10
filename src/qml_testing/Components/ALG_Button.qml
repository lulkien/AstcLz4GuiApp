import QtQuick 2.0

Rectangle {
    id: rootBtn

    property bool isEnabled: true
    property alias label: textBtn.text

    signal clicked()

    width: 100
    height: 100
    border.color: rootBtn.isEnabled ? "#000000" : "#CACACA"
    border.width: 2
    clip: true

    ALG_Text {
        id: textBtn
        anchors.centerIn: parent
        width: parent.width
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        text: ""
        font.pixelSize: 20
        font.bold: true
        color: rootBtn.isEnabled ? "#000000" : "#CACACA"
    }

    MouseArea {
        enabled: rootBtn.isEnabled
        anchors.fill: parent
        onClicked: {
            rootBtn.clicked()
        }
    }
}
