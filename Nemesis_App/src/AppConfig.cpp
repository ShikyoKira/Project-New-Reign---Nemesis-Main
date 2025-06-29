#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <sstream>
#include <unordered_map>

#include "AppConfig.h"

AppConfig::AppConfig(const std::filesystem::path& filepath, QObject* parent)
    : QObject{parent}
{
    std::vector<std::string> storelines;
    QFile file(filepath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::error_code ec(errno, std::system_category());
        throw std::runtime_error("Failed to open file: \"" + QString::fromStdWString(filepath).toStdString()
                                 + "\"\nMessage: " + ec.message());
    }

    QTextStream file_stream(&file);

    while (!file_stream.atEnd())
    {
        storelines.emplace_back(QString(file.readLine()).trimmed().toStdString());
    }

    file.close();

    constexpr std::array<const char*, 5> platforms = {"win32", "amd64", "ps3", "ps4", "xb360"};

    std::unordered_map<std::string, std::function<void(const std::string&)>> SetConfigMap = {
        {"DataDirectory",
         [this](const std::string& line)
         {
             DataDirectory
                 = QString(std::filesystem::path(std::u8string(line.begin(), line.end())).wstring());
         }},
        {"StageDirectory",
         [this](const std::string& line)
         {
             StageDirectory
                 = QString(std::filesystem::path(std::u8string(line.begin(), line.end())).wstring());
         }},
        {"Platform",
         [&platforms, this](const std::string& line)
         {
             Platform = QString::fromStdString(line).toLower();

             for (auto& platform : platforms)
             {
                 if (platform == Platform) return;
             }

             QMessageBox::critical(
                 nullptr,
                 tr("Configuration Error"),
                 tr("Unsupported platform. Only win32, amd64, ps3, ps4 and xb360 are supported"));
             exit(-1);
         }},
        {"Width", [this](const std::string& line) { Width = std::stoi(line); }},
        {"Height", [this](const std::string& line) { Height = std::stoi(line); }},
        {"ModNameWidth", [this](const std::string& line) { ModNameWidth = std::stoi(line); }},
        {"AuthorWidth", [this](const std::string& line) { AuthorWidth = std::stoi(line); }},
        {"PriorityWidth", [this](const std::string& line) { PriorityWidth = std::stoi(line); }},
        {"DevMode",
         [this](const std::string& line)
         {
             std::istringstream ss(QString::fromStdString(line).toLower().toStdString());
             ss >> std::boolalpha >> DevMode;
         }},
    };

    for (auto& line : storelines)
    {
        for (auto& set_config : SetConfigMap)
        {
            if (!line.starts_with(set_config.first + "=")) continue;

            set_config.second(line.substr(line.find("=") + 1));
            break;
        }
    }

    if (!Platform.isEmpty()) return;

    QMessageBox::critical(nullptr, tr("Configuration Error"), tr("Platform type cannot be found. Only win32, amd64, ps3, ps4 and xb360 are supported"));
    exit(-1);
}

QString AppConfig::getDataDirectory() const
{
    return DataDirectory;
}

QString AppConfig::getStageDirectory() const
{
    return StageDirectory;
}

QString AppConfig::getPlatform() const
{
    return Platform;
}

int AppConfig::getWidth() const
{
    return Width;
}

int AppConfig::getHeight() const
{
    return Height;
}

int AppConfig::getModNameWidth() const
{
    return ModNameWidth;
}

int AppConfig::getAuthorWidth() const
{
    return AuthorWidth;
}

int AppConfig::getPriorityWidth() const
{
    return PriorityWidth;
}

bool AppConfig::isDevMode() const
{
    return DevMode;
}
