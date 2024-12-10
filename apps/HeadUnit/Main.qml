import QtQuick

Window {
    width: 1024
    height: 600
    visible: true
    visibility: "FullScreen"
    title: qsTr("Head Unit")

    Text {
        text: "Head Unit"
        anchors.centerIn: parent
        font.pixelSize: 32
        color: "blue"
    }
}
