import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "./Components"

Item {
    id: root

    property int idx: 0
    property bool enterBtn: false
    property var tabs: [
        { icon: "qrc:/icons/Images/HomeIcon.png"    , tip: qsTr("Home")     },
        { icon: "qrc:/icons/Images/SettingIcon.png" , tip: qsTr("Setting")  },
        { icon: "qrc:/icons/Images/AboutIcon.png"   , tip: qsTr("About")    }
    ]

    Rectangle {
        anchors.fill: parent
        color: "#5D6D7E"
    }

    Column {
        Repeater {
            model: 3
            QButton {
                id: btnTab
                width: root.width
                height: root.height/10
                iconSrc: tabs[index].icon
                isFocus: root.idx === index
                color: isFocus ? "#ECF0F1" : "#7B7D7D"
                onClickedBtn: {
                    root.idx = index
                }
                Rectangle {
                    visible: root.idx === index
                    width: parent.width/20
                    height: parent.height - 2*parent.border.width
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#FFFFFF"
                }
                ToolTip {
                    visible: btnTab.isEnter
                    text: tabs[index].tip
                    delay: 500
                    timeout: 1000
                }
            }
        }
    }
}
