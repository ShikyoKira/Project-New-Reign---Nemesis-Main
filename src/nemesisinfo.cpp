#include "Global.h"
#include "debuglog.h"
#include "nemesisinfo.h"

#include <Windows.h>

#include <QtCore/QFile.h>
#include <QtCore/QTextStream.h>

#include "utilities/regex.h"
#include "utilities/writetextfile.h"

using namespace std;

bool SSE = false;
wstring stagePath = L"";

void NemesisInfo::iniFileUpdate()
{
    QFile file("nemesis.ini");

    if (file.open(QIODevice::Truncate | QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << QString::fromStdWString(L"SkyrimDataDirectory=" + dataPath + L"\r\n");
        stream << QString::fromStdWString(L"MaxAnimation=" + to_wstring(maxAnim) + L"\r\n");
        stream << QString::fromStdWString(L"first=" + wstring(first ? L"true" : L"false") + L"\r\n");
        stream << QString::fromStdWString(L"width=" + to_wstring(width) + L"\r\n");
        stream << QString::fromStdWString(L"height=" + to_wstring(height) + L"\r\n");
        stream << QString::fromStdWString(L"modNameWidth=" + to_wstring(modNameWidth) + L"\r\n");
        stream << QString::fromStdWString(L"authorWidth=" + to_wstring(authorWidth) + L"\r\n");
        stream << QString::fromStdWString(L"priorityWidth=" + to_wstring(priorityWidth) + L"\r\n");
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
    set<wstring> hasAuto;
    bool force = false;

    if (isFileExist("nemesis.ini"))
    {
        try
        {
            VecWstr storeline;

            if (GetFunctionLines(L"nemesis.ini", storeline))
            {
                for (auto& line : storeline)
                {
                    wstring path = nemesis::wregex_replace(
                        wstring(line), nemesis::wregex(L".*[\\s]*=[\\s]*(.*)"), wstring(L"\\1"));
                    wstring input = nemesis::to_lower_copy(nemesis::wregex_replace(
                        wstring(line), nemesis::wregex(L"(.*)[\\s]*=[\\s]*.*"), wstring(L"\\1")));

                    if (!nemesis::iequals(path, L"auto"))
                    {
                        const unordered_map<wstring, std::function<void()>> variables = 
                        {
                            {
                                L"maxanimation", 
                                [&] { maxAnim = stoi(path); }
                            },
                            {
                                L"first", 
                                [&] { first = path != L"false"; }
                            },
                            {
                                L"height", 
                                [&] { height = stoi(path); }
                            },
                            {
                                L"width", 
                                [&] { width = stoi(path); }
                            },
                            {
                                L"modNameWidth", 
                                [&] { modNameWidth = stoi(path); }
                            },
                            {
                                L"authorWidth",
                                [&] { authorWidth = stoi(path); }
                            },
                            {
                                L"priorityWidth", 
                                [&] { priorityWidth = stoi(path); }
                            },
                        };

                        if (input == L"skyrimdatadirectory")
                        {
                            if (isFileExist(path) && wordFind(path, L"data") != NOT_FOUND)
                            {
                                dataPath = path;
                                force    = true;

                                if (dataPath.back() != L'\\' && dataPath.back() != L'/')
                                {
                                    if (dataPath.find(L"\\") != NOT_FOUND)
                                    {
                                        dataPath.push_back(L'\\');
                                    }
                                    else
                                    {
                                        dataPath.push_back(L'/');
                                    }
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
                                        SSE = true;
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
                            auto it = variables.find(input);

                            if (it != variables.end())
                            {
                                it->second();
                            }
                        }
                    }
                    else if (input == L"skyrimdatadirectory" || input == L"maxanimation" || input == L"first")
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
    wstring curpath = sf::current_path().wstring();
    replace(curpath.begin(), curpath.end(), '/', '\\');

    if (nemesis::to_lower_copy(curpath).find(L"\\project new reign - nemesis\\test environment") != NOT_FOUND)
    {
        dataPath = sf::current_path().wstring() + L"\\data\\";
        curpath  = dataPath + L"Nemesis_Engine";
    }

    if (dataPath.length() == 0)
    {
        size_t pos = wordFind(curpath, L"\\Data\\");

        if (pos != NOT_FOUND)
        {
            VecWstr filelist;
            wstring skyrimDataDirect;

            {
                sf::path path(curpath);
                size_t counter = count(curpath.begin(), curpath.end(), L'\\');
                size_t i       = 0;

                while (i < counter)
                {
                    if (nemesis::iequals(path.stem().string(), "data"))
                    {
                        skyrimDataDirect = path.wstring();
                        break;
                    }

                    path = path.parent_path();
                    ++i;
                }

                read_directory(path.parent_path().wstring(), filelist);
            }

            for (auto& file : filelist)
            {
                if (nemesis::iequals(file, L"SkyrimSE.exe"))
                {
                    SSE = true;
                    break;
                }
                else if (nemesis::iequals(file, L"binkw64.dll"))
                {
                    SSE = true;
                    break;
                }
                else if (nemesis::iequals(file, L"binkw32.dll"))
                {
                    break;
                }
            }

            // get skyrim data directory from registry key
            DWORD dwType = REG_SZ;
            HKEY hKey    = 0;
            wchar_t value[1024];
            DWORD value_length = 1024;

            LPCSTR tm;

            if (SSE)
            {
                RegOpenKey(
                    HKEY_LOCAL_MACHINE, L"SOFTWARE\\Bethesda Softworks\\Skyrim Special Edition", &hKey);
            }
            else
            {
                RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Bethesda Softworks\\Skyrim", &hKey);
            }

            LONG result
                = RegQueryValueEx(hKey, L"installed path", NULL, &dwType, (LPBYTE) &value, &value_length);

            dataPath = value;
            dataPath.append(L"Data\\");

            if (result != ERROR_SUCCESS || !isFileExist(dataPath))
            {
                if (SSE)
                {
                    RegOpenKey(HKEY_LOCAL_MACHINE,
                               L"SOFTWARE\\Wow6432Node\\Bethesda Softworks\\Skyrim Special Edition",
                               &hKey);
                }
                else
                {
                    RegOpenKey(
                        HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\Bethesda Softworks\\Skyrim", &hKey);
                }

                result
                    = RegQueryValueEx(hKey, L"installed path", NULL, &dwType, (LPBYTE) &value, &value_length);
                dataPath = value;
                dataPath.append(L"Data\\");

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
                        dataPath.append(L"\\");
                    }
                }
            }
        }
        else
        {
            interMsg(L"Detected Path: " + curpath);
            DebugLogging(L"Detected Path: " + curpath);
            ErrorMessage(1008);
        }
    }

    if (!force && nemesis::to_lower_copy(dataPath + L"Nemesis_Engine") != nemesis::to_lower_copy(curpath))
    {
        ErrorMessage(6010, curpath, dataPath + L"Nemesis_Engine");
    }

    stageDirectory = stagePath.length() > 0 ? (stagePath + L"\\") : dataPath;
    iniFileUpdate();
}

const wstring& NemesisInfo::GetDataPath() const
{
    return dataPath;
}

const wstring& NemesisInfo::GetStagePath() const
{
    return stageDirectory;
}

const string& NemesisInfo::GetDataPathA() const
{
    return nemesis::transform_to<string>(dataPath);
}

const string& NemesisInfo::GetStagePathA() const
{
    return nemesis::transform_to<string>(stageDirectory);
}

const bool& NemesisInfo::IsFirst() const
{
    return first;
}

const uint& NemesisInfo::GetWidth() const
{
    return width;
}

const uint& NemesisInfo::GetHeight() const
{
    return height;
}

const uint& NemesisInfo::GetModNameWidth() const
{
    return modNameWidth;
}

const uint& NemesisInfo::GetMaxAnim() const
{
    return maxAnim;
}

const uint& NemesisInfo::GetAuthorWidth() const
{
    return authorWidth;
}

const uint& NemesisInfo::GetPriorityWidth() const
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
