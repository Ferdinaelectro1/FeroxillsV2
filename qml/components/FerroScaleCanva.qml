import QtQuick
import QtQuick.Controls
import QtQuick.Shapes 6.5

Item {
    id: root
    property real decalage : 0
    property color color : "#00FF00"
    property string firstText  : ""
    property string secondText : ""
    property real shapeHeight: 60

    Rectangle {
        anchors.fill : parent
        border.width : 1
        border.color : root.color
        radius: 5
        color : "transparent"
        Item {
            anchors.fill : parent
            anchors.margins : 8
            Row {
                id: main_row
                anchors.fill: parent
                spacing: 5
                Rectangle {
                    id: less_button
                    color: "transparent"
                    height: parent.height
                    width: 20
                    Button {
                        anchors.fill: parent
                        contentItem: Text {
                            anchors.fill: parent
                            text: "-"
                            color: "white"
                            font.pixelSize: 16
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            anchors.fill: parent
                            color: parent.hovered ? "#3c3f41" : "#2b2d30"
                            radius: 3
                            border.color: "#454749"
                            border.width: 1
                        }
                    }
                }

                Rectangle {
                    id: rect_root
                    width: parent.width - (20 + 20 + 5 * 2) //les widths des bouttons plus les 2 spacing
                    height: parent.height
                    color: "transparent"
                    Shape {
                        anchors.fill: parent
                        ShapePath {
                            strokeWidth: 2
                            strokeColor: root.color
                            fillColor: "transparent"
                            PathPolyline {
                                path: [Qt.point(0, rect_root.height), Qt.point(root.decalage, 0), Qt.point(rect_root.width, 0), Qt.point(rect_root.width, rect_root.height), Qt.point(0, rect_root.height)]
                            }
                        }

                        ShapePath {
                            strokeWidth: 2
                            strokeColor: root.color
                            fillColor: root.color
                            PathPolyline {
                                path: [Qt.point(0, rect_root.height), Qt.point(0, 0), Qt.point(root.decalage + rect_root.width * 0.2, 0), Qt.point(rect_root.width * 0.2, rect_root.height), Qt.point(0, rect_root.height)]
                            }
                        }
                    }
                    FAutoSizeText {
                        width: rect_root.width * 0.2
                        height: parent.height
                        text: root.firstText
                    }
                    FAutoSizeText {
                        width: rect_root.width
                        height: parent.height
                        text: root.secondText
                    }
                }

                Rectangle {
                    id: more_button
                    color: "transparent"
                    height: parent.height
                    width: 20
                    Button {
                        anchors.fill: parent
                        contentItem: Text {
                            anchors.fill: parent
                            text: "+"
                            color: "white"
                            font.pixelSize: 16
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            anchors.fill: parent
                            color: parent.hovered ? "#3c3f41" : "#2b2d30"
                            radius: 3
                            border.color: "#454749"
                            border.width: 1
                        }
                    }
                }
            }
        }
    }
}