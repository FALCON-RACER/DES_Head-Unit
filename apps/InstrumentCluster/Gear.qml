import QtQuick 2.15

Item {
    id: gearIndicator

    property string currentGear: "P"
    property var gearStates: ["P", "R", "N", "D"]

    width: 290
    height: 70
    clip: true

    Repeater {
        model: gearStates
        delegate: Text {
            text: modelData
            font.pixelSize: modelData === currentGear ? 70 : 50
            font.bold: true
            color: "white"
            anchors.verticalCenter: parent.verticalCenter

            x: gearIndicator.width / 2 - font.pixelSize / 4 + (index - gearStates.indexOf(currentGear)) * 70

        }
    }

    Rectangle {
        anchors.fill: parent
        radius: 10
        gradient: Gradient {
            orientation: Qt.Horizontal
            GradientStop { position: 0.0; color: mainWindow.color }
            GradientStop { position: 0.5; color: "#00000000" }
            GradientStop { position: 1.0; color: mainWindow.color }
        }
    }

    // todo: implement animation when gear state is changed
}
