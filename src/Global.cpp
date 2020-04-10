#include "Global.h"

#include <filesystem>

#include "utilities/algorithm.h"

#include "utilities/wstrconvert.h"

#include "utilities/readtextfile.h"
#include "utilities/writetextfile.h"

#pragma warning(disable : 4503)

using namespace std;
namespace sf = filesystem;

std::atomic_flag atomLock{};

bool debug = false;
int memory = 100;
int fixedkey[257];

NemesisInfo* nemesisInfo;

unordered_map<string, string> behaviorPath;

unordered_map<string, bool> activatedBehavior;
unordered_map<string, string> behaviorProjectPath;
unordered_map<string, VecStr> behaviorJoints;
unordered_map<string, VecStr> behaviorProject;
unordered_map<string, set<string>> usedAnim;
unordered_map<string, unordered_map<string, bool>> registeredAnim;
unordered_map<string, unordered_map<string, VecStr>> animList;
unordered_map<string, unordered_map<string, vector<set<string>>>> animModMatch;

unordered_map<string, string> AAGroup;
unordered_map<string, VecStr> AAEvent;
unordered_map<string, VecStr> AAHasEvent;
unordered_map<string, VecStr> groupAA;
unordered_map<string, VecStr> groupAAPrefix;
unordered_map<string, VecStr> alternateAnim;
unordered_map<string, unordered_map<string, int>> AAGroupCount;
set<string> groupNameList;

void read_directory(const string& name, VecStr& fv)
{
    fv.clear();

    for (const auto& entry : sf::directory_iterator(name))
    {
        wstring filename = entry.path().filename().wstring();

        if (filename != L"." && filename != L"..") { fv.push_back(WStringToString(filename)); }
    }

    for (uint i = 0; i < fv.size(); ++i)
    {
        if (nemesis::to_lower_copy(fv[i]).find("folder_managed_by_vortex") != NOT_FOUND)
        {
            fv.erase(fv.begin() + i);
            --i;
        }
    }
}

void read_directory(const wstring& name, vector<wstring>& fv)
{
    fv.clear();

    for (const auto& entry : sf::directory_iterator(name))
    {
        wstring filename = entry.path().filename().wstring();

        if (filename != L"." && filename != L"..") { fv.push_back(filename); }
    }

    for (uint i = 0; i < fv.size(); ++i)
    {
        if (nemesis::to_lower_copy(fv[i]).find(L"folder_managed_by_vortex") != NOT_FOUND)
        {
            fv.erase(fv.begin() + i);
            --i;
        }
    }
}

void read_directory(const char* name, VecStr& fv)
{
    fv.clear();

    for (const auto& entry : sf::directory_iterator(name))
    {
        wstring filename = entry.path().filename().wstring();

        if (filename != L"." && filename != L"..") { fv.push_back(WStringToString(filename)); }
    }

    for (uint i = 0; i < fv.size(); ++i)
    {
        if (nemesis::to_lower_copy(fv[i]).find("folder_managed_by_vortex") != NOT_FOUND)
        {
            fv.erase(fv.begin() + i);
            --i;
        }
    }
}

void read_directory(const wchar_t* name, vector<wstring>& fv)
{
    fv.clear();

    for (const auto& entry : sf::directory_iterator(name))
    {
        wstring filename = entry.path().filename().wstring();

        if (filename != L"." && filename != L"..") { fv.push_back(filename); }
    }

    for (uint i = 0; i < fv.size(); ++i)
    {
        if (nemesis::to_lower_copy(fv[i]).find(L"folder_managed_by_vortex") != NOT_FOUND)
        {
            fv.erase(fv.begin() + i);
            --i;
        }
    }
}

size_t fileLineCount(sf::path filepath)
{
    int linecount = 0;
    string line;
    FileReader input(filepath.c_str());

    if (input.GetFile())
    {
        string line;

        while (input.GetLines(line))
        {
            ++linecount;
        }
    }
    else
    {
        ErrorMessage(1002, filepath);
    }

    return linecount;
}

size_t fileLineCount(const char* filepath)
{
    int linecount = 0;
    string line;
    FileReader input(filepath);

    if (input.GetFile())
    {
        string line;

        while (input.GetLines(line))
        {
            ++linecount;
        }
    }
    else
    {
        ErrorMessage(1002, filepath);
    }

    return linecount;
}

inline std::tm localtime_xp(std::time_t timer)
{
    std::tm bt{};
#if defined(__unix__)
    localtime_r(&timer, &bt);
#elif defined(_MSC_VER)
    localtime_s(&bt, &timer);
#else
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    bt = *std::localtime(&timer);
#endif
    return bt;
}

