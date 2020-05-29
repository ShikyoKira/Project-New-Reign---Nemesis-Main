#include "Global.h"
#include "nemesisinfo.h"

#include <atomic>

#include <QtCore/QProcess>

#include "utilities/atomiclock.h"

#include "generate/alternateanimation.h"
#include "generate/behaviorgenerator.h"

#pragma warning(disable : 4503)

using namespace std;

std::atomic_flag failedBehaviorFlag{};
VecWstr failedBehaviors;

bool hkxcmdProcess(string xmlfile, string hkxfile, bool last)
{
    if (!last)
    {
        if (wordFind(xmlfile, ".xml") != xmlfile.length() - 4) xmlfile.append(".xml");
        if (wordFind(hkxfile, ".hkx") != hkxfile.length() - 4) hkxfile.append(".hkx");
    }

    if (QProcess::execute("hkxcmd.exe",
                          QStringList() << "convert" << (SSE ? "-v:AMD64" : "-v:WIN32")
                                        << QString::fromStdString(xmlfile) << QString::fromStdString(hkxfile))
            != 0
        || !isFileExist(hkxfile))
    {
        if (last) ErrorMessage(1003, xmlfile);

        {
            Lockless lock(failedBehaviorFlag);
            failedBehaviors.push_back(nemesis::transform_to<wstring>(xmlfile));
            failedBehaviors.push_back(nemesis::transform_to<wstring>(hkxfile));
        }
        return false;
    }

    return true;
}

bool hkxcmdProcess(wstring xmlfile, wstring hkxfile, bool last)
{
    if (!last)
    {
        if (wordFind(xmlfile, L".xml") != xmlfile.length() - 4) xmlfile.append(L".xml");
        if (wordFind(hkxfile, L".hkx") != hkxfile.length() - 4) hkxfile.append(L".hkx");
    }

    if (QProcess::execute("hkxcmd.exe",
                          QStringList() << "convert" << (SSE ? "-v:AMD64" : "-v:WIN32")
                                        << QString::fromStdWString(xmlfile) << QString::fromStdWString(hkxfile))
            != 0
        || !isFileExist(hkxfile))
    {
        if (last) ErrorMessage(1003, xmlfile);

        {
            Lockless lock(failedBehaviorFlag);
            failedBehaviors.push_back(xmlfile);
            failedBehaviors.push_back(hkxfile);
        }
        return false;
    }

    return true;
}

bool hkxcmdXmlInput(string hkxfile, VecStr& fileline)
{
    string xmlfile = hkxfile + ".xml";
    string args    = "convert -v:xml \"" + hkxfile + ".hkx\" \"" + xmlfile + "\"";

    if (QProcess::execute("hkxcmd " + QString::fromStdString(args)) != 0 || !isFileExist(xmlfile))
    {
        ErrorMessage(1207, hkxfile); 
    }
    else if (!std::filesystem::is_directory(xmlfile))
    {
        if (!GetFunctionLines(xmlfile, fileline)) return false;

        if (fileline.size() == 0) ErrorMessage(3001, xmlfile);

        if (!std::filesystem::remove(xmlfile)) ErrorMessage(1082, xmlfile, xmlfile);
    }

    return true;
}

bool hkxcmdXmlInput(wstring hkxfile, VecStr& fileline)
{
    wstring xmlfile = hkxfile + L".xml";
    wstring args    = L"convert -v:xml \"" + hkxfile + L".hkx\" \"" + xmlfile + L"\"";

    if (QProcess::execute("hkxcmd " + QString::fromStdWString(args)) != 0 || !isFileExist(xmlfile))
    {
        ErrorMessage(1207, hkxfile);
    }
    else if (!std::filesystem::is_directory(xmlfile))
    {
        if (!GetFunctionLines(xmlfile, fileline)) return false;

        if (fileline.size() == 0) ErrorMessage(3001, xmlfile);

        if (!std::filesystem::remove(xmlfile)) ErrorMessage(1082, xmlfile, xmlfile);
    }

    return true;
}
