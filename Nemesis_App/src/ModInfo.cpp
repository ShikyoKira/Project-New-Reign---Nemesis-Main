#include <QFile>
#include <QDir>
#include <QTextStream>
#include <sstream>
#include <iostream>

#include "ModInfo.h"

void ModInfo::ReadFile(const std::filesystem::path& infopath)
{
    std::vector<std::string> storelines;
#if WIN32
    auto u8_path = infopath.u8string();
    std::string spath(u8_path.begin(), u8_path.end());
#else
    std::string spath = infopath.string();
#endif
    QFile info_file(QDir::toNativeSeparators(QString::fromStdString(spath)));

    if (!info_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::error_code ec(errno, std::system_category());
        throw std::runtime_error("Failed to open file: \"" + spath + "\"\nMessage: " + ec.message());
    }

    QTextStream file_stream(&info_file);

    while (!file_stream.atEnd())
    {
        storelines.emplace_back(file_stream.readLine().trimmed().toStdString());
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
