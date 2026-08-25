import QtQuick
import QtQuick.Controls 6.5
import QtQuick.Layouts
import "components/"
import Feroxills.ProviderType 1.0

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
        Menu {
            title : qsTr("Change source")
            delegate: FerroMenuItem {}
            Action {
                checkable : true
                checked : backend.sourceController.currentProviderType === ProviderType.UART_SOURCE
                text: qsTr("Serial (UART)");
                onTriggered: {
                    //change to UartSource
                    backend.sourceController.switchToWithAutoSettings(ProviderType.UART_SOURCE);
                    checked = Qt.binding(function () {
                        return backend.sourceController.currentProviderType === ProviderType.UART_SOURCE
                    });
                }
            }
            Action {
                checkable : true
                checked : backend.sourceController.currentProviderType === ProviderType.SOFTWARE_SOURCE
                text: qsTr("Software");
                onTriggered: {
                    //change to SoftwareSource
                    backend.sourceController.switchToWithAutoSettings(ProviderType.SOFTWARE_SOURCE);
                    //rebind via C++ for validate check
                    checked = Qt.binding(function () {
                        return backend.sourceController.currentProviderType === ProviderType.SOFTWARE_SOURCE
                    });
                }
            }
        }
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