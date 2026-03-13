import QtQuick
import QtQuick.Shapes 6.5

Item {
    id: root
    property real decalage : 0
    property color color : "#00FF00"
    property string firstText  : ""
    property string secondText : ""
    Shape {
        anchors.fill : parent
        ShapePath {
            strokeWidth : 2
            strokeColor : root.color
            fillColor : "transparent"
            PathPolyline {
                path: [Qt.point(0,root.height),Qt.point(root.decalage,0),Qt.point(root.decalage+root.width,0),Qt.point(root.width, root.height),Qt.point(0, root.height )]
            }
        }

        ShapePath {
            strokeWidth : 2
            strokeColor : root.color
            fillColor : root.color
            PathPolyline {
                path: [Qt.point(0,root.height),Qt.point(root.decalage,0),Qt.point(root.decalage+ root.width*0.2,0),Qt.point(root.width*0.2, root.height),Qt.point(0, root.height )]
            }
        }
    }
    Text {
        x : (((root.width*0.2) / 2) + root.decalage) - implicitWidth
        y : root.height / 2 - implicitHeight / 2
        color: "#FFFFFF"
        font.bold : true
        font.pixelSize : 25
        text: root.firstText
    }
    Text {
        x : ((root.width*0.2 + root.width + root.decalage) / 2) - (implicitWidth/2)
        y : root.height / 2 - implicitHeight / 2
        color: "#FFFFFF"
        font.bold : true
        font.pixelSize : 20
        text: root.secondText
    }
}