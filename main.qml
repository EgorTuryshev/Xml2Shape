import QtQuick 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Window 2.15
import Qt5Compat.GraphicalEffects
import "qrc:/qres/"

ApplicationWindow
{

    Connections
    {
        target: appcore
    }

    width: 1000
    height: 600
    visible: true
    title: qsTr("Xml2Shape")

    Material.theme: Material.Light
    Material.accent: Material.Teal

    Item{

        anchors.fill: parent

        Rectangle
        {
            anchors.fill: parent
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            anchors.topMargin: 50
            anchors.bottomMargin: 20
            id: border
            antialiasing: true
            Material.background: Material.Grey
            Material.foreground: "#ffffff"
            border.color: "black"
            border.width: 0
            radius: 40
        }

        DropShadow
        {
            anchors.fill: border
            horizontalOffset: 1
            verticalOffset: 1
            radius: 50
            samples: 100
            color: "grey"
            spread: 0
            source: border
        }

        UI_Button
        {
            id: writeBtn
            anchors.top: border.top
            anchors.left: border.left
            anchors.leftMargin: 20
            anchors.topMargin: 50
            text: "Записать ShapeFile"
            onClicked:{
                appcore.test();
            }
        }

        UI_Combo
        {
            id: categoryCombo
            anchors.top: border.top
            anchors.left: writeBtn.right
            anchors.leftMargin: 5
            anchors.topMargin: 50
            width: 200
            model: ["Категория не выбрана", "1", "2"]
        }

        UI_Combo
        {
            id: xsltCombo
            anchors.top: border.top
            anchors.left: categoryCombo.right
            anchors.leftMargin: 5
            anchors.topMargin: 50
            width: 200
        }

        Label
        {
            id: categoryHeader
            anchors.top: categoryCombo.bottom
            anchors.left: categoryCombo.left
            anchors.topMargin: 25
            text: "Описание категории"
        }

        /*Label
        {
            anchors.top: categoryHeader.bottom
            anchors.left: categoryHeader.left
            anchors.topMargin: 5
            width: 50
            wrapMode: Text.WordWrap
            text: "Пример длииииииииииииииииииииииииииииииииииииииииииииииинного описания категории"
        }*/

        UI_DescriptionBox
        {
            anchors.top: categoryHeader.bottom
            anchors.left: categoryHeader.left
            anchors.topMargin: 5
        }

        ProgressBar
        {
            id: pBar
            anchors.bottom: border.bottom
            anchors.left: border.left
            anchors.right: border.right
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            indeterminate: true
            visible: true
            background: Rectangle
            {
                implicitWidth: 200
                implicitHeight: 6
                color: "#e6e6e6"
                radius: 40
            }
        }

    }

    MenuBar
    {

        Material.background: Material.Teal
        Material.foreground: "#ffffff"
        font.pixelSize: 14

        Menu
        {
            title: "Файл"

            MenuBarItem
            {
                text: "Закрыть"
                onClicked: {
                    window:close();
                }
            }
        }
        Menu
        {
            title: "Опции"

            MenuBarItem{
                text: "Структура XML"
            }
            MenuBarItem{
                text: "Отображение ShapeFile"
            }
        }
    }
}


