#include "debugmsg.h"

using namespace std;

bool error = false;
DebugMsg DLog;

void NewDebugMessage(string language)
{
	DebugMsg NewLog(language);
	DLog = NewLog;

	for (auto it = DLog.uilist.begin(); it != DLog.uilist.end(); ++it)
	{
		// if(it->first < this->buttons->size())
		// {
		//		this->buttons[it->first]->Text = it->second;
		// }
	}
}

DebugMsg::DebugMsg(string language)
{
	string directory = "languages\\";
	vector<string> storeline = readUTF8File(directory + language + ".txt");

	if (error)
	{
		return;
	}

	for (unsigned int i = 0; i < storeline.size(); ++i)
	{
		if (storeline[i][0] != '\'' && storeline[i].length() != 0)
		{
			__int64 counter = sameWordCount(storeline[i], "\\n");

			for (int j = 0; j < counter; ++j)
			{
				storeline[i].replace(storeline[i].find("\\n"), 2, "\n");
			}

			if (wordFind(storeline[i], "UIT ") == 0)
			{
				if (storeline[i].find("=") != NOT_FOUND)
				{
					string code = storeline[i].substr(4, storeline[i].find("=") - 4);

					if (isOnlyNumber(code))
					{
						uilist[stoi(code)] = storeline[i].substr(storeline[i].find("=") + 1);
					}
				}
			}
			else if (wordFind(storeline[i], "TBT ") == 0)
			{
				if (storeline[i].find("=") != NOT_FOUND)
				{
					string code = storeline[i].substr(4, storeline[i].find("=") - 4);

					if (isOnlyNumber(code))
					{
						textlist[stoi(code)] = storeline[i].substr(storeline[i].find("=") + 1);
					}
				}
			}
			else if (wordFind(storeline[i], "ERR ") == 0)
			{
				if (storeline[i].find("=") != NOT_FOUND)
				{
					string code = storeline[i].substr(4, storeline[i].find("=") - 4);

					if (isOnlyNumber(code))
					{
						errorlist[stoi(code)] = storeline[i].substr(storeline[i].find("=") + 1);
					}
				}
			}
			else if (wordFind(storeline[i], "WAR ") == 0)
			{
				if (storeline[i].find("=") != NOT_FOUND)
				{
					string code = storeline[i].substr(4, storeline[i].find("=") - 4);

					if (isOnlyNumber(code))
					{
						warninglist[stoi(code)] = storeline[i].substr(storeline[i].find("=") + 1);
					}
				}
			}
		}
	}
}

vector<string> readUTF8File(string filename)
{
	vector<string> storeline;
	ifstream file(filename);
	file.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
	string line;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			storeline.push_back(line);
		}

		file.close();
	}
	else
	{
		// this->textbox->append("CRITICAL ERROR: Fail to read language pack. Please re-install Nemesis");
		error = true;
	}

	return storeline;
}

void writeUTF8File(string filename, vector<string> storeline)
{
	ofstream output(filename);
	output.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));

	if (output.is_open())
	{
		for (unsigned int i = 0; i < storeline.size(); ++i)
		{
			output << storeline[i] << "\n";
		}

		output.close();
	}
	else
	{
		// this->textbox->append("CRITICAL ERROR: Fail to write file. Please re-install Nemesis");
		error = true;
	}
}

string DLogError(int errorcode)
{
	return DLog.errorlist[errorcode];
}

string DLogWarning(int warningcode)
{
	return DLog.warninglist[warningcode];
}
