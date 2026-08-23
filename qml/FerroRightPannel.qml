import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import Feroxills.DisplayMode 1.0
import Feroxills.Settings 1.0
import Feroxills.ProviderType 1.0
import "components/"

Item {
    id: root1
    property var currentDisplayModeObject: null
    property int currentMode : 0
    // --- RIGHT PANEL (buttons) ---
    implicitWidth: 250
    implicitHeight: 700
    Rectangle {
        color: "#1A1B20"
        border.color: "#2E2E34"
        anchors.fill: parent
        Column {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 15

            Rectangle {
                    property bool isRun : backend.run
                    width: parent.width
                    height: 45
                    color: isRun ? "red" : "#37b837"
                    radius: 6
                    border.color: "#3A3A42"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("Click ")
                            parent.isRun  = !parent.isRun
                            backend.run = parent.isRun
                        }
                    }

                    Text {
                        anchors.centerIn: parent
                        text: parent.isRun ? qsTr("Stop") : qsTr("Run")
                        color: "#eae5e5"
                        font.pixelSize: 30
                        font.bold: true
                    }
            }

            Repeater {
                id : repeater
                // Retrieve the initial display mode from C++ and initialize the QML button states accordingly.
                model : [
                    { name : qsTr("Roll")    ,mode : FDisplayMode.CONTINU} ,
                    { name : qsTr("Trigger") ,mode : FDisplayMode.TRIGGER } ,
                    { name : qsTr("Auto")    ,mode : FDisplayMode.AUTO }
                ]
                Rectangle {
                    property bool btnEnabled: !(backend.display_context.getCurrentMode() === modelData.mode)
                    width: parent.width
                    height: 45
                    border.color: "#3A3A42"
                    radius: 6
                    color: btnEnabled ? "#0000FF" : "#9a9af1"
                    Text {
                        anchors.centerIn: parent
                        text: modelData.name
                        color: "#eae5e5"
                        font.pixelSize: 30
                        font.bold: true
                    }
                    MouseArea {
                        anchors.fill : parent
                        enabled : btnEnabled
                        onClicked : {
                            console.log("Button pressed: ",modelData.name)
                            console.log("Mode: ",modelData.mode)
                            backend.display_context.setNewMode(modelData.mode)
                            root1.currentMode = modelData.mode;
                            // first activate all buttons
                            for (let i = 0; i < repeater.count; i++) {
                                if(index === i) continue;
                                let this_index = repeater.itemAt(i);
                                this_index.btnEnabled = true;
                            }
                            // then deactivate the currently clicked button
                            parent.btnEnabled = false;
                        }
                    }
                }
            }
            Button {
                text: qsTr("Reset Trigger")
                enabled : root1.currentMode === FDisplayMode.TRIGGER
                onClicked: {
                    triggerModePopup.open()
                }
            }
            Loader {
                function getPanelSource()  {
                    switch (backend.sourceController.currentProviderType) {
                        case ProviderType.SOFTWARE_SOURCE: return "SoftwareRightPanel.qml"
                        case ProviderType.UART_SOURCE : return "SerialRightPanel.qml"
                        case ProviderType.USB_SOURCE: return "UsbRightPanel.qml"
                        default : return ""
                    }
                }
                width: parent.width
                source: getPanelSource()
            }
        }//combo box
    }
    Connections {
        target: backend
        function onTriggerModeDisplayInvoked(triggerDisplayMode)
        {
            currentDisplayModeObject = triggerDisplayMode;
            triggerModePopup.open()
        }
    }
    FerroOneValuePopup {
        id : parametresPopup
        title : qsTr("Trigger Level")
        onValidate: function(value) {
            currentDisplayModeObject.setTriggerLevel(value)
            parametresPopup.close()
        }
    }
    FerroTriggerTypePopup {
        id: triggerModePopup
        selectedMode : Settings.triggerModeTriggerType
        onValidate: (mode) => {
            Settings.triggerModeTriggerType = mode;// 0 = continuous, 1 = single shot
            console.log("New mode: ",mode)
            parametresPopup.open()
        }
    }
}
