#include "renew.h"

using namespace std;

bool Renew(boost::filesystem::path const & source, boost::filesystem::path const & destination)
{
	try
	{
		boost::filesystem::remove_all(destination);
	}
	catch (boost::filesystem::filesystem_error const & error)
	{
		cout << error.what() << endl;
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
			cout << "ERROR(2100): Missing vanilla folder. Failed to renew output folder.";
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
		Renew(vanillaPath, newPath);
	}
}

void VanillaUpdate()
{
	string path = "vanilla_behaviors";

	if (!isFileExist(path))
	{
		CreateDirectory(path.c_str(), NULL);
	}

	path.append("/");
	vecstr filelist;
	string newPath;
	read_directory(path, filelist);

	for (size_t i = 0; i < filelist.size(); ++i)
	{
		boost::filesystem::path curfile(path + filelist[i]);

		if (!boost::filesystem::is_directory(curfile))
		{
			if (curfile.extension() == ".xml" || curfile.extension() == ".txt")
			{
				behaviorPath[filelist[i]] = path + filelist[i];
				VanillaDeassemble(path, filelist[i]);
			}
		}
		else
		{
			GetPathLoop(path + filelist[i]);
		}
	}
}

void GetPathLoop(string newPath)
{
	vecstr filelist;
	read_directory(newPath, filelist);

	for (size_t i = 0; i < filelist.size(); ++i)
	{
		boost::filesystem::path curfile(newPath + filelist[i]);

		if (!boost::filesystem::is_directory(curfile))
		{
			if (curfile.extension() == ".xml" || curfile.extension() == ".txt")
			{
				behaviorPath[filelist[i]] = newPath + filelist[i];
				VanillaDeassemble(newPath, filelist[i]);
			}
		}
		else
		{
			GetPathLoop(newPath + filelist[i]);
		}
	}
}

void VanillaDeassemble(string path, string filename)
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
				boost::filesystem::path curPath(path);

				if (!boost::filesystem::is_directory(curPath))
				{
					vecstr storeline;
					char line[5000];
					FILE* vanillafile;
					fopen_s(&vanillafile, path.c_str(), "r");
					string curID;

					if (vanillafile && (curPath.extension() == ".xml" || curPath.extension() == ".txt"))
					{
						while (fgets(line, 5000, vanillafile))
						{
							string curline = string(line);

							if (curline.find("<hkobject name=\"", 0) != string::npos && curline.find("signature=\"", curline.find("<hkobject name=\"")) != string::npos)
							{
								int pos = curline.find("<hkobject name=\"#") - 1;
								curID = curline.substr(pos, curline.find("\" class=\"", curline.find("<hkobject name=\"") - pos));

								if (storeline.size() != 0)
								{
									ofstream output(dir + curID);
									FunctionWriter fwriter(&output);

									if (output.is_open())
									{
										for (size_t i = 0; i < storeline.size(); ++i)
										{
											fwriter << storeline[i];
										}

										output.close();
									}
									else
									{
										cout << "ERROR(2603): Unable to write file " << endl << "File: " << dir << curID << endl << endl;
										error = true;
										return;
									}
								}
							}

							storeline.push_back(curline);
						}

						if (storeline.size() != 0)
						{
							ofstream output(dir + curID);
							FunctionWriter fwriter(&output);

							if (output.is_open())
							{
								for (size_t i = 0; i < storeline.size(); ++i)
								{
									fwriter << storeline[i];
								}

								output.close();
							}
							else
							{
								cout << "ERROR(2603): Unable to write file " << endl << "File: " << dir << curID << endl << endl;
								error = true;
								return;
							}
						}
					}
					else
					{
						cout << "ERROR(2602): Unable to open file " << endl << "File: " << dir << curID << endl << endl;
						error = true;
						return;
					}
				}

			}
		}
	}
}