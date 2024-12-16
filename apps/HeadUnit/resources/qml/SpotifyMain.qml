import QtQuick 2.15
import QtQuick.Controls 2.15
import QtWebEngine 1.15

Rectangle {
    anchors.fill: parent

    Component.onCompleted: {
        if (spotify.isLoggedIn) {
            Qt.createComponent("SpotifyNowPlaying.qml").createObject(parent);
        } else {

            webView.visible = true
            webView.url = spotify.getLoginURL();
        }
    }


    WebEngineView {
        id: webView
        visible: false
        anchors.fill: parent
        url: ""

        onUrlChanged: {
            if (webView.url.toString().startsWith("http://localhost")) {

                var url = webView.url;
                var regex = /code=([^&]+)/;
                var match = url.toString().match(regex);
                if (match) {
                    var authorizationCode = match[1];
                    console.log("[Qml] Authorization code: " + authorizationCode);
                    spotify.getAndSaveAccessToken(authorizationCode);

                    spotify.loginFinished.connect(function(success) {

                        if (success) {

                            webView.destroy();

                            var component = Qt.createComponent("SpotifyNowPlaying.qml");
                            if (component.status === Component.Ready)
                                component.createObject(parent);
                            else
                                console.error("[Qml] Failed to load SpotifyNowPlaying.qml");
                        } else {
                            console.log("[Qml] Spotify failed to login, go back to the login page.");
                            webView.url = spotify.getLoginURL();
                        }
                    });

                }
            }
        }
    }
}
