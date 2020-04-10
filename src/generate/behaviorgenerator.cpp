#include "Global.h"

#include <boost/atomic.hpp>

#include <QtCore/QProcess>

#include "generate/alternateanimation.h"
#include "generate/behaviorgenerator.h"

#pragma warning(disable : 4503)

using namespace std;

boost::atomic_flag failedBehaviorFlag = BOOST_ATOMIC_FLAG_INIT;
vecstr failedBehaviors;

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

        while (failedBehaviorFlag.test_and_set(boost::memory_order_acquire))
            ;
        failedBehaviors.push_back(xmlfile);
        failedBehaviors.push_back(hkxfile);
        failedBehaviorFlag.clear(boost::memory_order_release);

        return false;
    }

    return true;
}

bool hkxcmdXmlInput(string hkxfile, vecstr& fileline)
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
