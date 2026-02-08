import QtQuick 6.5
import QtQuick.Layouts 6.5

Item {
    id: root
    implicitHeight: 60
    implicitWidth: 700
    Rectangle {
        anchors.fill: parent
        color: "#1B1C20"
        border.color: "#3A3A40"

        Row {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Text {
                text: "RIGOL DS1104Z"
                //color: "#EAEAEA"
                color: "yellow"
                font.pixelSize: 40
                font.family: paper_sractch.name
            }

            Rectangle { width: 2; height: 40; color: "#3A3A40" }

            Row {
                spacing: 10
                anchors.margins: 10
                    Text {
                        id: channel1_text
                        text: qsTr("U = 0 V");
                        //color: "#B0B0B0"
                        color: "blue"
                        font.pixelSize: 20
                        font.bold: true
                    }
                }
                Text {
                    text: qsTr("|");
                    color: "#B0B0B0"
                    font.pixelSize: 16
                    font.bold: true
                }
                Text {
                    text: "CH1: 1V/div"
                    color: "red"
                    font.pixelSize: 20
                    font.bold: true
                }
            }
        }

    FontLoader
    {
        id: paper_sractch
        source: "font/Paper_Scratch.otf"
    }
    FontLoader
    {
        id: game_font
        source: "font/SPORT_GAME_PERSONAL_USE_ONLY!.ttf"
    }

    Connections {
        target: backend
        function onMaxVoltageChanged () {
            channel1_text.text = "U = "+Number(backend.maxVoltage.toFixed(2))+" V";
        }
    }
}

