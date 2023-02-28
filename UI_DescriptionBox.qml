import QtQuick 2.15
import QtQuick.Controls.Material 2.15

Item{
    id: root
    property string text
    property string headertext
    headertext: "Описание категории"
    text: "Категория не выбрана, но текст здесь может быть очень длинным"
Rectangle
{
    id: border
    antialiasing: true
    border.width: 0.5
    radius: 5
    border.color: "grey"
    opacity: 0.7

    Text {
        id: text_field
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 5
        anchors.leftMargin: 5
        font.pixelSize: 14
        height: parent.height
        width: parent.width
        text: root.text
        wrapMode: Text.WordWrap

    }

    states: [
            State
        {
                name: "wide text"
                when: root.text.length > 20
                PropertyChanges {
                    target: border
                    width: 205
                    height: text_field.paintedHeight + 5
                }
            },
            State
        {
                name: "not wide text"
                when: root.text.length <= 10
                PropertyChanges {
                    target: border
                    width: 200
                    border.width: 0
                    height: text_field.paintedHeight + 5
                }
            }
        ]
}

Rectangle
{
   color: "white"
   id: header
   radius: 5
   antialiasing: true
   anchors.bottom: border.top
   anchors.left: border.left
   anchors.leftMargin: 5
   anchors.bottomMargin: -5
   height: 10
   width: headertext.length * 9
   opacity: 1

   Text
   {
       id: header_text_field
       anchors.top: parent.top
       anchors.left: parent.left
       anchors.topMargin: -8
       anchors.leftMargin: 5
       font.pixelSize: 14
       font.bold: true
       opacity: 0.6
       height: parent.height
       width: parent.width
       text: root.headertext
       wrapMode: Text.WordWrap
   }
}
}
