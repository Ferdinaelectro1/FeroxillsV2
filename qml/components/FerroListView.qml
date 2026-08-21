import QtQuick
import QtQuick.Controls

Item {
    id : root
    required property var elements
    property var currentElement : null
    property int currentIndex : -1
    property bool isOpen : false
    property color baseColor : "#2b2b2b"
    property color backgroundColor : Qt.lighter(baseColor, 1.05)
    property color textColor : "#e0e0e0"
    property color hoverColor : "#3574F0"
    property color borderColor : Qt.lighter(baseColor,2)
    property int fontSize: 14
    z: isOpen ? 100 : 0
    height: 40
    Rectangle {
        id: closedBox
        anchors.fill : parent
        border.color : borderColor
        radius : 10
        border.width : 1
        color : backgroundColor
        Text {
            clip : true
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 8
            width: parent.width - 45
            color : textColor
            font.pixelSize: fontSize
            font.bold : true
            text : currentElement ? currentElement : qsTr("Select")
        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 8
            color : textColor
            font.pixelSize : fontSize + 10
            font.bold : true
            text : qsTr("⌄")
        }
        MouseArea {
            anchors.fill : parent
            onClicked : {
                isOpen = !isOpen;
            }
        }
    }
    Rectangle {
        visible : isOpen
        anchors.top  : closedBox.bottom
        anchors.left : closedBox.left
        width: closedBox.width
        height: 60
        color : backgroundColor
        border.color : borderColor
        radius : 10
        ListView {
            clip: true
            anchors.fill : parent
            model : elements
            delegate : Rectangle {
                width : ListView.view.width
                height : 30
                color : mouseArea.containsMouse ? hoverColor : backgroundColor
                border.color : borderColor
                radius: 5
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    color : textColor
                    width: parent.width - 16
                    font.pixelSize : fontSize
                    text: modelData
                    clip : true
                }
                MouseArea {
                    id : mouseArea
                    anchors.fill : parent
                    hoverEnabled : true
                    onClicked : {
                        root.currentElement = modelData
                        root.currentIndex  = index
                        root.isOpen = false
                    }
                }
            }
            ScrollBar.vertical : ScrollBar {
                policy: ScrollBar.AlwaysOn
            }
        }
    }
}