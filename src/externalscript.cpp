#include "Global.h"

#undef pyslots
#include <Python.h>
#define pyslots

#include <QtCore/QProcess>

#include "externalscript.h"

#include "generate/alternateanimation.h"

#include "utilities/algorithm.h"

using namespace std;

bool dummyScript = false;

void BatchScriptThread(string filename, string filepath, bool hidden);
void PythonScriptThread(string filename, const char* filepath);

void RunScript(string directory, bool& hasScript)
{
    if (!isFileExist(directory))
    {
        FolderCreate(directory);
        return;
    }

    bool warning = false;
    VecStr scriptlist;
    read_directory(directory, scriptlist);

    for (auto& file : scriptlist)
    {
        string scriptpath = directory + file;
        std::filesystem::path scriptfile(scriptpath);

        // hidden scripts
        if (!std::filesystem::is_directory(scriptfile))
        {
            // bat script
            if (nemesis::iequals(scriptfile.extension().string(), ".bat"))
            {
                hasScript = true;
                BatchScriptThread(scriptfile.filename().string(), scriptpath, false);
            }
            // python script
            else if (nemesis::iequals(scriptfile.extension().string(), ".py"))
            {
                hasScript = true;
                PythonScriptThread(scriptfile.filename().string(), scriptpath.c_str());
            }
        }
        // visible scripts
        else if (nemesis::iequals(file, "show") && std::filesystem::is_directory(scriptfile))
        {
            VecStr shownscriptlist;
            read_directory(scriptpath, shownscriptlist);

            for (auto& shown : shownscriptlist)
            {
                string shownpath = scriptpath + "\\" + shown;
                std::filesystem::path shownscript(shownpath);

                if (!std::filesystem::is_directory(shownscript))
                {
                    // bat script
                    if (nemesis::iequals(shownscript.extension().string(), ".bat"))
                    {
                        hasScript = true;
                        BatchScriptThread(shownscript.filename().string(), shownpath, true);
                    }
                    // python script
                    else if (nemesis::iequals(shownscript.extension().string(), ".py"))
                    {
                        hasScript = true;
                        PythonScriptThread(shownscript.filename().string(), shownpath.c_str());
                    }
                }
            }
        }
    }

    if (hasScript) interMsg("");
}

void BatchScriptThread(string filename, string filepath, bool hidden)
{
    try
    {
        interMsg(TextBoxMessage(1016) + ": " + filename);

        if (hidden)
        {
            QProcess* p = new QProcess();
            p->start(filepath.c_str());
            p->waitForFinished();
            delete p;
        }
        else
        {
            if (QProcess::execute(QString::fromStdString(filepath)) != 0) WarningMessage(1023, filepath);
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

void PythonScriptThread(string filename, const char* filepath)
{
    try
    {
        FILE* f;
        interMsg(TextBoxMessage(1016) + ": " + filename);
        fopen_s(&f, filepath, "r");

        if (f)
        {
            try
            {
                Py_Initialize();
                PyRun_SimpleFile(f, filepath);
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
