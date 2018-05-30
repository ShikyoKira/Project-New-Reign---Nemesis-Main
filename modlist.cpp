#include "modlist.h"

using namespace std;

void checkModList(vector<moddata>& modlist)
{
	string path = "mod\\";
	vecstr modcode;
	read_directory(path, modcode);

	for (auto& it : modcode)
	{
		vecstr filelist;
		string newpath = path + it;

		if (boost::filesystem::is_directory(newpath))
		{
			read_directory(path, filelist);

			for (auto& iter : filelist)
			{
				string newerpath = newpath + iter;
				boost::filesystem::path file(newerpath);
				
				if (!boost::filesystem::is_directory(file) && file.extension() == "ini")
				{
					moddata mod;
					vecstr storeline;
					GetFunctionLines(newerpath, storeline, false);
					double checker = 0;

					for (auto& line : storeline)
					{
						if (wordFind(line, "name=") == 0)
						{
							if (mod.name.length() != 0)
							{
								ErrorMessage(6003, "name", newerpath);
								return;
							}

							mod.name = line.substr(line.find("=") + 1);
							++checker;
						}
						else if (wordFind(line, "author=") == 0)
						{
							if (mod.author.length() != 0)
							{
								ErrorMessage(6003, "name", newerpath);
								return;
							}

							mod.author = line.substr(line.find("=") + 1);
						}
						else if (wordFind(line, "site=") == 0)
						{
							if (mod.site.length() != 0)
							{
								ErrorMessage(6003, "name", newerpath);
								return;
							}

							mod.site = line.substr(line.find("=") + 1);
							++checker;
						}
						else if (wordFind(line, "auto=") == 0)
						{
							if (mod.automate.length() != 0)
							{
								ErrorMessage(6003, "name", newerpath);
								return;
							}

							mod.automate = line.substr(line.find("=") + 1);
							checker += 0.5;
						}
					}

					if (checker < 3)
					{
						WarningMessage(1021, it);
					}
					else
					{
						modlist.push_back(mod);
					}
				}
			}
		}
	}
}

void autoSelect(vector<moddata>& modlist)
{
	for (auto& it : modlist)
	{

#ifdef DEBUG
		if (it.automate.length() != 0 && isFileExist(skyrimDataPath + "\\" + it.automate))
#else
		if (it.automate.length() != 0 && isFileExist("data\\" + it.automate))
#endif
		{
			
		}
	}
}

