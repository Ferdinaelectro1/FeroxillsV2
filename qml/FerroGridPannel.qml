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
                    path: []  // vide par défaut
                }
            }
            // Quand les données C++ changent → QML met à jour
            Connections {
                target: oscilloData

                function onRequestQmlRefresh() {
                    ch1Path.update()
                }

                function onReadEchChanged() {
                    var pts = []

                    let w = ch1Path.width
                    let h = ch1Path.height

                    for (var i = 0; i < oscilloData.readEch.length; i++) {

                        // Normalisation : transforme 0–5V en -1 → +1
                        let valNorm = (oscilloData.readEch[i] - 2.5) / 2.5

                        let x = (i / (oscilloData.readEch.length - 1)) * w
                        let y = h/2 - valNorm * (h/2)

                        pts.push(Qt.point(x, y))
                    }

                    ch1Polyline.path = pts
                }

            }
        }
    }
}
