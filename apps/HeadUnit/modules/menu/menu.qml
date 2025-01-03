import QtQuick 2.15
import QtQuick.Layouts

Item {

    width: parent.width
    height: parent.height


    GridLayout {
        columns: 5
        columnSpacing: 20
        anchors.centerIn: parent

        Rectangle {
            width: 50
            height: 50
            color: "transparent"

            Image {
                source: "qrc:/icons/spotify"
                width: 60
                height: 60
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

        Rectangle {
            width: 50
            height: 50
            color: "white"
            radius: 10

            Image {
                source: "qrc:/icons/youtube"
                width: 40
                height: 40
                anchors.centerIn: parent
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Youtube clicked")
                        contentLoader.source = "qrc:qml/youtube/main"
                    }
                }
            }
        }


        Rectangle {
            width: 50
            height: 50
            color: "skyblue"
            radius: 10

            Image {
                source: "qrc:/icons/youtube"
                width: 40
                height: 40
                anchors.centerIn: parent
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Youtube test clicked")
                        contentLoader.source = "qrc:qml/youtube/test"
                    }
                }
            }
        }
    }
}
