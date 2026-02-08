import QtQuick
import QtQuick.Controls

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

            Repeater {
                model: ["CH1", "CH2", "Math", "FFT", "Cursor", "Measure"]
                delegate: Rectangle {
                    width: parent.width
                    height: 45
                    color: "#272830"
                    radius: 6
                    border.color: "#3A3A42"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: console.log("Click " + modelData)
                    }

                    Text {
                        anchors.centerIn: parent
                        text: modelData
                        color: "white"
                        font.pixelSize: 18
                    }
                }
            }

            FerroComboxMenu {
                width: parent.width
            }
            //combo box
            /*GroupBox {
                title: "Paramètres"
                //anchors.centerIn: parent
                width: parent.width
                height: 60

                Column {
                    spacing: 10
                    anchors.fill: parent

                    CheckBox { text: "Option 1" }
                    CheckBox { text: "Option 2" }
                    Slider { width: parent.width }
                    Button { text: "Appliquer" }
                }
            }*/
        }
    }
}
