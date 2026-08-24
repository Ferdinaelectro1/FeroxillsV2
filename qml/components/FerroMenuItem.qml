import QtQuick
import QtQuick.Controls 6.5

MenuItem {
    implicitHeight: 24
    background : Rectangle {
        color: parent.highlighted ? "#3574F0" : "#333"
    }
}
