import QtQuick 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Window 2.15
import Qt5Compat.GraphicalEffects
import QtQml
import Qt.labs.settings 1.0
import QtQuick.Dialogs
import "qrc:/qres/"

ApplicationWindow
{
    id: window

    Connections
    {
        target: appcore
    }

    onClosing: {
        if (autoClear.checked)
        {
            appcore.clearLog(autoClear.checked);
        }
    }

    property int accent: Material.Teal

    width: 1000
    height: 600
    visible: true
    title: qsTr("Xml2Shape")
    Material.theme: Material.Light
    Material.accent: accent

    Settings
    {
        id: settings
            property alias x: window.x
            property alias y: window.y
            property alias width: window.width
            property alias height: window.height
            property alias accent: window.accent
            property alias i: themeSwitch.i
            property alias isAutoClear: autoClear.checked
            property alias autoDirtyFix: autoDirtyFix.checked
    }

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
            border.color: "black"
            border.width: 0
            radius: 40
        }

        DropShadow
        {
            anchors.fill: border
            horizontalOffset: 1
            verticalOffset: 5
            radius: 40
            samples: 100
            color: "grey"
            spread: 0
            source: border
        }

        UI_Button
        {
            id: writeBtn
            anchors.bottom: border.bottom
            anchors.right: border.right
            anchors.rightMargin: 20
            anchors.bottomMargin: 50
            text: "Записать ShapeFile"
            onClicked: {
                folderDialog.open();
                pBar.visible = true;
                appcore.test();
            }
        }

        FolderDialog {
            id: folderDialog
            currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0] // TO-DO: Изменить
            acceptLabel: "Выбрать"

            onAccepted: {
                for (var i = 0; i < selectXMLBtn.filePaths.length; i++)
                {
                    appcore.test(selectXMLBtn.filePaths[i], "../Xml2Shape/samples/kpt.xsl", folderDialog.selectedFolder); // TO-DO: переделать, это тест
                }
                pBar.visible = false;
            }
        }

        UI_Button
        {
            id: selectXMLBtn
            property var filePaths: []

            anchors.bottom: border.bottom
            anchors.right: writeBtn.left
            anchors.rightMargin: 5
            anchors.bottomMargin: 50
            text: "Выбрать XML-файл(ы)"
            onClicked:{
                xmlFileDialog.open();
            }
        }

        FileDialog {
            id: xmlFileDialog
            nameFilters: ["XML files (*.xml)"]
            currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0] // TO-DO: Изменить
            acceptLabel: "Выбрать"
            fileMode: FileDialog.OpenFiles
            onAccepted: {
                selectXMLBtn.filePaths = selectedFiles;
            }
        }

        UI_Combo
        {
            id: categoryCombo
            anchors.top: border.top
            anchors.left: border.left
            anchors.leftMargin: 15
            anchors.topMargin: 50
            width: 205
            textRole: "display"
            //["Категория не выбрана"]
            model: model_categories
            onCurrentTextChanged:
            {
                xsltCombo.currentIndex = 0;
                appcore.setCombo1_Index(currentIndex);
                model.display = currentText;
                generator.generate(currentText);
                categoryDesc.text = appcore.getCurrentCategoryDescription();
            }
        }

        UI_Combo
        {
            id: xsltCombo
            anchors.top: border.top
            anchors.left: categoryCombo.right
            anchors.leftMargin: 5
            anchors.topMargin: 50
            width: 205
            model: model_xslts
            textRole: "display"
            onCurrentTextChanged:
            {
                appcore.setCombo2_Index(currentIndex);
                xsltDesc.text = appcore.getCurrentXSLTDescription();
            }
        }

        UI_DescriptionBox
        {
            id: categoryDesc
            anchors.top: categoryCombo.bottom
            anchors.left: categoryCombo.left
            anchors.topMargin: 25
            headertext: "Описание категории"
            text: ""
        }

        UI_DescriptionBox
        {
            id: xsltDesc
            anchors.top: xsltCombo.bottom
            anchors.left: xsltCombo.left
            anchors.topMargin: 25
            headertext: "Описание шаблона"
            text: "Пусто"
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
            visible: false
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

        Material.background: Material.accentColor
        Material.foreground: "#ffffff"
        font.pixelSize: 14
        contentWidth: parent.width

        Menu
        {
            id: headmenu
            title: "ГБУ ТФИ РК"
        }
        Menu
        {
            title: "Файл"

            MenuBarItem
            {
                id: themeSwitch
                property var accents: [Material.Teal, Material.Green, Material.Indigo, Material.Cyan, Material.DeepOrange, Material.Grey, Material.BlueGrey];
                property int i: 0;
                text: "Сменить тему"
                onClicked:
                {
                    if (i < accents.length - 1)
                    {
                       i++;
                    }
                    else
                    {
                        i = 0;
                    }
                    window.accent = accents[i];
                }
            }
            MenuSeparator { }
            MenuBarItem
            {
                text: "Закрыть"
                onClicked: {
                    window.close();
                }
            }
        }
        Menu
        {
            title: "Лог"

            MenuBarItem
            {
                text: "Открыть лог"
                onClicked: {
                    appcore.openLog();
                }
            }
            CheckBox
            {
                id: autoClear
                text: "Автоочистка лога"
                checked: true

                onCheckedChanged:
                {
                   appcore.autoClearingLogChanged(checked);
                }
            }
            MenuSeparator { }
            MenuBarItem
            {
                text: "Очистить лог"
                onClicked: {
                    appcore.clearLog();
                }
            }
        }
        Menu
        {
            title: "Опции"

            MenuBarItem
            {
                text: "Обновить категории"
            }
            MenuBarItem
            {
                text: "Исправить разметку"
            }
            CheckBox
            {
                id: autoDirtyFix
                checked: true
                text: "Автоисправление грязных полигонов"
            }
            CheckBox
            {
                text: "Инвертировать X и Y"
            }
        }
        delegate: MenuBarItem
        {
            id: menuBarEmptyItem
            enabled: text !== headmenu.title
        }
    }
}


