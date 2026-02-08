import QtQuick
import QtQuick.Controls

Item {
    //Menu dropdown
    implicitWidth: 120
    implicitHeight: 40
    Button  {
        anchors.fill: parent
        id: button
        text: "dorpdown"
        contentItem :
            Rectangle {
                anchors.verticalCenter: parent.verticalCenter
                color: "black"
                radius: 6
                Row {
                spacing: 8
                   Canvas {
                        width: 10
                        height: 6
                        anchors.verticalCenter: parent.verticalCenter

                        onPaint: {
                            var ctx = getContext("2d")
                            ctx.reset()
                            ctx.moveTo(width,0)
                            ctx.lineTo(width,height)
                            ctx.lineTo(0,height/2)
                            ctx.closePath()
                            ctx.fillStyle = "white"
                            ctx.fill()
                        }
                    }
                    Text {
                        id : text_select_box
                        text: "1 v/div"
                        color : button.down ? "gray" : "white"
                        font.pixelSize: 20
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

        onClicked: menu.open()

        Menu {
            id: menu
            y : button.height
            x : parent.width - menu.width

            MenuItem {
                text : "1 mv"
                onTriggered:
                {
                    console.log("Option 1 choisie")
                    text_select_box.text = "1 mv/div";
                }
            }
            MenuItem {
                text : "10 mv"
                onTriggered:
                {
                    console.log("Option 2 choisie")
                    text_select_box.text = "10 mv/div";
                }
            }
            MenuItem {
                text : "100 mv"
                onTriggered:
                {
                    console.log("Option 3 choisie")
                    text_select_box.text = "100 mv/div";
                }
            }
            MenuItem {
                text : "1 v"
                onTriggered:
                {
                    console.log("Option 4 choisie")
                    text_select_box.text = "1 v/div";
                }
            }
        }
    }
}

