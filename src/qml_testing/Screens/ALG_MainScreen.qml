import QtQuick 2.14
import QmlCustomType 1.0
import "../Components"

Item {
    id: root

    QtObject {
        id: privateProperties
        readonly property int globalMargins: 20
        readonly property int globalButtonSize: 100
        readonly property int globalBarHeight: 35

        readonly property string inputPath: textInput.text.trim()

        readonly property bool allowProcess: !(AppModel.isScanning || AppModel.isProcessing)    // not in scanning and processing (GUI block)
                                             && AppModel.totalFound !== 0                       // found at least 1 images

        readonly property bool allowScan: !(AppModel.isScanning || AppModel.isProcessing)       // not in scanning and processing (GUI block)
                                          && privateProperties.inputPath.length !== 0           // input not empty
                                          && AppModel.sourcePathFound                           // input a valid path
    }

    Rectangle {
        id: textBox
        anchors {
            top: parent.top
            topMargin: privateProperties.globalMargins
            left: parent.left
            leftMargin: privateProperties.globalMargins
        }

        width: parent.width - 2 * privateProperties.globalMargins
        height: privateProperties.globalBarHeight
        border.color: "black"
        color: "#DADADA"

        TextInput {
            id: textInput
            enabled: !(AppModel.isScanning || AppModel.isProcessing)
            width: textBox.width - 20
            height: textBox.height
            anchors.centerIn: textBox
            verticalAlignment: TextInput.AlignVCenter
            color: AppModel.sourcePathFound ? "#000000" : "#FF4141"
            onTextChanged: {
                textInputUpdateTimer.restart()
//                AppModel.sourcePath = privateProperties.inputPath
            }
        }

        Text {
            id: guideText
            visible: textInput.text.length === 0
            anchors {
                verticalCenter: textBox.verticalCenter
                left: textInput.left
            }
            text: "Paste a valid path here or click Open button..."
            color: "#a9a9a9"
        }

        Timer {
            id: textInputUpdateTimer
            interval: 250
            onTriggered: {
                AppModel.sourcePath = privateProperties.inputPath
            }
        }
    }

    Item {
        id: buttons_area
        anchors {
            left: parent.left
            leftMargin: privateProperties.globalMargins
            top: textBox.bottom
            topMargin: privateProperties.globalMargins
        }
        width: 2 * privateProperties.globalButtonSize + privateProperties.globalMargins
        height: privateProperties.globalButtonSize

        ALG_Button {
            id: loadButton
            width: privateProperties.globalButtonSize
            height: privateProperties.globalButtonSize
            isEnabled: privateProperties.allowScan
            label: privateProperties.inputPath.length === 0 ? "Open" : "Load"
            onClicked: {
                EventHandler.qmlSendEvent(QmlEvents.REQ_LOAD_IMAGES)
            }
        }

        ALG_Button {
            id: startButton
            anchors {
                left: loadButton.right
                leftMargin: privateProperties.globalMargins
            }
            width: privateProperties.globalButtonSize
            height: privateProperties.globalButtonSize
            isEnabled: privateProperties.allowProcess
            label: "Run"
            onClicked: {
                EventHandler.qmlSendEvent(QmlEvents.REQ_GEN_IMAGE)
            }
        }
    }

    Item {
        id: statistics_area
        width: textBox.width - (privateProperties.globalMargins + privateProperties.globalButtonSize) * 2
        height: privateProperties.globalButtonSize
        anchors {
            left: buttons_area.right
            leftMargin: privateProperties.globalMargins
            top: buttons_area.top
        }

        ALG_ProgressBar {
            id: progressBar
            width: parent.width
            height: privateProperties.globalBarHeight
            totalCount: AppModel.totalFound
            successCount: AppModel.successCount
            failureCount: AppModel.failureCount
        }

        Text {
            id: statistic_text
            anchors.top: progressBar.bottom
            width: parent.width
            height: statistics_area.height - progressBar.height
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 14
            text: "Total found images: " + AppModel.totalFound + "\n"
                  + "Success count: " + AppModel.successCount + " (" + progressBar.successRate + "%)\n"
                  + "Failure count: " + AppModel.failureCount + " (" + progressBar.failureRate + "%)"
        }

        ALG_Loading {
            id: loading
            visible: (AppModel.isScanning || AppModel.isProcessing)
            icoSize: 40
            anchors {
                verticalCenter: statistic_text.verticalCenter
                right: statistics_area.right
                rightMargin: 50
            }
        }
    }

    Rectangle {
        id: log_area
        anchors {
            top: buttons_area.bottom
            topMargin: privateProperties.globalMargins
            left: parent.left
            leftMargin: privateProperties.globalMargins
        }

        width: parent.width - 2 * privateProperties.globalMargins
        height: parent.height - textBox.height - buttons_area.height - 4 * privateProperties.globalMargins
        border.color: "black"
    }

    Component.onCompleted: {
        textInput.text = "/home/ark/Pictures/png"
    }
}
