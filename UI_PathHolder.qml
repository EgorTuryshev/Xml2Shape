import QtQuick 2.15
import QtQuick.Controls.Material 2.15

Item {
    id: root
    property string text
    width: text.length * 9
    property color color: !enabled? "lightgrey" : Material.accent
    Rectangle
    {
        border.width: 2
        border.color: root.color
        color: "white"
        radius: 5
        antialiasing: true
        anchors.fill: parent
        Text
        {
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.leftMargin: 5
            font.pixelSize: 14
            font.bold: true
            anchors.bottomMargin: parent.height / 2 - height / 2 + 1
            text: root.text
            color: root.color.darker(2)
        }
    }
}
