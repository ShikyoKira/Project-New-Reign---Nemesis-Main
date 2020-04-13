#include "Global.h"

#include <boost/atomic.hpp>

#include <QtCore/QProcess>

#include "generate/alternateanimation.h"
#include "generate/behaviorgenerator.h"
#include "utilities/atomiclock.h"

#pragma warning(disable : 4503)

using namespace std;

std::atomic_flag failedBehaviorFlag{};
VecStr failedBehaviors;

bool hkxcmdProcess(string xmlfile, string hkxfile, bool last)
{
    if (!last)
    {
        if (xmlfile.find(".xml") != xmlfile.length() - 4) xmlfile.append(".xml");
        if (hkxfile.find(".hkx") != hkxfile.length() - 4) hkxfile.append(".hkx");
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
    { ErrorMessage(1207, hkxfile); }
    else if (!std::filesystem::is_directory(xmlfile))
    {
        if (!GetFunctionLines(xmlfile, fileline)) return false;

        if (fileline.size() == 0) ErrorMessage(3001, xmlfile);

        if (!std::filesystem::remove(xmlfile)) ErrorMessage(1082, xmlfile, xmlfile);
    }

    return true;
}
