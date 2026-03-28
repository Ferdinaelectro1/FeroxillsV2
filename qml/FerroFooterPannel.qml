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
            anchors.margins: 10
            spacing: 35
            FerroScaleCanva {
                color : "#F7D40A"
                height: parent.height
                width: 150
                decalage : 10
                firstText : "1"
                secondText : " = "+Settings.ch1VoltDiv.toFixed(2)+"V"
                onClickedMore : {
                    Settings.incrementCh1VoltDiv();
                }
                onClickedLess : {
                    Settings.decrementCh1VoltDiv();
                }
            }

            FerroScaleCanva {
                color: "#0000FF"
                height: parent.height
                width: 150
                decalage : 10
                firstText : "2"
                secondText : "= 1V"
            }

            FerroScaleCanva {
                color: "#FF0000"
                height: parent.height
                width: 170
                decalage : 10
                firstText : "3"
                secondText: {
                    let _timeDiv = Settings.timeDiv;
                    if (_timeDiv === undefined || _timeDiv === null) return "--";

                    if (_timeDiv < 0.000001) {
                        if((_timeDiv * 1000000000.0).toFixed(1) >= 1000.0)
                            return "  = " + (_timeDiv * 1000000.0).toFixed(1) + " us";
                        return "  = " + (_timeDiv * 1000000000.0).toFixed(1) + " ns";
                    } else if (_timeDiv < 0.001) {
                        return "  = " + (_timeDiv * 1000000.0).toFixed(1) + " us";
                    } else if (_timeDiv < 1.0) {
                        return "  = " + (_timeDiv * 1000.0).toFixed(1) + " ms";
                    } else {
                        return "  = " + (_timeDiv).toFixed(1) + " s";
                    }
                }
                onClickedMore : {
                    Settings.incrementTimeDiv();
                }
                onClickedLess : {
                    Settings.decrementTimeDiv();
                }
            }
        }
    }
}
