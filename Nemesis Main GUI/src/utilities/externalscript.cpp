#include <boost/process.hpp>
#include <boost/process/windows.hpp>
#include <boost/algorithm/string.hpp>

#undef slots
#include <Python.h>
#define slots

#include "externalscript.h"
#include "add animation/alternateanimation.h"

using namespace std;

bool dummyScript = false;

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
				try
				{
					hasScript = true;
					interMsg(TextBoxMessage(1016) + ": " + scriptfile.filename().string());

					if (boost::process::system(scriptpath, boost::process::windows::hide) != 0) WarningMessage(1023, scriptpath);
				}
				catch (const exception& ex)
				{
					ErrorMessage(6008, scriptfile.filename().string(), ex.what());
				}
				catch (...)
				{
					ErrorMessage(6008, scriptfile.filename().string(), "Unknown exception");
				}
			}
			// python script
			else if (boost::iequals(scriptfile.extension().string(), ".py"))
			{
				try
				{
					FILE* f;
					hasScript = true;
					interMsg(TextBoxMessage(1016) + ": " + scriptfile.filename().string());

					const char* pyScript = scriptfile.string().c_str();
					fopen_s(&f, pyScript, "r");

					if (f)
					{
						try
						{
							Py_Initialize();
							PyRun_SimpleFile(f, pyScript);
							Py_Finalize();
						}
						catch (const exception& ex)
						{
							ErrorMessage(6008, scriptfile.filename().string(), ex.what());
						}
						catch (...)
						{
							ErrorMessage(6008, scriptfile.filename().string(), "Unknown exception");
						}

						fclose(f);
					}
				}
				catch (const exception& ex)
				{
					ErrorMessage(6008, scriptfile.filename().string(), ex.what());
				}
				catch (...)
				{
					ErrorMessage(6008, scriptfile.filename().string(), "Unknown exception");
				}
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
						try
						{
							hasScript = true;
							interMsg(TextBoxMessage(1016) + ": " + shownscript.filename().string());

							if (boost::process::system(shownpath) != 0) WarningMessage(1023, shownscript);
						}
						catch (const exception& ex)
						{
							ErrorMessage(6008, shownscript.filename().string(), ex.what());
						}
						catch (...)
						{
							ErrorMessage(6008, shownscript.filename().string(), "Unknown exception");
						}
					}
					// python script
					else if (boost::iequals(shownscript.extension().string(), ".py"))
					{
						try
						{
							FILE* f;
							hasScript = true;
							interMsg(TextBoxMessage(1016) + ": " + scriptfile.filename().string());

							const char* pyScript = scriptfile.string().c_str();
							fopen_s(&f, pyScript, "r");

							if (f)
							{
								try
								{
									Py_Initialize();
									PyRun_SimpleFile(f, pyScript);
									Py_Finalize();
								}
								catch (const exception& ex)
								{
									ErrorMessage(6008, scriptfile.filename().string(), ex.what());
								}
								catch (...)
								{
									ErrorMessage(6008, scriptfile.filename().string(), "Unknown exception");
								}

								fclose(f);
							}
						}
						catch (const exception& ex)
						{
							ErrorMessage(6008, scriptfile.filename().string(), ex.what());
						}
						catch (...)
						{
							ErrorMessage(6008, scriptfile.filename().string(), "Unknown exception");
						}
					}
				}
			}
		}
	}

	if (hasScript) interMsg("");
}
