#include <QFile>
#include <QTextStream>
#include <sstream>
#include <iostream>

#include "ModInfo.h"

void ModInfo::ReadFile(const std::filesystem::path& infopath)
{
    std::vector<std::wstring> storelines;
    QFile info_file(infopath);

    if (!info_file.open(QIODevice::ReadOnly | QIODevice::Text)) throw std::runtime_error("Error opening file: " + infopath.string());

    QTextStream file_stream(&info_file);

    while (!file_stream.atEnd())
    {
        auto& line = storelines.emplace_back(QString(info_file.readLine()).trimmed().toStdWString());
    }

    info_file.close();

    for (auto& line : storelines)
    {
        if (line._Starts_with(L"name="))
        {
            name = line.substr(line.find(L"=") + 1);
            continue;
        }

        if (line._Starts_with(L"author="))
        {
            author = line.substr(line.find(L"=") + 1);
            continue;
        }

        if (line._Starts_with(L"site="))
        {
            site = line.substr(line.find(L"=") + 1);
            continue;
        }

        if (line._Starts_with(L"auto="))
        {
            automatic = line.substr(line.find(L"=") + 1);
            continue;
        }

        if (!line._Starts_with(L"hidden=")) continue;

        std::wstring hide = QString::fromStdWString(line.substr(line.find(L"=") + 1)).toLower().toStdWString();
        std::wistringstream stream(hide);
        stream >> std::boolalpha >> hidden;
    }
}

void ModInfo::SetModCode(const std::wstring& modcode)
{
    this->modcode = modcode;
}

std::wstring ModInfo::GetDisplayName() const noexcept
{
    return name + L" (" + site + L")";
}

std::wstring_view ModInfo::GetName() const noexcept
{
    return name;
}

std::wstring_view ModInfo::GetAuthor() const noexcept
{
    return author;
}

std::wstring_view ModInfo::GetSite() const noexcept
{
    return site;
}

std::wstring_view ModInfo::GetAutoRefFile() const noexcept
{
    return automatic;
}

std::wstring_view ModInfo::GetModCode() const noexcept
{
    return modcode;
}

std::string ModInfo::GetDisplayNameA() const noexcept
{
    return QString::fromStdWString(name + L" (" + site + L")").toStdString();
}

std::string ModInfo::GetNameA() const noexcept
{
    return QString::fromStdWString(name).toStdString();
}

std::string ModInfo::GetAuthorA() const noexcept
{
    return QString::fromStdWString(author).toStdString();
}

std::string ModInfo::GetSiteA() const noexcept
{
    return QString::fromStdWString(site).toStdString();
}

std::string ModInfo::GetAutoRefFileA() const noexcept
{
    return QString::fromStdWString(automatic).toStdString();
}

std::string ModInfo::GetModCodeA() const noexcept
{
    return QString::fromStdWString(modcode).toStdString();
}

bool ModInfo::IsHidden() const noexcept
{
    return hidden;
}
