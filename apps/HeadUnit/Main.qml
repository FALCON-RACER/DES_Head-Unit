import QtQuick 2.15
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Controls 2.12
import QtMultimedia
import HeadUnit 1.0

Window {
    id: window
    width: 1024
    height: 600
    visible: true
    color: "black"

    property int colorValue: 180
    property color myColor: Qt.hsva(colorValue / 360, 1, 1, 1)

    MediaPlayer {
        id: mediaPlayer
        audioOutput: AudioOutput {id: audioOutput}
        source: "file:/home/frederik/Downloads/Benz pt. 2.mp3"
    }

    Image {
        id: backgroundLines
        source: "qrc:/shared/images/background"
        anchors.horizontalCenter: parent.horizontalCenter
        y: 45
        fillMode: Image.PreserveAspectFit
        height: 600
        smooth: true
        mipmap: true
        z:0

        ColorOverlay {
            anchors.fill: backgroundLines
            source: backgroundLines
            color: myColor
            opacity: 0.5
            z:0
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        // Top Bar
        Rectangle {
            id: topBar
            Layout.fillWidth: true
            height: window.height * 0.075
            color: "transparent"


            Image {
                id: topLine
                source: "qrc:/shared/images/line"
                anchors.bottom: parent.bottom
                smooth: true
                mipmap: true
                z:1

                ColorOverlay {
                    anchors.fill: topLine
                    source: topLine
                    color: myColor
                    smooth: true
                    z:1
                }
            }

            // Time
            Text {
                id: timeDisplay
                property var currentDate: new Date()
                anchors.centerIn: parent
                text: Qt.formatDateTime(currentDate, "hh:mm")
                color: myColor
                y:5

                font {
                    family: "Inter"
                    pixelSize: 30
                }

                Timer {
                    interval: 1000
                    running: true
                    repeat: true
                    onTriggered: timeDisplay.currentDate = new Date()
                }
            }
        }


        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            z: 0

            ColumnLayout {
                anchors.fill: parent
                z:0
                spacing: 0

                Rectangle {
                    id: gear
                    width: window.width * 0.11
                    height: width * (gearButtons.height / gearButtons.width)
                    Layout.leftMargin: 5
                    Layout.topMargin: 7
                    z: 1
                    color: "transparent"

                    Image {
                        id: gearButtons
                        source: "qrc:/shared/images/gears_background"
                        width: parent.width
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        mipmap: true
                        z:1

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing:0

                            Button {
                                Layout.preferredWidth: 110
                                Layout.preferredHeight: 100
                                Layout.alignment: Qt.AlignHCenter
                                opacity: 0
                                onClicked: {
                                    console.log("D clicked");
                                    // Your action here
                                }
                            }

                            Button {
                                Layout.preferredWidth: 110
                                Layout.preferredHeight: 100
                                Layout.alignment: Qt.AlignHCenter
                                opacity: 0
                                onClicked: {
                                    console.log("N clicked");
                                    // Your action here
                                }
                            }

                            Button {
                                Layout.preferredWidth: 110
                                Layout.preferredHeight: 100
                                Layout.alignment: Qt.AlignHCenter
                                opacity: 0
                                onClicked: {
                                    console.log("R clicked");
                                    // Your action here
                                }
                            }
                            Button {
                                Layout.preferredWidth: 110
                                Layout.preferredHeight: 100
                                Layout.alignment: Qt.AlignHCenter
                                opacity: 0
                                onClicked: {
                                    console.log("P clicked");
                                    // Your action here
                                }
                            }
                        }
                    }
               }

                // Menu button
                Rectangle {
                    width: window.width * 0.11
                    height: width * (menuIcon.height / menuIcon.width)
                    Layout.leftMargin: 5
                    Layout.topMargin: -5
                    color: "transparent"

                    Image {
                        id: menuIcon
                        source: "qrc:/shared/images/menu"
                        fillMode: Image.PreserveAspectFit
                        width: 70
                        anchors.centerIn: parent
                        smooth: true
                        mipmap: true

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                stackView.pop()
                                stackView.push(menu)
                            }
                        }

                        ColorOverlay {
                            anchors.fill: menuIcon
                            source: menuIcon
                            color: myColor
                            smooth: true
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                Layout.bottomMargin: 20
                color: "transparent"

                StackView {
                    id: stackView
                    initialItem: home
                    anchors.fill: parent

                    pushEnter: Transition {
                        ParallelAnimation {
                            PropertyAnimation { property: "opacity"; from: 0; to: 1; duration: 300; easing.type: Easing.InOutQuad }
                            PropertyAnimation { property: "scale"; from: 1.2; to: 1; duration: 300; easing.type: Easing.OutBack }
                        }
                    }

                    pushExit: Transition {
                        ParallelAnimation {
                            PropertyAnimation { property: "opacity"; from: 1; to: 0; duration: 100; easing.type: Easing.InOutQuad }
                            PropertyAnimation { property: "scale"; from: 1; to: 0.8; duration: 300; easing.type: Easing.InBack }
                        }
                    }
                }
            }

        }
    }

    Image {
        id: gearHighlights
        source: "qrc:/shared/images/gear_letters"
        anchors.left: parent.left
        y: 65
        anchors.leftMargin: 5
        fillMode: Image.PreserveAspectFit
        width: 110
        smooth: true
        mipmap: true
        z:2
    }
    Slider {
        id: volumeSlider
        anchors.right: parent.right
        anchors.rightMargin: 100
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        width: 250
        from: 0.0
        to: 1.0
        value: 1.0
        onValueChanged: audioOutput.volume = value
        background: Rectangle {
            x: volumeSlider.leftPadding
            y: volumeSlider.topPadding + volumeSlider.availableHeight / 2 - height / 2
            implicitWidth: 250
            implicitHeight: 4
            width: volumeSlider.availableWidth
            height: implicitHeight
            radius: 2
            color: "grey"

            Rectangle {
                width: volumeSlider.visualPosition * parent.width
                height: parent.height
                color: myColor
                radius: 2
            }
        }

        handle: Rectangle {
            x: volumeSlider.leftPadding + volumeSlider.visualPosition * (volumeSlider.availableWidth - width)
            y: volumeSlider.topPadding + volumeSlider.availableHeight / 2 - height / 2
            implicitWidth: 26
            implicitHeight: 26
            radius: 13
            color: volumeSlider.pressed ? myColor : "black"
            border.color: myColor
        }
    }
    ColorOverlay {
        anchors.fill: backgroundLines
        source: backgroundLines
        color: myColor
        opacity: 0.5
        z:0
    }
    ColorOverlay {
        anchors.fill: gearHighlights
        source: gearHighlights
        color: myColor
        smooth: true
        z:2
    }
    ColorOverlay {
        anchors.fill: topLine
        source: topLine
        color: myColor
        smooth: true
        z:1
    }
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: home
        pushEnter: Transition {
            ParallelAnimation {
                PropertyAnimation { property: "opacity"; from: 0; to: 1; duration: 300; easing.type: Easing.InOutQuad }
                PropertyAnimation { property: "scale"; from: 1.2; to: 1; duration: 300; easing.type: Easing.OutBack }
            }
        }

        pushExit: Transition {
            ParallelAnimation {
                PropertyAnimation { property: "opacity"; from: 1; to: 0; duration: 100; easing.type: Easing.InOutQuad }
                PropertyAnimation { property: "scale"; from: 1; to: 0.8; duration: 300; easing.type: Easing.InBack }
            }
        }
    }
    Component{
        id:home
        Home {
        // anchors.centerIn: parent.centerIn
        }
    }
    Component{
        id:menu
        Menu{
        }
    }
    Component{
        id:media
        Media{
        }
    }
    Component{
        id: settings
        Settings {
            onColorValueChanged: (value) => {
                colorValue = value
                // Use the value here
            }
        }
    }

    // Loader {
    //     id: contentLoader
    //     anchors.fill: parent
    //     source: "modules/settings/settings.qml"
    // }
    /*
    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        // Top Bar
        Rectangle {
            Layout.fillWidth: true
            height: Screen.height * 0.03
            color: "lightblue"

            // todo display temperature, time , date
            Text {
                anchors.centerIn: parent
                text: "Header Area"
                font.pixelSize: 24
            }
        }


        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            z: 0

            // Gear
            ColumnLayout {
                width: Screen.width * 0.05
                Layout.fillHeight: true
                Layout.margins: 10
                z: 1


                // Parking Mode
                Rectangle {
                   Layout.fillHeight: true
                   Layout.preferredWidth: parent.width
                   color: "blue"
                   radius: 10
                   border.color: "black"
                   border.width: 2

                   MouseArea {
                       anchors.fill: parent
                       onClicked: {
                           console.log("Parking mode clicked")
                       }
                       onPressed: parent.color = "lightblue"
                       onReleased: parent.color = "blue"
                   }

                   Text {
                       anchors.centerIn: parent
                       text: "P"
                       color: "white"
                       font.pixelSize: 18
                   }
                }

                // Reverse Mode
                Rectangle {
                    Layout.fillHeight: true
                    Layout.preferredWidth: parent.width
                   color: "green"
                   radius: 10
                   border.color: "black"
                   border.width: 2

                   MouseArea {
                       anchors.fill: parent
                       onClicked: {
                           console.log("Reverse mode clicked")
                       }

                       onPressed: parent.color = "lightgreen"
                       onReleased: parent.color = "green"
                   }

                   Text {
                       anchors.centerIn: parent
                       text: "R"
                       color: "white"
                       font.pixelSize: 18
                   }
                }

                // Neutral Mode
                Rectangle {
                    Layout.fillHeight: true
                    Layout.preferredWidth: parent.width
                   color: "red"
                   radius: 10
                   border.color: "black"
                   border.width: 2

                   MouseArea {
                       anchors.fill: parent
                       onClicked: {
                           console.log("Neutral mode clicked")
                       }

                       onPressed: parent.color = "lightcoral"
                       onReleased: parent.color = "red"
                   }

                   Text {
                       anchors.centerIn: parent
                       text: "N"
                       color: "white"
                       font.pixelSize: 18
                   }
                }

                // Drive Mode
                Rectangle {
                    Layout.fillHeight: true
                    Layout.preferredWidth: parent.width
                   color: "red"
                   radius: 10
                   border.color: "black"
                   border.width: 2

                   MouseArea {
                       anchors.fill: parent
                       onClicked: {
                           console.log("Drive mode clicked")
                       }

                       onPressed: parent.color = "lightcoral"
                       onReleased: parent.color = "red"
                   }

                   Text {
                       anchors.centerIn: parent
                       text: "D"
                       color: "white"
                       font.pixelSize: 18
                   }
                }

                // Menu
                Rectangle {
                    Layout.preferredWidth: parent.width
                    Layout.fillHeight: true
                    color: "transparent"

                    Image {
                        source: "qrc:/icons/menu"
                        width: 50
                        height: 50
                        anchors.centerIn: parent

                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {

                            contentLoader.source = "modules/menu/menu.qml"

                            console.log("menu clicked")
                        }
                    }
                }
            }

            // Contents Area
                }
            }
    */
}

