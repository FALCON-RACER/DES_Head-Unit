import QtQuick 2.15
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Controls 2.12
import QtMultimedia

Item{
    id:home
    x: 0
    y: 0
    clip: false

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        Image {
            id: car
            source: "qrc:/shared/images/car"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenterOffset: -45
            anchors.verticalCenterOffset: -30
            width: 600
            fillMode: Image.PreserveAspectFit
            smooth: true
            mipmap: true
        }
    }

    Item {
        width: parent.width * 0.3
        height: parent.height * 0.1
        anchors.bottom:parent.bottom
        anchors.bottomMargin: 70
        anchors.left: parent.left
        anchors.leftMargin: 55

        ColumnLayout {
            anchors.fill: parent
            spacing: -10

            // Speed Data
            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter

                Text {
                    id: speed
                    text: HeadUnit.speed
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
                    text: "cm/s"
                    color: myColor
                    font {
                        family: "Inter"
                        pixelSize: 30
                        italic: true
                    }
                }
            }

            // Battery Data
            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter

                Item {
                    Layout.preferredWidth: 40
                    Layout.preferredHeight: 60
                    Layout.rightMargin: 10

                    Image {
                        id: batteryicon
                        source: "qrc:/shared/images/battery"
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        mipmap: true
                    }
                    ColorOverlay {
                        anchors.fill: parent
                        source: batteryicon
                        color: myColor
                    }
                }

                Text {
                    id: battery
                    text: batteryClient.getBatteryValue() + ""
                    color: myColor
                    Layout.rightMargin: -5
                    font {
                        family: "Inter"
                        pixelSize: 40
                        bold: true
                    }
                    Layout.alignment: Qt.AlignVCenter
                }

                Text {
                    id: percent
                    text: "%"
                    color: myColor
                    font {
                        family: "Inter"
                        pixelSize: 30
                        italic: true
                    }
                    Layout.alignment: Qt.AlignVCenter
                }
            }
        }
    }

    Rectangle{
        id: mediawindow
        width: 250
        height: 100
        color: "transparent"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 40
        anchors.right: parent.right
        anchors.rightMargin: 90
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
            ColorOverlay {
                anchors.fill: backward
                source: backward
                color: myColor
                smooth: true
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
            ColorOverlay {
                anchors.fill: play
                source: play
                color: myColor
                smooth: true
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
                anchors.fill: forward
                source: forward
                color: myColor
                smooth: true
            }
        }
    }
}