string currentTime()
{
    using namespace std::chrono;
    std::time_t t_c = system_clock::to_time_t(system_clock::now());
    auto tm         = localtime_xp(t_c);
    return (stringstream() << std::put_time(&tm, "%F %T")).str();
}

void produceBugReport(string directory, unordered_map<string, bool> chosenBehavior)
{
    string time = currentTime();
    string timer;

    for (uint i = 0; i < time.size(); ++i)
    {
        if (time[i] != ':') { timer.append(1, time[i]); }
    }

    FileWriter bugReport("Bug Report (" + timer + ").txt");

    if (bugReport.is_open())
    {
        bugReport << "File: " << directory << "\n\nChosen Behavior: {\n";

        for (auto it = chosenBehavior.begin(); it != chosenBehavior.end(); ++it)
        {
            bugReport << it->first << ",";

            if (it->second) { bugReport << "true\n"; }
            else
            {
                bugReport << "false\n";
            }
        }

        bugReport << "}";
    }
    else
    {
        ErrorMessage(3000);
    }
}

bool GetFunctionLines(sf::path filename, VecStr& functionlines, bool emptylast)
{
    functionlines = VecStr();

    if (!sf::is_directory(filename))
    {
        functionlines.reserve(fileLineCount(filename));
        FileReader BehaviorFormat(filename.wstring());

        if (BehaviorFormat.GetFile())
        {
            wstring line;

            while (BehaviorFormat.GetLines(line))
            {
                if (error) throw nemesis::exception();

                functionlines.push_back(WStringToString(line));
            }
        }
        else
        {
            ErrorMessage(3002, filename.string());
        }
    }
    else
    {
        ErrorMessage(3001, filename.string());
    }

    if (functionlines.size() == 0) return false;

    if (emptylast)
    {
        if (functionlines.size() != 0 && functionlines.back().length() != 0
            && functionlines.back().find("<!-- CONDITION END -->") == NOT_FOUND
            && functionlines.back().find("<!-- CLOSE -->") == NOT_FOUND)
        { functionlines.push_back(""); }
    }
    else
    {
        if (functionlines.size() != 0 && functionlines.back().length() == 0) { functionlines.pop_back(); }
    }

    return true;
}

bool GetFunctionLines(sf::path filename, vector<wstring>& functionlines, bool emptylast)
{
    functionlines = vector<wstring>();

    if (!sf::is_directory(filename))
    {
        functionlines.reserve(fileLineCount(filename));
        FileReader BehaviorFormat(filename.wstring());

        if (BehaviorFormat.GetFile())
        {
            wstring line;

            while (BehaviorFormat.GetLines(line))
            {
                if (error) throw nemesis::exception();

                functionlines.push_back(line);
            }
        }
        else
        {
            ErrorMessage(3002, filename.string());
        }
    }
    else
    {
        ErrorMessage(3001, filename.string());
    }

    if (functionlines.size() == 0) return false;

    if (emptylast)
    {
        if (functionlines.size() != 0 && functionlines.back().length() != 0
            && functionlines.back().find(L"<!-- CONDITION END -->") == NOT_FOUND
            && functionlines.back().find(L"<!-- CLOSE -->") == NOT_FOUND)
        { functionlines.push_back(L""); }
    }
    else
    {
        if (functionlines.size() != 0 && functionlines.back().length() == 0) { functionlines.pop_back(); }
    }

    return true;
}

size_t wordFind(string line, string word, bool isLast)
{
    nemesis::to_lower(line);
    nemesis::to_lower(word);

    return isLast ? line.rfind(word) : line.find(word);
}

int sameWordCount(std::string line, std::string word)
{
    size_t nextWord = -1;
    int wordCount   = 0;

    while (true)
    {
        nextWord = line.find(word, nextWord + 1);

        if (nextWord != NOT_FOUND) { wordCount++; }
        else
        {
            nextWord = -1;
            break;
        }
    }

    return wordCount;
}

bool isOnlyNumber(string line)
{
    char* end  = nullptr;
    double val = strtod(line.c_str(), &end);
    return end != line.c_str() && *end == '\0' && val != HUGE_VAL;
}

bool hasAlpha(string line)
{
    string lower = nemesis::to_lower_copy(line);
    string upper = nemesis::to_upper_copy(line);

    if (lower != upper) { return true; }

    return false;
}

void addUsedAnim(string behaviorFile, string animPath)
{
    while (atomLock.test_and_set(std::memory_order_acquire))
        ;

    try
    {
        usedAnim[behaviorFile].insert(animPath);
    }
    catch (const std::exception& ex)
    {
        atomLock.clear(std::memory_order_release);
        throw ex;
    }

    atomLock.clear(std::memory_order_release);
}
