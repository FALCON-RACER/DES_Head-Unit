import QtQuick 2.15
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Controls 2.12
import QtMultimedia

Item {
    Rectangle{
        id: menu
        color: "transparent"
        height:400
        width: 700
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        RowLayout {
            anchors.centerIn: parent
            spacing:0

            Button {
                text: "Button 1"
                Layout.preferredWidth: 200
                Layout.preferredHeight: 200
                Layout.alignment: Qt.AlignHCenter
                opacity: 0
                onClicked: {
                    contentLoader.source = "modules/home/home.qml"
                }
            }

            Button {
                text: "Button 2"
                Layout.preferredWidth: 200
                Layout.preferredHeight: 200
                Layout.alignment: Qt.AlignHCenter
                opacity: 0
                onClicked: {
                    contentLoader.source = "modules/settings/settings.qml"
                }
            }

            Button {
                text: "Button 3"
                Layout.preferredWidth: 200
                Layout.preferredHeight: 200
                Layout.alignment: Qt.AlignHCenter
                opacity: 0
                onClicked: {
                    contentLoader.source = "modules/media/media.qml"
                }
            }
        }
        Image {
            id: home
            source: "qrc:/shared/images/home"
            anchors.left: parent.left
            anchors.leftMargin: 50
            anchors.verticalCenter: parent.verticalCenter
            width: 170
            fillMode: Image.PreserveAspectFit
            smooth: true
            mipmap: true
        }
        Image {
            id: settings
            source: "qrc:/shared/images/settings"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width: 170
            fillMode: Image.PreserveAspectFit
            smooth: true
            mipmap: true
        }
        Image {
            id: media
            source: "qrc:/shared/images/media"
            anchors.right: parent.right
            anchors.rightMargin: 50
            anchors.verticalCenter: parent.verticalCenter
            width: 170
            fillMode: Image.PreserveAspectFit
            smooth: true
            mipmap: true
        }
        ColorOverlay {
            anchors.fill: home
            source: home
            color: myColor
            smooth: true
        }
        ColorOverlay {
            anchors.fill: settings
            source: settings
            color: myColor
            smooth: true
        }
        ColorOverlay {
            anchors.fill: media
            source: media
            color: myColor
            smooth: true
        }
    }

//     width: parent.width
//     height: parent.height


//     GridLayout {
//         columns: 4
//         anchors.fill: parent

//         // Spotify
//         Rectangle {
//             Layout.preferredWidth: parent.width
//             Layout.fillHeight: true
//             color: "transparent"

//             Image {
//                 source: "qrc:/icons/spotify_logo"
//                 width: 50
//                 height: 50
//                 anchors.centerIn: parent
//                 MouseArea {
//                     anchors.fill: parent
//                     onClicked: {
//                         console.log("Spotify clicked")
//                         contentLoader.source = "modules/spotify/spotify_main.qml"
//                     }
//                 }
//             }
//         }
//     }
}
