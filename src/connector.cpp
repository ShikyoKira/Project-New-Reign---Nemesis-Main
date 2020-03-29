#include "Global.h"

#include <boost/atomic.hpp>

#include <QtCore/QString>

#include "connector.h"

#include "ui/Terminator.h"

#pragma warning(disable : 4503)

using namespace std;

mutex processlock;
condition_variable cv;
bool processdone = false;
map<string, vecstr> modinfo;

vecstr hiddenMods;
atomic<int> m_RunningThread;
Terminator* p_terminate = new Terminator;

bool isRunning(Terminator*& curEvent)
{
    if (m_RunningThread == 0) return false;

    curEvent = p_terminate;
    return true;
}

bool readMod(string& errormsg)
{
    string folder = "mod\\";
    vecstr modlist;
    read_directory(folder, modlist);

    for (auto& modcode : modlist)
    {
        if (std::filesystem::is_directory(folder + modcode) && isFileExist(folder + modcode + "\\info.ini"))
        {
            string filename = folder + modcode + "\\info.ini";
            vecstr storeline;
            string name, author, site, automatic, hide;
            bool hidden = false;

            if (!GetFunctionLines(filename, storeline, false)) return false;

            for (auto& line : storeline)
            {
                while (true)
                {
                    size_t pos = wordFind(line, "name=");

                    if (pos == 0)
                    {
                        name = line.substr(line.find("=") + 1);
                        break;
                    }

                    pos = wordFind(line, "author=");

                    if (pos == 0)
                    {
                        author = line.substr(line.find("=") + 1);
                        break;
                    }

                    pos = wordFind(line, "site=");

                    if (pos == 0)
                    {
                        site = line.substr(line.find("=") + 1);
                        break;
                    }

                    pos = wordFind(line, "auto=");

                    if (pos == 0) { automatic = line.substr(line.find("=") + 1); }

                    pos = wordFind(line, "hidden=");

                    if (pos == 0)
                    {
                        hide = line.substr(line.find("=") + 1);
                        boost::to_lower(hide);
                        istringstream stream(hide);
                        stream >> boolalpha >> hidden;
                    }

                    break;
                }
            }

            if (name.length() == 0 || author.length() == 0 || site.length() == 0)
            {
                errormsg = TextBoxMessage(1014) + " " + modcode;
                return false;
            }

            if (!hidden)
            {
                modinfo[modcode].push_back(name + " (" + site + ")");
                modinfo[modcode].push_back(author);
                modinfo[modcode].push_back(automatic);
            }
            else
            {
                hiddenMods.push_back(modcode);
            }
        }
    }

    return true;
}

void DummyLog::message(std::string input)
{
    emit incomingMessage(QString::fromStdString(input));
}

vecstr getHiddenMods()
{
    return hiddenMods;
}
