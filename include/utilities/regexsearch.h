#pragma once

#include "utilities/regex.h"

namespace nemesis
{
    struct RegexSearch
    {
    private:
        nemesis::regex rgx;
        std::string keyword;

    public:
        RegexSearch(const nemesis::regex& rgx, const std::string& keyword) noexcept
            : rgx(rgx)
            , keyword(keyword)
        {
        }

        bool IsMatch(const std::string& line) const noexcept
        {
            return line.find(keyword) != NOT_FOUND;
        }

        const nemesis::regex& GetRegex() const noexcept
        {
            return rgx;
        }
    };
} // namespace nemesis
