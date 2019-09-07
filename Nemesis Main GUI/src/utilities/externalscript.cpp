#include <boost/process.hpp>
#include <boost/process/windows.hpp>
#include <boost/algorithm/string.hpp>

#undef slots
#include <Python.h>
#define slots

#include <QtCore\QProcess>

#include "externalscript.h"

#include "add animation/alternateanimation.h"

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
	vecstr scriptlist;
	read_directory(directory, scriptlist);

	for (auto& file : scriptlist)
	{
		string scriptpath = directory + file;
		boost::filesystem::path scriptfile(scriptpath);

		// hidden scripts
		if (!boost::filesystem::is_directory(scriptfile))
		{
			// bat script
			if (boost::iequals(scriptfile.extension().string(), ".bat"))
			{
				hasScript = true;
				BatchScriptThread(scriptfile.filename().string(), scriptpath, false);
			}
			// python script
			else if (boost::iequals(scriptfile.extension().string(), ".py"))
			{
				hasScript = true;
				PythonScriptThread(scriptfile.filename().string(), scriptpath.c_str());
			}
		}
		// visible scripts
		else if (boost::iequals(file, "show") && boost::filesystem::is_directory(scriptfile))
		{
			vecstr shownscriptlist;
			read_directory(scriptpath, shownscriptlist);

			for (auto& shown : shownscriptlist)
			{
				string shownpath = scriptpath + "\\" + shown;
				boost::filesystem::path shownscript(shownpath);

				if (!boost::filesystem::is_directory(shownscript))
				{
					// bat script
					if (boost::iequals(shownscript.extension().string(), ".bat"))
					{
						hasScript = true;
						BatchScriptThread(shownscript.filename().string(), shownpath, true);
					}
					// python script
					else if (boost::iequals(shownscript.extension().string(), ".py"))
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
