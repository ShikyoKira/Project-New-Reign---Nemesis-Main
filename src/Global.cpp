#include <filesystem>

#include "Global.h"

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

void read_directory(const sf::path& name, VecStr& fv)
{
	fv.clear();

	for (const auto& entry : sf::directory_iterator(name))
	{
		wstring filename = entry.path().filename().wstring();

		if (filename != L"." && filename != L"..")
		{
			fv.push_back(nemesis::transform_to<string>(filename));
		}
	}

	for (unsigned int i = 0; i < fv.size(); ++i)
	{
		if (nemesis::to_lower_copy(fv[i]).find("folder_managed_by_vortex") != NOT_FOUND)
		{
			fv.erase(fv.begin() + i);
			--i;
		}
	}
}

void read_directory(const sf::path& name, vector<wstring>& fv)
{
	fv.clear();

	for (const auto& entry : sf::directory_iterator(name))
	{
		wstring filename = entry.path().filename().wstring();

		if (filename != L"." && filename != L"..")
		{
			fv.push_back(filename);
		}
	}

	for (unsigned int i = 0; i < fv.size(); ++i)
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

int sameWordCount(string line, string word)
{
	size_t nextWord = -1;
	int wordCount = 0;

	while (true)
	{
		nextWord = line.find(word, nextWord + 1);

		if (nextWord != NOT_FOUND)
		{
			wordCount++;
		}
		else
		{
			nextWord = -1;
			break;
		}
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

        if (nextWord != NOT_FOUND)
        {
            wordCount++;
        }
        else
        {
            nextWord = -1;
            break;
        }
    }

    return wordCount;
}

bool GetFunctionLines(sf::path filename, VecStr& functionlines, bool emptylast)
{
	functionlines = VecStr();

	if (sf::is_directory(filename)) ErrorMessage(3001, filename.string());

    functionlines.reserve(fileLineCount(filename));
    FileReader BehaviorFormat(filename.wstring());

    if (!BehaviorFormat.GetFile()) ErrorMessage(3002, filename.string());

    wstring line;

    while (BehaviorFormat.GetLines(line))
    {
        if (error) throw nemesis::exception();

        functionlines.push_back(nemesis::transform_to<string>(line));
    }

	if (functionlines.size() == 0) return false;

	if (emptylast)
	{
		if (functionlines.size() != 0 && functionlines.back().length() != 0 && functionlines.back().find("<!-- CONDITION END -->") == NOT_FOUND && functionlines.back().find("<!-- CLOSE -->") == NOT_FOUND)
		{
			functionlines.push_back("");
		}
	}
	else
	{
		if (functionlines.size() != 0 && functionlines.back().length() == 0)
		{
			functionlines.pop_back();
		}
	}

	return true;
}

bool GetFunctionLines(sf::path filename, vector<wstring>& functionlines, bool emptylast)
{
	functionlines = vector<wstring>();

	if (sf::is_directory(filename)) ErrorMessage(3001, filename.string());

    functionlines.reserve(fileLineCount(filename));
    FileReader BehaviorFormat(filename.wstring());

    if (!BehaviorFormat.GetFile()) ErrorMessage(3002, filename.string());

    wstring line;

    while (BehaviorFormat.GetLines(line))
    {
        if (error) throw nemesis::exception();

        functionlines.push_back(line);
    }

	if (functionlines.size() == 0) return false;

	if (emptylast)
	{
		if (functionlines.size() != 0 && functionlines.back().length() != 0 && functionlines.back().find(L"<!-- CONDITION END -->") == NOT_FOUND &&
			functionlines.back().find(L"<!-- CLOSE -->") == NOT_FOUND)
		{
			functionlines.push_back(L"");
		}
	}
	else
	{
		if (functionlines.size() != 0 && functionlines.back().length() == 0)
		{
			functionlines.pop_back();
		}
	}

	return true;
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
