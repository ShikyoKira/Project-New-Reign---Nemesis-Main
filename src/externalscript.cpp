#include "Global.h"

//#undef pyslots
//#define MS_NO_COREDLL
#include <Python.h>
//#define pyslots

#include <QtCore/QProcess>

#include "externalscript.h"

#include "generate/alternateanimation.h"

#include "utilities/algorithm.h"

using namespace std;

bool dummyScript = false;

void BatchScriptThread(const wstring& filename, const filesystem::path& filepath, bool hidden);
void PythonScriptThread(const wstring& filename, const wchar_t* filepath);

void RunScript(const filesystem::path& directory, bool& hasScript)
{
    if (!isFileExist(directory))
    {
        FolderCreate(directory);
        return;
    }

    bool warning = false;
    VecWstr scriptlist;
    read_directory(directory, scriptlist);

    for (auto& file : scriptlist)
    {
        wstring scriptpath = directory.wstring() + file;
        std::filesystem::path scriptfile(scriptpath);

        // hidden scripts
        if (!std::filesystem::is_directory(scriptfile))
        {
            // bat script
            if (nemesis::iequals(scriptfile.extension().wstring(), L".bat"))
            {
                hasScript = true;
                BatchScriptThread(scriptfile.filename().wstring(), scriptpath, false);
            }
            // python script
            else if (nemesis::iequals(scriptfile.extension().wstring(), L".py"))
            {
                hasScript = true;
                PythonScriptThread(scriptfile.filename().wstring(), scriptpath.c_str());
            }
        }
        // visible scripts
        else if (nemesis::iequals(file, L"show") && std::filesystem::is_directory(scriptfile))
        {
            VecWstr shownscriptlist;
            read_directory(scriptpath, shownscriptlist);

            for (auto& shown : shownscriptlist)
            {
                wstring shownpath = scriptpath + L"\\" + shown;
                std::filesystem::path shownscript(shownpath);

                if (!std::filesystem::is_directory(shownscript))
                {
                    // bat script
                    if (nemesis::iequals(shownscript.extension().wstring(), L".bat"))
                    {
                        hasScript = true;
                        BatchScriptThread(shownscript.filename().wstring(), shownpath, true);
                    }
                    // python script
                    else if (nemesis::iequals(shownscript.extension().wstring(), L".py"))
                    {
                        hasScript = true;
                        PythonScriptThread(shownscript.filename().wstring(), shownpath.c_str());
                    }
                }
            }
        }
    }

    if (hasScript) interMsg("");
}

void BatchScriptThread(const wstring& filename, const filesystem::path& filepath, bool hidden)
{
    try
    {
        wstring msg = TextBoxMessage(1016) + L": " + filename;
        interMsg(msg);
        DebugLogging(msg);

        if (hidden)
        {
            QProcess* p = new QProcess();
            p->start(QString::fromStdWString(filepath));
            p->waitForFinished();
            delete p;
        }
        else
        {
            if (QProcess::execute(QString::fromStdWString(filepath)) != 0) WarningMessage(1023, filepath);
        }
    }
    catch (const exception& ex)
    {
        ErrorMessage(6008, filename, ex.what());
    }
    catch (...)
    {
        ErrorMessage(6008, filename, "Unknown exception");
    }
}

void PythonScriptThread(const wstring& filename, const wchar_t* filepath)
{
    try
    {
        FILE* f;
        wstring msg = TextBoxMessage(1016) + L": " + filename;
        interMsg(msg);
        DebugLogging(msg);
        _wfopen_s(&f, filepath, L"r");

        if (f)
        {
            try
            {
                Py_Initialize();
                PyRun_SimpleFile(f, nemesis::transform_to<string>(wstring(filepath)).c_str());
                Py_Finalize();
            }
            catch (const exception& ex)
            {
                ErrorMessage(6008, filename, ex.what());
            }
            catch (...)
            {
                ErrorMessage(6008, filename, "Unknown exception");
            }

            fclose(f);
        }
    }
    catch (const exception& ex)
    {
        ErrorMessage(6008, filename, ex.what());
    }
    catch (...)
    {
        ErrorMessage(6008, filename, "Unknown exception");
    }
}
