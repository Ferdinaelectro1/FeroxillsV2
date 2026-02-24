import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Popup {
    id: root
    property string title : "Settings";
    anchors.centerIn: Overlay.overlay
    modal: true
    focus: true

    implicitWidth: 340
    implicitHeight: 150

    signal validate(double value)
    signal cancel()

    background: Rectangle {
        color: "#2b2d30"
        radius: 8
        border.color: "#454749"
        border.width: 1

        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            horizontalOffset: 0
            verticalOffset: 4
            radius: 16
            samples: 33
            color: "#aa000000"
        }
    }

    Column {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        // Titre
        Text {
            text: root.title
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#dfe1e5"
            font.pixelSize: 16
            font.weight: Font.Medium
        }

        //Input
        TextField {
            id: triggerField
            width: parent.width
            placeholderText: "e.g. 2.5"
            color: "#dfe1e5"
            font.pixelSize: 20
            selectByMouse: true

            background: Rectangle {
                color: "#1e1f22"
                radius: 4
                border.color: triggerField.activeFocus ? "#4d78cc" : "#454749"
                border.width: 1
            }

            leftPadding: 8
            rightPadding: 8
        }

        // Boutons
        Row {
            anchors.right: parent.right
            spacing: 8

            Button {
                text: "Cancel"
                width: 80
                height: 30

                background: Rectangle {
                    color: parent.hovered ? "#3c3f41" : "transparent"
                    radius: 4
                    border.color: "#454749"
                    border.width: 1
                }

                contentItem: Text {
                    text: parent.text
                    color: "#dfe1e5"
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    cancel()
                    root.close()
                }
            }

            Button {
                text: "OK"
                width: 80
                height: 30

                background: Rectangle {
                    color: parent.hovered ? "#5a7fc4" : "#4d78cc"
                    radius: 4
                }

                contentItem: Text {
                    text: parent.text
                    color: "white"
                    font.pixelSize: 12
                    font.weight: Font.Medium
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked : {
                    root.validate(parseFloat(triggerField.text));
                }
            }
        }
    }
}