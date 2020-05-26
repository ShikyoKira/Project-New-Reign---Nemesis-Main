#include "Global.h"

#include "utilities/renew.h"
#include "utilities/algorithm.h"

#include "generate/behaviorprocess_utility.h"

#pragma warning(disable : 4503)

using namespace std;

void tryDelete(const string& file, bool xml, int repeated = 0)
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
        if (!std::filesystem::remove(file)) tryDelete(file, xml, repeated + 1);
    }
    catch (const std::exception&)
    {
        tryDelete(file, xml, repeated + 1);
    }
}

bool DeleteFileFolder(const string& directory, const string& file, bool xml)
{
    if (std::filesystem::is_directory(directory + file))
    {
        string tempbehavior = directory + file;
        VecStr filelist;
        read_directory(tempbehavior, filelist);
        tempbehavior.append("\\");

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
    VecStr filelist;
    string tempbehavior = getTempBhvrPath();

    if (isFileExist(tempbehavior) && std::filesystem::is_directory(tempbehavior))
    {
        read_directory(tempbehavior, filelist);
        tempbehavior.append("\\");

        for (auto& file : filelist)
        {
            if (!nemesis::iequals(file, "xml")) DeleteFileFolder(tempbehavior, file, false);
        }
    }
}

void ClearTempXml()
{
    VecStr filelist;
    string tempbehavior = getTempBhvrPath() + "\\xml";

    if (isFileExist(tempbehavior) && std::filesystem::is_directory(tempbehavior))
    {
        read_directory(tempbehavior, filelist);
        tempbehavior.append("\\");

        for (auto& file : filelist)
        {
            DeleteFileFolder(tempbehavior, file, true);
        }
    }
}
