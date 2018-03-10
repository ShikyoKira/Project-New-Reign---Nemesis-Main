#include "Global.h"

using namespace std;

bool debug = false;
bool error = false;
int memory = 100;

boost::posix_time::ptime time1;
unordered_map<string, string> behaviorPath;

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
	char line[5000];
	FILE* input;
	fopen_s(&input, filepath.c_str(), "r");
	if (input)
	{
		while (fgets(line, 5000, input))
		{
			linecount++;
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

	for (unsigned int i = 0; i < time.size(); i++)
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

inline string randomStringGenerator()
{
	string line;

	int randomLength = rand() % 16 + 5;

	for (int i = 0; i < randomLength; i++)
	{
		char key = (char)(rand() % 127);

		while (key == '\n' || key == '\x1a' || key == '\0' || key == '\r')
		{
			key = (char)(rand() % 127);
		}
		
		line.append(1, key);
	}

	return line;
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

inline vecchar getKey(string keyline)
{
	vecchar key;
	size_t linelen = keyline.length();

	key.reserve(linelen);

	for (size_t i = 0; i < linelen; i++)
	{
		key.push_back(keyline[i]);
	}

	return key;
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

		char line[5000];
		FILE* BehaviorFormat;
		fopen_s(&BehaviorFormat, filename.c_str(), "r");

		if (BehaviorFormat)
		{
			while (fgets(line, 5000, BehaviorFormat))
			{
				if (line[strlen(line) - 1] == '\n')
				{
					line[strlen(line) - 1] = '\0';
				}

				functionlines.push_back(line);
			}

			fclose(BehaviorFormat);
		}
		else
		{
			cout << "ERROR(3002): Failed to open behavior template" << endl << "File: " << filename << endl << endl;
			error = true;
		}
	}
	else
	{
		cout << "ERROR(3003): Unknown behavior template" << endl << "File: " << filename << endl << endl;
		error = true;
	}

	return functionlines;
}
