#include <QString>
#include <QtCore/QProcess>

#include "utilities/lastupdate.h"

#include "generate/papyruscompile.h"

using namespace std;
namespace sf = filesystem;

void forcedRemove(sf::path target, int counter = 0);
bool FolderCreate(std::filesystem::path curBehaviorPath);
bool PapyrusCompileProcess(sf::path pscfile,
                           sf::path import,
                           sf::path destination,
                           sf::path filepath,
                           sf::path compiling_path,
                           sf::path compiler,
                           bool tryagain = false);
void ByteCopyToData(sf::path target, sf::path destination);

sf::path papyrusTempCompile()
{
    return "cache\\papyrus";
}

void forcedRemove(sf::path target, int counter)
{
    try
    {
        if (!sf::remove(target)) ErrorMessage(1082, target);
    }
    catch (exception)
    {
        if (counter > 200)
        {
            // ErrorMessage(6002, nemesis::transform_to<string>(target), ex.what());
            return;
        }

        forcedRemove(target, counter + 1);
    }
}

bool PapyrusCompile(sf::path pscfile,
                    sf::path import,
                    sf::path destination,
                    sf::path filepath,
                    sf::path compiling_path,
                    sf::path target)
{
    if (!sf::exists(pscfile)) ErrorMessage(1092, pscfile);
    if (!sf::exists(destination)) ErrorMessage(1001, destination);

    wstring timeline;

    while (!nemesis::iequals(target.stem().wstring(), L"data"))
    {
        target = target.parent_path();
    }

    target = target.parent_path();
    target = sf::path(target.wstring() + L"\\Papyrus Compiler\\PapyrusCompiler.exe");

    if (isFileExist(filepath) && !sf::remove(filepath)) timeline = GetLastModified(filepath);

    sf::path desPsc(import.wstring() + L"\\" + pscfile.stem().wstring() + L".psc");

    if (sf::exists(desPsc) && !sf::remove(desPsc))
    {
        ErrorMessage(1082, pscfile.replace_extension(L".psc"), desPsc);
    }

    if (!sf::exists(target)
        || !PapyrusCompileProcess(pscfile, import, destination, filepath, compiling_path, target))
    {
        string compiler = "Papyrus Compiler\\PapyrusCompiler.exe";

        if (sf::exists(compiler))
        {
            if (!PapyrusCompileProcess(
                    pscfile, import, destination, filepath, compiling_path, compiler, true))
            {
                throw nemesis::exception();
            }
        }
        else
        {
            ErrorMessage(6007);
        }
    }

    if (timeline.length() > 0)
    {
        if (timeline == GetLastModified(filepath.wstring())) ErrorMessage(1185, filepath);
    }

    return true;
}

bool PapyrusCompileProcess(sf::path pscfile,
                           sf::path import,
                           sf::path destination,
                           sf::path filepath,
                           sf::path compiling_path,
                           sf::path compiler,
                           bool tryagain)
{
    sf::path importedSource(import.wstring() + L"\\" + pscfile.filename().wstring());
    wstring dep       = L"Papyrus Compiler\\scripts";
    wstring backUpDep = L"Papyrus Compiler\\backup scripts";

    if ((sf::exists(dep) || FolderCreate(dep)) && sf::exists(backUpDep))
    {
        vector<wstring> backUpDepList;
        read_directory(backUpDep, backUpDepList);

        for (wstring bkUp : backUpDepList)
        {
            sf::copy_file(backUpDep + L"\\" + bkUp, dep + L"\\" + bkUp, sf::copy_options::overwrite_existing);
        }
    }

    if (isFileExist(filepath) && !sf::is_directory(filepath) && !sf::remove(filepath))
    {
        ErrorMessage(1082, filepath);
    }
    else if (sf::exists(importedSource) && !sf::is_directory(importedSource) && !sf::remove(importedSource))
    {
        ErrorMessage(1082, importedSource);
    }

    QProcess process;
    QString exe = QString::fromStdWString(compiler.wstring());
    QStringList args{QString::fromStdWString(pscfile.wstring()),
                     "-f=TESV_Papyrus_Flags.flg",
                     "-i=" + QString::fromStdWString(compiling_path.wstring()) + ";"
                         + QString::fromStdWString(dep),
                     "-o=" + QString::fromStdWString(compiling_path.wstring())};

    wstring cmd = exe.toStdWString();

    for (auto& arg : args)
    {
        cmd.append(L" " + arg.toStdWString());
    }

    DebugLogging(cmd);
    process.setReadChannel(QProcess::StandardOutput);
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.setCurrentReadChannel(QProcess::StandardOutput);
    process.start(exe, args);
    process.waitForFinished();

    wstring tempfile     = filepath.filename().wstring();
    wstring tempfilepath = compiling_path.wstring() + L"\\" + tempfile;

    if (!sf::exists(tempfilepath))
    {
        while (!process.waitForReadyRead(50))
            ;

        string line = process.readAllStandardOutput().toStdString();

        if (line.find("Compilation succeeded") != NOT_FOUND && line.find("Assembly succeeded") != NOT_FOUND
            && line.find("0 error") != NOT_FOUND)
        {
            return true;
        }

        if (!tryagain) return false;

        try
        {
            interMsg("Output: \n" + line);
            DebugLogging("\nOutput: \n" + line, false);
            ErrorMessage(1185, filepath);
            wstring temp = pscfile.wstring();

            for (QString arg : args)
            {
                temp += L" " + arg.toStdWString();
            }

            interMsg(L"Command: " + temp);
        }
        catch (nemesis::exception)
        {
            return false;
        }
    }

    ByteCopyToData(tempfilepath, destination.wstring() + L"\\" + tempfile);
    return true;
}

void ByteCopyToData(sf::path target, sf::path destination)
{
    ifstream file(target, ios::binary);
    ofstream paste(destination, ios::binary);
    copy(istreambuf_iterator<char>(file), istreambuf_iterator<char>(), ostreambuf_iterator<char>(paste));

    while (file.is_open())
    {
        file.close();
    }

    forcedRemove(target);
}
