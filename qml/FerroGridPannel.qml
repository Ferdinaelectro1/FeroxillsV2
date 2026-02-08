import QtQuick
import QtQuick.Shapes 6.5

Item {
    implicitHeight: 300
    implicitWidth: 300
    // --- WAVEFORM AREA ---
    Rectangle {
        id: waveformArea
        color: "#0F0F13"
        border.color: "#2D2D33"
        anchors.fill: parent

        // grille style oscilloscope
        Canvas {
            anchors.fill: parent
            onPaint: {
                var ctx = getContext("2d");
                ctx.fillStyle = "#0F0F13";
                ctx.fillRect(0,0,width,height);

                ctx.strokeStyle = "#222228";
                ctx.lineWidth = 1;
                var step = 40;

                // lignes verticales
                for (var x=0; x<width; x+=step) {
                    ctx.beginPath();
                    ctx.moveTo(x,0);
                    ctx.lineTo(x,height);
                    ctx.stroke();
                }

                // lignes horizontales
                for (var y=0; y<height; y+=step) {
                    ctx.beginPath();
                    ctx.moveTo(0,y);
                    ctx.lineTo(width,y);
                    ctx.stroke();
                }
            }
        }

        // exemple vague CH1
        Shape {
            id: ch1Path
            anchors.fill: parent
            ShapePath {
                strokeWidth: 3
                strokeColor: "#F7D40A" // jaune Rigol CH1
                fillColor: "transparent"
                PathPolyline {
                    id: ch1Polyline
                    path: []  // vide par default
                }
            }
            ShapePath {
                strokeWidth : 2
                strokeColor : "#0FF000"
                fillColor : "transparent"
                PathPolyline {
                    property real y : 0
                    id : horizontalCursor
                    path: [Qt.point(0,y),Qt.point(ch1Path.width,y)]
                }
            }

            MouseArea {
                property bool isClic  : false
                anchors.fill: parent
                hoverEnabled: true
                onPositionChanged : {
                    if(isClic) {
                        console.log("x:", mouse.x, "y:", mouse.y)
                        horizontalCursor.y = mouse.y
                    }
                }
                onClicked : {
                    console.log("CLické ########",isClic)
                    isClic = !isClic
                }
            }
            // Quand les données C++ changent → QML met à jour
            Connections {
                target: backend

                /*function onRefreshQml() {
                    console.log("passé qml");
                    ch1Path.update()
                }*/

                function onSamplesChanged() {
                    var pts = []

                    let w = ch1Path.width
                    let h = ch1Path.height

                    for (var i = 0; i < backend.samples.length; i++) {

                        // Normalisation : transforme 0–5V en -1 → +1
                        let valNorm = (backend.samples[i] - 2.5) / 2.5

                        let x = (i / (backend.samples.length - 1)) * w
                        let y = h/2 - valNorm * (h/2)

                        pts.push(Qt.point(x, y))
                    }

                    ch1Polyline.path = pts
                }

            }
        }
    }
}
