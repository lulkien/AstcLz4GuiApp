import QtQuick 2.12
import QtQuick.Controls 2.12
import QmlCustomType 1.0
import "../Components"

Item {
    id: root

    QtObject {
        id: privateProperties
        readonly property int globalMargins: 20
        readonly property int globalButtonSize: 100
        readonly property int globalBarHeight: 35
        readonly property int globalLogFontSize: 11

        readonly property string inputPath: textInput.text.trim()

        readonly property bool allowProcess: !(AppModel.isScanning || AppModel.isProcessing)    // not in scanning and processing (GUI block)
                                             && AppModel.totalFound !== 0                       // found at least 1 images
                                             && AppModel.isRunable

        readonly property bool allowScan: !(AppModel.isScanning || AppModel.isProcessing)       // not in scanning and processing (GUI block)
                                          && (AppModel.sourcePathFound || inputPath.length === 0)

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
        clip: true

        ALG_TextInput {
            id: textInput
            enabled: !(AppModel.isScanning || AppModel.isProcessing)
            width: textBox.width - privateProperties.globalMargins
            height: textBox.height
            anchors.centerIn: textBox
            verticalAlignment: TextInput.AlignVCenter
            color: AppModel.sourcePathFound ? "#000000" : "#FF4141"
            onTextChanged: {
                textInputUpdateTimer.restart()
//                EventHandler.qmlSendEvent(QmlEvents.REQ_VERIFY_SOURCE, text.trim())
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
            interval: 100
            onTriggered: {
                EventHandler.qmlSendEvent(QmlEvents.REQ_VERIFY_SOURCE, privateProperties.inputPath)
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
                if (AppModel.sourcePathFound)
                    EventHandler.qmlSendEvent(QmlEvents.REQ_LOAD_IMAGES)
                else
                    menu.open()
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

        Menu {
            id: menu
            MenuItem {
                text: qsTr("Open file...")
                onTriggered: {
                    AppModel.isDirectory = false
//                    fileDialog.open()
                }
            }
            MenuSeparator {}
            MenuItem {
                text: qsTr("Open folder...")
                onTriggered: {
                    AppModel.isDirectory = true
//                    folderDialog.open()
                }
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

        ALG_Text {
            id: statistic_text
            anchors.top: progressBar.bottom
            width: parent.width
            height: statistics_area.height - progressBar.height
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 13
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
        clip: true

        ListModel {
            id: log_model
        }

        ListView {
            id: log_view
            anchors.fill: parent
            model: log_model
            boundsBehavior: Flickable.StopAtBounds
            delegate: log_item
            ScrollBar.vertical: ScrollBar {}
        }

        Component {
            id: log_item
            Item {
                width: log_view.width
                height: textArea.height

                Text {
                    id: textArea
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width - privateProperties.globalMargins / 2
                    wrapMode: Text.Wrap
                    textFormat: TextEdit.RichText
                    padding: 4
                    font.pixelSize: privateProperties.globalLogFontSize
                    font.family: "Hack"
                    text: _data
                }
            }
        }

        Connections {
            target: AppModel
            onReqPrintQmlLog: {
                if (log_model.count >= 100)
                    log_model.remove(0)
                log_model.append({ _data: logData })
                log_view.positionViewAtEnd()
            }

            onReqClearQmlLog: {
                log_model.clear()
                log_view.positionViewAtBeginning()
            }
        }
    }

    Component.onCompleted: {
        textInput.text = "/home/ark/Pictures/lv0"
    }
}
