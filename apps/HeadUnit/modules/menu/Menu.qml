import QtQuick 2.15
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Controls 2.12
import QtMultimedia

Item {
    id: menu
    Rectangle{
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
                    stackView.push(home)
                }
            }

            Button {
                text: "Button 2"
                Layout.preferredWidth: 200
                Layout.preferredHeight: 200
                Layout.alignment: Qt.AlignHCenter
                opacity: 0
                onClicked: {
                    stackView.push(settings)
                }
            }

            Button {
                text: "Button 3"
                Layout.preferredWidth: 200
                Layout.preferredHeight: 200
                Layout.alignment: Qt.AlignHCenter
                opacity: 0
                onClicked: {
                    stackView.push(media)
                }
            }
        }
        Image {
            id: homeicon
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
            id: settingsicon
            source: "qrc:/shared/images/settings"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width: 170
            fillMode: Image.PreserveAspectFit
            smooth: true
            mipmap: true
        }
        Image {
            id: mediaicon
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
            anchors.fill: homeicon
            source: homeicon
            color: myColor
            smooth: true
        }
        ColorOverlay {
            anchors.fill: settingsicon
            source: settingsicon
            color: myColor
            smooth: true
        }
        ColorOverlay {
            anchors.fill: mediaicon
            source: mediaicon
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
