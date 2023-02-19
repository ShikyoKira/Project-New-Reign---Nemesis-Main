#pragma once

#include <string>

#include "core/modinfo.h"

class UiModInfo
{
    std::wstring name, author, automatic;

public:
    UiModInfo(const nemesis::ModInfo& modinfo);

    std::string getNameA() const;
    std::string getAuthorA() const;
    std::string getAutomaticA() const;

    std::wstring getName() const;
    std::wstring getAuthor() const;
    std::wstring getAutomatic() const;
};
