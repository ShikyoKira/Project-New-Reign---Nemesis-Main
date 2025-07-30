#pragma once

#include <filesystem>
#include <QObject>
#include <QSettings>

class AppConfig : public QObject
{
    Q_OBJECT

private:
    QString formatPlatform(const QString& platform) const;

public:
    explicit AppConfig(const std::filesystem::path& filepath, QObject* parent = nullptr);
    ~AppConfig();

    Q_INVOKABLE QString getDataDirectory() const;
    Q_INVOKABLE QString getStageDirectory() const;
    Q_INVOKABLE QString getPlatform() const;
    Q_INVOKABLE int getWidth() const;
    Q_INVOKABLE int getHeight() const;
    Q_INVOKABLE int getModNameWidth() const;
    Q_INVOKABLE int getAuthorWidth() const;
    Q_INVOKABLE int getPriorityWidth() const;
    Q_INVOKABLE bool isDevMode() const;

    Q_INVOKABLE const QVariantList& getModListData() const;
    Q_INVOKABLE void setModListData(const QVariantList& mod_order_list);

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
    QVariantList ModListData;

    std::unique_ptr<QSettings> IniSettings;
};
