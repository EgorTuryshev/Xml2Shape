import QtQuick 2.15
import QtQuick.Controls.Material 2.15
import Qt5Compat.GraphicalEffects

Item {
    id: root
    property string text: "Путь не задан"
    property int maxwidth: 400
    width: 20
    function animStart()
    {
        slideInanim.start();
    }
    function animEnd()
    {
        slideOutanim.start();
    }

//    Timer
//    {
//        id: timer
//    }

//    function delay(delayTime, cb)
//    {
//        timer.interval = delayTime;
//        timer.repeat = false;
//        timer.triggered.connect(cb);
//        timer.start();
//    }

    //property full_width width: text.length * 9 + folderIcon.width
    property color color: !enabled? "lightgrey" : Material.accent
    Rectangle
    {
        id: borderRect
        width: root.height
        border.width: 2
        border.color: root.color
        color: "white"
        radius: 5
        antialiasing: true
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        clip: true
        Rectangle
        {
            id: backGround
            anchors.left: parent.left
            anchors.top: parent.top
            color: root.color
            width: parent.height
            height: width
            radius: 5
            MouseArea
            {
                anchors.fill: parent
                hoverEnabled: true
                onEntered:
                {
                    root.animStart();
                }
                onExited:
                {
                    /*delay(1000, function()
                    {
                        root.animEnd()
                    });*/
                    root.animEnd()
                }
            }
        }
        Image
        {
            id: folderIcon
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 5
            anchors.topMargin: 5
            width: parent.height - 10
            height: width
            source: "qrc:/folder.svg"
        }

        ColorOverlay
        {
            anchors.fill: folderIcon
            source: folderIcon
            color: "white"
        }

        Text
        {
            id: text
            anchors.left: backGround.right
            anchors.bottom: parent.bottom
            anchors.leftMargin: 5
            font.pixelSize: 14
            font.bold: true
            anchors.bottomMargin: parent.height / 2 - height / 2 + 1
            text: root.text
            color: root.color.darker(2)
        }
        ParallelAnimation
        {
            id: slideInanim
            NumberAnimation
            {
                target: borderRect
                property: "width"
                duration: 400
                to: root.text.length * 9 + folderIcon.width > root.maxwidth ? root.maxwidth : root.text.length * 9 + folderIcon.width
                easing.type: Easing.InOutQuad
            }
        }
        SequentialAnimation
        {
            id: slideOutanim

            PauseAnimation
            {
                duration: 1000
            }
            NumberAnimation
            {
                target: borderRect
                property: "width"
                duration: 400
                to: root.height
                easing.type: Easing.InOutQuad
            }
        }
    }
}
