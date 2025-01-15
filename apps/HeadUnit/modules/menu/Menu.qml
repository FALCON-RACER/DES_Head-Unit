import QtQuick 2.15
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Controls 2.12
import QtMultimedia

Item {
    id: menu
    Rectangle{
        width: parent.width
        height: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        color: "transparent"

        GridLayout {
            anchors.centerIn: parent
            columns: 3
            columnSpacing: 20
            rowSpacing: 20

            // Home
            Rectangle {
                width: 100
                height: 100
                color: "transparent"

                Image {
                    id: homeicon
                    source: "qrc:/shared/images/home"
                    anchors.left: parent.left
                    width: 100
                    height: 100
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    mipmap: true

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("Home button clicked")
                            stackView.pop()
                            stackView.push(home)
                        }
                    }

                    ColorOverlay {
                        anchors.fill: homeicon
                        source: homeicon
                        color: myColor
                        smooth: true
                    }
                }
            }

            // Settings
            Rectangle {
                width: 100
                height: 100
                color: "transparent"

                Image {
                    id: settingsicon
                    source: "qrc:/shared/images/settings"
                    width: 100
                    height: 100
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    mipmap: true

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("Settings button clicked")
                            stackView.pop()
                            stackView.push(settings)
                        }
                    }

                    ColorOverlay {
                        anchors.fill: settingsicon
                        source: settingsicon
                        color: myColor
                        smooth: true
                    }
                }
            }

            // Media
            Rectangle {
                width: 100
                height: 100
                color: "transparent"

                Image {
                    id: mediaicon
                    source: "qrc:/shared/images/media"
                    width: 100
                    height: 100
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    mipmap: true

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("Media button clicked")
                            stackView.pop()
                            stackView.push(media)
                        }
                    }

                    ColorOverlay {
                        anchors.fill: mediaicon
                        source: mediaicon
                        color: myColor
                        smooth: true
                    }
                }
            }

            // Spotify
            Rectangle {
                width: 100
                height: 100
                color: "transparent"

                Image {
                    source: "qrc:/icons/spotify"
                    width: 130
                    height: 130
                    anchors.centerIn: parent
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("Spotify clicked")
                            stackView.pop()
                            stackView.push("qrc:qml/spotify/player")
                        }
                    }
                }
            }

