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

    font.family: "Calibri"
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

    property int accent: Material.Green
    property bool isRightMenuActive: true;

    width: 1000
    height: 600
    minimumWidth: 800
    minimumHeight: 600
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
        property alias isAutoClear: autoClear.checked
        property alias autoDirtyFix: autoDirtyFix.checked
        property alias invertXY: invertXY.checked
        //property alias isRightMenuActive: window.isRightMenuActive
    }

    Timer
    {
        id: timer
    }

    function delay(delayTime, cb)
    {
        timer.interval = delayTime;
        timer.repeat = false;
        timer.triggered.connect(cb);
        timer.start();
    }

    Item {

        anchors.fill: parent

        Item
        {
            anchors.fill: parent
            id: leftGroup
            z: 1
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

                ParallelAnimation
                {
                    id: leftSlideOutAnimation
                    NumberAnimation
                    {
                        id: leftSlideOutTargets
                        targets: [categoryCombo, xsltCombo, categoryDesc, xsltDesc]
                        property: "opacity"
                        duration: 400
                        to: 0
                        easing.type: Easing.InOutQuad
                    }
                }
                ParallelAnimation
                {
                    id: leftSlideInAnimation
                    NumberAnimation
                    {
                        targets: leftSlideOutTargets.targets
                        property: "opacity"
                        duration: 400
                        to: 1
                        easing.type: Easing.InOutQuad
                    }
                    NumberAnimation
                    {
                        target: leftBorder
                        property: "width"
                        duration: 400
                        to: window.width - 100
                        easing.type: Easing.InOutQuad
                    }
                }
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
                opacity: 0
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
                opacity: 0
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
                opacity: 0
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
                opacity: 0
            }
            UI_Button
            {
                id: leftSwitchMenuBtn
                anchors.right: leftBorder.right
                text: "<<"
                font.bold: true
                anchors.top: leftBorder.top
                height: 60
                width: 40
                anchors.topMargin: leftBorder.height / 2 - height / 2
                onClicked:
                {
                    if(window.isRightMenuActive)
                    {
                        rightSlideOutAnimation.start();
                        leftSlideInAnimation.start();
                    }
                    else
                    {
                        leftSlideOutAnimation.start();
                        rightSlideInAnimation.start();
                    }
                    window.isRightMenuActive = !window.isRightMenuActive;
                }
            }

        }

        Item
        {
            id: rightGroup
            anchors.fill: parent
            z: 100
            Rectangle
            {
                id: rightBorder
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.rightMargin: 20
                width: window.width - 100
                anchors.topMargin: 50
                anchors.bottomMargin: 20
                antialiasing: true
                border.color: "black"
                border.width: 0
                radius: 40
                ParallelAnimation
                {
                    id: rightSlideInAnimation
                    NumberAnimation
                    {
                        target: rightBorder
                        property: "width"
                        duration: 400
                        to: window.width - 100
                        easing.type: Easing.InOutQuad
                    }
                    NumberAnimation
                    {
                        id: rightSlideOutTargets
                        targets: [selectXMLBtn, writeBtn, instructionDesc, writeBtnMark, selectXMLMark, rightSwitchMenuBtn]
                        property: "opacity"
                        duration: 300
                        to: 1
                        easing.type: Easing.InOutQuad
                    }
                    SequentialAnimation
                    {

                        PauseAnimation
                        {
                            duration: 0
                        }
                        NumberAnimation
                        {
                            target: rightGroup
                            property: "z"
                            duration: 0
                            to: 100
                        }
                    }
                }
                ParallelAnimation
                {
                    id: rightSlideOutAnimation
                    NumberAnimation
                    {
                        target: rightBorder
                        property: "width"
                        duration: 400
                        to: 600
                        easing.type: Easing.InOutQuad
                    }
                    NumberAnimation
                    {
                        targets: rightSlideOutTargets.targets
                        property: "opacity"
                        duration: 400
                        to: 0
                        easing.type: Easing.InOutQuad
                    }
                    SequentialAnimation
                    {

                        PauseAnimation
                        {
                            duration: 300
                        }
                        NumberAnimation
                        {
                            target: rightGroup
                            property: "z"
                            duration: 0
                            to: 0
                        }
                    }
                }
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

            UI_Button
            {
                id: rightSwitchMenuBtn
                text:">>"
                font.bold: true
                anchors.left: rightBorder.left
                anchors.top: rightBorder.top
                height: 60
                width: 40
                anchors.topMargin: rightBorder.height / 2 - height / 2
                onClicked:
                {
                    if(window.isRightMenuActive)
                    {
                        rightSlideOutAnimation.start();
                        leftSlideInAnimation.start();
                    }
                    else
                    {
                        leftSlideOutAnimation.start();
                        rightSlideInAnimation.start();
                    }
                    window.isRightMenuActive = !window.isRightMenuActive;
                }
            }

            FolderDialog {
                id: folderDialog
                currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0] // TO-DO: Изменить
                acceptLabel: "Выбрать"

                onAccepted: {
                    shpPathHolder.text = appcore.getFilePath(selectedFolder) + "/ ";
                    reviewShpDirBtnMark.animateAsActive();
                    writeBtn.enabled = true;
                    writeBtnMark.enabled = true;
                }
                onRejected:
                {
                    reviewShpDirBtnMark.reset();
                    writeBtn.enabled = false;
                    writeBtnMark.enabled = false;
                }
            }

            UI_Button
            {
                id: reviewShpDirBtn
                anchors.bottom: writeBtn.top
                anchors.left: writeBtn.left
                anchors.bottomMargin: 10
                enabled: false
                text: "Выбор директории ShapeFile"
                onClicked:
                {
                    folderDialog.open();
                }
            }

            UI_CheckItem
            {
                id: reviewShpDirBtnMark
                width: reviewShpDirBtn.height - 10
                height: reviewShpDirBtn.height - 10
                anchors.top: reviewShpDirBtn.top
                anchors.left: reviewShpDirBtn.right
                anchors.leftMargin: 5
                anchors.topMargin: 5
                enabled: false
            }

            UI_PathHolder
            {
                id: shpPathHolder
                anchors.left: reviewShpDirBtnMark.right
                anchors.top: reviewShpDirBtnMark.top
                anchors.leftMargin: 5
                height: selectXMLMark.height
                enabled: false
                maxwidth: rightBorder.width - x + selectXMLMark.height * 2
                imgSource: "qrc:/folder.svg"
            }

            UI_Button
            {
                id: writeBtn
                anchors.bottom: rightBorder.bottom
                anchors.left: rightBorder.left
                anchors.leftMargin: 45
                anchors.bottomMargin: 50
                anchors.right: reviewShpDirBtn.right
                enabled: false
                text: "Записать ShapeFile"
                onClicked:
                {
                    for (var i = 0; i < selectXMLBtn.filePaths.length; i++)
                    {
                        appcore.test(selectXMLBtn.filePaths[i], appcore.getCurrentXSLTPath(), folderDialog.selectedFolder);
                    }
                    writeBtnMark.animateAsActive();
                    delay(5000, function()
                    {
                        writeBtnMark.animateAsFlushed();
                    });
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
                enabled: false
            }

            UI_Button
            {
                id: selectXMLBtn
                property var filePaths: []
                width: reviewShpDirBtn.width
                anchors.bottom: reviewShpDirBtn.top
                anchors.left: reviewShpDirBtn.left
                anchors.leftMargin: 0
                anchors.bottomMargin: 0
                text: "Выбрать КПТ-файл(ы)"
                onClicked:
                {
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

            UI_PathHolder
            {
                id: xmlPathHolder
                anchors.left: selectXMLMark.right
                anchors.top: selectXMLMark.top
                anchors.leftMargin: 5
                height: selectXMLMark.height
                maxwidth: rightBorder.width - x + selectXMLMark.height * 2
                imgSource: "qrc:/files.svg"
            }

            FileDialog {
                id: xmlFileDialog
                nameFilters: ["ZIP archives (*.zip)", "XML files (*.xml)"]
                currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0] // TO-DO: Изменить
                acceptLabel: "Выбрать"
                fileMode: FileDialog.OpenFiles
                onAccepted: {
                    reviewShpDirBtnMark.enabled = true;
                    shpPathHolder.enabled = true;
                    xmlPathHolder.text = appcore.getFilePath(selectedFiles[0]);
                    reviewShpDirBtn.enabled = true;
                    selectXMLBtn.filePaths = selectedFiles;
                    selectXMLMark.animateAsActive();
                }
                onRejected:
                {

                }
            }

            UI_DescriptionBox
            {
                id: instructionDesc
                anchors.top: rightBorder.top
                anchors.left: selectXMLBtn.left
                anchors.topMargin: 25
                width: rightBorder.width - 100
                headertext: "Порядок работы"
                text: "
                <b>1. Выбрать КПТ-файл</b>
                <br>
                <b>2. Выбрать шаблон*</b>
                <br>
                <b>3. Сохранить ShapeFile<b>
                <br>
                <br>
                *- при необходимости
                "

            }


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
        Material.foreground: Material.accentColor.lighter(3)
        font.pixelSize: 14
        contentWidth: parent.width
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


