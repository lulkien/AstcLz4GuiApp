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
        frameText.text: "ASTC Encoder Settings"
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
        frameText.text: "Application Settings"
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
