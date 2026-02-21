import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import Feroxills.DisplayMode 1.0
import "components/"

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
                    property bool isRun : backend.run
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
                            backend.run = parent.isRun
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
                id : repeater
                // on récupère depuis c++ le mode de display initiale et on initial les états de boutton de qml avec ça.
                model : [
                    { name : "Roll",mode : FDisplayMode.CONTINU} ,
                    { name : "Trigger",mode : FDisplayMode.TRIGGER } ,
                    { name : "Auto",mode : FDisplayMode.AUTO }
                ]
                Rectangle {
                    property bool btnEnabled: !(backend.display_context.getCurrentMode() === modelData.mode)
                    width: parent.width
                    height: 45
                    border.color: "#3A3A42"
                    radius: 6
                    color: btnEnabled ? "#0000FF" : "#9a9af1"
                    Text {
                        anchors.centerIn: parent
                        text: modelData.name
                        color: "#eae5e5"
                        font.pixelSize: 30
                        font.bold: true
                    }
                    MouseArea {
                        anchors.fill : parent
                        enabled : btnEnabled
                        onClicked : {
                            console.log("Bouton pressé  : ",modelData.name)
                            console.log("Mode  : ",modelData.mode)
                            backend.display_context.setNewMode(modelData.mode)
                            //on active tout les bouttons d'abord
                            for (let i = 0; i < repeater.count; i++) {
                                if(index === i) continue;
                                let this_index = repeater.itemAt(i);
                                this_index.btnEnabled = true;
                            }
                            //puis on desactive le bouton actuel cliqué
                            parent.btnEnabled = false;
                        }
                    }
                }
            }

            GroupBox {
                title: "Paramètres du signal"
                width: parent.width
                label: Label {
                    text: parent.title
                    color: "#00b4d8"           // couleur du titre
                    font.bold: true
                    font.pixelSize: 13
                    font.letterSpacing: 1.5   // espacement des lettres (effet stylé)
                    leftPadding: 10
                }
                background: Rectangle {
                    color: "transparent"       // fond transparent
                    radius: 8                  // coins arrondis
                    border.color: "#00b4d8"    // couleur de la bordure
                    border.width: 1.5
                    opacity: 0.8
                }
                ColumnLayout {
                    anchors.fill : parent
                    spacing : 10
                    Slider {
                        from : 0.0
                        to : 1.0
                        value : backend.dutyCycle
                        stepSize : 0.01
                        width : parent.width
                        onValueChanged : {
                            backend.dutyCycle = value
                        }
                    }
                    Slider {
                        from : 0.0
                        to : 5.0
                        value : backend.voltage
                        stepSize : 0.01
                        width : parent.width
                        onValueChanged : {
                            backend.voltage = value
                        }
                    }
                    Slider {
                        from : 0.0
                        to : 1000.0
                        value : backend.frequency
                        stepSize : 10.0
                        width : parent.width
                        onValueChanged : {
                            backend.frequency = value
                        }
                    }
                }
            }
            FerroSlider {
                width : parent.width
                label: "Fréquence"
                unit: "Hz"
                from: 0
                to: 1000
                value: 500
                accentColor: "#00b4d8"
            }
        }//combo box
    }
}
