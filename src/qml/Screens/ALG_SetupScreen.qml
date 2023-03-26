import QtQuick 2.12
import QtQuick.Controls 2.12
import QmlCustomType 1.0
import "../Components" as Comp
import "../Components/Common" as Comm

Item {
    id: root

    QtObject {
        id: privateProperties
        readonly property int drawWidth: root.width - 2 * GUI.globalMargins
        readonly property int drawHeight: root.height - 2 * GUI.globalMargins
    }

    Comp.ALG_Frame {
        id: astcSettingsArea
        anchors {
            top: parent.top
            topMargin: GUI.globalMargins
            left: parent.left
            leftMargin: GUI.globalMargins
        }

        width: (root.width - 3 * GUI.globalMargins) / 2
        height: (root.height - 3 * GUI.globalMargins) / 2
        frameText.text: "Compression settings"

        Item {
            id: compressSettingLayout
            anchors.margins: GUI.globalMargins / 2
            anchors.fill: parent
            Column {
                spacing: 5
                Comm.ALG_CheckBox {
                    width: compressSettingLayout.width
                    box.checkState: AppSettings.noFlip ? Qt.Checked : Qt.Unchecked
                    box.onClicked: AppSettings.noFlip = !AppSettings.noFlip
                    label.text: "No flip image."
                }
                Comm.ALG_CheckBox {
                    width: compressSettingLayout.width
                    box.checkState: AppSettings.noPremult ? Qt.Checked : Qt.Unchecked
                    box.onClicked: AppSettings.noPremult = !AppSettings.noPremult
                    label.text: "No premultiply image."
                }
                Comm.ALG_CheckBox {
                    width: compressSettingLayout.width
                    box.checkState: AppSettings.useBackup ? Qt.Checked : Qt.Unchecked
                    box.onClicked: AppSettings.useBackup = !AppSettings.useBackup
                    label.text: "Use backup/pregenerated ASTC."
                }
                Comm.ALG_CheckBox {
                    width: compressSettingLayout.width
                    box.checkState: AppSettings.veryfast ? Qt.Checked : Qt.Unchecked
                    box.onClicked: AppSettings.veryfast = !AppSettings.veryfast
                    label.text: "Use veryfast compression."
                }
                Comm.ALG_ComboBox {
                    width: compressSettingLayout.width
                    box.currentIndex: AppSettings.blockSize
                    box.model: ListModel {
                        id: model
                        ListElement { text: "4x4"   }
                        ListElement { text: "5x5"   }
                        ListElement { text: "6x6"   }
                        ListElement { text: "8x6"   }
                        ListElement { text: "8x8"   }
                        ListElement { text: "10x8"  }
                        ListElement { text: "10x10" }
                        ListElement { text: "10x12" }
                        ListElement { text: "12x12" }
                    }
                    label.text: "Block size:"
                    box.onActivated: AppSettings.blockSize = index
                }
            }
        }
    }

    Comp.ALG_Frame {
        id: appSettingsArea
        anchors {
            top: parent.top
            topMargin: GUI.globalMargins
            right: parent.right
            rightMargin: GUI.globalMargins
        }

        width: astcSettingsArea.width
        height: astcSettingsArea.height
        frameText.text: "Application settings"

        Item {
            id: appSettingsLayout
            anchors.fill: parent
            anchors.margins: GUI.globalMargins
            Column {
                spacing: 5
                Comp.ALG_Slider {
                    width: appSettingsLayout.width
                    label {
                        text: "Log level: <b>" + getAliasedString(AppSettings.logLevel) + "<\b>"
                        textFormat: Text.RichText
                    }
                    slider {
                        from: QmlEvents.QML_DEBUG
                        to: QmlEvents.QML_FATAL
                        live: false
                        snapMode: Slider.SnapAlways
                    }
                    onValueChanged: AppSettings.logLevel = value
                    aliasedString: [ "DEBUG", "INFO", "WARN", "FATAL" ]
                    Component.onCompleted: slider.value = AppSettings.logLevel
                }
                Comm.ALG_CheckBox {
                    width: appSettingsLayout.width
                    box.checkState: AppSettings.showOutput ? Qt.Checked : Qt.Unchecked
                    box.onClicked: AppSettings.showOutput = !AppSettings.showOutput
                    label.text: "Auto open output directory."
                }
            }
        }
    }

    Comp.ALG_Frame {
        id: extensionsArea
        anchors {
            bottom: parent.bottom
            bottomMargin: GUI.globalMargins
            horizontalCenter: parent.horizontalCenter
        }

        width: root.width - 2 * GUI.globalMargins
        height: (root.height - 3 * GUI.globalMargins) / 2
        frameText.text: "Deploy"
    }
}
