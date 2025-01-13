// import QtQuick 2.15
// import QtQuick.Controls 2.15
// import QtWebEngine 1.15

// Rectangle {
//     anchors.fill: parent

//     WebEngineView {
//         id: webView
//         anchors.fill: parent
//         url: Qt.resolvedUrl("qrc:/html/spotify_player.html") // HTML 파일 경로

//         // WebChannel 객체 설정
//         settings.webAttribute(WebEngineSettings.LocalStorageEnabled, true)
//         settings.webAttribute(WebEngineSettings.LocalContentCanAccessFileUrls, true)

//         Component.onCompleted: {
//             webView.page().setWebChannel(webChannel); // QWebChannel 설정
//         }
//     }
// }

import QtQuick 2.15
// import QtQuick.Controls 2.15

Rectangle {

    anchors.fill: parent

    color: "transparent"
    // color: "blue"
    Text {
       anchors.centerIn: parent
       text: "now playing qml"
       font.pixelSize: 20
   }

}
