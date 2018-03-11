#include "renew.h"

using namespace std;

bool Renew(boost::filesystem::path const & source, boost::filesystem::path const & destination)
{
	try
	{
		boost::filesystem::remove_all(destination);
	}
	catch (boost::filesystem::filesystem_error const & ex)
	{
		cout << ex.what() << endl;
		return false;
	}

	if (!copyDir(source, destination))
	{
		return false;
	}
	
	return true;
}

bool copyDir(boost::filesystem::path const & source, boost::filesystem::path const & destination)
{
	try
	{
		if (!boost::filesystem::exists(source))
		{
			cout << "ERROR(2100): Missing vanilla folder. Failed to renew output folder";
			return false;
		}
	}
	catch (boost::filesystem::filesystem_error const & error)
	{
		cout << error.what() << endl;
		return false;
	}

	try
	{
		try
		{
			boost::filesystem::copy_directory(source, destination);
		}
		catch (boost::filesystem::filesystem_error const & error)
		{
			cout << error.what() << endl;
			return false;
		}

		for (boost::filesystem::directory_iterator vanillafile(source); vanillafile != boost::filesystem::directory_iterator(); vanillafile++)
		{
			try
			{
				boost::filesystem::path currentfile(vanillafile->path());

				if (boost::filesystem::is_directory(currentfile))
				{
					if (!copyDir(currentfile, destination / currentfile.filename()))
					{
						return false;
					}
				}
				else
				{
					boost::filesystem::copy_file(currentfile, destination / currentfile.filename(), boost::filesystem::copy_option::overwrite_if_exists);
				}
			}
			catch (boost::filesystem::filesystem_error const& error)
			{
				cout << error.what() << endl;
				return false;
			}
		}
	}
	catch (boost::filesystem::filesystem_error const& error)
	{
		cout << error.what() << endl;
		return false;
	}

	return true;
}

