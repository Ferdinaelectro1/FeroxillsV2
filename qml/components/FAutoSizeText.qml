import QtQuick

Rectangle {
    id: root
    property string text: ""
    property color textColor: "white"
    property color bgColor: "transparent"

    color: bgColor

    Text {
        anchors.fill: parent
        text: root.text
        color: root.textColor
        fontSizeMode: Text.Fit
        minimumPixelSize: 8
        font.pixelSize: 100
        font.bold : true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}