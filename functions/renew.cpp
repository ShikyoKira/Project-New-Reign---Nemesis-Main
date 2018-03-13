#include "renew.h"

#pragma warning(disable:4503)

using namespace std;

bool VanillaUpdate(unordered_map<string, map<string, vecstr>>& newFile)
{
#ifndef DEBUG
	string path = "data";
#else
	string path = skyrimDataPath.dataPath + "/meshes";
#endif

	if (!isFileExist(path))
	{
		CreateDirectory(path.c_str(), NULL);
	}
	else
	{
		if (!GetPathLoop(path + "/", newFile))
		{
			error = true;
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
				error = true;
				return false;
			}
		}		
	}

	return true;
}

bool GetPathLoop(string path, unordered_map<string, map<string, vecstr>>& newFile)
{
	vecstr filelist;
	read_directory(path, filelist);
	vector<thread> multithreads;

	for (size_t i = 0; i < filelist.size(); ++i)
	{
		string newPath = path + filelist[i];
		boost::filesystem::path curfile(newPath);

		if (!boost::filesystem::is_directory(curfile))
		{
			if (curfile.extension() == ".xml" || curfile.extension() == ".txt")
			{
				string curFileName = curfile.stem().string();
				
				if (curFileName.find("Nemesis_") == 0 && wordFind(curFileName, "_List") == -1)
				{
					curFileName = curFileName.substr(8);

					if (!VanillaDeassemble(newPath, curFileName, newFile))
					{
						return false;
					}

					newPath = path + filelist[i].substr(8);
					behaviorPath[curFileName] = newPath.substr(0, newPath.find_last_of("."));
				}
			}
		}
		else
		{
			// look deeper into the folder for behavior file
			multithreads.emplace_back(GetPathLoop, newPath + "/", ref(newFile));
		}
	}

	for (auto& t : multithreads)
	{
		t.join();
	}

	return true;
}

bool VanillaDeassemble(string path, string filename, unordered_map<string, map<string, vecstr>>& newFile)
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
			curline.pop_back();

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
						for (size_t i = 0; i < storeline.size(); ++i)
						{
							newFile[filename][curID].push_back(storeline[i]);
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
			for (size_t i = 0; i < storeline.size(); ++i)
			{
				newFile[filename][curID].push_back(storeline[i]);
			}
		}
	}
	else
	{
		cout << "ERROR(2602): Unable to open file" << endl << "File: " << path << endl << endl;
		return false;
	}

	return true;
}