#include "envmanager.h"

#include <QFile>
#include <QDebug>

EnvManager& EnvManager::instance() {
    static EnvManager instance;
    return instance;
}

void EnvManager::loadEnvFile(const QString &envFilePath) {

    QFile file(envFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open .env file.";
        return;
    }

    filePath = envFilePath;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        // Skip empty lines or comments
        if (line.isEmpty() || line.startsWith("#"))
            continue;

        // Split key=value
        QStringList parts = line.split("=", Qt::SkipEmptyParts);
        if (parts.size() == 2) {
            QString key = parts[0].trimmed();
            QString value = parts[1].trimmed();

            env.insert(key, value);
        }
    }
}

void EnvManager::saveEnvFile() {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open .env file for writing.";
        return;
    }

    QTextStream out(&file);

    for (const QString &key : env.keys()) {
        out << key << "=" << env.value(key) << "\n";
    }
}


QString EnvManager::get(const QString &key, const QString &defaultValue) const {
    return env.value(key, defaultValue);
}

void EnvManager::set(const QString &key, const QString &value) {
    env.insert(key, value);
    saveEnvFile();
}
