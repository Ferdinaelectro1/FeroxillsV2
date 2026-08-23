import QtQuick
import QtQuick.Controls 6.5
import QtQuick.Layouts
import "components/"

MenuBar {
    implicitHeight: 25
    delegate: MenuBarItem {
        implicitHeight: 25
        background: Rectangle {
            color: parent.highlighted ? "#333" : "transparent"
        }
    }

    Menu {
        title: qsTr("File")
        delegate: FerroMenuItem {}
        Action {
            text: qsTr("Quit")
            onTriggered: Qt.quit()
        }
    }
    Menu {
        title: qsTr("Help")
        delegate: FerroMenuItem {}
        Action {
            text : qsTr("About")
            onTriggered: aboutDialog.open()
        }
    }
    Dialog {
        id: aboutDialog
        title: qsTr("About")
        standardButtons: Dialog.Ok
        modal: true
        anchors.centerIn: Overlay.overlay

        ColumnLayout {
            spacing: 5
            Text {
                text: "Feroxills"
                font.bold: true
                font.pixelSize: 16
            }
            Text {
                text: qsTr("Personal oscilloscope acquisition system built with Qt/QML and C++.")
                wrapMode: Text.WordWrap
                Layout.maximumWidth: 300
            }
            Text {
                text: qsTr("Author: ATI Tchaa-Sousso Ferdinand")
            }
            Text {
                text: qsTr("License: PolyForm Noncommercial 1.0.0")
            }
            Text {
                text: '<a href="https://github.com/Ferdinaelectro1/FeroxillsV2.git">GitHub Repository</a>'
                onLinkActivated: Qt.openUrlExternally(link)
            }
        }
    }
}