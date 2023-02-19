#include <filesystem>

#include "Global.h"

#include "utilities/line.h"
#include "utilities/algorithm.h"
#include "utilities/readtextfile.h"
#include "utilities/writetextfile.h"

#pragma warning(disable:4503)

using namespace std;
namespace sf = filesystem;

atomic_flag atomLock{};

bool debug = false;
int memory = 100;
int fixedkey[257];

chrono::steady_clock::time_point time1;

unordered_map<wstring, wstring> behaviorPath;

unordered_map<string, bool> activatedBehavior;
unordered_map<wstring, wstring> behaviorProjectPath;
unordered_map<string, VecStr> behaviorJoints;
unordered_map<string, VecStr> behaviorProject;
unordered_map<string, set<string>> usedAnim;
unordered_map<string, set<string>> registeredAnim;
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

template<typename LineType>
std::shared_ptr<FileReader> CreateReader(const sf::path& filename, Vec<LineType>& functionlines)
{
    if (sf::is_directory(filename)) ErrorMessage(3001, filename);

    SPtr<FileReader> filereader = std::make_shared<FileReader>(filename.wstring());

    if (!filereader->GetFile()) ErrorMessage(3002, filename);

    functionlines = Vec<LineType>();
    functionlines.reserve(100000);
    return filereader;
}

template<typename LineType>
bool ValidateEndLine(bool emptylast, Vec<LineType>& storeline, const Vec<LineType>& exclusions)
{
    if (storeline.size() == 0) return false;

    if (emptylast)
    {
        if (storeline.back().empty()) return true;

        for (auto& each : exclusions)
        {
            if (storeline.back().find(each) != NOT_FOUND) return true;
        }

        LineType line;
        storeline.emplace_back(line);
    }
    else if (storeline.back().empty())
    {
        storeline.pop_back();
    }

    return true;
}

template <typename LineType>
bool ValidateEndLine(bool emptylast, Vec<LineType>& storeline, const Vec<LineType>& exclusions, size_t linenum)
{
    if (storeline.size() == 0) return false;

    if (emptylast)
    {
        if (storeline.back().empty()) return true;

        for (auto& each : exclusions)
        {
            if (storeline.back().find(each) != NOT_FOUND) return true;
        }

        LineType line(linenum);
        storeline.emplace_back(line);
    }
    else if (storeline.back().empty())
    {
        storeline.pop_back();
    }

    return true;
}

void read_directory(sf::path dirpath, VecStr& fv)
{
	fv.clear();

    if (dirpath.string().empty())
    {
        dirpath = sf::current_path();
    }

	for (const auto& entry : sf::directory_iterator(dirpath))
	{
		wstring filename = entry.path().filename().wstring();

		if (filename == L"." || filename == L"..") continue;

        if (nemesis::isearch(filename, L"folder_managed_by_vortex") != NOT_FOUND) continue;

        fv.emplace_back(nemesis::transform_to(filename));
	}
}

void read_directory(sf::path dirpath, VecWstr& fv)
{
	fv.clear();

    if (dirpath.string().empty())
    {
        dirpath = sf::current_path();
    }

	for (const auto& entry : sf::directory_iterator(dirpath))
	{
		wstring filename = entry.path().filename().wstring();

        if (filename == L"." || filename == L"..") continue;

        if (nemesis::isearch(filename, L"folder_managed_by_vortex") != NOT_FOUND) continue;

		fv.emplace_back(filename);
	}
}

size_t fileLineCount(sf::path filepath)
{
	int linecount = 0;
	FileReader input(filepath.c_str());

	if (!input.GetFile()) ErrorMessage(1002, filepath);

    string line;

    while (input.GetLines(line))
    {
        ++linecount;
    }

	return linecount;
}

size_t fileLineCount(const char* filepath)
{
	int linecount = 0;
	FileReader input(filepath);

	if (!input.GetFile()) ErrorMessage(1002, filepath);

    string line;

    while (input.GetLines(line))
    {
        ++linecount;
    }

	return linecount;
}

int sameWordCount(string line, string word)
{
	size_t nextWord = -1;
	int wordCount = 0;

	while (true)
	{
		nextWord = line.find(word, nextWord + 1);

		if (nextWord == NOT_FOUND)
        {
            nextWord = -1;
            break;
		}

        wordCount++;
	}

	return wordCount;
}

int sameWordCount(wstring line, wstring word)
{
    size_t nextWord = -1;
    int wordCount   = 0;

    while (true)
    {
        nextWord = line.find(word, nextWord + 1);

        if (nextWord == NOT_FOUND)
        {
            nextWord = -1;
            break;
        }

        wordCount++;
    }

    return wordCount;
}

//std::mutex mtxreader;
static const VecStr condend      = {"<!-- CONDITION END -->", "<!-- CLOSE -->"};
static const VecWstr wcondend    = {L"<!-- CONDITION END -->", L"<!-- CLOSE -->"};
static const VecNstr n_condend   = {"<!-- CONDITION END -->", "<!-- CLOSE -->"};
static const VecNwstr wn_condend = {L"<!-- CONDITION END -->", L"<!-- CLOSE -->"};

