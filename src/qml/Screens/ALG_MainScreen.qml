import QtQuick 2.12
import QtQuick.Controls 2.12
import QmlCustomType 1.0
import Qt.labs.platform 1.1
import "../Components" as Comp
import "../Components/Common" as Comm

Item {
    id: root
    Item {
        id: input_area
        anchors {
            top: parent.top
            left: parent.left
            margins: GUI.globalMargins
        }
        width: parent.width - 2 * GUI.globalMargins
        height: GUI.globalBarHeight

        Comm.ALG_TextField {
            id: textBox
            width: parent.width - GUI.globalBarHeight
            height: parent.height
            enabled: !(AppModel.isScanning || AppModel.isProcessing)
            verticalAlignment: TextInput.AlignVCenter
            color: AppModel.sourcePathFound ? GUI.black : GUI.tomato
            placeholderText: qsTr("<i>Paste a valid path here or click Open button...<i>")
            placeholderTextColor: "#7F8C8D"
            rightInset: -GUI.globalBarHeight
            selectByMouse: true
            onTextChanged: {
                if (text.trim().length === 0)
                    AppModel.currentBtnState = QmlEvents.BROWSE
                else
                    AppModel.currentBtnState = QmlEvents.LOAD
                textInputUpdateTimer.restart()
            }
        }

        Comp.ALG_Button {
            anchors.left: textBox.right
            width: GUI.globalBarHeight
            height: GUI.globalBarHeight
            icoSource: "qrc:/images/resources/png/delele_filled.png"
            tip.text: qsTr("Clear")
            icoRatio: 1
            border.color: GUI.invisible
            enabled: !(AppModel.isProcessing || AppModel.isScanning)
            color: GUI.invisible
            allowOverlay: false
            onClicked: {
                if (textBox.text !== "")
                    textBox.text = ""
            }
        }

        Timer {
            id: textInputUpdateTimer
            interval: 100
            onTriggered: {
                EventHandler.qmlSendEvent(QmlEvents.REQ_VERIFY_SOURCE, textBox.text.trim())
            }
        }
    }

    Item {
        id: buttons_area
        anchors {
            left: parent.left
            leftMargin: GUI.globalMargins
            top: input_area.bottom
            topMargin: GUI.globalMargins
        }
        width: GUI.globalButtonSize
        height: GUI.globalButtonSize
        Comp.ALG_Button {
            id: btn
            width: GUI.globalButtonSize
            height: GUI.globalButtonSize
            isEnabled: getEnableState(AppModel.currentBtnState)
            icoSource: getIcoSource(AppModel.currentBtnState)[0]
            tip.text: getIcoSource(AppModel.currentBtnState)[1]
            tip.topMargin: input_area.height + GUI.globalMargins + GUI.globalButtonSize
            icoRatio: 0.5
            onClicked: {
                var _state = AppModel.currentBtnState
                if (_state === QmlEvents.BROWSE) {
                    menuView.show()
                } else if (_state === QmlEvents.LOAD) {
                    EventHandler.qmlSendEvent(QmlEvents.REQ_LOAD_IMAGES)
                } else if (_state === QmlEvents.EXECUTE) {
                    EventHandler.qmlSendEvent(QmlEvents.REQ_GEN_IMAGE)
                } else {
                    console.warn("Do nothing")
                }
            }

            function getIcoSource(btnState) {
                switch (btnState) {
                case QmlEvents.BROWSE:
                    return ["qrc:/images/resources/png/search.png", qsTr("Open")]
                case QmlEvents.LOAD:
                    return ["qrc:/images/resources/png/reload.png", qsTr("Load")]
                case QmlEvents.EXECUTE:
                    return ["qrc:/images/resources/png/start_filled.png", qsTr("Execute")]
                default:
                    return ["",""]
                }
            }

            function getEnableState(btnState) {
                if (btnState === QmlEvents.LOAD && !AppModel.sourcePathFound)
                    return false
                if (btnState === QmlEvents.EXECUTE) {
                    if (AppModel.totalFound === 0 || AppModel.isProcessing)
                        return false
                }
                return true
            }

            ListModel {
                id: menuModel
                ListElement { _title: "Select file." ; _isDirectory: false }
                ListElement { _title: "Open folder." ; _isDirectory: true  }
            }

            Comp.ALG_SplitButton {
                id: menuView
                anchors.centerIn: btn
                width: GUI.globalButtonSize - 2 // border offset
                height: width
                model: menuModel
                visible: false
                delegate: menuItem
            }

            Component {
                id: menuItem
                Comp.ALG_Button {
                    width: menuView.width
                    height: menuView.height / 2
                    label {
                        text: _title
                        font.pixelSize: 14
                        font.bold: false
                        color: GUI.black
                    }
                    enableTooltip: false
                    border.width: 1
                    onClicked: {
                        AppModel.isDirectory = _isDirectory
                        menuView.hide()
                        if (_isDirectory)
                            folderDialog.open()
                        else
                            fileDialog.open()
                    }
                }
            }

            FileDialog {
                id: fileDialog
                title: qsTr("Select image")
                nameFilters: [ ".PNG File (*.png)", "All File (*)"]
                fileMode: FileDialog.OpenFile | FolderDialog.ReadOnly
                folder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
                options: FolderDialog.ReadOnly
                onAccepted: {
                    textBox.text = file.toString().replace("file://", "")
                }
            }

            FolderDialog {
                id: folderDialog
                title: qsTr("Select folder")
                options: FolderDialog.ReadOnly
                folder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
                onAccepted: {
                    textBox.text = folder.toString().replace("file://", "")
                }
            }
        }
    }

    Item {
        id: statistics_area
        width: parent.width - 3 * GUI.globalMargins - buttons_area.width
        height: GUI.globalButtonSize
        anchors {
            left: buttons_area.right
            leftMargin: GUI.globalMargins
            top: buttons_area.top
        }

        Comp.ALG_ProgressBar {
            id: progressBar
            width: parent.width
            height: GUI.globalBarHeight
            totalCount: AppModel.totalFound
            successCount: AppModel.successCount
            failureCount: AppModel.failureCount
        }

        Comm.ALG_Text {
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

        Comp.ALG_Loading {
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
            left: parent.left
            margins: GUI.globalMargins
        }

        width: parent.width - 2 * GUI.globalMargins
        height: parent.height - textBox.height - buttons_area.height - 4 * GUI.globalMargins
        color: GUI.whitesmoke
        border.color: GUI.black
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

                Comm.ALG_Text {
                    id: textArea
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width - GUI.globalMargins / 2
                    wrapMode: Text.Wrap
                    textFormat: TextEdit.RichText
                    padding: 4
                    font.pixelSize: 11
                    text: _data
                }
            }
        }
    }

    Connections {
        target: AppModel
        function onReqPrintQmlLog(logData) {
//        onReqPrintQmlLog: {
            if (log_model.count >= 100)
                log_model.remove(0)
            log_model.append({ _data: logData })
            log_view.positionViewAtEnd()
        }

        function onReqClearQmlLog() {
//        onReqClearQmlLog: {
            log_model.clear()
            log_view.positionViewAtBeginning()
        }

        function onTotalFoundChanged() {
//        onTotalFoundChanged: {
            if (AppModel.totalFound !== 0) {
                AppModel.currentBtnState = QmlEvents.EXECUTE
            }
        }
    }
}
