#include "utilities/stringextension.h"


size_t GetNewPosition(const std::string& _ch, const std::string& oldvalue)
{
    return _ch.find(oldvalue);
}

size_t GetNewPosition_NC(const std::string& _ch, const std::string& oldvalue)
{
    return nemesis::to_lower_copy(_ch).find(nemesis::to_lower_copy(oldvalue));
}

size_t GetNewPosition(const std::wstring& _ch, const std::wstring& oldvalue)
{
    return _ch.find(oldvalue);
}

size_t GetNewPosition_NC(const std::wstring& _ch, const std::wstring& oldvalue)
{
    return nemesis::to_lower_copy(_ch).find(nemesis::to_lower_copy(oldvalue));
}

void StringReplace(std::string& _ch, const std::string& oldvalue, const std::string& newvalue)
{
    size_t position = GetNewPosition(_ch, oldvalue);

    while (position != NOT_FOUND)
    {
        _ch.replace(position, position + oldvalue.length(), newvalue);
        position = GetNewPosition(_ch, oldvalue);
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

    std::istringstream iss(line);
    container.assign(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
}

void StringSplit(const std::string& line, VecStr& container, char delimiter)
{
    container.clear();
    container.emplace_back();

    for (auto& each : line)
    {
        if (each == delimiter)
        {
            container.emplace_back();
            continue;
        }

        container.back().push_back(each);
    }
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

void StringReplace(std::wstring& _ch, const std::wstring& oldvalue, const std::wstring& newvalue)
{
    size_t position = GetNewPosition(_ch, oldvalue);

    while (position != NOT_FOUND)
    {
        _ch.replace(position, position + oldvalue.length(), newvalue);
        position = GetNewPosition(_ch, oldvalue);
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
