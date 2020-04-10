#include "Global.h"

#include <filesystem>

#include "utilities/algorithm.h"

#include "utilities/wstrconvert.h"

#include "utilities/readtextfile.h"
#include "utilities/writetextfile.h"

#pragma warning(disable:4503)

using namespace std;
namespace sf = filesystem;

boost::atomic_flag atomLock = BOOST_ATOMIC_FLAG_INIT;

bool debug = false;
int memory = 100;
int fixedkey[257];

NemesisInfo* nemesisInfo;

boost::posix_time::ptime time1;

unordered_map<string, string> behaviorPath;

unordered_map<string, bool> activatedBehavior;
unordered_map<string, string> behaviorProjectPath;
unordered_map<string, vecstr> behaviorJoints;
unordered_map<string, vecstr> behaviorProject;
unordered_map<string, set<string>> usedAnim;
unordered_map<string, unordered_map<string, bool>> registeredAnim;
unordered_map<string, unordered_map<string, vecstr>> animList;
unordered_map<string, unordered_map<string, vector<set<string>>>> animModMatch;

unordered_map<string, string> AAGroup;
unordered_map<string, vecstr> AAEvent;
unordered_map<string, vecstr> AAHasEvent;
unordered_map<string, vecstr> groupAA;
unordered_map<string, vecstr> groupAAPrefix;
unordered_map<string, vecstr> alternateAnim;
unordered_map<string, unordered_map<string, int>> AAGroupCount;
set<string> groupNameList;

wstring StringToWString(string line)
{
    return QString::fromStdString(line).toStdWString();
}

string WStringToString(wstring line)
{
    return QString::fromStdWString(line).toStdString();
}

void read_directory(const string& name, vecstr& fv)
{
	fv.clear();

	for (const auto& entry : sf::directory_iterator(name))
	{
		wstring filename = entry.path().filename().wstring();

		if (filename != L"." && filename != L"..")
		{
			fv.push_back(WStringToString(filename));
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

void read_directory(const wstring& name, vector<wstring>& fv)
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

void read_directory(const char* name, vecstr& fv)
{
	fv.clear();

	for (const auto& entry : sf::directory_iterator(name))
	{
		wstring filename = entry.path().filename().wstring();

		if (filename != L"." && filename != L"..")
		{
			fv.push_back(WStringToString(filename));
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

void read_directory(const wchar_t* name, vector<wstring>& fv)
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

void produceBugReport(string directory, unordered_map<string, bool> chosenBehavior)
{
	boost::posix_time::ptime time1 = boost::posix_time::second_clock::local_time();
	string time = to_simple_string(time1);
	string timer;

	for (unsigned int i = 0; i < time.size(); ++i)
	{
		if (time[i] != ':')
		{
			timer.append(1, time[i]);
		}
	}

	FileWriter bugReport("Bug Report (" + timer + ").txt");

	if (bugReport.is_open())
	{
		bugReport << "File: " << directory << "\n\nChosen Behavior: {\n";

		for (auto it = chosenBehavior.begin(); it != chosenBehavior.end(); ++it)
		{
			bugReport << it->first << ",";

			if (it->second)
			{
				bugReport << "true\n";
			}
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

bool GetFunctionLines(sf::path filename, vecstr &functionlines, bool emptylast)
{
	functionlines = vecstr();

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

int sameWordCount(std::string line, std::string word)
{
    size_t nextWord = -1;
    int wordCount = 0;

    while (true) {
        nextWord = line.find(word, nextWord + 1);

        if (nextWord != NOT_FOUND) {
            wordCount++;
        } else {
            nextWord = -1;
            break;
        }
    }

    return wordCount;
}

bool isOnlyNumber(string line)
{
	try
	{
		boost::lexical_cast<double>(line);
	}
	catch (boost::bad_lexical_cast&)
	{
		return false;
	}

	return true;
}

bool hasAlpha(string line)
{
	string lower = nemesis::to_lower_copy(line);
	string upper = boost::to_upper_copy(line);

	if (lower != upper)
	{
		return true;
	}

	return false;
}

void addUsedAnim(string behaviorFile, string animPath)
{
	while (atomLock.test_and_set(boost::memory_order_acquire));

	try
	{
		usedAnim[behaviorFile].insert(animPath);
	}
	catch (const std::exception& ex)
	{
		atomLock.clear(boost::memory_order_release);
		throw ex;
	}

	atomLock.clear(boost::memory_order_release);
}
