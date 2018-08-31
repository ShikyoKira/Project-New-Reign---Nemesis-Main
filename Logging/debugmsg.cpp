#include "debugmsg.h"
#include "Nemesis Main GUI/master.h"

using namespace std;

int filenum;
atomic<int> progressPercentage;
bool error = false;
DebugMsg DMLog;
UpdateFilesStart* process1;
BehaviorStart* process2;
DummyLog* process3;

std::wstring_convert<std::codecvt_utf8<wchar_t>> wstrConv;

std::vector<std::string> readUTF8File(std::wstring filename);
void writeUTF8File(std::string filename, std::vector<std::string> storeline);

void NewDebugMessage(DebugMsg NewLog)
{
	DMLog = NewLog;
}

DebugMsg::DebugMsg(string language)
{
	wstring filename = L"languages\\" + wstrConv.from_bytes(language) + L".txt";
	vector<string> storeline = readUTF8File(filename);

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

vector<string> readUTF8File(wstring filename)
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
		interMsg("CRITICAL ERROR: Fail to read language pack. Please re-install Nemesis\n");
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
		interMsg("CRITICAL ERROR: Fail to write file. Please re-install Nemesis\n");
		error = true;
	}
}

string DMLogError(int errorcode)
{
	return DMLog.errorlist[errorcode];
}

string DMLogWarning(int warningcode)
{
	return DMLog.warninglist[warningcode];
}

string TextBoxMessage(int textcode)
{
	if (DMLogWarning(textcode).length() == 0)
	{
		interMsg("CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis\n");
		error = true;
		return "";
	}

	return DMLog.textlist[textcode];
}

string UIMessage(int uicode)
{
	if (DMLogWarning(uicode).length() == 0)
	{
		interMsg("CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis\n");
		error = true;
		return "";
	}

	return DMLog.uilist[uicode];
}

void interMsg(string input)
{
	if (process1 != nullptr)
	{
		process1->message(input);
	}
	else if (process2 != nullptr)
	{
		process2->message(input);
	}
	else if (process3 != nullptr)
	{
		process3->message(input);
	}
	else
	{
		QMessageBox* msgbox = new QMessageBox;
		msgbox->setWindowTitle("CRITICAL ERROR");
		msgbox->setText("Access process violtion. Running process not found. Report to Nemesis' author immediately.");
		error = true;
		msgbox->show();
	}
}

void connectProcess(UpdateFilesStart* newProcess)
{
	process1 = newProcess;
}

void connectProcess(BehaviorStart* newProcess)
{
	process2 = newProcess;
}

void connectProcess(DummyLog* newProcess)
{
	process3 = newProcess;
}

void disconnectProcess()
{
	process1 = nullptr;
	process2 = nullptr;
	process3 = nullptr;
}
