#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <sstream>
#include <unordered_map>

#include "AppConfig.h"

AppConfig::AppConfig(const std::filesystem::path& filepath, QObject* parent)
    : QObject{parent}
{
    std::vector<std::wstring> storelines;
    QFile file(filepath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) throw std::runtime_error("Error opening file: " + filepath.string());

    QTextStream file_stream(&file);

    while (!file_stream.atEnd())
    {
        auto& line = storelines.emplace_back(QString(file.readLine()).trimmed().toStdWString());
    }

    file.close();

    std::unordered_map<std::wstring, std::function<void(const std::wstring&)>> SetConfigMap = {
        {L"DataDirectory", [this](const std::wstring& line){
             DataDirectory = QString(std::filesystem::path(line).wstring());
         }},
        {L"StageDirectory", [this](const std::wstring& line){
             StageDirectory = QString(std::filesystem::path(line).wstring());
         }},
        {L"Width", [this](const std::wstring& line){
             Width = std::stoi(line);
         }},
        {L"Height", [this](const std::wstring& line){
             Height = std::stoi(line);
         }},
        {L"ModNameWidth", [this](const std::wstring& line){
             ModNameWidth = std::stoi(line);
         }},
        {L"AuthorWidth", [this](const std::wstring& line){
             AuthorWidth = std::stoi(line);
         }},
        {L"PriorityWidth", [this](const std::wstring& line){
             PriorityWidth = std::stoi(line);
         }},
        {L"DevMode", [this](const std::wstring& line){
             std::wistringstream ss(QString::fromStdWString(line).toLower().toStdWString());
             ss >> std::boolalpha >> DevMode;
         }},
    };

    for (auto& line : storelines)
    {
        for (auto& set_config : SetConfigMap)
        {
            if (!line._Starts_with(set_config.first + L"=")) continue;

            set_config.second(line.substr(line.find(L"=") + 1));
            break;
        }
    }
}

QString AppConfig::getDataDirectory() const
{
    return DataDirectory;
}

QString AppConfig::getStageDirectory() const
{
    return StageDirectory;
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
