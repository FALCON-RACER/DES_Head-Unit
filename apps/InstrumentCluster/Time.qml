import QtQuick 2.15

Item {
    Text {
       id: currentTime
       anchors.centerIn: parent
       font.pixelSize: 35
       font.bold: true
       color: instrumentClusterController.ambientLighting
       text: Qt.formatTime(new Date(), "hh:mm")
    }

    Timer {
       interval: 1000
       running: true
       repeat: true
       onTriggered: currentTime.text = Qt.formatTime(new Date(), "hh:mm")
    }
}
