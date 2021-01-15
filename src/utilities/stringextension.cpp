#include "utilities/stringextension.h"

using namespace std;

void StringReplace(string& _ch, const string& oldvalue, const string& newvalue)
{
    auto& GetNewPosition = [&]() { return _ch.find(oldvalue); };

    uint position = GetNewPosition();

    while (position != NOT_FOUND)
    {
        _ch.replace(position, position + oldvalue.length(), newvalue);
        GetNewPosition();
    }
}

void StringReplace_NC(string& _ch, const string& oldvalue, const string& newvalue)
{
    auto& GetNewPosition
        = [&]() { return nemesis::to_lower_copy(_ch).find(nemesis::to_lower_copy(oldvalue)); };

    uint position = GetNewPosition();

    while (position != NOT_FOUND)
    {
        _ch.replace(position, position + oldvalue.length(), newvalue);
        GetNewPosition();
    }
}

void StringSplit(const string& line, VecStr& container)
{
    if (line.length() == 0)
    {
        container.emplace_back("");
        return;
    }

    istringstream iss(line);
    container.assign(istream_iterator<string>{iss}, istream_iterator<string>());
}

void StringSplit(const std::string& line, VecStr& container, char delimiter)
{
    container.clear();
    container.emplace_back("");

    for (auto& each : line)
    {
        if (each == delimiter)
        {
            container.emplace_back("");
        }
        else
        {
            container.back().push_back(each);
        }
    }
}

void StringSplit(const string& line, VecStr& container, const string& delimiters)
{
    container.clear();
    container.emplace_back("");

    for (auto& each : line)
    {
        if (delimiters.find(each) != NOT_FOUND)
        {
            container.emplace_back("");
        }
        else
        {
            container.back().push_back(each);
        }
    }
}

VecStr StringSplit(const string& line)
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

VecStr StringSplit(const string& line, const string& delimiters)
{
    VecStr container;
    StringSplit(line, container, delimiters);
    return container;
}

bool StringStartWith(const string& line, const string& start) noexcept
{
    return line.find(start) == 0;
}

bool StringEndWith(const string& line, const string& end) noexcept
{
    if (line.length() < end.length()) return false;

    uint pos = line.rfind(end);

    return pos != NOT_FOUND && pos == line.length() - end.length();
}

bool StringStartWith_NC(const string& line, const string& start) noexcept
{
    return StringStartWith(nemesis::to_lower_copy(line), nemesis::to_lower_copy(start));
}

bool StringEndWith_NC(const string& line, const string& end) noexcept
{
    return StringEndWith(nemesis::to_lower_copy(line), nemesis::to_lower_copy(end));
}