bool GetFileLines(const sf::path& filename,
                  VecNstr& functionlines,
                  std::function<const std::string(const std::string&)> selector,
                  bool emptylast)
{
    SPtr<FileReader> BehaviorFormat = CreateReader(filename, functionlines);
    QString line;
    size_t linenum = 0;

    if (BehaviorFormat->GetLines(line))
    {
        auto filepath_ptr = std::make_shared<nemesis::SharableWrapper<sf::path>>(filename);
        functionlines.emplace_back(line, ++linenum, filepath_ptr);

        while (BehaviorFormat->GetLines(line))
        {
            if (error) throw nemesis::exception();

            functionlines.emplace_back(selector(line.toStdString()), ++linenum, filepath_ptr.get());
        }
    }

    bool rst = ValidateEndLine(emptylast, functionlines, n_condend);
    functionlines.shrink_to_fit();
    return rst;
}

bool GetFileLines(const sf::path& filename,
                  VecNwstr& functionlines,
                  std::function<const std::wstring(const std::wstring&)> selector,
                  bool emptylast)
{
    SPtr<FileReader> BehaviorFormat = CreateReader(filename, functionlines);
    QString line;
    size_t linenum = 0;

    if (BehaviorFormat->GetLines(line))
    {
        auto filepath_ptr = std::make_shared<nemesis::SharableWrapper<sf::path>>(filename);
        functionlines.emplace_back(line, ++linenum, filepath_ptr);

        while (BehaviorFormat->GetLines(line))
        {
            if (error) throw nemesis::exception();

            functionlines.emplace_back(selector(line.toStdWString()), ++linenum, filepath_ptr.get());
        }
    }

    bool rst = ValidateEndLine(emptylast, functionlines, wn_condend);
    functionlines.shrink_to_fit();
    return rst;
}

bool GetFileLines(const sf::path& filename,
                  VecStr& functionlines,
                  std::function<const std::string(const std::string&)> selector,
                  bool emptylast)
{
    SPtr<FileReader> BehaviorFormat = CreateReader(filename, functionlines);
    QString line;

    while (BehaviorFormat->GetLines(line))
    {
        if (error) throw nemesis::exception();

        functionlines.emplace_back(selector(line.toStdString()));
    }

    bool rst = ValidateEndLine(emptylast, functionlines, condend);
    functionlines.shrink_to_fit();
    return rst;
}

bool GetFileLines(const sf::path& filename,
                  VecWstr& functionlines,
                  std::function<const std::wstring(const std::wstring&)> selector,
                  bool emptylast)
{
    SPtr<FileReader> BehaviorFormat = CreateReader(filename, functionlines);
    QString line;

    while (BehaviorFormat->GetLines(line))
    {
        if (error) throw nemesis::exception();

        functionlines.emplace_back(selector(line.toStdWString()));
    }

    bool rst = ValidateEndLine(emptylast, functionlines, wcondend);
    functionlines.shrink_to_fit();
    return rst;
}

bool GetFileLines(const sf::path& filename,
                  VecNstr& functionlines,
                  std::function<bool(std::string&)> predicament,
                  bool emptylast)
{
    SPtr<FileReader> BehaviorFormat = CreateReader(filename, functionlines);
    std::string line;
    size_t linenum = 0;

    if (BehaviorFormat->GetLines(line))
    {
        auto filepath_ptr = std::make_shared<nemesis::SharableWrapper<sf::path>>(filename);
        functionlines.emplace_back(line, ++linenum, filepath_ptr);

        while (BehaviorFormat->GetLines(line))
        {
            if (error) throw nemesis::exception();

            if (!predicament(line)) continue;

            functionlines.emplace_back(line, ++linenum, filepath_ptr.get());
        }
    }

    bool rst = ValidateEndLine(emptylast, functionlines, n_condend);
    functionlines.shrink_to_fit();
    return rst;
}

bool GetFileLines(const sf::path& filename,
                  VecNwstr& functionlines,
                  std::function<bool(std::wstring&)> predicament,
                  bool emptylast)
{
    SPtr<FileReader> BehaviorFormat = CreateReader(filename, functionlines);
    std::wstring line;
    size_t linenum = 0;

    if (BehaviorFormat->GetLines(line))
    {
        auto filepath_ptr = std::make_shared<nemesis::SharableWrapper<sf::path>>(filename);
        functionlines.emplace_back(line, ++linenum, filepath_ptr);

        while (BehaviorFormat->GetLines(line))
        {
            if (error) throw nemesis::exception();

            if (!predicament(line)) continue;

            functionlines.emplace_back(line, ++linenum, filepath_ptr.get());
        }
    }

    bool rst = ValidateEndLine(emptylast, functionlines, wn_condend);
    functionlines.shrink_to_fit();
    return rst;
}

