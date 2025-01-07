import QtQuick 2.15
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Controls 2.12
import QtMultimedia

Item{
    property color myColor: Qt.rgba(redSlider.value, greenSlider.value, blueSlider.value, 1) //enable when slider settings are on

    Image {
        id: menuIcon
        source: "qrc:/shared/images/menu"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.bottomMargin: 20
        anchors.leftMargin: 25
        width: 70
        height: 70
        smooth: true
        mipmap: true
        MouseArea {
            anchors.fill: parent
            onClicked: {
                contentLoader.source = "modules/menu/menu.qml"
            }
        }
    }
    ColorOverlay {
        anchors.fill: menuIcon
        source: menuIcon
        color: myColor
        smooth: true
    }
    Column {
        y: 200
        spacing: 100
        anchors.centerIn: parent
        Slider {
            id: redSlider
            anchors.horizontalCenter: parent.horizontalCenter
            width: 500
            from: 0
            to: 1
            value: 0.5
            background: Rectangle {
                x: redSlider.leftPadding
                y: redSlider.topPadding + redSlider.availableHeight / 2 - height / 2
                implicitWidth: 700
                implicitHeight: 4
                width: redSlider.availableWidth
                height: implicitHeight
                radius: 2
                color: "black"
                border.color: myColor

                Rectangle {
                    width: redSlider.visualPosition * parent.width
                    height: parent.height
                    color: myColor
                    radius: 2
                }
            }

            handle: Rectangle {
                x: redSlider.leftPadding + redSlider.visualPosition * (redSlider.availableWidth - width)
                y: redSlider.topPadding + redSlider.availableHeight / 2 - height / 2
                implicitWidth: 26
                implicitHeight: 26
                radius: 13
                color: redSlider.pressed ? myColor : "white"
                border.color: myColor
            }
        }
        Slider {
            id: greenSlider
            anchors.horizontalCenter: parent.horizontalCenter
            width: 500
            from: 0
            to: 1
            value: 0.5
            background: Rectangle {
                x: greenSlider.leftPadding
                y: greenSlider.topPadding + greenSlider.availableHeight / 2 - height / 2
                implicitWidth: 700
                implicitHeight: 4
                width: greenSlider.availableWidth
                height: implicitHeight
                radius: 2
                color: "black"
                border.color: myColor

                Rectangle {
                    width: greenSlider.visualPosition * parent.width
                    height: parent.height
                    color: myColor
                    radius: 2
                }
            }

            handle: Rectangle {
                x: greenSlider.leftPadding + greenSlider.visualPosition * (greenSlider.availableWidth - width)
                y: greenSlider.topPadding + greenSlider.availableHeight / 2 - height / 2
                implicitWidth: 26
                implicitHeight: 26
                radius: 13
                color: greenSlider.pressed ? myColor : "white"
                border.color: myColor
            }
        }
        Slider {
            id: blueSlider
            anchors.horizontalCenter: parent.horizontalCenter
            width: 500
            from: 0
            to: 1
            value: 0.5
            background: Rectangle {
                x: blueSlider.leftPadding
                y: blueSlider.topPadding + blueSlider.availableHeight / 2 - height / 2
                implicitWidth: 700
                implicitHeight: 4
                width: blueSlider.availableWidth
                height: implicitHeight
                radius: 2
                color: "black"
                border.color: myColor

                Rectangle {
                    width: blueSlider.visualPosition * parent.width
                    height: parent.height
                    color: myColor
                    radius: 2
                }
            }

            handle: Rectangle {
                x: blueSlider.leftPadding + blueSlider.visualPosition * (blueSlider.availableWidth - width)
                y: blueSlider.topPadding + blueSlider.availableHeight / 2 - height / 2
                implicitWidth: 26
                implicitHeight: 26
                radius: 13
                color: blueSlider.pressed ? myColor : "white"
                border.color: myColor
            }
        }
    }
    ColorOverlay {
          anchors.fill: backgroundLines
          source: backgroundLines
          color: myColor
          opacity: 0.5
    }
}
