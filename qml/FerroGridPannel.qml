import QtQuick
import QtQuick.Shapes 6.5
import Feroxills.Settings 1.0

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
        property real vPerDiv : Settings.ch1VoltDiv;
        property real msPerDiv : Settings.timeDiv;
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

                // lignes verticales
                for (let x=0; x<width; x+=horizontalStep) {
                    ctx.beginPath();
                    ctx.moveTo(x,0);
                    ctx.lineTo(x,height);
                    ctx.stroke();
                    let horizontalSubDivStep = horizontalStep / 5;
                    ctx.strokeStyle = "#FFFFFF";
                    ctx.lineWidth = 2;
                    for(let xSubDiv = x; xSubDiv < horizontalStep + x; xSubDiv += horizontalSubDivStep) {
                        ctx.beginPath();
                        ctx.moveTo(xSubDiv,(height/2 - 3));
                        ctx.lineTo(xSubDiv,(height/2 + 3));
                        ctx.stroke();
                    }
                    ctx.lineWidth = 1;
                    ctx.strokeStyle = "#222228";
                }

                // lignes horizontales
                for (let y=0; y<height; y+=verticalStep) {
                    ctx.beginPath();
                    ctx.moveTo(0,y);
                    ctx.lineTo(width,y);
                    ctx.stroke();
                    let verticalSubDivStep = verticalStep / 5;
                    ctx.strokeStyle = "#FFFFFF";
                    ctx.lineWidth = 2;
                    for(let ySubDiv = y; ySubDiv < verticalStep + y; ySubDiv += verticalSubDivStep) {
                        ctx.beginPath();
                        ctx.moveTo((width/2 - 3),ySubDiv);
                        ctx.lineTo((width/2 + 3),ySubDiv);
                        ctx.stroke();
                    }
                    ctx.lineWidth = 1;
                    ctx.strokeStyle = "#222228";
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

            // Quand les du modelView C++ changent → QML met à jour
            Connections {
                target: viewModel
                function onDisplayValuesChanged() {
                    let pts = []
                    let w = ch1Path.width
                    let h = ch1Path.height

                    //Denormalization of normalized values
                    for (let i = 0; i < viewModel.displayValues.length; i++) {
                        let px = viewModel.displayValues[i].x * w
                        let py = h/2 - viewModel.displayValues[i].y * (h/2)
                        pts.push(Qt.point(px, py))
                    }

                    ch1Polyline.path = pts
                }

            }
        }
    }
}
