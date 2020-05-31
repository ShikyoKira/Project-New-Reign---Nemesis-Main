#include "Global.h"
#include "nemesisinfo.h"

#include <atomic>

#include <QtCore/QProcess>

#include "utilities/atomiclock.h"

#include "generate/alternateanimation.h"
#include "generate/behaviorgenerator.h"

#pragma warning(disable : 4503)

using namespace std;

namespace sf = filesystem;

std::atomic<int> fileprotek = 0;
atomic_flag failedBehaviorFlag{};
VecWstr failedBehaviors;

class HkxCompileCount
{
    int cur;
    mutable sf::path input;
    mutable sf::path output;

public:
    HkxCompileCount()
    {
        cur = fileprotek++;
    }

    ~HkxCompileCount()
    {
        --fileprotek;

        if (sf::exists(input)) sf::remove(input);
        if (sf::exists(output)) sf::remove(output);
    }

    void AddPath(const string& _input, const string& _output) const
    {
        input  = _input;
        output = _output;
    }

    int GetNum() const
    {
        return cur;
    }
};

sf::path tryGetRelative(sf::path filepath)
{
    sf::path current = sf::current_path();
    wstring target  = L"";

    do
    {
        if (nemesis::to_lower_copy(filepath).find(nemesis::to_lower_copy(current.wstring())) == 0)
        {
            return target.append(filepath.wstring().substr(current.wstring().length() + 1));
        }

        target.append(L"..\\");
        current = current.parent_path();
    } while (current.has_filename());

    return filepath;
}

bool hkxcmdProcess(sf::path xmlfile, sf::path hkxfile, bool last)
{
    if (!last)
    {
        if (xmlfile.has_extension() || xmlfile.extension().wstring() != L".xml")
        {
            xmlfile.replace_extension(L".xml");
        }
        
        if (hkxfile.has_extension() || hkxfile.extension().wstring() != L".hkx")
        {
            hkxfile.replace_extension(L".hkx");
        }
    }

    if (!isFileExist(hkxTempCompile()) || !sf::is_directory(hkxTempCompile()))
    {
        sf::create_directories(hkxTempCompile());
    }

    const HkxCompileCount hkxcount;
    string input = hkxTempCompile() + "\\" + to_string(hkxcount.GetNum()) + "_" + xmlfile.filename().string();
    string output = hkxTempCompile() + "\\" + to_string(hkxcount.GetNum()) + "_" + hkxfile.filename().string();
    hkxcount.AddPath(input, output);
    sf::copy_file(xmlfile, input, sf::copy_options::overwrite_existing);

    if (QProcess::execute("hkxcmd.exe",
                          QStringList() << "convert"
                                        << (SSE ? "-v:AMD64" : "-v:WIN32") 
                                        << input.data()
                                        << output.data())
            != 0
        || !isFileExist(output))
    {
        if (last) ErrorMessage(1003, xmlfile);

        Lockless lock(failedBehaviorFlag);
        failedBehaviors.push_back(xmlfile);
        failedBehaviors.push_back(hkxfile);
        return false;
    }

    sf::copy_file(output, hkxfile, sf::copy_options::overwrite_existing);
    return true;
}

bool hkxcmdXmlInput(sf::path hkxfile, VecStr& fileline)
{
    sf::path xmlfile = hkxfile;

    if (xmlfile.has_extension() || xmlfile.extension().wstring() != L".xml")
    {
        xmlfile.replace_extension(L".xml");
    }

    if (hkxfile.has_extension() || hkxfile.extension().wstring() != L".hkx")
    {
        hkxfile.replace_extension(L".hkx");
    }

    if (!isFileExist(hkxTempCompile()) || !sf::is_directory(hkxTempCompile()))
    {
        sf::create_directories(hkxTempCompile());
    }

    const HkxCompileCount hkxcount;
    string input  = hkxTempCompile() + "\\" + to_string(hkxcount.GetNum()) + "_" + hkxfile.filename().string();
    string output = hkxTempCompile() + "\\" + to_string(hkxcount.GetNum()) + "_" + xmlfile.filename().string();
    hkxcount.AddPath(input, output);
    sf::copy_file(hkxfile, input, sf::copy_options::overwrite_existing);

    if (QProcess::execute("hkxcmd.exe",
                          QStringList() << "convert"
                                        << "-v:xml" 
                                        << input.data() 
                                        << output.data())
            != 0
        || !isFileExist(output))
    {
        ErrorMessage(1207, hkxfile); 
    }

    if (!GetFunctionLines(output, fileline)) return false;

    if (fileline.size() == 0) ErrorMessage(3001, output);

    if (!sf::remove(output)) ErrorMessage(1082, output, output);

    return true;
}

bool hkxcmdXmlInput(sf::path hkxfile, VecWstr& fileline)
{
    sf::path xmlfile = hkxfile;

    if (xmlfile.has_extension() || xmlfile.extension().wstring() != L".xml")
    {
        xmlfile.replace_extension(L".xml");
    }

    if (hkxfile.has_extension() || hkxfile.extension().wstring() != L".hkx")
    {
        hkxfile.replace_extension(L".hkx");
    }

    if (!isFileExist(hkxTempCompile()) || !sf::is_directory(hkxTempCompile()))
    {
        sf::create_directories(hkxTempCompile());
    }

    const HkxCompileCount hkxcount;
    string input  = hkxTempCompile() + "\\" + to_string(hkxcount.GetNum()) + "_" + hkxfile.filename().string();
    string output = hkxTempCompile() + "\\" + to_string(hkxcount.GetNum()) + "_" + xmlfile.filename().string();
    hkxcount.AddPath(input, output);
    sf::copy_file(hkxfile, input, sf::copy_options::overwrite_existing);

    if (QProcess::execute("hkxcmd.exe",
                          QStringList() << "convert"
                                        << "-v:xml" 
                                        << input.data() 
                                        << output.data())
            != 0
        || !isFileExist(output))
    {
        ErrorMessage(1207, hkxfile);
    }

    if (!GetFunctionLines(output, fileline)) return false;

    if (fileline.size() == 0) ErrorMessage(3001, output);

    if (!sf::remove(output)) ErrorMessage(1082, output, output);

    return true;
}

std::string hkxTempCompile()
{
    return "cache\\hkx";
}
