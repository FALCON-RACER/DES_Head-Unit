import QtQuick

Window {
    width: 1280
    height: 400
    visible: true
	visibility: "FullScreen"
    title: qsTr("Instrument Cluster")

    Text {
        text: "Instrument Cluster"
        anchors.centerIn: parent
        font.pixelSize: 32
        color: "blue"
    }
}
