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
                width: 120
                height: 120
                color: "transparent"

                Image {
                    id: homeicon
                    source: "qrc:/shared/images/home"
                    anchors.left: parent.left
                    width: 120
                    height: 120
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    mipmap: true

                }
                ColorOverlay {
                    anchors.fill: homeicon
                    source: homeicon
                    color: myColor
                    smooth: true
                }
                Glow {
                    id: glowEffect1
                    anchors.fill: homeicon
                    source: homeicon
                    color: myColor.lighter(1.1) // Slightly lighter color for a subtle glow
                    radius: 0 // Initially no glow
                    spread: 0 // Initially no spread
                    samples: 41 // High-quality blur for smooth edges
                    cached: true
                    opacity: 0 // Initially hidden
                    smooth: true
                    states: [
                        State {
                            name: "pressed"
                            when: mouseArea1.pressed
                            PropertyChanges { target: glowEffect1; radius: 16; spread: 0.6; opacity: 0.5; visible: true }
                        },
                        State {
                            name: "default"
                            when: !mouseArea1.containsMouse && !mouseArea1.pressed
                            PropertyChanges { target: glowEffect1; radius: 0; spread: 0; opacity: 0; visible: false }
                        }
                    ]
                    transitions: [
                        Transition {
                            NumberAnimation {
                                properties: "radius, spread, opacity";
                                duration: 50; // Slower transition for smoother effect
                                easing.type: Easing.OutCubic
                            }
                        }
                    ]
                }

                MouseArea {
                    id: mouseArea1
                    anchors.fill: parent
                    onClicked: {
                        console.log("Home clicked")
                        stackView.pop()
                        stackView.push(home)
                    }
                }
            }

            // Settings
            Rectangle {
                width: 120
                height: 120
                color: "transparent"

                Image {
                    id: settingsicon
                    source: "qrc:/shared/images/settings"
                    width: 120
                    height: 120
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    mipmap: true
                }
                ColorOverlay {
                    anchors.fill: settingsicon
                    source: settingsicon
                    color: myColor
                    smooth: true
                }
                Glow {
                    id: glowEffect2
                    anchors.fill: settingsicon
                    source: settingsicon
                    color: myColor.lighter(1.1) // Slightly lighter color for a subtle glow
                    radius: 0 // Initially no glow
                    spread: 0 // Initially no spread
                    samples: 41 // High-quality blur for smooth edges
                    cached: true
                    opacity: 0 // Initially hidden
                    smooth: true
                    states: [
                        State {
                            name: "pressed"
                            when: mouseArea2.pressed
                            PropertyChanges { target: glowEffect2; radius: 16; spread: 0.6; opacity: 0.5; visible: true }
                        },
                        State {
                            name: "default"
                            when: !mouseArea2.containsMouse && !mouseArea2.pressed
                            PropertyChanges { target: glowEffect2; radius: 0; spread: 0; opacity: 0; visible: false }
                        }
                    ]
                    transitions: [
                        Transition {
                            NumberAnimation {
                                properties: "radius, spread, opacity";
                                duration: 50; // Slower transition for smoother effect
                                easing.type: Easing.OutCubic
                            }
                        }
                    ]
                }

                MouseArea {
                    id: mouseArea2
                    anchors.fill: parent
                    onClicked: {
                        console.log("Settings button clicked")
                        stackView.pop()
                        stackView.push(settings)
                    }
                }
            }

            // Media
            Rectangle {
                width: 120
                height: 120
                color: "transparent"

                Image {
                    id: mediaicon
                    source: "qrc:/shared/images/media"
                    width: 120
                    height: 120
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    mipmap: true
                }
                ColorOverlay {
                    anchors.fill: mediaicon
                    source: mediaicon
                    color: myColor
                    smooth: true
                }
                Glow {
                    id: glowEffect3
                    anchors.fill: mediaicon
                    source: mediaicon
                    color: myColor.lighter(1.1) // Slightly lighter color for a subtle glow
                    radius: 0 // Initially no glow
                    spread: 0 // Initially no spread
                    samples: 41 // High-quality blur for smooth edges
                    cached: true
                    opacity: 0 // Initially hidden
                    smooth: true
                    states: [
                        State {
                            name: "pressed"
                            when: mouseArea3.pressed
                            PropertyChanges { target: glowEffect3; radius: 16; spread: 0.6; opacity: 0.5; visible: true }
                        },
                        State {
                            name: "default"
                            when: !mouseArea3.containsMouse && !mouseArea3.pressed
                            PropertyChanges { target: glowEffect3; radius: 0; spread: 0; opacity: 0; visible: false }
                        }
                    ]
                    transitions: [
                        Transition {
                            NumberAnimation {
                                properties: "radius, spread, opacity";
                                duration: 50; // Slower transition for smoother effect
                                easing.type: Easing.OutCubic
                            }
                        }
                    ]
                }

                MouseArea {
                    id: mouseArea3
                    anchors.fill: parent
                    onClicked: {
                        console.log("Media clicked")
                        stackView.pop()
                        stackView.push(media)
                    }
                }
            }

            // Spotify
            Rectangle {
                width: 120
                height: 120
                color: "transparent"

                Image {
                    id: spotifyicon
                    source: "qrc:/icons/spotify"
                    width: 120
                    height: 120
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    mipmap: true
                    }
                Glow {
                    id: glowEffect4
                    anchors.fill: spotifyicon
                    source: spotifyicon
                    color: myColor.lighter(1.1) // Slightly lighter color for a subtle glow
                    radius: 0 // Initially no glow
                    spread: 0 // Initially no spread
                    samples: 41 // High-quality blur for smooth edges
                    cached: true
                    opacity: 0 // Initially hidden
                    smooth: true
                    states: [
                        State {
                            name: "pressed"
                            when: mouseArea4.pressed
                            PropertyChanges { target: glowEffect4; radius: 16; spread: 0.6; opacity: 0.5; visible: true }
                        },
                        State {
                            name: "default"
                            when: !mouseArea4.containsMouse && !mouseArea4.pressed
                            PropertyChanges { target: glowEffect4; radius: 0; spread: 0; opacity: 0; visible: false }
                        }
                    ]
                    transitions: [
                        Transition {
                            NumberAnimation {
                                properties: "radius, spread, opacity";
                                duration: 50; // Slower transition for smoother effect
                                easing.type: Easing.OutCubic
                            }
                        }
                    ]
                }
                MouseArea {
                    id: mouseArea4
                    anchors.fill: parent
                    onClicked: {
                        console.log("Spotify clicked")
                        stackView.pop()
                        stackView.push("qrc:qml/spotify/player")
                    }
                }
            }

            // Youtube
            Rectangle {
                width: 120
                height: 120
                color: "transparent"
                radius: 10
                Image {
                    id: yticon
                    source: "qrc:/icons/youtube"
                    width: 100
                    height: 100
                    anchors.centerIn: parent
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    mipmap: true
                    }
                Glow {
                    id: glowEffect5
                    anchors.fill: yticon
                    source: yticon
                    color: myColor.lighter(1.1) // Slightly lighter color for a subtle glow
                    radius: 0 // Initially no glow
                    spread: 0 // Initially no spread
                    samples: 41 // High-quality blur for smooth edges
                    cached: true
                    opacity: 0 // Initially hidden
                    smooth: true
                    states: [
                        State {
                            name: "pressed"
                            when: mouseArea5.pressed
                            PropertyChanges { target: glowEffect5; radius: 16; spread: 0.6; opacity: 0.5; visible: true }
                        },
                        State {
                            name: "default"
                            when: !mouseArea5.containsMouse && !mouseArea5.pressed
                            PropertyChanges { target: glowEffect5; radius: 0; spread: 0; opacity: 0; visible: false }
                        }
                    ]
                    transitions: [
                        Transition {
                            NumberAnimation {
                                properties: "radius, spread, opacity";
                                duration: 50; // Slower transition for smoother effect
                                easing.type: Easing.OutCubic
                            }
                        }
                    ]
                }
                MouseArea {
                    id:mouseArea5
                    anchors.fill: parent
                    onClicked: {
                        console.log("Youtube clicked")
                        stackView.pop()
                        stackView.push("qrc:qml/youtube/main")
                    }
                }
            }
        }
    }
}
