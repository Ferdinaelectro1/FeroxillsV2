import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Popup {
    id: root

    property string title: qsTr("Trigger Mode")
    property int selectedMode: 0  // 0 = Continuous, 1 = Single Shot

    signal validate(int mode) // 0 = Continuous, 1 = single shot
    signal cancel()

    anchors.centerIn: Overlay.overlay
    modal: true
    focus: true

    implicitWidth: 300
    implicitHeight: 180

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
        spacing: 16

        Text {
            text: root.title
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#dfe1e5"
            font.pixelSize: 16
            font.weight: Font.Medium
        }

        Column {
            width: parent.width
            spacing: 10

            Repeater {
                model: [qsTr("Continuous Trigger"), qsTr("Single Shot Trigger")]

                delegate: Item {
                    width: parent.width
                    height: 28

                    Row {
                        spacing: 10
                        anchors.verticalCenter: parent.verticalCenter

                        // Radio button circulaire custom
                        Item {
                            width: 18
                            height: 18
                            anchors.verticalCenter: parent.verticalCenter

                            Rectangle {
                                anchors.fill: parent
                                radius: width / 2
                                color: "transparent"
                                border.color: root.selectedMode === index ? "#4d78cc" : "#454749"
                                border.width: 2

                                Rectangle {
                                    anchors.centerIn: parent
                                    width: 8
                                    height: 8
                                    radius: width / 2
                                    color: "#4d78cc"
                                    visible: root.selectedMode === index
                                }
                            }
                        }

                        Text {
                            text: modelData
                            color: root.selectedMode === index ? "#dfe1e5" : "#8c8f93"
                            font.pixelSize: 13
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: root.selectedMode = index
                        cursorShape: Qt.PointingHandCursor
                    }
                }
            }
        }

        Row {
            anchors.right: parent.right
            spacing: 8

            Button {
                text: qsTr("Cancel")
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

                onClicked: {
                    root.validate(root.selectedMode)
                    root.close()
                }
            }
        }
    }
}
