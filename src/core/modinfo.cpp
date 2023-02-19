#include <ios>
#include <sstream>

#include "core/modinfo.h"

#include "utilities/algorithm.h"

void nemesis::ModInfo::ReadFile(const std::filesystem::path& infopath)
{
    VecWstr storeline;

    if (!GetFileLines(infopath, storeline, false)) return;

    for (auto& line : storeline)
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

        std::wstring hide = line.substr(line.find(L"=") + 1);
        nemesis::to_lower(hide);
        std::wistringstream stream(hide);
        stream >> std::boolalpha >> hidden;
    }
}

void nemesis::ModInfo::SetModCode(const std::wstring& modcode)
{
    this->modcode = modcode;
}

std::wstring nemesis::ModInfo::GetDisplayName() const noexcept
{
    return name + L" (" + site + L")";
}

std::wstring_view nemesis::ModInfo::GetName() const noexcept
{
    return name;
}

std::wstring_view nemesis::ModInfo::GetAuthor() const noexcept
{
    return author;
}

std::wstring_view nemesis::ModInfo::GetSite() const noexcept
{
    return site;
}

std::wstring_view nemesis::ModInfo::GetAutoRefFile() const noexcept
{
    return automatic;
}

std::wstring_view nemesis::ModInfo::GetModCode() const noexcept
{
    return modcode;
}

std::string nemesis::ModInfo::GetDisplayNameA() const noexcept
{
    return nemesis::transform_to<std::string>(name + L" (" + site + L")");
}

std::string nemesis::ModInfo::GetNameA() const noexcept
{
    return nemesis::transform_to<std::string>(name);
}

std::string nemesis::ModInfo::GetAuthorA() const noexcept
{
    return nemesis::transform_to<std::string>(author);
}

std::string nemesis::ModInfo::GetSiteA() const noexcept
{
    return nemesis::transform_to<std::string>(site);
}

std::string nemesis::ModInfo::GetAutoRefFileA() const noexcept
{
    return nemesis::transform_to<std::string>(automatic);
}

std::string nemesis::ModInfo::GetModCodeA() const noexcept
{
    return nemesis::transform_to<std::string>(modcode);
}

bool nemesis::ModInfo::IsHidden() const noexcept
{
    return hidden;
}
