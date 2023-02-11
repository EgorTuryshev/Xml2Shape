import QtQuick 2.15
import QtQuick.Controls.Material 2.15

Rectangle {
    id: border
    property string text

    Material.background: Material.Teal
    Material.foreground: "#ffffff"

    border.color: Material.Teal
    border.width: 0.5
    radius: 5

    text: "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat"
    //text: "a short string"

    Text {
        id: text_field
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 5
        anchors.leftMargin: 5
        font.pixelSize: 14

        height: parent.height
        width: parent.width
        text: parent.text
        wrapMode: Text.WordWrap

    }

    Text {
        id: dummy_text
        text: parent.text
        visible: false
    }

    states: [
            State {
                name: "wide text"
                when: border.text.length > 20
                PropertyChanges {
                    target: border
                    width: 200
                    height: text_field.paintedHeight + 5
                }
            },
            State {
                name: "not wide text"
                when: border.text.length <= 20
                PropertyChanges {
                    target: border
                    width: 200
                    border.width: 0
                    height: text_field.paintedHeight + 5
                }
            }
        ]
}
