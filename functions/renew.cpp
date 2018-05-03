#include "renew.h"

#pragma warning(disable:4503)

using namespace std;

bool VanillaUpdate(unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader)
{
#ifndef DEBUG
	string path = "data";
#else
	string path = skyrimDataPath.dataPath + "\\meshes";
#endif

	unordered_map<string, string> emptyPath;
	behaviorPath = emptyPath;

	if (!isFileExist(path))
	{
		CreateDirectory(path.c_str(), NULL);
	}
	else
	{
		if (!GetPathLoop(path + "\\", newFile, newAnimData, animDataChar, animDataHeader))
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

				output.close();
			}
			else
			{
				ErrorMessage(2000, "behavior_path.txt");
				return false;
			}			
		}
	}

	return true;
}

bool GetPathLoop(string path, unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader)
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

				if (boost::iequals(curFileName, "nemesis_animationdatasinglefile"))
				{
					curFileName = curFileName.substr(8);

					if (!AnimDataDisassemble(newPath, newAnimData, animDataChar, animDataHeader))
					{
						return false;
					}

					string parent = curfile.parent_path().filename().string();
					newPath = path + filelist[i].substr(8);
					boost::algorithm::to_lower(parent);
					boost::algorithm::to_lower(newPath);
					boost::algorithm::to_lower(curFileName);
					behaviorPath[curFileName] = newPath.substr(0, newPath.find_last_of("."));
				}
				else if (wordFind(curFileName, "Nemesis_") == 0 && wordFind(curFileName, "_List") == -1)
				{
					curFileName = curFileName.substr(8);

					if (!VanillaDisassemble(newPath, curFileName, newFile))
					{
						return false;
					}

					string parent = curfile.parent_path().filename().string();
					newPath = path + filelist[i].substr(8);
					boost::algorithm::to_lower(parent);
					boost::algorithm::to_lower(newPath);
					boost::algorithm::to_lower(curFileName);
					behaviorPath[curFileName] = newPath.substr(0, newPath.find_last_of("."));

					if (parent.find("characters") == 0)
					{
						unordered_map<string, bool> empty;
						registeredAnim[boost::algorithm::to_lower_copy(curFileName)] = empty;
					}
				}
			}
		}
		else
		{
			// look deeper into the folder for behavior file
			multithreads.emplace_back(GetPathLoop, newPath + "\\", ref(newFile), ref(newAnimData), ref(animDataChar), ref(animDataHeader));
		}
	}

	for (auto& t : multithreads)
	{
		t.join();
	}

	return true;
}

bool VanillaDisassemble(string path, string filename, unordered_map<string, map<string, vecstr>>& newFile)
{
	vecstr storeline;
	storeline.reserve(2000);
	char line[2000];
	FILE* vanillafile;
	fopen_s(&vanillafile, path.c_str(), "r");
	string curID;

	if (vanillafile)
	{
		bool skip = true;

		while (fgets(line, 2000, vanillafile))
		{
			string curline = line;

			if (curline[curline.length() - 1] == '\n')
			{
				curline.pop_back();
			}

			if (curline.find("	</hksection>") != string::npos)
			{
				break;
			}

			if (curline.find("SERIALIZE_IGNORED") == string::npos && !skip)
			{
				if (curline.find("<hkobject name=\"", 0) != string::npos && curline.find("signature=\"", curline.find("<hkobject name=\"")) != string::npos)
				{
					if (storeline.size() != 0 && curID.length() != 0)
					{
						storeline.shrink_to_fit();
						newFile[filename][curID] = storeline;
						storeline.reserve(2000);
						storeline.clear();
					}

					size_t pos = curline.find("<hkobject name=\"#") + 16;
					curID = curline.substr(pos, curline.find("\" class=\"", curline.find("<hkobject name=\"")) - pos);
				}

				storeline.push_back(curline);
			}
			else if (skip && curline.find("<hkobject name=\"#") != string::npos)
			{
				skip = false;
				size_t pos = curline.find("<hkobject name=\"#") + 16;
				curID = curline.substr(pos, curline.find("\" class=\"", curline.find("<hkobject name=\"")) - pos);
				storeline.push_back(curline);
			}
		}

		fclose(vanillafile);

		if (storeline.size() != 0 && curID.length() != 0)
		{
			storeline.shrink_to_fit();
			newFile[filename][curID] = storeline;
		}
	}
	else
	{
		ErrorMessage(2000, path);
		return false;
	}

	return true;
}

