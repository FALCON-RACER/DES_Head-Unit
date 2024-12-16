#include <QStandardPaths>
#include <QDir>
#include <QJsonArray>
#include <QDesktopServices>
#include "spotify.h"
#include "envmanager.h"
#include <QDebug>

Spotify::Spotify(QObject *parent) : QObject(parent), networkManager(new QNetworkAccessManager(this))
{
    loadConfig();

    if (alreadyLogin()) {
        refreshAccessToken();
        loginStatus = true;
    }
    else
        loginStatus = false;
}

Spotify::~Spotify()
{
    delete networkManager;
}

void Spotify::loadConfig()
{
    clientId = EnvManager::instance().get("SPOTIFY_CLIENT_ID");
    clientSecret = EnvManager::instance().get("SPOTIFY_CLIENT_SECRET");
    accessToken = EnvManager::instance().get("SPOTIFY_ACCESS_TOKEN");
    refreshToken = EnvManager::instance().get("SPOTIFY_REFRESH_TOKEN");

    if (clientId.isEmpty() || clientSecret.isEmpty())
        qDebug() << "[Spotify] Missing client_id or client_secret in";
}

bool Spotify::alreadyLogin()
{
    if (accessToken.isEmpty() && refreshToken.isEmpty())
        return false;
    return true;
}

void Spotify::login()
{
    QString authUrl = QString("https://accounts.spotify.com/authorize?"
                              "client_id=%1"
                              "&response_type=code"
                              "&redirect_uri=%2"
                              "&scope=user-read-private user-read-email playlist-read-private")
                          .arg(clientId, "http://localhost");
    QDesktopServices::openUrl(QUrl(authUrl));
}

void Spotify::refreshAccessToken()
{
    QUrl url("https://accounts.spotify.com/api/token");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery postData;
    postData.addQueryItem("grant_type", "refresh_token");
    postData.addQueryItem("refresh_token", refreshToken);
    postData.addQueryItem("client_id", clientId);

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, &QNetworkReply::finished, this, [=]()
            {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(response);
            QJsonObject obj = doc.object();
            QString newAccessToken = obj["access_token"].toString();
            QString newRefreshToken = obj["refresh_token"].toString();

            qDebug() << "obj : " << obj;

            if (newRefreshToken.isEmpty())
                newRefreshToken = refreshToken;

            updateAccessToken(newAccessToken, newRefreshToken);            
            loginStatus = true;

        } else {
            qWarning() << "Failed to refresh access token";
            loginStatus = false;
            reply->deleteLater();
            return;
        }
        reply->deleteLater(); });
}

void Spotify::updateAccessToken(QString newAccessToken, QString newRefreshToken)
{
    accessToken = newAccessToken;
    refreshToken = newRefreshToken;

    EnvManager::instance().set("SPOTIFY_ACCESS_TOKEN", newAccessToken);
    EnvManager::instance().set("SPOTIFY_REFRESH_TOKEN", newRefreshToken);
}
