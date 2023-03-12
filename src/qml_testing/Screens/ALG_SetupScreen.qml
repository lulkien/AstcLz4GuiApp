import QtQuick 2.12
import QtQuick.Controls 2.12
import QmlCustomType 1.0
import "../Components"
import "../Components/Common"

Item {
    id: root

    QtObject {
        id: privateProperties
        readonly property int globalMargins: 20
        readonly property int drawWidth: root.width - 2 * globalMargins
        readonly property int drawHeight: root.height - 2 * globalMargins
    }

    ALG_Frame {
        id: astcSettingsArea
        anchors {
            top: parent.top
            topMargin: privateProperties.globalMargins
            left: parent.left
            leftMargin: privateProperties.globalMargins
        }

        width: (privateProperties.drawWidth - privateProperties.globalMargins) / 2
        height: (privateProperties.drawHeight - privateProperties.globalMargins) * 2 / 3
        frameText.text: "Compression settings"

        Item {
            id: astcSettingLayout
            anchors.margins: privateProperties.globalMargins / 2
            anchors.fill: parent
            Column {
                spacing: 5
                ALG_CheckBox {
                    width: astcSettingLayout.width
                    checkState: AppSettings.noFlip ? Qt.Checked : Qt.Unchecked
                    text: "No flip image."
                    onClicked: {
                        AppSettings.noFlip = !AppSettings.noFlip
                    }
                }

                ALG_CheckBox {
                    width: astcSettingLayout.width
                    checkState: AppSettings.noPremult ? Qt.Checked : Qt.Unchecked
                    text: "No premultiply image."
                    onClicked: {
                        AppSettings.noPremult = !AppSettings.noPremult
                    }
                }

                ALG_CheckBox {
                    width: astcSettingLayout.width
                    checkState: AppSettings.useBackup ? Qt.Checked : Qt.Unchecked
                    text: "Use backup/pregenerated ASTC."
                    onClicked: {
                        AppSettings.useBackup = !AppSettings.useBackup
                    }
                }

                ALG_CheckBox {
                    width: astcSettingLayout.width
                    checkState: AppSettings.veryfast ? Qt.Checked : Qt.Unchecked
                    text: "Use veryfast compression."
                    onClicked: {
                        AppSettings.veryfast = !AppSettings.veryfast
                    }
                }
            }
        }
    }

    ALG_Frame {
        id: appSettingsArea
        anchors {
            top: parent.top
            topMargin: privateProperties.globalMargins
            right: parent.right
            rightMargin: privateProperties.globalMargins
        }

        width: (privateProperties.drawWidth - privateProperties.globalMargins) / 2
        height: (privateProperties.drawHeight - privateProperties.globalMargins) * 2 / 3
        frameText.text: "Application settings"

        Item {
            id: appSettingsColumn
            anchors.margins: privateProperties.globalMargins
            anchors.fill: parent
            Column {
                spacing: 5
                ALG_Slider {
                    id: resoSlider
                    width: appSettingsColumn.width
                    label {
                        text: "Resolution: <b>" + getAliasedString(AppSettings.resolutionID) + "<\b>"
                        textFormat: Text.RichText
                    }
                    slider {
                        from: QmlEvents.RES_640X480
                        to: QmlEvents.RES_1280X960
                        value: AppSettings.resolutionID
                        live: false
                        snapMode: Slider.SnapAlways
                    }
                    aliasedString: [ "640x480", "800x600", "960x720", "1024x768", "1280x960" ]
                }

                ALG_Slider {
                    id: logSlider
                    width: appSettingsColumn.width
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
                    aliasedString: [ "DEBUG", "INFO", "WARN", "FATAL" ]
                }

                ALG_Button {
                    id: save
                    width: 50
                    height: 50
                    label.text: "Save"
                    label.font.pixelSize: 12
                    label.font.bold: false
                    onClicked: {
                        AppSettings.resolutionID = resoSlider.value
                        AppSettings.logLevel = logSlider.value
                    }
                }
            }
        }
    }

    ALG_Frame {
        id: extensionsArea
        anchors {
            bottom: parent.bottom
            bottomMargin: privateProperties.globalMargins
            horizontalCenter: parent.horizontalCenter
        }

        width: privateProperties.drawWidth
        height: (privateProperties.drawHeight - privateProperties.globalMargins) * 1 / 3
        frameText.text: "Extensions"
    }

    onVisibleChanged:  {
        if (visible) {
            console.warn("Reload all settings")
            resoSlider.slider.value = AppSettings.resolutionID
        }
    }
}
