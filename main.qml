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
        property alias invertXY: invertXY.checked
    }

    Item{

        anchors.fill: parent

        Rectangle
        {
            id: leftBorder
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 20
            width: 450
            anchors.topMargin: 50
            anchors.bottomMargin: 20
            antialiasing: true
            border.color: "black"
            border.width: 0
            radius: 40
        }

        DropShadow
        {
            anchors.fill: leftBorder
            horizontalOffset: 1
            verticalOffset: 5
            radius: 40
            samples: 100
            color: "grey"
            spread: 0
            source: leftBorder
        }

        Rectangle
        {
            id: rightBorder
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.rightMargin: 20
            width: 600
            anchors.topMargin: 50
            anchors.bottomMargin: 20
            antialiasing: true
            border.color: "black"
            border.width: 0
            radius: 40
        }

        DropShadow
        {
            anchors.fill: rightBorder
            horizontalOffset: 1
            verticalOffset: 5
            radius: 40
            samples: 100
            color: "grey"
            spread: 0
            source: rightBorder
        }

        FolderDialog {
            id: folderDialog
            currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0] // TO-DO: Изменить
            acceptLabel: "Выбрать"

            onAccepted: {
                for (var i = 0; i < selectXMLBtn.filePaths.length; i++)
                {
                    appcore.test(selectXMLBtn.filePaths[i], appcore.getCurrentXSLTPath(), folderDialog.selectedFolder);
                }
                pBar.visible = false;
            }
            onRejected: {
                pBar.visible = false;
            }
        }

        UI_Button
        {
            id: selectXMLBtn
            property var filePaths: []

            anchors.bottom: rightBorder.bottom
            anchors.left: rightBorder.left
            anchors.leftMargin: 15
            anchors.bottomMargin: 50
            text: "Выбрать КПТ-файл(ы)"
            onClicked:{
                xmlFileDialog.open();
            }
        }

        UI_CheckItem
        {
            id: selectXMLMark
            width: selectXMLBtn.height - 10
            height: selectXMLBtn.height - 10
            anchors.top: selectXMLBtn.top
            anchors.left: selectXMLBtn.right
            anchors.leftMargin: 5
            anchors.topMargin: 5
        }

        UI_Button
        {
            id: writeBtn
            anchors.bottom: rightBorder.bottom
            anchors.left: selectXMLMark.right
            anchors.leftMargin: 20
            anchors.bottomMargin: 50
            text: "Записать ShapeFile"
            onClicked: {
                pBar.visible = true;
                folderDialog.open();
            }
        }

        UI_CheckItem
        {
            id: writeBtnMark
            width: writeBtn.height - 10
            height: writeBtn.height - 10
            anchors.top: writeBtn.top
            anchors.left: writeBtn.right
            anchors.leftMargin: 5
            anchors.topMargin: 5
        }

        UI_Button
        {
            id: animTest
            anchors.bottom: selectXMLBtn.top
            anchors.left: selectXMLBtn.left
            anchors.bottomMargin: 0
            text: "ТЕСТ АНИМАЦИИ"
            onClicked:
            {
                cItem.switchState();
            }
        }

        UI_CheckItem
        {
            id: cItem
            width: animTest.height - 10
            height: animTest.height - 10
            anchors.bottom: writeBtn.top
            anchors.left: animTest.right
            anchors.leftMargin: 5
            anchors.bottomMargin: 5
        }

        FileDialog {
            id: xmlFileDialog
            nameFilters: ["XML files (*.xml)", "ZIP archives (*.zip)"]
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
            anchors.top: leftBorder.top
            anchors.left: leftBorder.left
            anchors.leftMargin: 15
            anchors.topMargin: 25
            width: 205
            textRole: "display"
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
            anchors.top: leftBorder.top
            anchors.left: categoryCombo.right
            anchors.leftMargin: 5
            anchors.topMargin: categoryCombo.anchors.topMargin
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
            width: 205
            headertext: "Описание категории"
            text: ""
        }

        UI_DescriptionBox
        {
            id: xsltDesc
            anchors.top: xsltCombo.bottom
            anchors.left: xsltCombo.left
            anchors.topMargin: 25
            width: 205
            headertext: "Описание шаблона"
            text: "Пусто"
        }

        UI_DescriptionBox
        {
            id: instructionDesc
            anchors.top: categoryCombo.top
            anchors.left: selectXMLBtn.left
            anchors.topMargin: 0
            width: 400
            headertext: "Порядок работы"
            text: "
            <h3>Тут будет краткое руководство пользователя</h3>
            "
        }

        ProgressBar
        {
            id: pBar
            anchors.bottom: leftBorder.bottom
            anchors.left: leftBorder.left
            anchors.right: leftBorder.right
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
            title: "ГБУ \"ТФИ РК\""
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

                onClicked: {
                    appcore.refreshCategories();
                }
            }
            MenuBarItem
            {
                text: "Исправить разметку"
            }
            CheckBox
            {
                id: autoDirtyFix
                checked: true
                text: "Коррекция полигонов"

                onCheckedChanged:
                {
                    appcore.autoDirtyFixChanged(checked);
                }
            }
            CheckBox
            {
                id: invertXY
                text: "Инвертировать X и Y"

                onCheckedChanged:
                {
                    appcore.invertXYChanged(checked);
                }
            }
        }
        delegate: MenuBarItem
        {
            id: menuBarEmptyItem
            enabled: text !== headmenu.title
        }
    }
}


