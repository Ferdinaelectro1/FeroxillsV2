import QtQuick

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

            Text {
                text: "TimeBase: 500us/div"
                color: "#D0D0D0"
                font.pixelSize: 16
            }

            Text {
                text: "Trigger: Edge | Source: CH1 | Level: 1.25V"
                color: "#949494"
                font.pixelSize: 16
            }
        }
    }
}
