import QtQuick 2.15
import QtWebEngine 1.15
import QtWebChannel

Item {

    anchors.fill: parent

    WebEngineView {
        id: webEngine
        anchors.fill: parent
        url: "qrc:/html/spotify-player"
        webChannel: webChannel
    }
}
