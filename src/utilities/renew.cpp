#include "Global.h"

#include "utilities/renew.h"
#include "utilities/algorithm.h"

#include "generate/papyruscompile.h"
#include "generate/behaviorgenerator.h"
#include "generate/behaviorprocess_utility.h"

#pragma warning(disable : 4503)

using namespace std;

namespace sf = std::filesystem;

void tryDelete(const wstring& file, bool xml, int repeated = 0)
{
    if (repeated > 100)
    {
        if (xml)
        {
            WarningMessage(1009, file);
        }
        else
        {
            WarningMessage(1006);
        }
    }

    try
    {
        if (!sf::remove(file)) tryDelete(file, xml, repeated + 1);
    }
    catch (const std::exception&)
    {
        tryDelete(file, xml, repeated + 1);
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

void ClearTempBehaviors(const NemesisInfo* nemesisInfo)
{
    VecWstr filelist;
    wstring tempbehavior = getTempBhvrPath(nemesisInfo).wstring();

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

void ClearTempXml(const NemesisInfo* nemesisInfo)
{
    VecWstr filelist;
    wstring tempbehavior = getTempBhvrPath(nemesisInfo).wstring() + L"\\xml";

    if (isFileExist(tempbehavior) && sf::is_directory(tempbehavior))
    {
        read_directory(tempbehavior, filelist);
        tempbehavior.append(L"\\");

        for (auto& file : filelist)
        {
            DeleteFileFolder(tempbehavior, file, true);
        }
    }

    if (isFileExist(hkxTempCompile())) sf::remove_all(hkxTempCompile());

    if (isFileExist(papyrusTempCompile())) sf::remove_all(papyrusTempCompile());
}
