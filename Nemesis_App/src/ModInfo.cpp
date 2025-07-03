#include <QFile>
#include <QTextStream>
#include <sstream>
#include <iostream>

#include "ModInfo.h"

void ModInfo::ReadFile(const std::filesystem::path& infopath)
{
    std::vector<std::string> storelines;
    QFile info_file(infopath);

    if (!info_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::error_code ec(errno, std::system_category());
#if WIN32
        auto u8str = infopath.u8string();
        throw std::runtime_error("Failed to open file: \"" + std::string(u8str.begin(), u8str.end())
                                 + "\"\nMessage: " + ec.message());
#else
        throw std::runtime_error("Failed to open file: \"" + filepath.string()
                                 + "\"\nMessage: " + ec.message());
#endif
    }

    QTextStream file_stream(&info_file);

    while (!file_stream.atEnd())
    {
        storelines.emplace_back(QString(info_file.readLine()).trimmed().toStdString());
    }

    info_file.close();

    for (auto& line : storelines)
    {
        if (line.starts_with("name="))
        {
            name = line.substr(line.find("=") + 1);
            continue;
        }

        if (line.starts_with("author="))
        {
            author = line.substr(line.find("=") + 1);
            continue;
        }

        if (line.starts_with("site="))
        {
            site = line.substr(line.find("=") + 1);
            continue;
        }

        if (line.starts_with("auto="))
        {
            automatic = line.substr(line.find("=") + 1);
            continue;
        }
    }
}

void ModInfo::SetModCode(const std::string& modcode)
{
    this->modcode = modcode;
}

std::string ModInfo::GetDisplayName() const noexcept
{
    return name + " (" + site + ")";
}

const std::string& ModInfo::GetName() const noexcept
{
    return name;
}

const std::string& ModInfo::GetAuthor() const noexcept
{
    return author;
}

const std::string& ModInfo::GetSite() const noexcept
{
    return site;
}

const std::string& ModInfo::GetAutoRefFile() const noexcept
{
    return automatic;
}

const std::string& ModInfo::GetModCode() const noexcept
{
    return modcode;
}
