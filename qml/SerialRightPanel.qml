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
            function findPortInfo(portName)  {
                for (let i = 0; i < availablePorts.length; i++) {
                    if(availablePorts[i].portName ===  portName) {
                        return availablePorts[i].portName+" - "+availablePorts[i].description
                    }
                }
                return null;
            }
            currentElement : findPortInfo(backend.sourceController.currentProviderSettings.portName)
            onCurrentIndexChanged : {
                if(currentIndex >= 0 && currentIndex < availablePorts.length) {
                    const portSelectedName = availablePorts[currentIndex].systemLocation;
                    console.log("Right selected port :", portSelectedName)
                    backend.sourceController.currentProviderSettings.portName = portSelectedName
                }
            }
            onIsOpenChanged : {
                availablePorts = SerialPortUtils.getAvailablePorts();
                portsDisplayList = availablePorts.map(function(p){
                    return p.portName + " - " + p.description
                })
            }
        }
        FerroListView {
            property var baudRates : [9600,19200,38400,57600,230400,115200]
            Layout.fillWidth: true
            elements : baudRates
            currentElement : backend.sourceController.currentProviderSettings.baudRate
            onCurrentIndexChanged  : {
                backend.sourceController.currentProviderSettings.baudRate = baudRates[currentIndex];
            }
        }
    }
}