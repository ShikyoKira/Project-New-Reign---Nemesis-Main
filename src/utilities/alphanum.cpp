#include "utilities/alphanum.hpp"

bool alphanum_less::operator()(const std::string& x, const std::string& y) const
{
    auto l1 = x.length();
    auto l2 = y.length();

    if (l1 == l2) return x.compare(y) < 0;

    return l1 > l2;
}

bool alphanum_less::operator()(const std::string_view& x, const std::string_view& y) const
{
    auto l1 = x.length();
    auto l2 = y.length();

    if (l1 == l2) return x.compare(y) < 0;

    return l1 > l2;
}
