import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts


Window {
    visible: true
    visibility: "FullScreen"
    title: qsTr("Head Unit")
    color: "gray"
    // todo background design

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        // Top Bar
        Rectangle {
            Layout.fillWidth: true
            height: Screen.height * 0.03
            color: "lightblue"

            // todo display temperature, time , date
            Text {
                anchors.centerIn: parent
                text: "Header Area"
                font.pixelSize: 24
            }
        }


        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            z: 0

            // Gear
            ColumnLayout {
                width: Screen.width * 0.05
                Layout.fillHeight: true
                Layout.margins: 10
                z: 1


                // Parking Mode
                Rectangle {
                   Layout.fillHeight: true
                   Layout.preferredWidth: parent.width
                   color: "blue"
                   radius: 10
                   border.color: "black"
                   border.width: 2

                   MouseArea {
                       anchors.fill: parent
                       onClicked: {
                           console.log("Parking mode clicked")
                       }
                       onPressed: parent.color = "lightblue"
                       onReleased: parent.color = "blue"
                   }

                   Text {
                       anchors.centerIn: parent
                       text: "P"
                       color: "white"
                       font.pixelSize: 18
                   }
                }

                // Reverse Mode
                Rectangle {
                    Layout.fillHeight: true
                    Layout.preferredWidth: parent.width
                   color: "green"
                   radius: 10
                   border.color: "black"
                   border.width: 2

                   MouseArea {
                       anchors.fill: parent
                       onClicked: {
                           console.log("Reverse mode clicked")
                       }

                       onPressed: parent.color = "lightgreen"
                       onReleased: parent.color = "green"
                   }

                   Text {
                       anchors.centerIn: parent
                       text: "R"
                       color: "white"
                       font.pixelSize: 18
                   }
                }

                // Neutral Mode
                Rectangle {
                    Layout.fillHeight: true
                    Layout.preferredWidth: parent.width
                   color: "red"
                   radius: 10
                   border.color: "black"
                   border.width: 2

                   MouseArea {
                       anchors.fill: parent
                       onClicked: {
                           console.log("Neutral mode clicked")
                       }

                       onPressed: parent.color = "lightcoral"
                       onReleased: parent.color = "red"
                   }

                   Text {
                       anchors.centerIn: parent
                       text: "N"
                       color: "white"
                       font.pixelSize: 18
                   }
                }

                // Drive Mode
                Rectangle {
                    Layout.fillHeight: true
                    Layout.preferredWidth: parent.width
                   color: "red"
                   radius: 10
                   border.color: "black"
                   border.width: 2

                   MouseArea {
                       anchors.fill: parent
                       onClicked: {
                           console.log("Drive mode clicked")
                       }

                       onPressed: parent.color = "lightcoral"
                       onReleased: parent.color = "red"
                   }

                   Text {
                       anchors.centerIn: parent
                       text: "D"
                       color: "white"
                       font.pixelSize: 18
                   }
                }
            }

            // Contents Area
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.rightMargin: 10

                Loader {
                    id: contentLoader
                    anchors.fill: parent
                    source: "FirstContent.qml"  // Default Screen
                }
            }

        }

        // Bottom Bar
        RowLayout {
        Layout.fillWidth: true
        spacing: 10
        height: Screen.height * 0.05

            // First Button
            Rectangle {
               Layout.fillWidth: true
               Layout.preferredHeight: parent.height
               color: "blue"
               radius: 10
               border.color: "black"
               border.width: 2

               MouseArea {
                   anchors.fill: parent
                   onClicked: {
                       console.log("Home clicked")
                       contentLoader.source = "FirstContent.qml"
                   }

                   onPressed: parent.color = "lightblue"
                   onReleased: parent.color = "blue"
               }

               Text {
                   anchors.centerIn: parent
                   text: "Home"
                   color: "white"
                   font.pixelSize: 18
               }
            }

            // Second Button
            Rectangle {
               Layout.fillWidth: true
               Layout.preferredHeight: parent.height
               color: "green"
               radius: 10
               border.color: "black"
               border.width: 2

               MouseArea {
                   anchors.fill: parent
                   onClicked: {
                       console.log("Menu clicked")
                       contentLoader.source = "SecondContent.qml"
                   }

                   onPressed: parent.color = "lightgreen"
                   onReleased: parent.color = "green"
               }

               Text {
                   anchors.centerIn: parent
                   text: "Menu"
                   color: "white"
                   font.pixelSize: 18
               }
            }

            // Third Button
            Rectangle {
               Layout.fillWidth: true
               Layout.preferredHeight: parent.height
               color: "red"
               radius: 10
               border.color: "black"
               border.width: 2

               MouseArea {
                   anchors.fill: parent
                   onClicked: {
                       console.log("Settings clicked")
                       contentLoader.source = "ThirdContent.qml"
                   }

                   onPressed: parent.color = "lightcoral"
                   onReleased: parent.color = "red"
               }

               Text {
                   anchors.centerIn: parent
                   text: "Settings"
                   color: "white"
                   font.pixelSize: 18
               }
            }
        }
    }
}
