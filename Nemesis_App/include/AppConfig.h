#pragma once

#include <filesystem>
#include <QObject>

class AppConfig : public QObject
{
    Q_OBJECT

public:
    explicit AppConfig(const std::filesystem::path& filepath, QObject* parent = nullptr);

    Q_INVOKABLE QString getDataDirectory() const;
    Q_INVOKABLE QString getStageDirectory() const;
    Q_INVOKABLE QString getPlatform() const;
    Q_INVOKABLE int getWidth() const;
    Q_INVOKABLE int getHeight() const;
    Q_INVOKABLE int getModNameWidth() const;
    Q_INVOKABLE int getAuthorWidth() const;
    Q_INVOKABLE int getPriorityWidth() const;
    Q_INVOKABLE bool isDevMode() const;

signals:
    void outputReceived(const QString &output);
    void errorReceived(const QString &error);
    void finishedReceived();
    void progressUp(int step, int max);

private:
    QString DataDirectory;
    QString StageDirectory;
    QString Platform;
    int Width;
    int Height;
    int ModNameWidth;
    int AuthorWidth;
    int PriorityWidth;
    bool DevMode = false;
};
