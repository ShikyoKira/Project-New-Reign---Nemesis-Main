#include "Global.h"

#include <Windows.h>

#include <QtCore/QFile.h>
#include <QtCore/QTextStream.h>

#include "debuglog.h"
#include "nemesisinfo.h"

#include "utilities/regex.h"
#include "utilities/writetextfile.h"

using namespace std;

const bool SSE = false;
string stagePath = "";

void NemesisInfo::iniFileUpdate()
{
    QFile file("nemesis.ini");

    if (file.open(QIODevice::Truncate | QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << QString::fromStdString("SkyrimDataDirectory=" + dataPath + "\r\n");
        stream << QString::fromStdString("MaxAnimation=" + to_string(maxAnim) + "\r\n");
        stream << QString::fromStdString("first=" + string(first ? "true" : "false") + "\r\n");
        stream << QString::fromStdString("width=" + to_string(width) + "\r\n");
        stream << QString::fromStdString("height=" + to_string(height) + "\r\n");
        stream << QString::fromStdString("modNameWidth=" + to_string(modNameWidth) + "\r\n");
        stream << QString::fromStdString("authorWidth=" + to_string(authorWidth) + "\r\n");
        stream << QString::fromStdString("priorityWidth=" + to_string(priorityWidth) + "\r\n");
        stream.flush();
        file.close();
    }
}

NemesisInfo::NemesisInfo()
{
    try
    {
        setup();
    }
    catch (nemesis::exception&)
    {}
}

NemesisInfo::NemesisInfo(bool& exception)
{
    try
    {
        setup();
    }
    catch (nemesis::exception&)
    {
        exception = true;
    }
}

void NemesisInfo::setup()
{
    set<string> hasAuto;
    bool force = false;

    if (isFileExist("nemesis.ini"))
    {
        try
        {
            VecStr storeline;

            if (GetFunctionLines(string("nemesis.ini"), storeline))
            {
                for (auto& line : storeline)
                {
                    string path = nemesis::regex_replace(
                        string(line), nemesis::regex(".*[\\s]*=[\\s]*(.*)"), string("\\1"));
                    string input = nemesis::to_lower_copy(nemesis::regex_replace(
                        string(line), nemesis::regex("(.*)[\\s]*=[\\s]*.*"), string("\\1")));

                    if (!nemesis::iequals(path, "auto"))
                    {
                        const unordered_map<string, std::function<void()>> variables = 
                        {
                            {
                                "maxanimation", 
                                [&] { maxAnim = stoi(path); }
                            },
                            {
                                "first", 
                                [&] { first = path != "false"; }
                            },
                            {
                                "height", 
                                [&] { height = stoi(path); }
                            },
                            {
                                "width", 
                                [&] { width = stoi(path); }
                            },
                            {
                                "modNameWidth", 
                                [&] { modNameWidth = stoi(path); }
                            },
                            {
                                "authorWidth",
                                [&] { authorWidth = stoi(path); }
                            },
                            {
                                "priorityWidth", 
                                [&] { priorityWidth = stoi(path); }
                            },
                        };



                        if (input == "skyrimdatadirectory")
                        {
                            if (isFileExist(path) && wordFind(path, "data") != NOT_FOUND)
                            {
                                dataPath = path;
                                force    = true;

                                if (dataPath.back() != '\\' && dataPath.back() != '/')
                                {
                                    if (dataPath.find("\\") != NOT_FOUND)
                                        dataPath.push_back('\\');
                                    else
                                        dataPath.push_back('/');
                                }

                                VecStr filelist;
                                filesystem::path fspath(dataPath);

                                while (!nemesis::iequals(fspath.stem().string(), "data"))
                                {
                                    fspath = fspath.parent_path();
                                }

                                read_directory(fspath.parent_path().string(), filelist);

                                for (auto& file : filelist)
                                {
                                    if (nemesis::iequals(file, "SkyrimSE.exe")
                                        || nemesis::iequals(file, "binkw64.dll"))
                                    {
                                        bool* cheatSSE = (bool*) &SSE;
                                        *cheatSSE      = true;
                                        break;
                                    }
                                    else if (nemesis::iequals(file, "binkw32.dll"))
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                        else
                        {
                            auto& it = variables.find(input);

                            if (it != variables.end())
                            {
                                it->second();
                            }
                        }
                    }
                    else if (input == "skyrimdatadirectory" || input == "maxanimation" || input == "first")
                    {
                        hasAuto.insert(input);
                    }
                }
            }
        }
        catch (const exception&)
        {
            dataPath.clear();
        }
    }

    namespace sf   = filesystem;
    string curpath = sf::current_path().string();
    replace(curpath.begin(), curpath.end(), '/', '\\');

    if (sf::current_path().string().find("\\Project New Reign - Nemesis\\test environment") != NOT_FOUND)
    {
        dataPath = sf::current_path().string() + "\\data\\";
        curpath  = dataPath + "nemesis_engine";
    }

    if (dataPath.length() == 0)
    {
        size_t pos = wordFind(curpath, "\\Data\\");

        if (pos != NOT_FOUND)
        {
            VecStr filelist;
            string skyrimDataDirect;

            {
                sf::path path(curpath);
                size_t counter = count(curpath.begin(), curpath.end(), '\\');
                size_t i       = 0;

                while (i < counter)
                {
                    if (nemesis::iequals(path.stem().string(), "data"))
                    {
                        skyrimDataDirect = path.string();
                        break;
                    }

                    path = path.parent_path();
                    ++i;
                }

                read_directory(path.parent_path().string(), filelist);
            }

            for (auto& file : filelist)
            {
                if (nemesis::iequals(file, "SkyrimSE.exe"))
                {
                    bool* cheatSSE = (bool*) &SSE;
                    *cheatSSE      = true;
                    break;
                }
                else if (nemesis::iequals(file, "binkw64.dll"))
                {
                    bool* cheatSSE = (bool*) &SSE;
                    *cheatSSE      = true;
                    break;
                }
                else if (nemesis::iequals(file, "binkw32.dll"))
                {
                    break;
                }
            }

            // get skyrim data directory from registry key
            DWORD dwType = REG_SZ;
            HKEY hKey    = 0;
            char value[1024];
            DWORD value_length = 1024;

            LPCSTR tm;

            if (SSE)
                RegOpenKey(
                    HKEY_LOCAL_MACHINE, L"SOFTWARE\\Bethesda Softworks\\Skyrim Special Edition", &hKey);
            else
                RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Bethesda Softworks\\Skyrim", &hKey);

            LONG result
                = RegQueryValueEx(hKey, L"installed path", NULL, &dwType, (LPBYTE) &value, &value_length);

            dataPath = value;
            dataPath = dataPath + "Data\\";

            if (result != ERROR_SUCCESS || !isFileExist(dataPath))
            {
                if (SSE)
                    RegOpenKey(HKEY_LOCAL_MACHINE,
                               L"SOFTWARE\\Wow6432Node\\Bethesda Softworks\\Skyrim Special Edition",
                               &hKey);
                else
                    RegOpenKey(
                        HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\Bethesda Softworks\\Skyrim", &hKey);

                result
                    = RegQueryValueEx(hKey, L"installed path", NULL, &dwType, (LPBYTE) &value, &value_length);
                dataPath = value;
                dataPath = dataPath + "Data\\";

                // data path directly from address
                if (result != ERROR_SUCCESS || !isFileExist(dataPath))
                {
                    dataPath = skyrimDataDirect;

                    if (dataPath.back() == '\\')
                    {
                        interMsg(dataPath);
                        ErrorMessage(6005);
                    }
                    else
                    {
                        dataPath.append("\\");
                    }
                }
            }
        }
        else
        {
            interMsg("Detected Path: " + curpath);
            DebugLogging("Detected Path: " + curpath);
            ErrorMessage(1008);
        }
    }

    if (!force && nemesis::to_lower_copy(dataPath + "nemesis_engine") != nemesis::to_lower_copy(curpath))
        ErrorMessage(6010, curpath, dataPath + "nemesis_engine");

    if (stagePath.length() > 0)
    {
        sf::path stage(stagePath);
        dataPath = stage.string() + "\\";
    }

    iniFileUpdate();
}

string NemesisInfo::GetDataPath() const
{
    return dataPath;
}

bool NemesisInfo::IsFirst() const
{
    return first;
}

uint NemesisInfo::GetWidth() const
{
    return width;
}

uint NemesisInfo::GetHeight() const
{
    return height;
}

uint NemesisInfo::GetModNameWidth() const
{
    return modNameWidth;
}

uint NemesisInfo::GetMaxAnim() const
{
    return maxAnim;
}

uint NemesisInfo::GetAuthorWidth() const
{
    return authorWidth;
}

uint NemesisInfo::GetPriorityWidth() const
{
    return priorityWidth;
}

void NemesisInfo::setFirst(bool _first)
{
    first = _first;
}

void NemesisInfo::setWidth(uint _width)
{
    width = _width;
}

void NemesisInfo::setHeight(uint _height)
{
    height = _height;
}

void NemesisInfo::setModNameWidth(uint _width)
{
    modNameWidth = _width;
}

void NemesisInfo::setAuthorWidth(uint _width)
{
    authorWidth = _width;
}

void NemesisInfo::setPriorityWidth(uint _width)
{
    priorityWidth = _width;
}
