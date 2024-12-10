import QtQuick

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Instrument Cluster")

    Text {
        text: "Instrument Cluster"
        anchors.centerIn: parent
        font.pixelSize: 32
        color: "blue"
    }
}
