import QtQuick 2.15
import QtQuick.Controls.Material 2.15
import Qt5Compat.GraphicalEffects

Item {
    id: root
    antialiasing: true
    property color color: Material.accent
    property bool isActive: false
    function switchState()
    {
        if(!isActive)
        {
            animation.start();
            border_animation.start();
        }
        else
        {
            flush.start();
            border_animation.start();
        }

        isActive = !isActive;
    }
    Rectangle
    {
        id: outerLine
        width: root.width
        height: root.height
        anchors.left: root.left
        anchors.top: root.top
        color: "white"
        border.color: root.color
        border.width: 2
        radius: 5

        SequentialAnimation
        {
            id: border_animation
            NumberAnimation {
                target: outerLine
                property: "border.width"
                from: outerLine.border.width
                to: 4
                easing.type: Easing.InOutQuad
                duration: 200
            }

            PauseAnimation {
                duration: 800
            }
            NumberAnimation {
                target: outerLine
                property: "border.width"
                from: 4
                to: outerLine.border.width
                easing.type: Easing.InOutQuad
                duration: 100
            }
        }



        Item
        {
            id: checkMark
            anchors.fill: parent
            Rectangle
            {
                id: leftPart
                width: parent.width / 10
                height: parent.height / 2.1
                color: "white"
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: parent.width / 4
                anchors.topMargin: parent.height / 3
                rotation: -45
                radius: 5
                opacity: 1
            }
            Rectangle
            {
                id: rightPart
                width: leftPart.width
                height: parent.height / 1.6
                color: leftPart.color
                anchors.left: leftPart.right
                anchors.top: parent.top
                anchors.leftMargin: 9
                anchors.topMargin: 6
                rotation: 40
                radius: leftPart.radius
                opacity: leftPart.opacity
            }

            Rectangle
            {
                id: mask
                color: "transparent"
                width: parent.width
                height: parent.height

                Rectangle
                {
                    id: animRect
                    color: root.color
                    width: 10
                    height: parent.height
                    radius: 30
                    x: -100
                    SequentialAnimation
                    {
                        id: animation
                        NumberAnimation
                        {
                            target: animRect
                            property: "width"
                            from: root.width;
                            to: animRect.width;
                            duration: 0 ;
                        }
                        NumberAnimation
                        {
                            target: animRect
                            property: "x"
                            from: -50 ;
                            to: 50 ;
                            easing.type: Easing.InOutQuad
                            duration: 800 ;
                        }
                        NumberAnimation
                        {
                            target: animRect
                            property: "width"
                            from: animRect.width;
                            to: root.width;
                            duration: 0 ;
                        }
                        NumberAnimation
                        {
                            target: animRect
                            property: "x"
                            from: 50;
                            to: 0;
                            easing.type: Easing.InOutQuad
                            duration: 400 ;
                        }
                    }
                    SequentialAnimation
                    {
                        id: flush
                        ParallelAnimation
                        {
                            NumberAnimation
                            {
                                target: checkMark
                                property: "scale"
                                from: 1;
                                to: 0;
                                easing.type: Easing.InSine
                                duration: 600;
                            }
                            NumberAnimation
                            {
                                target: checkMark
                                property: "rotation"
                                from: 0;
                                to: 360;
                                easing.type: Easing.InSine
                                duration: 800;
                            }
                        }
                        NumberAnimation
                        {
                            target: animRect
                            property: "x"
                            from: 0;
                            to: -50;
                            duration: 0;
                        }
                        NumberAnimation
                        {
                            target: checkMark
                            property: "scale"
                            from: 0;
                            to: 1;
                            duration: 0;
                        }
                    }
                }

                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: checkMark
                    cached: false
                }

            }

        }
    }
}
