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

HkxCompiler::HkxCompiler() 
{
    tempdir = sf::current_path().root_path().string() + "Nemesis_TempHkx\\";

    DebugLogging(tempdir);

    if (!isFileExist(tempdir)) sf::create_directories(tempdir);

    tempcompiler = tempdir + "hkxcmd.exe";
    sf::copy_file("hkxcmd.exe", tempcompiler, sf::copy_options::overwrite_existing);
}

HkxCompiler::~HkxCompiler()
{
    if (isFileExist(tempdir)) sf::remove_all(tempdir);
}

bool HkxCompiler::hkxcmdProcess(fpath xmlfile, fpath hkxfile, bool last) const
{
    if (!last) ensureExtension(xmlfile, L".xml", hkxfile, L".hkx");

    const HkxCompileCount hkxcount;
    string input  = tempdir + to_string(hkxcount.GetNum()) + "_" + xmlfile.filename().string();
    string output = tempdir + to_string(hkxcount.GetNum()) + "_" + hkxfile.filename().string();
    hkxcount.AddPath(input, output);
    sf::copy_file(xmlfile, input, sf::copy_options::overwrite_existing);
    DebugLogging("HKX Input: " + input + "\nHKX Output: " + output);

    if (QProcess::execute(QString::fromStdString(tempcompiler),
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

bool HkxCompiler::hkxcmdXmlInput(fpath hkxfile, VecStr& fileline) const
{
    fpath xmlfile = hkxfile;
    ensureExtension(xmlfile, L".xml", hkxfile, L".hkx");
    HkxCompileCount hkxcount;
    string output = xmlDecompile(hkxfile, xmlfile, hkxcount);

    if (!GetFunctionLines(output, fileline)) return false;

    if (fileline.size() == 0) ErrorMessage(3001, output);

    if (!sf::remove(output)) ErrorMessage(1082, output, output);

    return true;
}

bool HkxCompiler::hkxcmdXmlInput(fpath hkxfile, VecWstr& fileline) const
{
    fpath xmlfile = hkxfile;
    ensureExtension(xmlfile, L".xml", hkxfile, L".hkx");
    HkxCompileCount hkxcount;
    string output = xmlDecompile(hkxfile, xmlfile, hkxcount);

    if (!GetFunctionLines(output, fileline)) return false;

    if (fileline.size() == 0) ErrorMessage(3001, output);

    if (!sf::remove(output)) ErrorMessage(1082, output, output);

    return true;
}

void HkxCompiler::ensureExtension(fpath& file1, const wstring& ext1, fpath& file2, const wstring& ext2) const
{
    if (file1.has_extension() || file1.extension().wstring() != ext1)
    {
        file1.replace_extension(ext1);
    }

    if (file2.has_extension() || file2.extension().wstring() != ext2)
    {
        file2.replace_extension(ext2);
    }
}

std::string HkxCompiler::xmlDecompile(fpath hkxfile, fpath xmlfile, const HkxCompileCount& hkxcount) const 
{
    string input  = tempdir + to_string(hkxcount.GetNum()) + "_" + hkxfile.filename().string();
    string output = tempdir + to_string(hkxcount.GetNum()) + "_" + xmlfile.filename().string();
    hkxcount.AddPath(input, output);
    sf::copy_file(hkxfile, input, sf::copy_options::overwrite_existing);
    DebugLogging("XML HKX Input: " + input + "\nXML HKX Output: " + output);

    if (QProcess::execute(QString::fromStdString(tempcompiler),
                          QStringList() << "convert"
                                        << "-v:xml" << input.data() << output.data())
            != 0
        || !isFileExist(output))
    {
        ErrorMessage(1207, hkxfile);
    }

    return output;
}
