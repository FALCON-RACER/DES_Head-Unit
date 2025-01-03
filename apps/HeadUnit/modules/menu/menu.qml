import QtQuick 2.15
import QtQuick.Layouts

Item {

    width: parent.width
    height: parent.height


    GridLayout {
        columns: 5
        anchors.centerIn: parent

        Rectangle {
            width: 50
            height: 50
            color: "transparent"

            Image {
                source: "qrc:/icons/spotify"
                width: 50
                height: 50
                anchors.centerIn: parent
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Spotify clicked")
                        contentLoader.source = "qrc:qml/spotify/main"
                    }
                }
            }
        }
    }
}
