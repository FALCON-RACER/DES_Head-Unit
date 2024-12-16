#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QUrl>
#include <QUrlQuery>
#include <QStandardPaths>

class Spotify : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isLoggedIn READ isLoggedIn NOTIFY isLoggedInChanged)

public:
    explicit Spotify(QObject *parent = nullptr);
    ~Spotify();

    Q_INVOKABLE void getAndSaveAccessToken(QString autorizationCode);
    Q_INVOKABLE void refreshAccessToken();

    Q_INVOKABLE bool isLoggedIn();
    Q_INVOKABLE QUrl getLoginURL();

signals:

    Q_SIGNAL void loginFinished(bool success);
    void isLoggedInChanged();
    void playlistsFetched(const QString &playlists);
    void musicSearchResult(const QString &results);
    void playbackStarted();

private:
    QNetworkAccessManager *networkManager;
    QString clientId;
    QString clientSecret;
    QString accessToken;
    QString refreshToken;

    bool loginStatus;
    void saveAccessToken(QString newAccessToken, QString newRefreshToken);
    void loadConfig();

    bool alreadyLogin();

};

#endif // SPOTIFY_H
