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

        readonly property real nombre_total_division_vertical : 8
        readonly property real nombre_total_division_horizontal : 10
        readonly property real echantillonage_period : 0.000022
        property real vPerDiv : 4;
        property real msPerDiv : 0.001
        // grille style oscilloscope
        Canvas {
            anchors.fill: parent
            onPaint: {
                let ctx = getContext("2d");
                ctx.fillStyle = "#0F0F13";
                ctx.fillRect(0,0,width,height);

                ctx.strokeStyle = "#222228";
                ctx.lineWidth = 1;
                let verticalStep = height / waveformArea.nombre_total_division_vertical;
                let horizontalStep = width / waveformArea.nombre_total_division_horizontal;

                // lignes horizontales
                for (let x=0; x<width; x+=horizontalStep) {
                    ctx.beginPath();
                    ctx.moveTo(x,0);
                    ctx.lineTo(x,height);
                    ctx.stroke();
                }

                // lignes verticales
                for (let y=0; y<height; y+=verticalStep) {
                    ctx.beginPath();
                    ctx.moveTo(0,y);
                    ctx.lineTo(width,y);
                    ctx.stroke();
                }
            }
        }

        // exemple vague CH1
        Shape {
            antialiasing: false
            id: ch1Path
            anchors.fill: parent
            //Ligne centrale du zero
            ShapePath {
                strokeWidth : 2
                strokeColor : "#FFFFFF"
                fillColor : "transparent"
                PathPolyline {
                    id : cursorHorizontal
                    path: [Qt.point(0,ch1Path.height / 2),Qt.point(ch1Path.width,ch1Path.height / 2)]
                }
            }

            ShapePath {
                strokeWidth : 2
                strokeColor : "#FFFFFF"
                fillColor : "transparent"
                PathPolyline {
                    id : cursorVertical
                    path: [Qt.point(ch1Path.width / 2,0),Qt.point(ch1Path.width / 2,ch1Path.height)]
                }
            }

            ShapePath {
                strokeWidth: 3
                strokeColor: "#F7D40A" // jaune Rigol CH1
                fillColor: "transparent"
                PathPolyline {
                    id: ch1Polyline
                    path: []  // vide par default
                }
            }

            // Quand les données C++ changent → QML met à jour
            Connections {
                target: backend

                function  onSuggestVoltPerDiv(newValue) {
                    console.log("NewValue = ",newValue);
                    waveformArea.vPerDiv = newValue;
                }
                function onSamplesChanged() {
                    let pts = []
                    let w = ch1Path.width
                    let h = ch1Path.height
                    for (let i = 0; i < backend.samples.length; i++) {

                        //let vPerDiv = 4 // 1V/div
                        let Yrange = waveformArea.vPerDiv * waveformArea.nombre_total_division_vertical / 2;
                        let Xrange = waveformArea.msPerDiv * waveformArea.nombre_total_division_horizontal;
                        // Normalisation : transforme [-5V,5V] en [-1,+1]
                        let valNorm = (backend.samples[i]) / Yrange
                        let x = x = (i * waveformArea.echantillonage_period) * (w / (waveformArea.nombre_total_division_horizontal * waveformArea.msPerDiv));
                        let y = h/2 - valNorm * (h/2)
                        if(x <= w)
                          pts.push(Qt.point(x, y))
                    }
                    ch1Polyline.path = pts
                }

            }
        }
    }
}
