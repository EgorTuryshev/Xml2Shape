import QtQuick 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Window 2.15
import "qrc:/qres/"

ApplicationWindow {

    Connections {
        target: appcore
    }

    width: 800
    height: 600
    visible: true
    title: qsTr("Xml2Shape")

    Material.theme: Material.Light
    Material.accent: Material.LightGreen

    Grid {
        property int orientation: Qt.Vertical
        columns: orientation === Qt.Horizontal ? -1 : 1

        MenuBar{

            Material.background: Material.Teal
            Material.foreground: "#ffffff"
            font.pixelSize: 16

            Menu{
                title: "Файл"

                MenuBarItem{
                    text: "Закрыть"
                    onClicked: {
                        window:close();
                    }
                }
            }
            Menu{
                title: "Опции"

                MenuBarItem{
                    text: "Структура XML"
                }
                MenuBarItem{
                    text: "Отображение ShapeFile"
                }
            }
        }

        UI_Button{

            text: "Записать ShapeFile"
            onClicked:{
                appcore.test();
            }
        }
    }
}


