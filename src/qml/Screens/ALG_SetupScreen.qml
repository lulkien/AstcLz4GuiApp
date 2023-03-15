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
        height: (root.height - 3 * GUI.globalMargins) * 2 / 3
        frameText.text: "Compression settings"

        Item {
            id: compressSettingLayout
            anchors.margins: GUI.globalMargins / 2
            anchors.fill: parent
            Column {
                spacing: 5
                Comm.ALG_CheckBox {
                    width: compressSettingLayout.width
                    checkState: AppSettings.noFlip ? Qt.Checked : Qt.Unchecked
                    text: "No flip image."
                    onClicked: {
                        AppSettings.noFlip = !AppSettings.noFlip
                    }
                }
                Comm.ALG_CheckBox {
                    width: compressSettingLayout.width
                    checkState: AppSettings.noPremult ? Qt.Checked : Qt.Unchecked
                    text: "No premultiply image."
                    onClicked: {
                        AppSettings.noPremult = !AppSettings.noPremult
                    }
                }
                Comm.ALG_CheckBox {
                    width: compressSettingLayout.width
                    checkState: AppSettings.useBackup ? Qt.Checked : Qt.Unchecked
                    text: "Use backup/pregenerated ASTC."
                    onClicked: {
                        AppSettings.useBackup = !AppSettings.useBackup
                    }
                }
                Comm.ALG_CheckBox {
                    width: compressSettingLayout.width
                    checkState: AppSettings.veryfast ? Qt.Checked : Qt.Unchecked
                    text: "Use veryfast compression."
                    onClicked: {
                        AppSettings.veryfast = !AppSettings.veryfast
                    }
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
                        value: AppSettings.logLevel
                        live: false
                        snapMode: Slider.SnapAlways
                    }
                    onValueChanged: AppSettings.logLevel = value
                    aliasedString: [ "DEBUG", "INFO", "WARN", "FATAL" ]
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
        height: (root.height - 3 * GUI.globalMargins) / 3
        frameText.text: "About"
    }
}
