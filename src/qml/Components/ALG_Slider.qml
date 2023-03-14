import QtQuick 2.0
import QtQuick.Controls 2.12
import "Common"

Item {
    id: root
    property alias label: sliderLabel
    property var aliasedString: []
    property alias slider: sliderBar
    property alias value: sliderBar.value

    width: 100
    height: sliderLabel.height + sliderBar.anchors.topMargin + sliderBar.height

    ALG_Text {
        id: sliderLabel
    }

    Slider {
        id: sliderBar
        anchors.top: sliderLabel.bottom
        anchors.topMargin: 5
        width: parent.width
        height: GUI.globalBarHeight
        from: 0 ; to: 1 ; stepSize: 1 ; value: 0
    }

    function getAliasedString(value) {
        return value < root.aliasedString.length ? root.aliasedString[value]
                                                 : "NaN"
    }
}
