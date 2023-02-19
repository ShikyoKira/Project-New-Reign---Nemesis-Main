#include "ui/UiModInfo.h"

#include "utilities/algorithm.h"

UiModInfo::UiModInfo(const nemesis::ModInfo& modinfo)
{
    name      = std::wstring(modinfo.GetName()) + L" (" + std::wstring(modinfo.GetSite()) + L")";
    author    = modinfo.GetAuthor();
    automatic = modinfo.GetAutoRefFile();
}

std::string UiModInfo::getNameA() const
{
    return nemesis::transform_to(name);
}

std::string UiModInfo::getAuthorA() const
{
    return nemesis::transform_to(author);
}

std::string UiModInfo::getAutomaticA() const
{
    return nemesis::transform_to(automatic);
}

std::wstring UiModInfo::getName() const
{
    return name;
}

std::wstring UiModInfo::getAuthor() const
{
    return author;
}

std::wstring UiModInfo::getAutomatic() const
{
    return automatic;
}
