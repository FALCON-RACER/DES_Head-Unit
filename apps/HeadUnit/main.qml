import QtQuick 2.15
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Controls 2.12
import QtMultimedia

Window {
    id: window
    width: 1024
    height: 600
    visible: true
    property color myColor: "#ffff1a"
    // property color myColor: Qt.rgba(redSlider.value, greenSlider.value, blueSlider.value, 1) //enable when slider settings are on

    property Item previousItem: null

    MediaPlayer {
        id: mediaPlayer
        audioOutput: AudioOutput {id: audioOutput}
        source: "file:/home/frederik/Downloads/geeked up.mp3"
    }

    Rectangle {
        id: backgroundRect
        anchors.fill: parent
        color: "black"
        Image {
            id: backgroundLines
            source: "qrc:/shared/images/background"
            anchors.horizontalCenter: parent.horizontalCenter
            y: 50
            fillMode: Image.PreserveAspectFit
            height: 600
            smooth: true
            mipmap: true
        }
        ColorOverlay {
              anchors.fill: backgroundLines
              source: backgroundLines
              color: myColor
              opacity: 0.5
        }
        Image {
            id: topLine
            source: "qrc:/shared/images/line"
            y: 45
            smooth: true
            mipmap: true
        }
        ColorOverlay {
            anchors.fill: topLine
            source: topLine
            color: myColor
            smooth: true
        }
        Text {
            id: timeDisplay
            property var currentDate: new Date()
            anchors.horizontalCenter: parent.horizontalCenter
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
        Image {
            id: gearButtons
            source: "qrc:/shared/images/gears_background"
            anchors.left: parent.left
            y: 65
            anchors.leftMargin: 5
            fillMode: Image.PreserveAspectFit
            width: 110
            smooth: true
            mipmap: true
            ColumnLayout {
                anchors.centerIn: parent
                spacing:0

                Button {
                    text: "Button 1"
                    Layout.preferredWidth: 110
                    Layout.preferredHeight: 100
                    Layout.alignment: Qt.AlignHCenter
                    opacity: 0
                    onClicked: {
                        console.log("Dclicked");
                        // Your action here
                    }
                }

                Button {
                    text: "Button 2"
                    Layout.preferredWidth: 110
                    Layout.preferredHeight: 100
                    Layout.alignment: Qt.AlignHCenter
                    opacity: 0
                    onClicked: {
                        console.log("Nclicked");
                        // Your action here
                    }
                }

                Button {
                    text: "Button 3"
                    Layout.preferredWidth: 110
                    Layout.preferredHeight: 100
                    Layout.alignment: Qt.AlignHCenter
                    opacity: 0
                    onClicked: {
                        console.log("Rclicked");
                        // Your action here
                    }
                }
                Button {
                    text: "Button 3"
                    Layout.preferredWidth: 110
                    Layout.preferredHeight: 100
                    Layout.alignment: Qt.AlignHCenter
                    opacity: 0
                    onClicked: {
                        console.log("Pclicked");
                        // Your action here
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
        }
        ColorOverlay {
            anchors.fill: gearHighlights
            source: gearHighlights
            color: myColor
            smooth: true
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
    }

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
        Loader {
            id: contentLoader
            anchors.fill: parent
            source: "modules/home/home.qml"
            // onItemChanged: {
            // if (contentLoader.item) {

                // console.log("previousItem:", previousItem);
                // console.log("contentLoader.item:", contentLoader.item);

                // Destroy (or clean up memory) the previous component if it exists.
                // if (contentLoader.item !== previousItem) {

                //     console.log("different");
                //     if (previousItem) {
                //         previousItem.destroy();  // Explicitly destroying the previous component
                //         console.log("Destruction");
                //     }
                //       // Set current item to previous item
                //     previousItem = contentLoader.item;
                // }
            // }
        // }
    }
}

