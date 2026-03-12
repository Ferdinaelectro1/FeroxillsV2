import QtQuick
import "components/"

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
            }

            FerroScaleCanva {
                color: "#0000FF"
                height: parent.height
                width: 100
                decalage : 10
            }

            FerroScaleCanva {
                color: "#FF0000"
                height: parent.height
                width: 100
                decalage : 10
            }
        }
    }
}
