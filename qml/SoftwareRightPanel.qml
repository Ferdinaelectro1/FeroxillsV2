import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "components/"

GroupBox {
    title: qsTr("Paramètres du signal")
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
        FerroSlider {
            label: qsTr("Duty")
            unit: ""
            accentColor: "#00b4d8"
            from : 0.0
            to : 1.0
            value : backend.sourceController.currentProviderSettings.duty
            stepSize : 0.01
            width : parent.width
            onCurrentValueChanged : {
                backend.sourceController.currentProviderSettings.duty = currentValue
            }
        }
        FerroSlider {
            label: qsTr("Voltage")
            unit: "V"
            accentColor: "#00b4d8"
            from : 0.0
            to : 5.0
            value : backend.sourceController.currentProviderSettings.voltage
            stepSize : 0.01
            width : parent.width
            onCurrentValueChanged : {
                backend.sourceController.currentProviderSettings.voltage = currentValue
            }
        }
        FerroSlider {
            label: qsTr("Fréquence")
            unit: "Hz"
            accentColor: "#00b4d8"
            from : 0.0
            to : 1000.0
            value : backend.sourceController.currentProviderSettings.frequency
            stepSize : 10.0
            width : parent.width
            onCurrentValueChanged : {
                backend.sourceController.currentProviderSettings.frequency = currentValue
            }
        }
    }
}