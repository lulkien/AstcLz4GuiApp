import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import Qt.labs.platform 1.0

import "./Components"
Item {
    id: root
    property bool isSelectFiles

    TextField {
        id: path
        width: root.width/2 + 50
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -60
        placeholderText: qsTr("<i>Select folder images...<i>")
//        text: "" + fileDialog.folder
//        text: "" + folderDialog.folder
        text: isSelectFiles ? fileDialog.files.toString() : folderDialog.folder.toString()
        enabled: false//path.text !== ""
        selectByMouse: true
        selectionColor: "#64B7F7"
        selectedTextColor: "#FFFFFF"
        clip: true
    }

    QButton {
        id: btnBrowse
        height: path.height
        radius: 10
        txtBtn: qsTr("Browse")
        anchors {
            left: path.right
            leftMargin: 20
        }
        onClickedBtn: {
            menu.open()
        }
    }

    Menu {
        id:menu
        MenuItem {text: qsTr("Choose files...") ; onTriggered: { isSelectFiles = true   ; fileDialog.open()   }}
        MenuItem {text: qsTr("Choose folder..."); onTriggered: { isSelectFiles = false  ; folderDialog.open() }}
    }

    FileDialog {
        id: fileDialog
        title: qsTr("Choose images files")
        nameFilters: [ "Image files (*.png)", "All File (*)" ]
    }

    FolderDialog {
        id: folderDialog
        title: qsTr("Choose images folder")
    }
}
