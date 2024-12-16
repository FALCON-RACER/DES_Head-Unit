import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {

    width: parent.width
    height: parent.height


    GridLayout {
        columns: 4
        anchors.fill: parent

        // Spotify
        Rectangle {
            Layout.preferredWidth: parent.width
            Layout.fillHeight: true
            color: "transparent"

            Image {
                source: "qrc:/icons/spotify_logo"
                width: 50
                height: 50
                anchors.centerIn: parent

            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("Spotify clicked")
                    contentLoader.source = "Spotify.qml"
                }
            }
        }
    }

}
