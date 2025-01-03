#include <QStandardPaths>
#include <QDir>
#include <QJsonArray>
#include <QDesktopServices>
#include <QDebug>
#include <QByteArray>
#include "spotify.h"
#include "../../shared/utils/envmanager.h"
#include "../../shared/utils/utils.h"

Spotify::Spotify(QObject *parent) : QObject(parent), networkManager(new QNetworkAccessManager(this)) {

    loadConfig();

    if (alreadyLogin())
        refreshAccessToken();
    else
        loginStatus = false;
}

Spotify::~Spotify() {

    delete networkManager;
}

void Spotify::loadConfig() {

    clientId = EnvManager::instance().get("SPOTIFY_CLIENT_ID");
    clientSecret = EnvManager::instance().get("SPOTIFY_CLIENT_SECRET");
    accessToken = EnvManager::instance().get("SPOTIFY_ACCESS_TOKEN");
    refreshToken = EnvManager::instance().get("SPOTIFY_REFRESH_TOKEN");

    if (clientId.isEmpty() || clientSecret.isEmpty())
        qDebug() << "[Spotify] client_id or client_secret doesn't exist.";
}

bool Spotify::alreadyLogin() {

    if (accessToken.isEmpty() && refreshToken.isEmpty())
        return false;
    return true;
}

void Spotify::getAndSaveAccessToken(QString authorizationCode) {

    QUrl url("https://accounts.spotify.com/api/token");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", QByteArray("Basic ") + Utils::encodeBase64(clientId + ":" + clientSecret).toUtf8());

    QUrlQuery postData;
    postData.addQueryItem("grant_type", "authorization_code");
    postData.addQueryItem("code", authorizationCode);
    postData.addQueryItem("redirect_uri", "http://localhost");

    QNetworkReply *reply = networkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, &QNetworkReply::finished, this, [=]()
            {
        if (reply->error() == QNetworkReply::NoError) {

            QByteArray response = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(response);
            QJsonObject obj = doc.object();
            QString newAccessToken = obj["access_token"].toString();
            QString newRefreshToken = obj["refresh_token"].toString();
            // token_type, scope, expires_in

            qDebug() << "[Spotify] API response " << obj;

            saveAccessToken(newAccessToken, newRefreshToken);
            loginStatus = true;
            qDebug() << "[Spoitfy] Login successful";

        } else {

            qDebug() << "[Spoitfy] Login Failed";
            qWarning() << "[Spoitfy] Error:" << reply->errorString();
            qDebug() << "[Spoitfy] HTTP Status Code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qWarning() << "[Spoitfy] Failed to refresh access token";

            loginStatus = false;
            reply->deleteLater();

            return;
        }
        emit loginFinished(loginStatus);
        reply->deleteLater(); });
}

void Spotify::refreshAccessToken() {

    QUrl url("https://accounts.spotify.com/api/token");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", QByteArray("Basic ") + Utils::encodeBase64(clientId + ":" + clientSecret).toUtf8());

    QUrlQuery postData;
    postData.addQueryItem("grant_type", "refresh_token");
    postData.addQueryItem("refresh_token", refreshToken);
    postData.addQueryItem("client_id", clientId);

    QNetworkReply *reply = networkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, &QNetworkReply::finished, this, [=]()
            {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(response);
            QJsonObject obj = doc.object();
            QString newAccessToken = obj["access_token"].toString();
            QString newRefreshToken = obj["refresh_token"].toString();

            qDebug() << "[Spotify] API response " << obj;

            if (newRefreshToken.isEmpty())
                newRefreshToken = refreshToken;

            saveAccessToken(newAccessToken, newRefreshToken);
            loginStatus = true;

            qDebug() << "[Spoitfy] Login successful";

        } else {

            qDebug() << "[Spoitfy] Login Failed";
            qWarning() << "[Spoitfy] Failed to refresh access token";
            qDebug() << "[Spoitfy] HTTP Status Code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qDebug() << "[Spoitfy] Response Body: " << reply->readAll();

            loginStatus = false;
            reply->deleteLater();
            return;
        }
        reply->deleteLater(); });
}

void Spotify::saveAccessToken(QString newAccessToken, QString newRefreshToken) {

    accessToken = newAccessToken;
    refreshToken = newRefreshToken;

    EnvManager::instance().set("SPOTIFY_ACCESS_TOKEN", newAccessToken);
    EnvManager::instance().set("SPOTIFY_REFRESH_TOKEN", newRefreshToken);
}

bool Spotify::isLoggedIn() {

    qDebug() << "[Spotify] loginStatus : " << loginStatus;

    return loginStatus;
}

QUrl Spotify::getLoginURL() {

    QString urlString = QString("https://accounts.spotify.com/authorize?client_id=%1&response_type=%2&redirect_uri=%3&scope=%4")
                            .arg(clientId)
                            .arg("code")
                            .arg("http://localhost")
                            .arg("streaming playlist-modify-private user-read-private user-read-email playlist-read-private user-read-playback-state user-modify-playback-state user-read-currently-playing");

    QUrl url(urlString);
    return url;
}

void Spotify::getPlaylists() {
    QUrl url("https://api.spotify.com/v1/me/playlists");
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + accessToken.toUtf8());

    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
            QJsonArray playlists = json.object()["items"].toArray();

            // todo 값을 구조체에 저장해야 함.
            for (const QJsonValue &playlist : playlists) {
                QString name = playlist.toObject()["name"].toString();
                QString id = playlist.toObject()["id"].toString();
                qDebug() << "Playlist:" << name << ", ID:" << id;
            }
        } else {
            qDebug() << "Error fetching playlists:" << reply->errorString();
        }
        reply->deleteLater();
    });
    // todo 토큰 유효성 검사. 빠꾸 나오면 다시 이 함수 실행해야 돼.ㅡ 함수를 기능별로 분리해야할듯

}

void Spotify::playPlaylist(QString playlistId) {

    playlistId = "1h3rqwj2jSzSs7OoETi5Yy";

    QUrl url(QString("https://api.spotify.com/v1/me/player/play"));
    QNetworkRequest request(url);

    request.setRawHeader("Authorization", "Bearer " + accessToken.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject body;
    body["context_uri"] = QString("spotify:playlist:%1").arg(playlistId);

    QNetworkReply *reply = networkManager->put(request, QJsonDocument(body).toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Playlist started playing.";
        } else {
            qDebug() << "Error playing playlist:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void Spotify::pausePlayback() {
    QUrl url("https://api.spotify.com/v1/me/player/pause");
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + accessToken.toUtf8());

    QNetworkReply *reply = networkManager->put(request, QByteArray());
    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
}

void Spotify::setVolume(int volume) {
    QUrl url(QString("https://api.spotify.com/v1/me/player/volume?volume_percent=%1").arg(volume));
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + accessToken.toUtf8());

    QNetworkReply *reply = networkManager->put(request, QByteArray());
    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
}
