#include "ui\UiModInfo.h"

#include "utilities/algorithm.h"

UiModInfo::UiModInfo(const std::wstring& _name, const std::wstring& _author, const std::wstring& _auto)
    : name(_name)
    , author(_author)
    , automatic(_auto)
{
}

const std::string& UiModInfo::getNameA() const
{
    return nemesis::transform_to<std::string>(name);
}

const std::string& UiModInfo::getAuthorA() const
{
    return nemesis::transform_to<std::string>(author);
}

const std::string& UiModInfo::getAutomaticA() const
{
    return nemesis::transform_to<std::string>(automatic);
}

const std::wstring& UiModInfo::getName() const
{
    return name;
}

const std::wstring& UiModInfo::getAuthor() const
{
    return author;
}

const std::wstring& UiModInfo::getAutomatic() const
{
    return automatic;
}
