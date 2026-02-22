import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// ─────────────────────────────────────────────
//  FerroSlider — Slider custom propre et bien espacé
//
//  Propriétés exposées :
//    label       : string  — nom du paramètre  (ex: "Fréquence")
//    unit        : string  — unité affichée    (ex: "Hz", "V", "°")
//    from        : real    — valeur minimale
//    to          : real    — valeur maximale
//    value       : real    — valeur courante
//    stepSize    : real    — pas du slider
//    decimals    : int     — nb de décimales affichées
//    accentColor : color   — couleur principale
//    showMinMax  : bool    — afficher min/max sous la barre
// ─────────────────────────────────────────────

Item {
    id: root

    // ── Propriétés publiques ──────────────────
    property string label:       "Paramètre"
    property string unit:        ""
    property real   from:        0
    property real   to:          100
    property real   value:       50
    property real   stepSize:    0
    property int    decimals:    1
    property color  accentColor: "#00b4d8"

    readonly property real currentValue: internalSlider.value

    // ── Taille par défaut ─────────────────────
    implicitWidth:  190
    implicitHeight: mainColumn.implicitHeight + 24

    // ─────────────────────────────────────────
    //  GroupBox stylisé
    // ─────────────────────────────────────────
    GroupBox {
        id: groupBox
        anchors.fill: parent
        /*leftPadding:  12
        rightPadding: 12
        bottomPadding: 12*/

        // ── Titre custom ──────────────────────
        label:
            Text {
                    text:               root.label
                    color:              root.accentColor
                    font.pixelSize:     12
                    font.bold:          true
                    font.letterSpacing: 1.2
        }

        // ── Contenu principal ─────────────────
        ColumnLayout {
            id:           mainColumn
            anchors.fill: parent

            // ── Bloc 2 : Slider seul ─────────
            Slider {
                id:               internalSlider
                Layout.fillWidth: true
                implicitHeight:   32   // hauteur fixe, claire, sans débordement
                from:             root.from
                to:               root.to
                value:            root.value
                stepSize:         root.stepSize

                // ── Barre ─────────────────────
                background: Rectangle {
                    x:      internalSlider.leftPadding
                    y:      internalSlider.topPadding +
                        internalSlider.availableHeight / 2 -
                        height / 2
                    width:  internalSlider.availableWidth
                    height: 4
                    radius: 2
                    color:  "#2a2a2a"

                    // Progression colorée
                    Rectangle {
                        width:  internalSlider.visualPosition * parent.width
                        height: parent.height
                        radius: 2

                        gradient: Gradient {
                            orientation: Gradient.Horizontal
                            GradientStop {
                                position: 0.0
                                color:    Qt.darker(root.accentColor, 1.2)
                            }
                            GradientStop {
                                position: 1.0
                                color:    root.accentColor
                            }
                        }
                    }
                }

                // ── Handle circulaire ─────────
                handle: Item {
                    x: internalSlider.leftPadding +
                        internalSlider.visualPosition *
                        internalSlider.availableWidth -
                        width / 2
                    y: internalSlider.topPadding +
                        internalSlider.availableHeight / 2 -
                        height / 2
                    width:  26
                    height: 26

                    // Halo au clic
                    Rectangle {
                        anchors.centerIn: parent
                        width:   parent.width + 4
                        height:  parent.width + 4
                        radius:  width / 2
                        color:   Qt.rgba(
                            root.accentColor.r,
                            root.accentColor.g,
                            root.accentColor.b,
                            internalSlider.pressed ? 0.25 : 0.0
                        )
                        Behavior on color {
                            ColorAnimation { duration: 150 }
                        }
                    }

                    // Cercle principal
                    Rectangle {
                        anchors.centerIn: parent
                        width:   parent.width - 2
                        height:  parent.height - 2
                        radius:  width / 2
                        color:   internalSlider.pressed
                            ? Qt.lighter(root.accentColor, 1.2)
                            : root.accentColor
                        border.color: "white"
                        border.width: 2

                        // Point central
                        Rectangle {
                            anchors.centerIn: parent
                            width:   3
                            height:  3
                            radius:  1.5
                            color:   "white"
                            opacity: 0.7
                        }

                        Behavior on color {
                            ColorAnimation { duration: 100 }
                        }
                    }
                }
            }

        } // fin ColumnLayout
    } // fin GroupBox
} // fin Item
