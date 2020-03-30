#include "Global.h"

#include "utilities/stringsplit.h"

using namespace std;

void StringSplit(const string& line, vecstr& container)
{
    std::istringstream iss(line);
    container = vecstr(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
}

void StringSplit(const string& line, vecstr& container, const string& anyChar)
{
    container.clear();
    container.push_back(string());

    for (auto& each : line)
    {
        if (anyChar.find(each) != NOT_FOUND) { container.push_back(string()); }
        else
        {
            container.back().push_back(each);
        }
    }
}
