import QtQuick
import QtQuick.Controls
import Feroxills.DisplayMode 1.0

Item {
    id: root1
    // --- RIGHT PANEL (buttons) ---
    implicitWidth: 250
    implicitHeight: 700
    Rectangle {
        color: "#1A1B20"
        border.color: "#2E2E34"
        anchors.fill: parent
        Column {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 15

            Rectangle {
                    property bool isRun : false
                    width: parent.width
                    height: 45
                    color: isRun ? "red" : "#37b837"
                    radius: 6
                    border.color: "#3A3A42"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("Click ")
                            parent.isRun  = !parent.isRun
                        }
                    }

                    Text {
                        anchors.centerIn: parent
                        text: parent.isRun ? "Stop" : "Run"
                        color: "#eae5e5"
                        font.pixelSize: 30
                        font.bold: true
                    }
            }
            Repeater {
                model : [
                    { name : "Roll",mode : FDisplayMode.CONTINU , enabled : true } ,
                    { name : "Trigger",mode : FDisplayMode.TRIGGER , enabled : true } ,
                    { name : "Auto",mode : FDisplayMode.AUTO , enabled : true }
                ]
                Rectangle {
                    width: parent.width
                    height: 45
                    border.color: "#3A3A42"
                    radius: 6
                    color: "#0000FF"
                    Text {
                        anchors.centerIn: parent
                        text: modelData.name
                        color: "#eae5e5"
                        font.pixelSize: 30
                        font.bold: true
                    }
                    MouseArea {
                        anchors.fill : parent
                        onClicked : {
                            console.log("Bouton pressé  : ",modelData.name)
                            console.log("Mode  : ",modelData.mode)
                            backend.display_context.setNewMode(modelData.mode)
                        }
                    }
                }
            }

            FerroComboxMenu {
                width: parent.width
            }
            //combo box
        }
    }
}