bool AnimDataDisassemble(string path, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader)
{
	int num;
	vecstr newline;
	newline.reserve(500);
	vecstr storeline = GetFunctionLines(path);

	if (error)
	{
		return false;
	}
	
	{
		string strnum = boost::regex_replace(string(storeline[0]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

		if (!isOnlyNumber(strnum) || stoi(strnum) < 10)
		{
			ErrorMessage(3014);
			return false;
		}

		num = stoi(strnum) + 1;
	}
	
	for (int i = 0; i < num; ++i)
	{
		newline.push_back(storeline[i]);
	}

	newline.shrink_to_fit();
	string character = "$header$";
	string header = character;
	bool special = false;
	bool isInfo = false;
	bool end = false;
	newAnimData[character][header] = newline;
	animDataChar.push_back(header);
	animDataHeader[header].push_back(header);
	newline.reserve(20);
	newline.clear();

	for (unsigned int j = num; j < storeline.size(); ++j)
	{
		if (wordFind(storeline[j], "Characters") == 0)
		{
			character = GetFileName(boost::algorithm::to_lower_copy(storeline[j]));
			animDataChar.push_back(character);
			header = "$header$";
			animDataHeader[character].push_back(header);
			break;
		}
	}

	for (unsigned int i = num; i < storeline.size(); ++i)
	{
		if (!end)
		{
			if (i + 3 < storeline.size() && (storeline[i - 1] == ""))
			{
				if (storeline[i + 3].find("Behaviors") != string::npos && storeline[i + 3].find(".hkx") == storeline[i + 3].length() - 4)
				{
					newline.shrink_to_fit();
					newAnimData[character][header] = newline;
					newline.reserve(20);
					newline.clear();
					isInfo = false;

					if (storeline[i + 3].find("Behaviors\\Behavior00.hkx") != string::npos)
					{
						end = true;
						character = "$end$";
						header = character;
						animDataChar.push_back(character);
						animDataHeader[character].push_back(header);
					}
					else
					{
						for (unsigned int j = i + 4; j < storeline.size(); ++j)
						{
							if (wordFind(storeline[j], "Characters") == 0)
							{
								character = GetFileName(boost::algorithm::to_lower_copy(storeline[j]));
								animDataChar.push_back(character);
								header = "$header$";
								animDataHeader[character].push_back(header);
								break;
							}
						}
					}
				}
				else if (wordFind(storeline[i - 3], "Characters") == 0)
				{
					if (hasAlpha(storeline[i]))
					{
						if (isOnlyNumber(storeline[i + 1]))
						{
							newline.shrink_to_fit();
							newAnimData[character][header] = newline;
							newline.reserve(20);
							newline.clear();
							header = storeline[i] + " " + storeline[i + 1];
							animDataHeader[character].push_back(header);
						}
					}
				}
				else
				{
					if (!isInfo)
					{
						if (isOnlyNumber(storeline[i]))
						{
							isInfo = true;
							special = true;
							newline.shrink_to_fit();
							newAnimData[character][header] = newline;
							newline.clear();
							header = "$info header$";
							animDataHeader[character].push_back(header);
						}
						else if (hasAlpha(storeline[i]))
						{
							if (isOnlyNumber(storeline[i + 1]))
							{
								newline.shrink_to_fit();
								newAnimData[character][header] = newline;
								newline.reserve(20);
								newline.clear();
								header = storeline[i] + " " + storeline[i + 1];
								animDataHeader[character].push_back(header);
							}
						}
					}
					else
					{
						if (isOnlyNumber(storeline[i]))
						{
							newline.shrink_to_fit();
							newAnimData[character][header] = newline;
							newline.reserve(20);
							newline.clear();
							header = storeline[i];
							animDataHeader[character].push_back(header);
						}
					}
				}
			}
			else if (wordFind(storeline[i - 3], "Characters") == 0)
			{
				if (hasAlpha(storeline[i]) && i + 1 != storeline.size())
				{
					if (isOnlyNumber(storeline[i + 1]))
					{
						newline.shrink_to_fit();
						newAnimData[character][header] = newline;
						newline.reserve(20);
						newline.clear();
						header = storeline[i] + " " + storeline[i + 1];
						animDataHeader[character].push_back(header);
					}
				}
			}

			if (special)
			{
				special = false;
				newline.push_back(storeline[i]);
				newline.shrink_to_fit();
				newAnimData[character][header] = newline;
				newline.reserve(20);
				newline.clear();
				header = storeline[i + 1];
				animDataHeader[character].push_back(header);
			}
			else
			{
				newline.push_back(storeline[i]);
			}
		}
		else
		{
			newline.push_back(storeline[i]);
		}
	}

 	if (newline.size() != 0)
	{
		if (newline.back().length() == 0)
		{
			newline.pop_back();
		}

		newline.shrink_to_fit();
		newAnimData[character][header] = newline;
	}

	return true;
}

void ClearTempBehaviors()
{
	vecstr filelist;
	string tempbehavior = "temp_behaviors\\";
	read_directory(tempbehavior, filelist);
	bool failed = false;

	for (unsigned int i = 0; i < filelist.size(); ++i)
	{
		if (!boost::iequals(filelist[i], "xml"))
		{
			if (remove((tempbehavior + filelist[i]).c_str()) != 0 && !failed)
			{
				WarningMessage(1006);
			}
		}
	}
}
