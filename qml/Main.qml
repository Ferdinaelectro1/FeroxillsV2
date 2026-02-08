import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts 6.5
import QtQuick.Shapes 6.5

Window {
    visible: true
    width: 1280
    height: 720
    minimumWidth: 1280
    maximumWidth: 1280
    minimumHeight: 720
    maximumHeight: 720
    title: "Feroxills 0.0.1"

    Rectangle {
        id: root
        width: 1280
        height: 720
        color: "#111216"    // Fond Rigol sombre

        FerroHeaderPannel {
            id: header
            anchors.top: parent.top
            height: 60
            width: parent.width
        }

        FerroGridPannel
        {
            anchors.top: header.bottom
            anchors.left: parent.left
            anchors.right: rightPanel.left
            anchors.bottom: footer.top
        }

        FerroRightPannel
        {
            id: rightPanel
            width: 250
            anchors.top: header.bottom
            anchors.bottom: footer.top
            anchors.right: parent.right
        }

        FerroFooterPannel
        {
            id: footer
            height: 60
            width: parent.width
            anchors.bottom: parent.bottom
        }
    }

}
