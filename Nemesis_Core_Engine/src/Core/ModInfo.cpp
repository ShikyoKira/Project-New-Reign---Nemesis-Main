#include <ios>
#include <sstream>

#include "Core/ModInfo.h"

#include "Utilities/File.h"
#include "Utilities/Algorithm.h"

void nemesis::ModInfo::ReadFile(const std::filesystem::path& infopath)
{
    VecStr storeline;
    GetFileLines(infopath, storeline, false);

    for (auto& line : storeline)
    {
        if (line._Starts_with("name="))
        {
            name = line.substr(line.find("=") + 1);
            continue;
        }

        if (line._Starts_with("author="))
        {
            author = line.substr(line.find("=") + 1);
            continue;
        }

        if (line._Starts_with("site="))
        {
            site = line.substr(line.find("=") + 1);
            continue;
        }

        if (!line._Starts_with("auto=")) continue;

        automatic = line.substr(line.find("=") + 1);
    }
}

void nemesis::ModInfo::SetModCode(const std::string& modcode)
{
    this->modcode = modcode;
}

std::string nemesis::ModInfo::GetDisplayName() const noexcept
{
    return name + " (" + site + ")";
}

const std::string& nemesis::ModInfo::GetName() const noexcept
{
    return name;
}

const std::string& nemesis::ModInfo::GetAuthor() const noexcept
{
    return author;
}

const std::string& nemesis::ModInfo::GetSite() const noexcept
{
    return site;
}

const std::string& nemesis::ModInfo::GetAutoRefFile() const noexcept
{
    return automatic;
}

const std::string& nemesis::ModInfo::GetModCode() const noexcept
{
    return modcode;
}
