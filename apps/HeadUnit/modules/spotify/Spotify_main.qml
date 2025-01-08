import QtQuick 2.15
import QtWebEngine 1.15

Item {
    anchors.fill: parent

    Loader {
        id: componentLoader
        anchors.fill: parent
        active: false
    }

    Component.onCompleted: {
        if (spotify.isLoggedIn) {
            componentLoader.source = "player";
            componentLoader.active = true;
        } else {
            webView.visible = true;
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
                            webView.visible = false;

                            componentLoader.source = "player";
                            componentLoader.active = true;

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
