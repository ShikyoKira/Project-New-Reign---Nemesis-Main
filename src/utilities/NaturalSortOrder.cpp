#include "utilities/NaturalSortOrder.h"

#include <cwctype>

QCollator nemesis::NaturalSortOrder::Collator;
nemesis::NaturalSortOrder nemesis::NaturalSortOrder::StaticOrder;

bool nemesis::NaturalSortAscend::operator()(const char* x, const char* y) const
{
    return nemesis::NaturalSortOrder::Compare(x, y) < 0;
}

bool nemesis::NaturalSortAscend::operator()(const std::string& x, const std::string& y) const
{
    return nemesis::NaturalSortOrder::Compare(x.c_str(), y.c_str()) < 0;
}

bool nemesis::NaturalSortAscend::operator()(const std::string_view& x, const std::string_view& y) const
{
    return nemesis::NaturalSortOrder::Compare(std::string(x).c_str(), std::string(y).c_str()) < 0;
}

bool nemesis::NaturalSortAscend::operator()(const std::wstring& x, const std::wstring& y) const
{
    return nemesis::NaturalSortOrder::Compare(x.c_str(), y.c_str()) < 0;
}

bool nemesis::NaturalSortAscend::operator()(const std::wstring_view& x, const std::wstring_view& y) const
{
    return nemesis::NaturalSortOrder::Compare(std::wstring(x).c_str(), std::wstring(y).c_str()) < 0;
}

bool nemesis::NaturalSortAscend::operator()(const nemesis::Line& x, const nemesis::Line& y) const
{
    return nemesis::NaturalSortOrder::Compare(x.c_str(), y.c_str()) < 0;
}

bool nemesis::NaturalSortDescend::operator()(const char* x, const char* y) const
{
    return nemesis::NaturalSortOrder::Compare(x, y) > 0;
}

bool nemesis::NaturalSortDescend::operator()(const std::string& x, const std::string& y) const
{
    return nemesis::NaturalSortOrder::Compare(x.c_str(), y.c_str()) > 0;
}

bool nemesis::NaturalSortDescend::operator()(const std::string_view& x, const std::string_view& y) const
{
    return nemesis::NaturalSortOrder::Compare(std::string(x).c_str(), std::string(y).c_str()) > 0;
}

bool nemesis::NaturalSortDescend::operator()(const std::wstring& x, const std::wstring& y) const
{
    return nemesis::NaturalSortOrder::Compare(x.c_str(), y.c_str()) > 0;
}

bool nemesis::NaturalSortDescend::operator()(const std::wstring_view& x, const std::wstring_view& y) const
{
    return nemesis::NaturalSortOrder::Compare(std::wstring(x).c_str(), std::wstring(y).c_str()) > 0;
}

bool nemesis::NaturalSortDescend::operator()(const nemesis::Line& x, const nemesis::Line& y) const
{
    return nemesis::NaturalSortOrder::Compare(x.c_str(), y.c_str()) > 0;
}

nemesis::NaturalSortOrder::NaturalSortOrder()
{
    Collator.setCaseSensitivity(Qt::CaseInsensitive);
    Collator.setNumericMode(true);
}

int nemesis::NaturalSortOrder::Compare(const char* x, const char* y)
{
    assert(x);
    assert(y);
    return Collator.compare(x, y);
}

int nemesis::NaturalSortOrder::Compare(const wchar_t* x, const wchar_t* y)
{
    assert(x);
    assert(y);
    return Collator.compare(x, y);
}
