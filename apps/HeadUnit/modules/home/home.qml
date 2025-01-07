import QtQuick 2.15
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Controls 2.12
import QtMultimedia

Item{
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
                // Handle the click event here
                // console.log("menu");
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
    Rectangle {
        id: home
        color: "transparent"
        height: 600
        width: 1000
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        Image {
            id: car
            source: "qrc:/shared/images/car"
            anchors.horizontalCenter: parent.horizontalCenter
            y: 100
            width: 600
            fillMode: Image.PreserveAspectFit
            smooth: true
            mipmap: true
        }
        Text {
            id: battery
            anchors.right: parent.right
            anchors.rightMargin: 635
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 18
            //get battery data
            text: "100"
            color: myColor
            font {
                family: "Inter"
                pixelSize: 40
                bold: true
                // italic: true
            }
        }
        Text {
            id: percent
            anchors.right: parent.right
            anchors.rightMargin: 600
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 22
            text: "%"
            color: myColor
            font {
                family: "Inter"
                pixelSize: 30
                italic: true
            }
        }
        Image {
            id: batteryicon
            source: "qrc:/shared/images/battery"
            anchors.left: parent.left
            anchors.leftMargin: 210
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            width: 50
            fillMode: Image.PreserveAspectFit
            smooth: true
            mipmap: true
        }
        ColorOverlay {
            anchors.fill: batteryicon
            source: batteryicon
            color: myColor
            smooth: true
        }
        Text {
            id: speed
            anchors.left: parent.left
            anchors.leftMargin: 200
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 65
            //get speed data
            text: "100"
            color: myColor
            font {
                family: "Inter"
                pixelSize: 50
                bold: true
                italic: true
            }
        }
        Text {
            id: cms
            anchors.right: parent.right
            anchors.rightMargin: 600
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 70
            text: "cm/s"
            color: myColor
            font {
                family: "Inter"
                pixelSize: 30
                italic: true
            }
        }
    }
    Rectangle{
        id: mediawindow
        width: 250
        height: 100
        color: "transparent"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        anchors.right: parent.right
        anchors.rightMargin: 100
        Slider {
            id: songtime
            to: mediaPlayer.duration
            value: mediaPlayer.position
            onMoved: mediaPlayer.setPosition(value)
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 55

            background: Rectangle {
                x: songtime.leftPadding
                y: songtime.topPadding + songtime.availableHeight / 2 - height / 2
                implicitWidth: 250
                implicitHeight: 4
                width: songtime.availableWidth
                height: implicitHeight
                radius: 2
                color: "grey"

                Rectangle {
                    width: songtime.visualPosition * parent.width
                    height: parent.height
                    color: myColor
                    radius: 2
                }
            }

            handle: Rectangle {
                x: songtime.leftPadding + songtime.visualPosition * (songtime.availableWidth - width)
                y: songtime.topPadding + songtime.availableHeight / 2 - height / 2
                implicitWidth: 15
                implicitHeight: 15
                radius: 13
                color: songtime.pressed ? myColor : "black"
                border.color: myColor
            }
        }
        Rectangle{
            width:250
            height: 50
            color:"transparent"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            Image {
                id: backward
                property bool isFirstImage: true
                height: 50
                fillMode: Image.PreserveAspectFit
                source: "qrc:/shared/images/backward"
                smooth: true
                mipmap: true
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("backward");
                    }
                }
            }
            Image {
                id: play
                anchors.horizontalCenter: parent.horizontalCenter
                property bool isFirstImage: true
                height: 50
                fillMode: Image.PreserveAspectFit
                source: mediaPlayer.playbackState === MediaPlayer.PlayingState ?  "qrc:/shared/images/pause" : "qrc:/shared/images/play"
                smooth: true
                mipmap: true
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (mediaPlayer.playbackState === MediaPlayer.PlayingState)
                            mediaPlayer.pause()
                        else
                            mediaPlayer.play()
                    }
                }
            }
            Image {
                id: forward
                anchors.right:parent.right
                property bool isFirstImage: true
                height: 50
                fillMode: Image.PreserveAspectFit
                source: "qrc:/shared/images/forward"
                smooth: true
                mipmap: true
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("forward");
                    }
                }
            }
            ColorOverlay {
                anchors.fill: backward
                source: backward
                color: myColor
                smooth: true
            }
            ColorOverlay {
                anchors.fill: play
                source: play
                color: myColor
                smooth: true
            }
            ColorOverlay {
                anchors.fill: forward
                source: forward
                color: myColor
                smooth: true
            }
        }
    }
}
