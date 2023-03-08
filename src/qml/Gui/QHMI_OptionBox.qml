import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "./Components"

Item {
    id: root
    ColumnLayout {
        id: colOption
        GroupBox {
            id: boxOptions
            title: qsTr("<i><b>Options<b><i>")
            implicitWidth: root.width
            Column {
                QRadioButton {
                    id: radio1
                    text: qsTr("Normal")
                    checked: true
                }
                QRadioButton {
                    id: radio2
                    text: qsTr("Push to Board")
                }
            }
        }
        GroupBox {
            id: boxInput
            visible: radio2.checked
            implicitWidth: boxOptions.implicitWidth
            Column {
                spacing: 10
                QFormInput {
                    width: 9*boxInput.width/10
                    height: 30
                    label: qsTr("IP.")
                    validdata: RegExpValidator{regExp: /^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$/}
                }
                QFormInput {
                    width: 9*boxInput.width/10
                    height: 30
                    label: qsTr("Path")
                }
            }
        }
    }
    QButton {
        id: btnCompress
        height: 30
        txtBtn: qsTr("Compress")
        radius: 10
        anchors {
            top: colOption.bottom
            topMargin: 10
            right: colOption.right
        }
        onClickedBtn: {
            ///
        }
    }
    states: [
        State {
            name: "visible"
            when: boxInput.visible
            PropertyChanges { target: boxInput; opacity: 1.0 }
            PropertyChanges { target: btnCompress; y: 220}
        },
        State {
            name: "invisible"
            when: !boxInput.visible
            PropertyChanges { target: boxInput; opacity: 0.0}
            PropertyChanges { target: btnCompress; y: 121}
        }
    ]
    transitions: [
        Transition {
            from: "*"
            to: "*"
            ParallelAnimation {
                NumberAnimation {
                    target: boxInput
                    property: "opacity"
                    duration: 500
                    easing.type: Easing.InOutQuad
                }

                NumberAnimation {
                    target: btnCompress
                    property: "y"
                    duration: 2000
                    easing.type: Easing.OutElastic
                }
            }
        }
    ]
}
