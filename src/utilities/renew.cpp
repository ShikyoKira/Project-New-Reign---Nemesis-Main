#include "Global.h"

#include "utilities/renew.h"
#include "utilities/algorithm.h"

#include "generate/hkxcompiler.h"
#include "generate/papyruscompile.h"
#include "generate/behaviorprocess_utility.h"

#pragma warning(disable : 4503)

using namespace std;

namespace sf = std::filesystem;

void tryDelete(const wstring& file, bool xml, int retry = 0)
{
    try
    {
        if (!sf::remove(file)) tryDelete(file, xml, --retry);
    }
    catch (const std::exception&)
    {
        if (retry > 0)
        {
            tryDelete(file, xml, --retry);
            return;
        }

        if (xml)
        {
            WarningMessage(1009, file);
            return;
        }

        WarningMessage(1006);
    }
}

bool DeleteFileFolder(const wstring& directory, const wstring& file, bool xml)
{
    if (sf::is_directory(directory + file))
    {
        wstring tempbehavior = directory + file;
        VecWstr filelist;
        read_directory(tempbehavior, filelist);
        tempbehavior.append(L"\\");

        for (auto& curfile : filelist)
        {
            DeleteFileFolder(tempbehavior, curfile, xml);
        }
    }

    tryDelete(directory + file, xml);
    return true;
}

void ClearTempBehaviors()
{
    VecWstr filelist;
    wstring tempbehavior = getTempBhvrPath().wstring();

    if (isFileExist(tempbehavior) && sf::is_directory(tempbehavior))
    {
        read_directory(tempbehavior, filelist);
        tempbehavior.append(L"\\");

        for (auto& file : filelist)
        {
            if (!nemesis::iequals(file, L"xml")) DeleteFileFolder(tempbehavior, file, false);
        }
    }
}

void ClearTempXml()
{
    VecWstr filelist;
    wstring tempbehavior = getTempBhvrPath().wstring() + L"\\xml";

    if (isFileExist(tempbehavior) && sf::is_directory(tempbehavior))
    {
        read_directory(tempbehavior, filelist);
        tempbehavior.append(L"\\");

        for (auto& file : filelist)
        {
            DeleteFileFolder(tempbehavior, file, true);
        }
    }

    if (isFileExist(papyrusTempCompile())) sf::remove_all(papyrusTempCompile());
}
