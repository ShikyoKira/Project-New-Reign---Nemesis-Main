#include "Global.h"

#pragma warning(disable:4503)

using namespace std;

bool debug = false;
bool error = false;
int memory = 100;
int fixedkey[257];

#ifdef DEBUG
DataPath skyrimDataPath;
#endif

boost::posix_time::ptime time1;
unordered_map<string, string> behaviorPath;
unordered_map<string, string> AAGroup;
unordered_map<string, unordered_map<string, vector<string>>> animList;
unordered_map<string, unordered_map<string, vector<set<string>>>> animModMatch;
unordered_map<string, unordered_map<string, bool>> registeredAnim;
unordered_map<string, set<string>> usedAnim;
unordered_map<string, set<string>> characterHeaders;
unordered_map<string, vecstr> behaviorJoints;
unordered_map<string, bool> activatedBehavior;

unordered_map<string, vecstr> alternateAnim;
unordered_map<string, vecstr> groupAA;
unordered_map<string, vecstr> groupAAPrefix;
unordered_map<string, vecstr> AAEvent;
unordered_map<string, vecstr> AAHasEvent;
unordered_map<string, unordered_map<string, int>> AAGroupCount;
vecstr groupNameList;

struct path_leaf_string
{
	string operator()(const boost::filesystem::directory_entry& entry) const
	{
		return entry.path().leaf().string();
	}
};

void read_directory(const string& name, vecstr& fv)
{
	boost::filesystem::path p(name);
	boost::filesystem::directory_iterator start(p);
	boost::filesystem::directory_iterator end;
	transform(start, end, back_inserter(fv), path_leaf_string());
}

inline bool isFileExist(const string& filename)
{
	struct stat buffer;
	return (stat(filename.c_str(), &buffer) == 0);
}

size_t fileLineCount(string filepath)
{
	int linecount = 0;
	char line[2000];
	FILE* input;
	fopen_s(&input, filepath.c_str(), "r");

	if (input)
	{
		while (fgets(line, 2000, input))
		{
			++linecount;
		}

		fclose(input);
		return linecount;
	}
	else
	{
		cout << "ERROR(1002): Failed to compute file size" << endl << "File: " << filepath << endl << endl;
		error = true;
		return 0;
	}
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

	ofstream bugReport("Bug Report (" + timer + ").txt");

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
		bugReport.close();
	}
	else
	{
		cout << "ERROR(3000): Fail to create bug report. Report to Nemesis' author." << endl;
		error = true;
	}
}

inline int sameWordCount(string line, string word)
{
	size_t nextWord = 0;
	int wordCount = 0;
	while (true)
	{
		nextWord = line.find(word, nextWord + 1);
		if (nextWord != -1)
		{
			wordCount++;
		}
		else
		{
			break;
		}
	}
	return wordCount;
}

vecstr GetFunctionLines(string filename)
{
	vecstr functionlines;
	string fileformat = filename.substr(filename.find_last_of(".") + 1);

	if (fileformat == "txt")
	{
		functionlines.reserve(fileLineCount(filename));

		if (error)
		{
			return functionlines;
		}

		char line[2000];
		FILE* BehaviorFormat;
		fopen_s(&BehaviorFormat, filename.c_str(), "r");

		if (BehaviorFormat)
		{
			while (fgets(line, 2000, BehaviorFormat))
			{
				int size = strlen(line);

				if (size != 0 && line[size - 1] == '\n')
				{
					line[size - 1] = '\0';
				}

				functionlines.push_back(line);
			}

			fclose(BehaviorFormat);
		}
		else
		{
			cout << "ERROR(3002): Failed to open file" << endl << "File: " << filename << endl << endl;
			error = true;
		}
	}
	else
	{
		cout << "ERROR(3001): Unknown file format" << endl << "File: " << filename << endl << endl;
		error = true;
	}

	if (functionlines.size() != 0 && functionlines.back().length() != 0 && functionlines.back().find("<!-- CONDITION END -->") == string::npos && functionlines.back().find("<!-- CLOSE -->") == string::npos)
	{
		functionlines.push_back("");
	}

	return functionlines;
}

size_t wordFind(string line, string word, bool isLast)
{
	if (hasAlpha(line))
	{
		boost::algorithm::to_lower(line);
	}

	if (hasAlpha(word))
	{
		boost::algorithm::to_lower(word);
	}

	if (line.find(word) == string::npos)
	{
		return -1;
	}

	if (isLast)
	{
		size_t pos = 0;
		__int64 ref = sameWordCount(line, word);

		for (__int64 i = 0; i < ref; ++i)
		{
			pos = line.find(word, pos + 1);
		}

		return pos;
	}

	return line.find(word);
}

bool isOnlyNumber(string line)
{
	for (unsigned int j = 0; j < line.length(); ++j)
	{
		if (!isalnum(line[j]) || isalpha(line[j]))
		{
			return false;
		}
	}

	return true;
}

bool hasAlpha(string line)
{
	for (unsigned int j = 0; j < line.length(); ++j)
	{
		if (isalpha(line[j]))
		{
			return true;
		}
	}

	return false;
}
