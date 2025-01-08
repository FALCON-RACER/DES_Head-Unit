import QtQuick 2.15
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Controls 2.12
import QtMultimedia

Item{
    id:media
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
                stackView.push(menu)
            }
        }
    }
    ColorOverlay {
        anchors.fill: menuIcon
        source: menuIcon
        color: myColor
        smooth: true
    }
    Rectangle{
        id: mediawindow
        width: 900
        height: 550
        color: "transparent"
        anchors.bottom: parent.bottom
        // anchors.right: parent.right
        anchors.horizontalCenter: parent.horizontalCenter
        Column {
            spacing: 10
            // anchors.centerIn: parent
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 150
            anchors.horizontalCenter: parent.horizontalCenter
            Slider {
                id: songtime
                to: mediaPlayer.duration
                value: mediaPlayer.position
                onMoved: mediaPlayer.setPosition(value)

                background: Rectangle {
                    x: songtime.leftPadding
                    y: songtime.topPadding + songtime.availableHeight / 2 - height / 2
                    implicitWidth: 700
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
                    implicitWidth: 26
                    implicitHeight: 26
                    radius: 13
                    color: songtime.pressed ? myColor : "black"
                    border.color: myColor
                }
            }
            Rectangle{
                width:500
                height: 100
                color:"transparent"
                anchors.horizontalCenter: parent.horizontalCenter
                Image {
                    id: backward
                    property bool isFirstImage: true
                    height: 100
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/shared/images/backward"
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
                    height: 100
                    fillMode: Image.PreserveAspectFit
                    source: mediaPlayer.playbackState === MediaPlayer.PlayingState ?  "qrc:/shared/images/pause" : "qrc:/shared/images/play"
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
                    height: 100
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/shared/images/forward"
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
}
