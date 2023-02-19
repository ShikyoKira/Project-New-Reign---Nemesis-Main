#include "utilities/modpriority.h"

nemesis::ModPriority::ModPriority(const VecStr& behaviorpriority)
{
    for (size_t i = 0; i < behaviorpriority.size(); ++i)
    {
        ranking[behaviorpriority[i]] = i;
    }
}

bool nemesis::ModPriority::IsHigherThan(const std::string& code1, const std::string& code2) const
{
    return ranking.at(code1) < ranking.at(code2);
}

bool nemesis::ModPriority::IsLowerThan(const std::string& code1, const std::string& code2) const
{
    return ranking.at(code1) > ranking.at(code2);
}

bool nemesis::ModPriority::Contains(const std::string& code) const noexcept
{
    return ranking.find(code) != ranking.end();
}

bool nemesis::ModPriority::Contains(std::string_view code) const noexcept
{
    return ranking.find(std::string(code)) != ranking.end();
}
