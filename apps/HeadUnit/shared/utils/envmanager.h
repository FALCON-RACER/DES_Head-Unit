#ifndef ENVMANAGER_H
#define ENVMANAGER_H

#include <QString>
#include <QProcessEnvironment>

class EnvManager
{
public:
    static EnvManager& instance();

    void loadEnvFile(const QString &filePath);
    void saveEnvFile();

    QString get(const QString &key, const QString &defaultValue = QString()) const;
    void set(const QString &key, const QString &value);

private:
    QProcessEnvironment env;
    QString filePath;

    EnvManager() = default;
    EnvManager(const EnvManager&) = delete;
    EnvManager& operator=(const EnvManager&) = delete;
};

#endif // ENVMANAGER_H
