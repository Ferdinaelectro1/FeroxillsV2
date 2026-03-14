import QtQuick
import "components/"
import Feroxills.Settings 1.0


Item {
    implicitWidth: 700
    implicitHeight: 60
    // --- FOOTER AREA ---
    Rectangle {
        anchors.fill: parent
        color: "#1B1C20"
        border.color: "#3A3A40"

        Row {
            anchors.fill: parent
            anchors.margins: 15
            spacing: 25
            FerroScaleCanva {
                color : "#F7D40A"
                height: parent.height
                width: 100
                decalage : 10
                firstText : "1"
                secondText : "= "+Settings.ch1VoltDiv.toFixed(2)+"V"
            }

            FerroScaleCanva {
                color: "#0000FF"
                height: parent.height
                width: 100
                decalage : 10
                firstText : "2"
                secondText : "= 1V"
            }

            FerroScaleCanva {
                color: "#FF0000"
                height: parent.height
                width: 120
                decalage : 10
                firstText : "3"
                secondText : "= "+(Settings.timeDiv*1000.0).toFixed(2)+" ms"
            }
        }
    }
}
