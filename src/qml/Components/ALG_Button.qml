import QtQuick 2.0
import QtQuick.Controls 2.5
import "Common"

Rectangle {
    id: root

    property alias label: textBtn
    property alias icon: ico
    property alias overlay: rec_overlay
    property alias tip: toolTip

    property bool enableTooltip: true
    property bool isEnabled: true
    property bool allowOverlay: true
    property bool isEnter: false
    property double icoRatio: 0.8
    property alias icoSource: ico.source

    border {
        width: 2
        color: GUI.black
    }
    color: GUI.whitesmoke

    signal clicked()

    width: 100
    height: 100
    clip: true

    ALG_Text {
        id: textBtn
        anchors.centerIn: parent
        width: parent.width
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20
        font.bold: true
        color: root.isEnabled ? GUI.invisible : GUI.gray
    }

    Image {
        id: ico
        visible: source !== ""
        anchors.centerIn: parent
        sourceSize.width: parent.width * root.icoRatio
        sourceSize.height: parent.height * root.icoRatio
        smooth: true
    }

    ALG_Overlay {
        id: rec_overlay
        anchors.fill: root
        visible: root.allowOverlay && !root.isEnabled
    }

    MouseArea {
        id: mouseArea
        enabled: root.isEnabled
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            root.clicked()
        }
        onEntered: {
            root.isEnter = true
        }
        onExited: {
            root.isEnter = false
        }
    }

    ToolTip {
        id: toolTip
        visible: root.enableTooltip && root.isEnter
        delay: 200
    }
}