bool GetFileLines(const sf::path& filename,
                  VecStr& functionlines,
                  std::function<bool(std::string&)> predicament,
                  bool emptylast)
{
    SPtr<FileReader> BehaviorFormat = CreateReader(filename, functionlines);
    std::string line;

    while (BehaviorFormat->GetLines(line))
    {
        if (error) throw nemesis::exception();

        if (!predicament(line)) continue;

        functionlines.emplace_back(line);
    }

    bool rst = ValidateEndLine(emptylast, functionlines, condend);
    functionlines.shrink_to_fit();
    return rst;
}

bool GetFileLines(const sf::path& filename,
                  VecWstr& functionlines,
                  std::function<bool(std::wstring&)> predicament,
                  bool emptylast)
{
    SPtr<FileReader> BehaviorFormat = CreateReader(filename, functionlines);
    wstring line;

    while (BehaviorFormat->GetLines(line))
    {
        if (error) throw nemesis::exception();

        if (!predicament(line)) continue;

        functionlines.emplace_back(line);
    }

    bool rst = ValidateEndLine(emptylast, functionlines, wcondend);
    functionlines.shrink_to_fit();
    return rst;
}

bool GetFileLines(const sf::path& filename, VecNstr& functionlines, bool emptylast)
{
    SPtr<FileReader> BehaviorFormat = CreateReader(filename, functionlines);
    QString line;
    size_t linenum = 0;

    if (BehaviorFormat->GetLines(line))
    {
        auto filepath_ptr = std::make_shared<nemesis::SharableWrapper<sf::path>>(filename);
        functionlines.emplace_back(line, ++linenum, filepath_ptr);

        while (BehaviorFormat->GetLines(line))
        {
            if (error) throw nemesis::exception();

            functionlines.emplace_back(line, ++linenum, filepath_ptr.get());
        }
    }

    bool rst = ValidateEndLine(emptylast, functionlines, n_condend);
    functionlines.shrink_to_fit();
    return rst;
}

bool GetFileLines(const sf::path& filename, VecNwstr& functionlines, bool emptylast)
{
    SPtr<FileReader> BehaviorFormat = CreateReader(filename, functionlines);
    QString line;
    size_t linenum = 0;

    if (BehaviorFormat->GetLines(line))
    {
        auto filepath_ptr = std::make_shared<nemesis::SharableWrapper<sf::path>>(filename);
        functionlines.emplace_back(line, ++linenum, filepath_ptr);

        while (BehaviorFormat->GetLines(line))
        {
            if (error) throw nemesis::exception();

            functionlines.emplace_back(line, ++linenum, filepath_ptr.get());
        }
    }

    bool rst = ValidateEndLine(emptylast, functionlines, wn_condend);
    functionlines.shrink_to_fit();
    return rst;
}

bool GetFileLines(const sf::path& filename, VecStr& functionlines, bool emptylast)
{
    SPtr<FileReader> BehaviorFormat = CreateReader(filename, functionlines);
    QString line;

    while (BehaviorFormat->GetLines(line))
    {
        if (error) throw nemesis::exception();

        functionlines.emplace_back(line.toStdString());
    }

    bool rst = ValidateEndLine(emptylast, functionlines, condend);
    functionlines.shrink_to_fit();
    return rst;
}

bool GetFileLines(const sf::path& filename, VecWstr& functionlines, bool emptylast)
{
    SPtr<FileReader> BehaviorFormat = CreateReader(filename, functionlines);
    QString line;

    while (BehaviorFormat->GetLines(line))
    {
        if (error) throw nemesis::exception();

        functionlines.emplace_back(line.toStdWString());
    }

    bool rst = ValidateEndLine(emptylast, functionlines, wcondend);
    functionlines.shrink_to_fit();
    return rst;
}

size_t wordFind(string line, string word, bool isLast)
{
	nemesis::to_lower(line);
	nemesis::to_lower(word);

	return isLast ? line.rfind(word) : line.find(word);
}

size_t wordFind(wstring line, wstring word, bool isLast)
{
    nemesis::to_lower(line);
    nemesis::to_lower(word);

    return isLast ? line.rfind(word) : line.find(word);
}

bool isOnlyNumber(string line)
{
	char* end = nullptr;
	double val = strtod(line.c_str(), &end);
	return end != line.c_str() && *end == '\0' && val != HUGE_VAL;
}

bool hasAlpha(string line)
{
	return nemesis::to_lower_copy(line) != nemesis::to_upper_copy(line);
}

bool isOnlyNumber(wstring line)
{
    wchar_t* end  = nullptr;
    double val = wcstod(line.c_str(), &end);
    return end != line.c_str() && *end == '\0' && val != HUGE_VAL;
}

bool hasAlpha(wstring line)
{
    return nemesis::to_lower_copy(line) != nemesis::to_upper_copy(line);
}

void addUsedAnim(string behaviorFile, string animPath)
{
	Lockless lock(atomLock);
	usedAnim[nemesis::to_lower_copy(behaviorFile)].insert(nemesis::to_lower_copy(animPath));
}
