import QtQuick 2.12
import QtQuick.Controls 2.12
import "../Components"

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
            id: astcSettingsColumn
            anchors.margins: privateProperties.globalMargins / 2
            anchors.fill: parent
            Column {
                spacing: 5
                CheckBox {
                    width: astcSettingsColumn.width
                    checkState: AstcSettings.noFlip ? Qt.Checked : Qt.Unchecked
                    text: "No flip image."
                    onClicked: {
                        AstcSettings.noFlip = !AstcSettings.noFlip
                    }
                }

                CheckBox {
                    width: astcSettingsColumn.width
                    checkState: AstcSettings.noPremult ? Qt.Checked : Qt.Unchecked
                    text: "No premultiply image."
                    onClicked: {
                        AstcSettings.noPremult = !AstcSettings.noPremult
                    }
                }

                CheckBox {
                    width: astcSettingsColumn.width
                    checkState: AstcSettings.useBackup ? Qt.Checked : Qt.Unchecked
                    text: "Use backup/pregenerated ASTC."
                    onClicked: {
                        AstcSettings.useBackup = !AstcSettings.useBackup
                    }
                }

                CheckBox {
                    width: astcSettingsColumn.width
                    checkState: AstcSettings.veryfast ? Qt.Checked : Qt.Unchecked
                    text: "Use veryfast compression."
                    onClicked: {
                        AstcSettings.veryfast = !AstcSettings.veryfast
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
}
