import QtQuick 2.15

Item {
    id: batteryIndicator
    property bool isCharging: instrumentClusterController.chargingState
    property real batteryPercentage: instrumentClusterController.batteryPercentage
    width: 100
    height: 30

    Rectangle {
        id: batteryAnode
        width: 78
        height: 18
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        color: "white"
        radius: 3
    }

    Rectangle {
        id: batteryBar
        width: 70
        height: 30
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        color: "white"
        radius: 5

        Rectangle {
            id: batteryFill
            width: batteryPercentage / 100 * (parent.width - 6)
            height: parent.height - 6
            color: batteryPercentage > 20 ? "lime" : "orangered"
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 3
            radius: 5
        }

        Image {
            anchors.centerIn: parent
            source: "qrc:/images/lightning.png"
            visible: isCharging
        }

    }

    Text {
        id: batteryPercentText
        anchors.left: parent.right
        anchors.verticalCenter: parent.verticalCenter
        text: batteryPercentage + '%'
        color: instrumentClusterController.ambientLighting
        font.bold: true
        font.pixelSize: 30
    }
}
