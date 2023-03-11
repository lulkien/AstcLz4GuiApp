import QtQuick 2.0

Item {
    id: root
    height: 30
    width: 100
    clip: true

    // statistic
    property int totalCount: 0
    property int successCount: 0
    property int failureCount: 0
    readonly property int successWidth: (width / totalCount) * successCount
    readonly property int failureWidth: (width / totalCount) * failureCount
    readonly property int remainWidth: width - successWidth - failureWidth
    readonly property double successRate: totalCount !== 0 ? (Math.round((successCount / totalCount) * 10000) / 100)
                                                           : 0
    readonly property double failureRate: totalCount !== 0 ? (Math.round((failureCount / totalCount) * 10000) / 100)
                                                           : 0

    // color
    property color successColor: "green"
    property color failureColor: "red"
    property color remainingColor: "lightgray"
    property color invalidColor: "gray"
    property color borderColor: "black"
    readonly property string invisible: "transparent"

    Rectangle {
        id: successBar
        anchors {
            left: root.left
            verticalCenter: root.verticalCenter
        }

        width: root.successWidth
        height: root.height
        color: root.successColor
    }

    Rectangle {
        id: failureBar
        anchors {
            left: successBar.right
            verticalCenter: root.verticalCenter
        }

        width: root.failureWidth
        height: root.height
        color:  root.failureColor
    }

    Rectangle {
        id: remainingBar
        anchors {
            left: failureBar.right
            verticalCenter: root.verticalCenter
        }
        height: root.height
        width: root.remainWidth
        color: root.remainingColor
    }

    Rectangle {
        id: overlay
        anchors.fill: root
        color: root.totalCount === 0 ? root.invalidColor : root.invisible
        border.color: root.borderColor
    }

    // signal handler
    onSuccessCountChanged: {
        if (successCount > totalCount
                || (successCount + failureCount) > totalCount)
            Qt.quit()
    }


    onFailureCountChanged: {
        if (failureCount > totalCount
                || (successCount + failureCount) > totalCount)
            Qt.quit()
    }
}
