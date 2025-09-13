#include <sstream>

#include "Utilities/StringExtension.h"
#include "Utilities/Algorithm.h"

size_t GetNewPosition_NC(const std::string& _ch, const std::string& oldvalue, size_t offset = 0ULL)
{
    return nemesis::to_lower_copy(_ch).find(nemesis::to_lower_copy(oldvalue), offset);
}

size_t GetNewPosition_NC(const std::wstring& _ch, const std::wstring& oldvalue, size_t offset = 0ULL)
{
    return nemesis::to_lower_copy(_ch).find(nemesis::to_lower_copy(oldvalue), offset);
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
    if (oldvalue.empty()) return;

    size_t position = _ch.find(oldvalue);

    while (position != NOT_FOUND)
    {
        _ch.replace(position, oldvalue.length(), newvalue);
        position = _ch.find(oldvalue, position + newvalue.length());
    }
}

void StringReplace_NC(std::string& _ch, const std::string& oldvalue, const std::string& newvalue)
{
    if (oldvalue.empty()) return;

    size_t position = GetNewPosition_NC(_ch, oldvalue);

    while (position != NOT_FOUND)
    {
        _ch.replace(position, oldvalue.length(), newvalue);
        position = GetNewPosition_NC(_ch, oldvalue, position + newvalue.length());
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

void StringSplit(const std::string& line, VecStr& container, const std::string& delimiter)
{
    container.clear();
    container.emplace_back();

    for (auto& each : line)
    {
        if (delimiter.find(each) != NOT_FOUND)
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

std::string StringJoin(const VecStr& list, char delimiter)
{
    if (list.empty()) return "";

    std::ostringstream os;
    os << list.front();

    for (size_t i = 1; i < list.size(); ++i)
    {
        os << delimiter << list[i];
    }

    return os.str();
}

std::string StringJoin(const VecStr& list, const std::string& delimiter)
{
    if (list.empty()) return "";

    std::ostringstream os;
    os << list.front();

    for (size_t i = 1; i < list.size(); ++i)
    {
        os << delimiter << list[i];
    }

    return os.str();
}

std::string StringJoin(const VecNstr& list, char delimiter)
{
    if (list.empty()) return "";

    std::ostringstream os;
    os << list.front().ToString();

    for (size_t i = 1; i < list.size(); ++i)
    {
        os << delimiter << list[i].ToString();
    }

    return os.str();
}

std::string StringJoin(const VecNstr& list, const std::string& delimiter)
{
    if (list.empty()) return "";

    std::ostringstream os;
    os << list.front().ToString();

    for (size_t i = 1; i < list.size(); ++i)
    {
        os << delimiter << list[i].ToString();
    }

    return os.str();
}

bool StringStartWith(const std::string_view& line, const std::string_view& start) noexcept
{
    return line.starts_with(start);
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
    if (oldvalue.empty()) return;

    size_t position = _ch.find(oldvalue);

    while (position != NOT_FOUND)
    {
        _ch.replace(position, oldvalue.length(), newvalue);
        position = _ch.find(oldvalue, position + newvalue.length());
    }
}

void StringReplace_NC(std::wstring& _ch, const std::wstring& oldvalue, const std::wstring& newvalue)
{
    if (oldvalue.empty()) return;

    size_t position = GetNewPosition_NC(_ch, oldvalue);

    while (position != NOT_FOUND)
    {
        _ch.replace(position, oldvalue.length(), newvalue);
        position = GetNewPosition_NC(_ch, oldvalue, position + newvalue.length());
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

void StringSplit(const std::wstring& line, VecWstr& container, const std::wstring& delimiter)
{
    container.clear();
    container.emplace_back(L"");

    for (auto& each : line)
    {
        if (delimiter.find(each) != NOT_FOUND)
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

VecWstr StringSplit(const std::wstring& line, const std::wstring& delimiter)
{
    VecWstr container;
    StringSplit(line, container, delimiter);
    return container;
}

std::wstring StringJoin(const VecWstr& list, wchar_t delimiter)
{
    if (list.empty()) return L"";

    std::wostringstream os;
    os << list.front();

    for (size_t i = 1; i < list.size(); ++i)
    {
        os << delimiter << list[i];
    }

    return os.str();
}

std::wstring StringJoin(const VecWstr& list, const std::wstring& delimiter)
{
    if (list.empty()) return L"";

    std::wostringstream os;
    os << list.front();

    for (size_t i = 1; i < list.size(); ++i)
    {
        os << delimiter << list[i];
    }

    return os.str();
}

std::wstring StringJoin(const VecNwstr& list, wchar_t delimiter)
{
    if (list.empty()) return L"";

    std::wostringstream os;
    os << list.front().ToWstring();

    for (size_t i = 1; i < list.size(); ++i)
    {
        os << delimiter << list[i].ToWstring();
    }

    return os.str();
}

std::wstring StringJoin(const VecNwstr& list, const std::wstring& delimiter)
{
    if (list.empty()) return L"";

    std::wostringstream os;
    os << list.front().ToWstring();

    for (size_t i = 1; i < list.size(); ++i)
    {
        os << delimiter << list[i].ToWstring();
    }

    return os.str();
}

bool StringStartWith(const std::wstring_view& line, const std::wstring_view& start) noexcept
{
    return line.starts_with(start);
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

std::string XmlStringEncode(const std::string& data)
{
    std::string buffer;
    buffer.reserve(data.size());

    for (size_t pos = 0; pos != data.size(); ++pos)
    {
        switch (data[pos])
        {
            case '&':
                buffer.append("&amp;");
                break;
            case '\"':
                buffer.append("&quot;");
                break;
            case '\'':
                buffer.append("&apos;");
                break;
            case '<':
                buffer.append("&lt;");
                break;
            case '>':
                buffer.append("&gt;");
                break;
            default:
                buffer.append(&data[pos], 1);
                break;
        }
    }

    return buffer;
}

std::wstring XmlStringEncode(const std::wstring& data)
{
    std::wstring buffer;
    buffer.reserve(data.size());

    for (size_t pos = 0; pos != data.size(); ++pos)
    {
        switch (data[pos])
        {
            case L'&':
                buffer.append(L"&amp;");
                break;
            case L'\"':
                buffer.append(L"&quot;");
                break;
            case L'\'':
                buffer.append(L"&apos;");
                break;
            case L'<':
                buffer.append(L"&lt;");
                break;
            case L'>':
                buffer.append(L"&gt;");
                break;
            default:
                buffer.append(&data[pos], 1);
                break;
        }
    }

    return buffer;
}
