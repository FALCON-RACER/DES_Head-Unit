import QtQuick 2.15
import QtQuick.Layouts


Window {
    visible: true
    // visibility: "FullScreen"
    width: 800
    height: 800

    title: qsTr("Head Unit")
    color: "gray"
    // todo background design
    property Item previousItem: null

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

                // Menu
                Rectangle {
                    Layout.preferredWidth: parent.width
                    Layout.fillHeight: true
                    color: "transparent"

                    Image {
                        source: "qrc:/icons/menu"
                        width: 50
                        height: 50
                        anchors.centerIn: parent

                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {

                            contentLoader.source = "modules/menu/menu.qml"

                            console.log("menu clicked")
                        }
                    }
                }
            }

            // Contents Area
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.rightMargin: 10

                color: "transparent"
                Loader {
                    id: contentLoader
                    anchors.fill: parent
                    source: "home.qml"
                    // onItemChanged: {
                    //     if (contentLoader.item) {

                    //         console.log("새로운 컴포넌트 로드됨:", contentLoader.item);
                    //         console.log("previousItem:", previousItem);
                    //         console.log("contentLoader.item:", contentLoader.item);

                    //         // 이전 컴포넌트가 존재하면 파괴 (혹은 메모리 정리 작업)
                    //         if (contentLoader.item !== previousItem) {

                    //             console.log("다르잔아");
                    //             if (previousItem) {
                    //                 previousItem.destroy();  // 이전 컴포넌트를 명시적으로 파괴
                    //                 console.log("파괴ㅣㅣㅣ");
                    //             }
                    //               // 현재 아이템을 이전 아이템으로 설정
                    //             previousItem = contentLoader.item;
                    //         }
                    //     }
                    // }
                }
            }
        }
        }
    }