void Clearing(string directory)
{
	string newDirectory = directory + "new";
	string vanillaDirectory = directory + "vanilla";

	if (CreateDirectory(newDirectory.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		boost::filesystem::path vanillaPath(vanillaDirectory);
		boost::filesystem::path newPath(newDirectory);
		
		if (!Renew(vanillaPath, newPath))
		{
			error = true;
			return;
		}
	}
}

bool VanillaUpdate()
{
#ifndef DEBUG
	string path = "vanilla_behaviors";
#else
	string path = skyrimDataPath.dataPath;
#endif

	if (!isFileExist(path))
	{
		CreateDirectory(path.c_str(), NULL);
	}
	else
	{
		if (!GetPathLoop(path + "/"))
		{
			return false;
		}

		if (behaviorPath.size() != 0)
		{
			ofstream output("behavior_path.txt");

			if (output.is_open())
			{
				FunctionWriter fwriter(&output);

				for (auto it = behaviorPath.begin(); it != behaviorPath.end(); ++it)
				{
					fwriter << it->first << " " << it->second << "\n";
				}
			}
			else
			{
				cout << "ERROR(2602): Unable to open file" << endl << "File: behavior_path.txt" << endl << endl;
				return false;
			}
		}		
	}

	return true;
}

bool GetPathLoop(string path)
{
	vecstr filelist;
	read_directory(path, filelist);
	unordered_map<string, time_t> lastEdit;
	unordered_map<string, time_t> curEdit;
	unordered_map<string, string> behaviorFile;

	for (size_t i = 0; i < filelist.size(); ++i)
	{
		string newPath = path + filelist[i];
		boost::filesystem::path curfile(newPath);

		if (!boost::filesystem::is_directory(curfile))
		{
			if (curfile.extension() == ".xml" || curfile.extension() == ".txt")
			{
				string curFileName = curfile.stem().string();

				if (curFileName.find("- last_edit (Do Not Modify)") == curFileName.length() - 27)
				{
					string behaviorName = curFileName.substr(0, curFileName.find(" - last_edit (Do Not Modify)"));
					char line[2000];
					FILE* editFile;
					fopen_s(&editFile, newPath.c_str(), "r");

					if (editFile)
					{
						while (fgets(line, 2000, editFile))
						{
							if (strlen(line) != 0)
							{
								time_t fileEdit;

								if (line[strlen(line) - 1] == '\n' && strlen(line) != 0)
								{
									line[strlen(line) - 1] = '\0';
								}

								try
								{
									fileEdit = boost::lexical_cast<time_t>(line);
								}
								catch (boost::bad_lexical_cast& ex)
								{
									cout << ex.what() << endl;
									return false;
								}

								// check if current file last write has been recorded
								if (curEdit[behaviorName] != 0)
								{
									// check last modified date ignore if same
									if (fileEdit < curEdit[behaviorName])
									{
										if (!VanillaDeassemble(behaviorFile[behaviorName], behaviorName))
										{
											return false;
										}
									}

									curEdit[behaviorName] = 0;
								}
								else
								{
									lastEdit[behaviorName] = fileEdit;
								}
							}

							break;
						}
					}
					else
					{
						cout << "ERROR(2602): Unable to open file" << endl << "File: " << newPath << endl << endl;
						return false;
					}
				}
				else if (curFileName.find("Nemesis_") == 0)
				{
					curFileName = curFileName.substr(8);
					time_t fileEdit = boost::filesystem::last_write_time(curfile);

					// check if last edit has been recorded
					if (lastEdit[curFileName] != 0)
					{
						// check last modified date ignore if same
						if (lastEdit[curFileName] < fileEdit)
						{
							if (!VanillaDeassemble(newPath, curFileName))
							{
								return false;
							}
						}

						lastEdit[curFileName] = 0;
					}
					else
					{
						curEdit[curFileName] = fileEdit;
						behaviorFile[curFileName] = newPath;
					}

					newPath = path + filelist[i].substr(8);
					behaviorPath[curFileName] = newPath.substr(0, newPath.find_last_of("."));
				}
			}
		}
		else
		{
			// look deeper into the folder for behavior file
			GetPathLoop(newPath + "/");
		}
	}

	for (auto it = curEdit.begin(); it != curEdit.end(); ++it)
	{
		if (it->second != 0)
		{
			string filename = path + it->first + " - last_edit (Do Not Modify).txt";
			ofstream output(filename);

			if (output.is_open())
			{
				FunctionWriter fwriter(&output);

				try
				{
					fwriter << boost::lexical_cast<string>(it->second);
				}
				catch (boost::bad_lexical_cast& ex)
				{
					cout << ex.what() << endl;
					return false;
				}

				output.close();
			}
			else
			{
				cout << "ERROR(2603): Unable to write file" << endl << "File: " << filename << endl << endl;
				return false;
			}

			if (!VanillaDeassemble(behaviorFile[it->first], it->first))
			{
				return false;
			}
		}
	}

	for (auto it = lastEdit.begin(); it != lastEdit.end(); ++it)
	{
		if (it->second != 0)
		{
			try
			{
				boost::filesystem::remove_all(path + it->first + " - last_edit (Do Not Modify)");
			}
			catch (boost::filesystem::filesystem_error const & ex)
			{
				cout << ex.what() << endl;
				return false;
			}

		}
	}

	return true;
}

bool VanillaDeassemble(string path, string filename)
{
	string dir = "cache/";
	
	if (CreateDirectory(dir.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		dir = "cache/vanilla/";

		if (CreateDirectory(dir.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
		{
			dir = "cache/vanilla/" + filename + "/";

			if (CreateDirectory(dir.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
			{
				vecstr storeline;
				storeline.reserve(2000);
				char line[2000];
				FILE* vanillafile;
				fopen_s(&vanillafile, path.c_str(), "r");
				string curID;

				if (vanillafile)
				{
					while (fgets(line, 2000, vanillafile))
					{
						string curline = string(line);

						if (curline.find("	</hksection>") != string::npos)
						{
							break;
						}

						if (curline.find("SERIALIZE_IGNORED") == string::npos)
						{
							if (curline.find("<hkobject name=\"", 0) != string::npos && curline.find("signature=\"", curline.find("<hkobject name=\"")) != string::npos)
							{
								if (storeline.size() != 0 && curID.length() != 0)
								{
									ofstream output(dir + curID + ".txt");

									if (output.is_open())
									{
										FunctionWriter fwriter(&output);

										for (size_t i = 0; i < storeline.size(); ++i)
										{
											fwriter << storeline[i];
										}

										output.close();
									}
									else
									{
										cout << "ERROR(2603): Unable to write file" << endl << "File: " << dir << curID << endl << endl;
										return false;
									}
								}

								size_t pos = curline.find("<hkobject name=\"#") + 16;
								curID = curline.substr(pos, curline.find("\" class=\"", curline.find("<hkobject name=\"")) - pos);
								storeline.clear();
							}

							storeline.push_back(curline);
						}
					}

					fclose(vanillafile);

					if (storeline.size() != 0 && curID.length() != 0)
					{
						ofstream output(dir + curID + ".txt");

						if (output.is_open())
						{
							FunctionWriter fwriter(&output);

							for (size_t i = 0; i < storeline.size(); ++i)
							{
								fwriter << storeline[i];
							}

							output.close();
						}
						else
						{
							cout << "ERROR(2603): Unable to write file" << endl << "File: " << dir << curID << endl << endl;
							return false;
						}
					}
				}
				else
				{
					cout << "ERROR(2602): Unable to open file" << endl << "File: " << dir << curID << endl << endl;
					return false;
				}
			}
		}
	}

	return true;
}