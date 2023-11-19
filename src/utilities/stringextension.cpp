#include "utilities/stringextension.h"


size_t GetNewPosition_NC(const std::string& _ch, const std::string& oldvalue)
{
    return nemesis::to_lower_copy(_ch).find(nemesis::to_lower_copy(oldvalue));
}

size_t GetNewPosition_NC(const std::wstring& _ch, const std::wstring& oldvalue)
{
    return nemesis::to_lower_copy(_ch).find(nemesis::to_lower_copy(oldvalue));
}

std::string StringTrim(const std::string& str, const std::string& whitespace)
{
    auto strBegin = str.find_first_not_of(whitespace);

    if (strBegin == NOT_FOUND) return "";

    auto strEnd   = str.find_last_not_of(whitespace);
    return str.substr(strBegin, strEnd - strBegin + 1);
}

void StringReplace(std::string& _ch, const std::string& oldvalue, const std::string& newvalue)
{
    size_t position = _ch.find(oldvalue);

    while (position != NOT_FOUND)
    {
        _ch.replace(position, position + oldvalue.length(), newvalue);
        position = _ch.find(oldvalue);
    }
}

void StringReplace_NC(std::string& _ch, const std::string& oldvalue, const std::string& newvalue)
{
    size_t position = GetNewPosition_NC(_ch, oldvalue);

    while (position != NOT_FOUND)
    {
        _ch.replace(position, position + oldvalue.length(), newvalue);
        position = GetNewPosition_NC(_ch, oldvalue);
    }
}

void StringSplit(const std::string& line, VecStr& container)
{
    if (line.empty())
    {
        container.emplace_back();
        return;
    }

    std::stringstream ss(line);
    std::string s;

    while (ss >> s)
    {
        container.emplace_back(s);
    }
}

void StringSplit(const std::string& line, VecStr& container, char delimiter)
{
    container.clear();
    std::string* last = &container.emplace_back();

    for (auto& each : line)
    {
        if (each == delimiter)
        {
            if (last->empty()) continue;

            last = &container.emplace_back();
            continue;
        }

        last->push_back(each);
    }

    if (container.size() == 1 || !last->empty()) return;

    container.pop_back();
}

void StringSplit(const std::string& line, VecStr& container, const std::string& delimiters)
{
    container.clear();
    container.emplace_back();

    for (auto& each : line)
    {
        if (delimiters.find(each) != NOT_FOUND)
        {
            container.emplace_back();
            continue;
        }

        container.back().push_back(each);
    }
}

VecStr StringSplit(const std::string& line)
{
    VecStr container;
    StringSplit(line, container);
    return container;
}

VecStr StringSplit(const std::string& line, char delimiter)
{
    VecStr container;
    StringSplit(line, container, delimiter);
    return container;
}

VecStr StringSplit(const std::string& line, const std::string& delimiters)
{
    VecStr container;
    StringSplit(line, container, delimiters);
    return container;
}

bool StringStartWith(const std::string_view& line, const std::string_view& start) noexcept
{
    return line._Starts_with(start);
}

bool StringEndWith(const std::string_view& line, const std::string_view& end) noexcept
{
    if (line.length() < end.length()) return false;

    auto pos = line.rfind(end);

    return pos != NOT_FOUND && pos == line.length() - end.length();
}

bool StringStartWith_NC(const std::string& line, const std::string& start) noexcept
{
    return StringStartWith(nemesis::to_lower_copy(line), nemesis::to_lower_copy(start));
}

bool StringEndWith_NC(const std::string& line, const std::string& end) noexcept
{
    return StringEndWith(nemesis::to_lower_copy(line), nemesis::to_lower_copy(end));
}

std::wstring StringTrim(const std::wstring& str, const std::wstring& whitespace)
{
    auto strBegin = str.find_first_not_of(whitespace);

    if (strBegin == NOT_FOUND) return L"";

    auto strEnd = str.find_last_not_of(whitespace);
    return str.substr(strBegin, strEnd - strBegin + 1);
}

void StringReplace(std::wstring& _ch, const std::wstring& oldvalue, const std::wstring& newvalue)
{
    size_t position = _ch.find(oldvalue);

    while (position != NOT_FOUND)
    {
        _ch.replace(position, position + oldvalue.length(), newvalue);
        position = _ch.find(oldvalue);
    }
}

void StringReplace_NC(std::wstring& _ch, const std::wstring& oldvalue, const std::wstring& newvalue)
{
    size_t position = GetNewPosition_NC(_ch, oldvalue);

    while (position != NOT_FOUND)
    {
        _ch.replace(position, position + oldvalue.length(), newvalue);
        position = GetNewPosition_NC(_ch, oldvalue);
    }
}

void StringSplit(const std::wstring& line, VecWstr& container)
{
    if (line.empty())
    {
        container.emplace_back(L"");
        return;
    }

    std::wistringstream iss(line);
    container.assign(std::istream_iterator<std::wstring, wchar_t>{iss},
                     std::istream_iterator<std::wstring, wchar_t>());
}

void StringSplit(const std::wstring& line, VecWstr& container, wchar_t delimiter)
{
    container.clear();
    container.emplace_back(L"");

    for (auto& each : line)
    {
        if (each == delimiter)
        {
            container.emplace_back(L"");
            continue;
        }

        container.back().push_back(each);
    }
}

void StringSplit(const std::wstring& line, VecWstr& container, const std::wstring& delimiters)
{
    container.clear();
    container.emplace_back(L"");

    for (auto& each : line)
    {
        if (delimiters.find(each) != NOT_FOUND)
        {
            container.emplace_back(L"");
            continue;
        }

        container.back().push_back(each);
    }
}

VecWstr StringSplit(const std::wstring& line)
{
    VecWstr container;
    StringSplit(line, container);
    return container;
}

VecWstr StringSplit(const std::wstring& line, wchar_t delimiter)
{
    VecWstr container;
    StringSplit(line, container, delimiter);
    return container;
}

VecWstr StringSplit(const std::wstring& line, const std::wstring& delimiters)
{
    VecWstr container;
    StringSplit(line, container, delimiters);
    return container;
}

bool StringStartWith(const std::wstring_view& line, const std::wstring_view& start) noexcept
{
    return line._Starts_with(start);
}

bool StringEndWith(const std::wstring_view& line, const std::wstring_view& end) noexcept
{
    if (line.length() < end.length()) return false;

    auto pos = line.rfind(end);

    return pos != NOT_FOUND && pos == line.length() - end.length();
}

bool StringStartWith_NC(const std::wstring& line, const std::wstring& start) noexcept
{
    return StringStartWith(nemesis::to_lower_copy(line), nemesis::to_lower_copy(start));
}

bool StringEndWith_NC(const std::wstring& line, const std::wstring& end) noexcept
{
    return StringEndWith(nemesis::to_lower_copy(line), nemesis::to_lower_copy(end));
}
