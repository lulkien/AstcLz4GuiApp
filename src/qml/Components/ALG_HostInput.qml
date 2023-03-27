import QtQuick 2.12
import "Common" as Comm

Item {
    id: root
    property int userW: 75
    readonly property var ipRegexPattern: /^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$/
    readonly property var userRegexPattern: /^[a-zA-Z0-9]([._-](?![._-])|[a-zA-Z0-9])*[a-zA-Z0-9]$/
    readonly property string username: userInputBox.text
    readonly property string hostname: hostInputBox.text
    readonly property bool ipValidated: hostInputBox.text.match(ipRegexPattern) && username.length !== 0
    height: GUI.globalBarHeight
    Comm.ALG_TextField {
        id: userInputBox
        height: parent.height
        width: root.userW
        placeholderText: qsTr("User")
        placeholderTextColor: "#7F8C8D"
        selectByMouse: true
        text: "root"
        validator: RegExpValidator { regExp: root.userRegexPattern }
    }

    Comm.ALG_Text {
        id: textAt
        text: " @ "
        anchors {
            left: userInputBox.right
            verticalCenter: parent.verticalCenter
        }
    }
    Comm.ALG_TextField {
        id: hostInputBox
        height: parent.height
        width: parent.width - userInputBox.width - textAt.width
        anchors.left: textAt.right
        placeholderText: qsTr("Host IP")
        placeholderTextColor: "#7F8C8D"
        selectByMouse: true
        validator: RegExpValidator { regExp: root.ipRegexPattern }
        color: ipValidated ? GUI.black : GUI.tomato
    }
}
