import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts 6.5
import QtQuick.Shapes 6.5
import Feroxills.ProviderType 1.0
import QtQuick.Controls.Material

Window {
    visible: true
    width: 1280
    height: 720
    maximumWidth: 1280
    maximumHeight: 720
    Material.theme: Material.Dark
    Material.accent: "#4dabf7"
    Material.primary: "#2b2b2b"
    title: {
        const beginWord = "Feroxills 0.0.1 - ";
        switch (backend.sourceController.currentProviderType) {
            case ProviderType.SOFTWARE_SOURCE:
                return beginWord + "SOFTWARE";
            case ProviderType.UART_SOURCE:
                return beginWord + "UART SOURCE";
            case ProviderType.USB_SOURCE:
                return beginWord + "USB SOURCE";
            default:
                return beginWord + "OTHER";
        }
    }

    Rectangle {
        id: root
        width: parent.width
        height: parent.height
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
