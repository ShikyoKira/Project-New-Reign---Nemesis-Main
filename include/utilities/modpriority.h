#pragma once

#include "utilities/types.h"

namespace nemesis
{
	struct ModPriority
    {
        UMap<std::string, uint> ranking;

        ModPriority(const VecStr& behaviorpriority);

        bool IsHigherThan(const std::string& code1, const std::string& code2) const;
        bool IsLowerThan(const std::string& code1, const std::string& code2) const;
        bool Contains(const std::string& code) const noexcept;
    };
}
