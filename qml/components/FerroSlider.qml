import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// ------------------------------------------------------
// FerroSlider - custom slider with clean spacing
//
// Exposed properties:
//   label       : string  - parameter name    (example: "Frequency")
//   unit        : string  - displayed unit    (example: "Hz", "V", "deg")
//   from        : real    - minimum value
//   to          : real    - maximum value
//   value       : real    - current value
//   stepSize    : real    - slider step
//   decimals    : int     - number of displayed decimals
//   accentColor : color   - main color
//   showMinMax  : bool    - show min/max below the bar
// ------------------------------------------------------

Item {
    id: root

    // --- Public properties ---
    property string label:       "Parameter"
    property string unit:        ""
    property real   from:        0
    property real   to:          100
    property real   value:       50
    property real   stepSize:    0
    property int    decimals:    1
    property color  accentColor: "#00b4d8"

    readonly property real currentValue: internalSlider.value

    // --- Default size ---
    implicitWidth:  190
    implicitHeight: mainColumn.implicitHeight + 24

    // ------------------------------
    // Styled GroupBox
    // ------------------------------
    GroupBox {
        id: groupBox
        anchors.fill: parent
        /*leftPadding:  12
        rightPadding: 12
        bottomPadding: 12*/

        // --- Custom title ---
        label:
            Text {
                    text:               root.label
                    color:              root.accentColor
                    font.pixelSize:     12
                    font.bold:          true
                    font.letterSpacing: 1.2
        }

        // --- Main content ---
        ColumnLayout {
            id:           mainColumn
            anchors.fill: parent

            // --- Block 2: slider only ---
            Slider {
                id:               internalSlider
                Layout.fillWidth: true
                implicitHeight:   32   // fixed height, clear and uncluttered
                from:             root.from
                to:               root.to
                value:            root.value
                stepSize:         root.stepSize

                // --- Bar ---
                background: Rectangle {
                    x:      internalSlider.leftPadding
                    y:      internalSlider.topPadding +
                        internalSlider.availableHeight / 2 -
                        height / 2
                    width:  internalSlider.availableWidth
                    height: 4
                    radius: 2
                    color:  "#2a2a2a"

                    // Colored progression
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

                // --- Circular handle ---
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

                    // Click halo
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

                    // Main circle
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

                        // Center dot
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

        } // end ColumnLayout
    } // end GroupBox
} // end Item
