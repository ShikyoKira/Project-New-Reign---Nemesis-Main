#include <QtCore/QString>

#include "Global.h"

#include "connector.h"

#include "ui/UiModInfo.h"
#include "ui/Terminator.h"

#include "utilities/algorithm.h"

#pragma warning(disable : 4503)

using namespace std;

mutex processlock;
condition_variable cv;
bool processdone = false;
map<wstring, shared_ptr<UiModInfo>> modinfo;

VecStr hiddenMods;
atomic<int> m_RunningThread;
Terminator* p_terminate = new Terminator;

bool isRunning(Terminator*& curEvent)
{
	if (m_RunningThread == 0) return false;

	curEvent = p_terminate;
	return true;
}

bool readMod(wstring& errormsg)
{
	wstring folder = L"mod\\";
	VecWstr modlist;
	read_directory(folder, modlist);


	for (auto& modcode : modlist)
	{
		if (std::filesystem::is_directory(folder + modcode) && isFileExist(folder + modcode + L"\\info.ini"))
		{
			wstring filename = folder + modcode + L"\\info.ini";
			VecWstr storeline;
			wstring name, author, site, automatic, hide;
			bool hidden = false;

			if (!GetFunctionLines(filename, storeline, false)) return false;

			for (auto& line : storeline)
			{
				while (true)
				{
					size_t pos = wordFind(line, L"name=");

					if (pos == 0)
					{
						name = line.substr(line.find(L"=") + 1);
						break;
					}

					pos = wordFind(line, L"author=");

					if (pos == 0)
					{
						author = line.substr(line.find(L"=") + 1);
						break;
					}

					pos = wordFind(line, L"site=");

					if (pos == 0)
					{
						site = line.substr(line.find(L"=") + 1);
						break;
					}

					pos = wordFind(line, L"auto=");

					if (pos == 0)
					{
						automatic = line.substr(line.find(L"=") + 1);
					}

					pos = wordFind(line, L"hidden=");

					if (pos == 0)
					{
						hide = line.substr(line.find(L"=") + 1);
						nemesis::to_lower(hide);
						wistringstream stream(hide);
						stream >> boolalpha >> hidden;
					}

					break;
				}
			}

			if (name.length() == 0 || author.length() == 0 || site.length() == 0)
			{
				errormsg = TextBoxMessage(1014) + L" " + modcode;
				return false;
			}

			if (!hidden)
            {
                modinfo[modcode] = make_shared<UiModInfo>(name + L" (" + site + L")", author, automatic);
			}
			else
			{
				hiddenMods.push_back(nemesis::transform_to<string>(modcode));
			}
		}
	}

	return true;
}

void DummyLog::message(std::string input)
{
	emit incomingMessage(QString::fromStdString(input));
}

void DummyLog::message(std::wstring input)
{
    emit incomingMessage(QString::fromStdWString(input));
}

VecStr getHiddenMods()
{
	return hiddenMods;
}
