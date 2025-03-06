import QtQuick 2
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Controls 2
import QtMultimedia
import HeadUnit 1.0

Window {
    id: window
    visible: true
    width: 1024
    height: 600
    title: qsTr("HeadUnit")
    color: "black"

    property int ambientLighting: HeadUnit.ambientLighting
    property color myColor: Qt.hsva(ambientLighting / 360, 1, 1, 1)

    Component.onCompleted: {
        console.log("ambientLighting value:", ambientLighting)
    }

    onAmbientLightingChanged: {
        console.log("ambientLighting changed to:", ambientLighting)
    }
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

    }
    ColorOverlay {
        anchors.fill: backgroundLines
        source: backgroundLines
        color: myColor
        opacity: 0.5
        z:0
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

            }
            ColorOverlay {
                anchors.fill: topLine
                source: topLine
                color: myColor
                smooth: true
                z:1
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
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
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
                            id: gearColumn
                            anchors.centerIn: parent
                            spacing:0
                            property string activeGear: "park"

                            Rectangle {
                                Layout.preferredWidth: 110
                                Layout.preferredHeight: 100
                                Layout.alignment: Qt.AlignHCenter
                                color: "transparent"
                                Image {
                                    id: drive
                                    height: 40
                                    x: 40
                                    y: 15
                                    source: "qrc:/shared/images/drive"
                                    fillMode: Image.PreserveAspectFit
                                    smooth: true
                                    mipmap: true
                                    opacity: 0
                                }
                                ColorOverlay {
                                    anchors.fill: drive
                                    source: drive
                                    color: gearColumn.activeGear === "drive" ? myColor : Qt.darker(myColor, 2.5)
                                    smooth: true
                                }
                                //Glowing Effect
                                Glow {
                                    id: glowEffectD
                                    anchors.fill: drive
                                    source: drive
                                    color: myColor.lighter(1.1) // Slightly lighter color for a subtle glow
                                    radius: 0 // Initially no glow
                                    spread: 0 // Initially no spread
                                    samples: 41 // High-quality blur for smooth edges
                                    cached: true
                                    opacity: 0 // Initially hidden
                                }
                                states: [
                                    State {
                                        name: "pressed"
                                        when: mouseAreaD.pressed
                                        PropertyChanges { target: glowEffectD; radius: 16; spread: 0.6; opacity: 0.5; visible: true }
                                    },
                                    State {
                                        name: "default"
                                        when: !mouseAreaD.containsMouse && !mouseAreaD.pressed
                                        PropertyChanges { target: glowEffectD; radius: 0; spread: 0; opacity: 0; visible: false }
                                    }
                                ]
                                Connections {
                                    target: gearClient
                                    onGearValueChanged: {
                                        if (newGearValue == 3) {
                                            glowEffectD.radius = 16;
                                            glowEffectD.spread = 0.6;
                                            glowEffectD.opacity = 0.5;
                                            glowEffectD.visible = true;
                                        } else {
                                            glowEffectD.radius = 0;
                                            glowEffectD.spread = 0;
                                            glowEffectD.opacity = 0;
                                            glowEffectD.visible = false;
                                        }
                                    }
                                }

                                transitions: [
                                    Transition {
                                        NumberAnimation {
                                            properties: "radius, spread, opacity";
                                            duration: 100; // Slower transition for smoother effect
                                            easing.type: Easing.OutCubic
                                        }
                                    }
                                ]
                                MouseArea {
                                    id: mouseAreaD
                                    anchors.fill: parent
                                    onClicked: {
                                        console.log("D clicked")
                                        gearColumn.activeGear = "drive"
                                        someIP.send_gear_data(3);
                                    }
                                }
                            }

                            Rectangle {
                                Layout.preferredWidth: 110
                                Layout.preferredHeight: 100
                                Layout.alignment: Qt.AlignHCenter
                                color: "transparent"
                                Image {
                                    id: neutral
                                    height: 40
                                    x: 38
                                    y: 22
                                    source: "qrc:/shared/images/neutral"
                                    fillMode: Image.PreserveAspectFit
                                    smooth: true
                                    mipmap: true
                                    opacity: 0
                                }
                                ColorOverlay {
                                    anchors.fill: neutral
                                    source: neutral
                                    color: gearColumn.activeGear === "neutral" ? myColor : Qt.darker(myColor, 2.5)
                                    smooth: true
                                }
                                //Glowing Effect
                                Glow {
                                    id: glowEffectN
                                    anchors.fill: neutral
                                    source: neutral
                                    color: myColor.lighter(1.1) // Slightly lighter color for a subtle glow
                                    radius: 0 // Initially no glow
                                    spread: 0 // Initially no spread
                                    samples: 41 // High-quality blur for smooth edges
                                    cached: true
                                    opacity: 0 // Initially hidden
                                }
                                states: [
                                    State {
                                        name: "pressed"
                                        when: mouseAreaN.pressed
                                        PropertyChanges { target: glowEffectN; radius: 16; spread: 0.6; opacity: 0.5; visible: true }
                                    },
                                    State {
                                        name: "default"
                                        when: !mouseAreaN.containsMouse && !mouseAreaN.pressed
                                        PropertyChanges { target: glowEffectN; radius: 0; spread: 0; opacity: 0; visible: false }
                                    }
                                ]
                                Connections {
                                    target: gearClient
                                    onGearValueChanged: {
                                        if (newGearValue == 2) {
                                            glowEffectN.radius = 16;
                                            glowEffectN.spread = 0.6;
                                            glowEffectN.opacity = 0.5;
                                            glowEffectN.visible = true;
                                        } else {
                                            glowEffectN.radius = 0;
                                            glowEffectN.spread = 0;
                                            glowEffectN.opacity = 0;
                                            glowEffectN.visible = false;
                                        }
                                    }
                                }
                                transitions: [
                                    Transition {
                                        NumberAnimation {
                                            properties: "radius, spread, opacity";
                                            duration: 100; // Slower transition for smoother effect
                                            easing.type: Easing.OutCubic
                                        }
                                    }
                                ]
                                MouseArea {
                                    id: mouseAreaN
                                    anchors.fill: parent
                                    onClicked: {
                                        console.log("N clicked")
                                        gearColumn.activeGear = "neutral"
                                        someIP.send_gear_data(2);
                                    }
                                }
                            }

                            Rectangle {
                                Layout.preferredWidth: 110
                                Layout.preferredHeight: 100
                                Layout.alignment: Qt.AlignHCenter
                                color: "transparent"
                                Image {
                                    id: reverse
                                    height: 40
                                    x: 40
                                    y: 35
                                    source: "qrc:/shared/images/reverse"
                                    fillMode: Image.PreserveAspectFit
                                    smooth: true
                                    mipmap: true
                                    opacity: 0
                                }
                                ColorOverlay {
                                    anchors.fill: reverse
                                    source: reverse
                                    color: gearColumn.activeGear === "reverse" ? myColor : Qt.darker(myColor, 2.5)
                                    smooth: true
                                }
                                //Glowing Effect
                                Glow {
                                    id: glowEffectR
                                    anchors.fill: reverse
                                    source: reverse
                                    color: myColor.lighter(1.1) // Slightly lighter color for a subtle glow
                                    radius: 0 // Initially no glow
                                    spread: 0 // Initially no spread
                                    samples: 41 // High-quality blur for smooth edges
                                    cached: true
                                    opacity: 0 // Initially hidden
                                }
                                states: [
                                    State {
                                        name: "pressed"
                                        when: mouseAreaR.pressed
                                        PropertyChanges { target: glowEffectR; radius: 16; spread: 0.6; opacity: 0.5; visible: true }
                                    },
                                    State {
                                        name: "default"
                                        when: !mouseAreaR.containsMouse && !mouseAreaR.pressed
                                        PropertyChanges { target: glowEffectR; radius: 0; spread: 0; opacity: 0; visible: false }
                                    }
                                ]
                                Connections {
                                    target: gearClient
                                    onGearValueChanged: {
                                        if (newGearValue == 1) {
                                            glowEffectR.radius = 16;
                                            glowEffectR.spread = 0.6;
                                            glowEffectR.opacity = 0.5;
                                            glowEffectR.visible = true;
                                        } else {
                                            glowEffectR.radius = 0;
                                            glowEffectR.spread = 0;
                                            glowEffectR.opacity = 0;
                                            glowEffectR.visible = false;
                                        }
                                    }
                                }
                                transitions: [
                                    Transition {
                                        NumberAnimation {
                                            properties: "radius, spread, opacity";
                                            duration: 100; // Slower transition for smoother effect
                                            easing.type: Easing.OutCubic
                                        }
                                    }
                                ]
                                MouseArea {
                                    id: mouseAreaR
                                    anchors.fill: parent
                                    onClicked: {
                                        console.log("R clicked")
                                        gearColumn.activeGear = "reverse"
                                        someIP.send_gear_data(1);
                                    }
                                }
                            }
                            Rectangle {
                                Layout.preferredWidth: 110
                                Layout.preferredHeight: 100
                                Layout.alignment: Qt.AlignHCenter
                                color: "transparent"
                                Image {
                                    id: park
                                    height: 40
                                    x: 40
                                    y: 43
                                    source: "qrc:/shared/images/park"
                                    fillMode: Image.PreserveAspectFit
                                    smooth: true
                                    mipmap: true
                                    opacity: 0
                                }
                                ColorOverlay {
                                    anchors.fill: park
                                    source: park
                                    color: gearColumn.activeGear === "park" ? myColor : Qt.darker(myColor, 2.5)
                                    smooth: true
                                }
                                //Glowing Effect
                                Glow {
                                    id: glowEffectP
                                    anchors.fill: park
                                    source: park
                                    color: myColor.lighter(1.1) // Slightly lighter color for a subtle glow
                                    radius: 0 // Initially no glow
                                    spread: 0 // Initially no spread
                                    samples: 41 // High-quality blur for smooth edges
                                    cached: true
                                    opacity: 0 // Initially hidden
                                }
                                states: [
                                    State {
                                        name: "pressed"
                                        when: mouseAreaP.pressed
                                        PropertyChanges { target: glowEffectP; radius: 16; spread: 0.6; opacity: 0.5; visible: true }
                                    },
                                    State {
                                        name: "default"
                                        when: !mouseAreaP.containsMouse && !mouseAreaP.pressed
                                        PropertyChanges { target: glowEffectP; radius: 0; spread: 0; opacity: 0; visible: false }
                                    }
                                ]
                                Connections {
                                    target: gearClient
                                    onGearValueChanged: {
                                        if (newGearValue == 0) {
                                            glowEffectP.radius = 16;
                                            glowEffectP.spread = 0.6;
                                            glowEffectP.opacity = 0.5;
                                            glowEffectP.visible = true;
                                        } else {
                                            glowEffectP.radius = 0;
                                            glowEffectP.spread = 0;
                                            glowEffectP.opacity = 0;
                                            glowEffectP.visible = false;
                                        }
                                    }
                                }
                                transitions: [
                                    Transition {
                                        NumberAnimation {
                                            properties: "radius, spread, opacity";
                                            duration: 100; // Slower transition for smoother effect
                                            easing.type: Easing.OutCubic
                                        }
                                    }
                                ]
                                MouseArea {
                                    id: mouseAreaP
                                    anchors.fill: parent
                                    onClicked: {
                                        console.log("P clicked")
                                        gearColumn.activeGear = "park"
                                        someIP.send_gear_data(0);
                                    }
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
                        opacity: 0
                    }
                    ColorOverlay {
                        anchors.fill: menuIcon
                        source: menuIcon
                        color: myColor
                        smooth: true
                    }
                    Glow {
                        id: glowEffect1
                        anchors.fill: menuIcon
                        source: menuIcon
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
                                PropertyChanges { target: glowEffect1; radius: 15; spread: 0.5; opacity: 0.5; visible: true }
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
                            console.log("menu clicked")
                            stackView.pop()
                            stackView.push(menu)
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
                    popExit: Transition {
                        ParallelAnimation {
                            PropertyAnimation { property: "opacity"; from: 1; to: 0; duration: 100; easing.type: Easing.InOutQuad }
                            PropertyAnimation { property: "scale"; from: 1; to: 0.8; duration: 300; easing.type: Easing.InBack }
                        }
                    }
                }
            }
        }
    }
    ColumnLayout{
        x: 25
        y: 165
        spacing: 110
        opacity: 0.7
        Rectangle{
            width:70
            height: 2
            color: myColor
        }
        Rectangle{
            width: 70
            height: 2
            color: myColor
        }
        Rectangle{
            width: 70
            height: 2
            color: myColor
        }
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

    Component{
        id:home
        Home {
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
                ambientLighting = value
                // Use the value here
            }
        }
    }
}
