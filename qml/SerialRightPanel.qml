import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Feroxills.SerialPortUtils 1.0
import "components/"

GroupBox {
    title : qsTr("Serial Pannel")
    label: Label {
        text: parent.title
        color: "#00b4d8"           // couleur du titre
        font.bold: true
        font.pixelSize: 13
        font.letterSpacing: 1.5   // espacement des lettres (effet stylé
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
        spacing : 30
        FerroListView {
            property var portsDisplayList : SerialPortUtils.getAvailablePorts().map(function(p){
                return  p.portName + " - " +p.description
            })
            property var availablePorts : SerialPortUtils.getAvailablePorts()
            Layout.fillWidth: true
            elements : portsDisplayList
            onCurrentIndexChanged : {
                if(currentIndex >= 0 && currentIndex < availablePorts.length) {
                    console.log("Right selected port :", availablePorts[currentIndex].systemLocation)
                }
            }
        }
        Button {
            text: qsTr("Connect Oscillo")
        }
    }
}