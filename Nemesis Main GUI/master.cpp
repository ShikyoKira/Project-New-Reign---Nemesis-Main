#include "master.h"
#include <QThread>

#pragma warning(disable:4503)

using namespace std;

unordered_map<string, vecstr> modinfo;

UpdateFilesStart::UpdateFilesStart()
{
}

UpdateFilesStart::~UpdateFilesStart()
{
	if (error)
	{
		error = false;
	}
}

void UpdateFilesStart::UpdateFiles()
{
	string directory = "mod\\";
	string newAnimDirectory = "behavior templates\\";
	milestoneStart();

	unordered_map<string, map<string, vecstr>> newFile;		// behavior file, function/node ID, vector<string>; memory to access each node

	unordered_map<string, unordered_map<string, vecstr>> newAnimData;		// character, unique code, vector<string>; memory to access each node
	vecstr animDataChar;													// order of the character
	unordered_map<string, vecstr> animDataHeader;							// order of the character's header

	unordered_map<string, map<string, vecstr, alphanum_less>> newAnimDataSet;	// project, header, vector<string>; memory to access each node
	vecstr projectList;																	// order of the project

	unordered_map<string, string> lastUpdate;

	ClearGlobal();

	try
	{
		// Check the existence of required files
		if (FileCheck(true))
		{
			// clear the temp_behaviors folder to prevent it from bloating
			ClearTempBehaviors();

			emit progressUp();

			// copy latest vanilla into memory
			if (VanillaUpdate(newFile, newAnimData, animDataChar, animDataHeader, newAnimDataSet, projectList))
			{
				if (!error)
				{
					emit progressUp();
					unordered_map<string, set<string>> oriADH, oriASDH;

					for (auto& it : animDataHeader)
					{
						for (auto& iter : it.second)
						{
							oriADH[it.first].insert(iter);
						}
					}

					for (auto& it : newAnimDataSet)
					{
						for (auto& iter : it.second)
						{
							oriASDH[it.first].insert(iter.first);
						}
					}

					// check template for association with vanilla nodes from behavior template file
					if (newAnimUpdate(newAnimDirectory, newFile, newAnimData, animDataChar, animDataHeader, newAnimDataSet, projectList, oriADH, oriASDH, lastUpdate))
					{
						emit progressUp();

						// comparing if different from mod file
						JoiningEdits(directory, newFile, newAnimData, animDataChar, animDataHeader, newAnimDataSet, projectList, oriADH, oriASDH, lastUpdate);

						if (!error)
						{
							emit progressUp();

							// compiling all behaviors in "new" to "temp_behaviors" folder
							CombiningFiles(newFile, newAnimData, animDataChar, animDataHeader, newAnimDataSet, projectList);
						}
					}
				}
			}
		}
	}
	catch (exception& ex)
	{
		ErrorMessage(6001, ex.what());
	}

	ClearGlobal();

	QTimer* timer = new QTimer;
	timer->setSingleShot(true);
	connect(timer, SIGNAL(timeout()), this, SLOT(unregisterProcess()));
	timer->start(1500);
}

bool UpdateFilesStart::VanillaUpdate(unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader, unordered_map<string, map<string, vecstr, alphanum_less>>& newAnimDataSet, vecstr& projectList)
{
#ifdef DEBUG
	string path = "data";
#else
	string path = skyrimDataPath->GetDataPath() + "\\meshes";
#endif

	unordered_map<string, string> emptyPath;
	behaviorPath = emptyPath;

	if (!isFileExist(path))
	{
		CreateDirectoryA(path.c_str(), NULL);
		emit progressUp();
		emit progressUp();
		emit progressUp();		// 4
	}
	else
	{
		if (!GetPathLoop(path + "\\", newFile, newAnimData, animDataChar, animDataHeader, newAnimDataSet, projectList))
		{
			return false;
		}

		emit progressUp();		// 4

		if (behaviorPath.size() != 0)
		{
			ofstream output("behavior_path");

			if (output.is_open())
			{
				FunctionWriter fwriter(&output);

				for (auto it = behaviorPath.begin(); it != behaviorPath.end(); ++it)
				{
					fwriter << it->first << "=" << it->second << "\n";
				}

				output.close();
			}
			else
			{
				ErrorMessage(2009, "behavior_path");
				return false;
			}
		}

		if (behaviorProject.size() != 0)
		{
			ofstream output("behavior_project");

			if (output.is_open())
			{
				FunctionWriter fwriter(&output);

				for (auto it = behaviorProject.begin(); it != behaviorProject.end(); ++it)
				{
					fwriter << it->first << "\n";

					for (unsigned int i = 0; i < it->second.size(); ++i)
					{
						fwriter << it->second[i] << "\n";
					}

					fwriter << "\n";
				}

				output.close();
			}
			else
			{
				ErrorMessage(2009, "behavior_project");
				return false;
			}
		}
	}

	return true;
}

bool UpdateFilesStart::GetPathLoop(string path, unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader, unordered_map<string, map<string, vecstr, alphanum_less>>& newAnimDataSet, vecstr& projectList)
{
	vecstr filelist;
	read_directory(path, filelist);
	vector<std::thread> multithreads;

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
					emit progressUp();
				}
				else if (boost::iequals(curFileName, "nemesis_animationsetdatasinglefile"))
				{
					curFileName = curFileName.substr(8);

					if (!AnimDataSetDisassemble(newPath, newAnimDataSet, projectList))
					{
						return false;
					}

					string parent = curfile.parent_path().filename().string();
					newPath = path + filelist[i].substr(8);
					boost::algorithm::to_lower(parent);
					boost::algorithm::to_lower(newPath);
					boost::algorithm::to_lower(curFileName);
					behaviorPath[curFileName] = newPath.substr(0, newPath.find_last_of("."));
					emit progressUp();
				}
				else if (wordFind(curFileName, "Nemesis_") == 0 && wordFind(curFileName, "_List") == NOT_FOUND && wordFind(curFileName, "_Project") == NOT_FOUND)
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
				else if (wordFind(curFileName, "Nemesis_") == 0 && wordFind(curFileName, "_Project") + 8 == curFileName.length())
				{
					curFileName = curFileName.substr(8, curFileName.length() - 16);
					vecstr storeline;
					GetFunctionLines(newPath, storeline);
					bool record = false;

					if (error)
					{
						return false;
					}

					for (unsigned int j = 0; j < storeline.size(); ++j)
					{
						string line = storeline[j];

						if (record && line.find("</hkparam>") != NOT_FOUND)
						{
							break;
						}

						if (record)
						{
							if (line.find("<hkcstring>") == NOT_FOUND)
							{
								ErrorMessage(1093, newPath, j + 1);
								return false;
							}

							int pos = line.find("<hkcstring>") + 11;
							string characterfile = boost::to_lower_copy(line.substr(pos, line.find("</hkcstring>", pos) - pos));
							characterfile = GetFileName(characterfile);
							behaviorProject[characterfile].push_back(boost::to_lower_copy(curFileName));
						}

						if (line.find("<hkparam name=\"characterFilenames\" numelements=\"") != NOT_FOUND && line.find("</hkparam>") == NOT_FOUND)
						{
							record = true;
						}
					}
				}
			}
		}
		else
		{
			// look deeper into the folder for behavior file
			multithreads.emplace_back(&UpdateFilesStart::GetPathLoop, this, newPath + "\\", ref(newFile), ref(newAnimData), ref(animDataChar), ref(animDataHeader), ref(newAnimDataSet), ref(projectList));
		}
	}

	for (auto& t : multithreads)
	{
		t.join();
	}

	return true;
}

bool UpdateFilesStart::VanillaDisassemble(string path, string filename, unordered_map<string, map<string, vecstr>>& newFile)
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

			if (curline.find("	</hksection>") != NOT_FOUND)
			{
				break;
			}

			if (curline.find("SERIALIZE_IGNORED") == NOT_FOUND && !skip)
			{
				if (curline.find("<hkobject name=\"", 0) != NOT_FOUND && curline.find("signature=\"", curline.find("<hkobject name=\"")) != NOT_FOUND)
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
			else if (skip && curline.find("<hkobject name=\"#") != NOT_FOUND)
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

bool UpdateFilesStart::AnimDataDisassemble(string path, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader)
{
	int num;
	vecstr newline;
	newline.reserve(500);
	vecstr storeline;
	GetFunctionLines(path, storeline);

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
	animDataChar.push_back(character);
	animDataHeader[character].push_back(header);
	newline.reserve(20);
	newline.clear();

	for (unsigned int j = num; j < storeline.size(); ++j)
	{
		if (wordFind(storeline[j], ".txt") == storeline[j].length() - 4)
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
				if (wordFind(storeline[i + 3], "Behaviors") != NOT_FOUND && storeline[i + 3].find(".hkx") == storeline[i + 3].length() - 4)
				{
					newline.shrink_to_fit();
					newAnimData[character][header] = newline;
					newline.reserve(20);
					newline.clear();
					isInfo = false;

					if (storeline[i + 3].find("Behaviors\\Behavior00.hkx") != NOT_FOUND)
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

bool UpdateFilesStart::AnimDataSetDisassemble(string path, unordered_map<string, map<string, vecstr, alphanum_less>>& newAnimDataSet, vecstr& projectList)
{
	vecstr storeline;
	GetFunctionLines(path, storeline);
	int num;
	vecstr newline;
	newline.reserve(500);

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

	unordered_map<string, vecstr> animDataSetHeader;
	string project = "$header$";
	string header = project;
	int projectcounter = 1;
	int headercounter = 0;
	bool special = false;
	bool isInfo = false;
	projectList.push_back(project);
	animDataSetHeader[project].push_back(header);
	newline.push_back(storeline[0]);

	for (int i = 1; i < num; ++i)
	{
		newline.push_back(storeline[i]);
		projectList.push_back(storeline[i]);
	}

	for (unsigned int i = num; i < storeline.size(); ++i)
	{
		if (i != storeline.size() - 1 && wordFind(storeline[i + 1], ".txt") != NOT_FOUND)
		{
			header = animDataSetHeader[project][headercounter];
			newline.shrink_to_fit();
			newAnimDataSet[project][header] = newline;
			newline.reserve(100);
			newline.clear();
			project = projectList[projectcounter];
			++projectcounter;
			headercounter = 0;
			animDataSetHeader[project].push_back("$header$");
			newline.push_back(storeline[i]);
			++i;

			if (animDataSetHeader[project].size() != 1)
			{
				ErrorMessage(5005, path, i + 1);
				return false;
			}

			while (i < storeline.size())
			{
				if (wordFind(storeline[i], ".txt") != NOT_FOUND)
				{
					animDataSetHeader[project].push_back(GetFileName(boost::algorithm::to_lower_copy(storeline[i])));
				}
				else if (wordFind(storeline[i], "V3") != NOT_FOUND)
				{
					header = animDataSetHeader[project][headercounter];
					++headercounter;
					newline.shrink_to_fit();
					newAnimDataSet[project][header] = newline;
					newline.reserve(100);
					newline.clear();
					break;
				}
				else
				{
					ErrorMessage(5001, path, i + 1);
					return false;
				}

				newline.push_back(storeline[i]);
				++i;
			}
		}
		else if (wordFind(storeline[i], "V3") != NOT_FOUND)
		{
			header = animDataSetHeader[project][headercounter];
			++headercounter;
			newline.shrink_to_fit();
			newAnimDataSet[project][header] = newline;
			newline.reserve(100);
			newline.clear();
		}

		newline.push_back(storeline[i]);
	}

	if (newline.size() != 0)
	{
		if (newline.back().length() == 0)
		{
			newline.pop_back();
		}

		header = animDataSetHeader[project][headercounter];
		newline.shrink_to_fit();
		newAnimDataSet[project][header] = newline;
	}

	return true;
}

void UpdateFilesStart::SeparateMod(string directory, string modcode, vecstr behaviorfilelist, unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader, unordered_map<string, map<string, vecstr, alphanum_less>>& newAnimDataSet, vecstr& projectList, unordered_map<string, set<string>>& oriADH, unordered_map<string, set<string>>& oriASDH, unordered_map<string, string>& lastUpdate)
{
	emit progressUp();

	for (unsigned int j = 0; j < behaviorfilelist.size(); ++j)
	{
		boost::filesystem::path curPath(directory + modcode + "\\" + behaviorfilelist[j]);

		if (boost::filesystem::is_directory(curPath))
		{
			vecstr nodelist;
			read_directory(directory + modcode + "\\" + behaviorfilelist[j] + "\\", nodelist);

			if (boost::iequals(behaviorfilelist[j], "animationdatasinglefile"))
			{
				if (newAnimData.size() == 0)
				{
					ErrorMessage(3017, "nemesis_animationdatasinglefile.txt");
					return;
				}

				for (unsigned int k = 0; k < nodelist.size(); ++k)
				{
					boost::filesystem::path curPath(directory + modcode + "\\" + behaviorfilelist[j] + "\\" + nodelist[k]);

					if (boost::filesystem::is_directory(curPath))
					{
						bool newChar = false;

						if (newAnimData.find(nodelist[k]) == newAnimData.end())
						{
							animDataChar.push_back(nodelist[k]);
							newChar = true;
						}

						vecstr uniquecodelist;
						string filepath = directory + modcode + "\\" + behaviorfilelist[j] + "\\" + nodelist[k];
						read_directory(filepath, uniquecodelist);

						for (unsigned int i = 0; i < uniquecodelist.size(); ++i)
						{
							if (!AnimDataUpdate(modcode, behaviorfilelist[j], nodelist[k], filepath + "\\" + uniquecodelist[k], newAnimData, animDataChar, animDataHeader, newChar, oriADH, lastUpdate))
							{
								if (error)
								{
									return;
								}
							}
						}

						if (newChar)
						{
							if (animDataHeader[nodelist[k]].size() == 0)
							{
								animDataChar.pop_back();
								newAnimData.erase(nodelist[k]);
								animDataHeader.erase(nodelist[k]);
							}
							else
							{
								vecstr header;
								vecstr infoheader;

								for (auto& it : newAnimData[nodelist[k]])
								{
									if (!boost::iequals(it.first, "$header$") && !boost::iequals(it.first, "$info header$"))
									{
										if (isOnlyNumber(it.first))
										{
											infoheader.push_back(it.first);
										}
										else
										{
											header.push_back(it.first);
										}
									}
								}

								newAnimData[nodelist[k]]["$header$"].insert(newAnimData[nodelist[k]]["$header$"].begin(), "<!-- NEW *" + modcode + "* -->");
								animDataHeader[nodelist[k]].clear();
								animDataHeader[nodelist[k]].reserve(header.size() + infoheader.size() + 2);
								animDataHeader[nodelist[k]].push_back("$header$");
								animDataHeader[nodelist[k]].insert(animDataHeader[nodelist[k]].end(), header.begin(), header.end());
								animDataHeader[nodelist[k]].push_back("$info header$");
								animDataHeader[nodelist[k]].insert(animDataHeader[nodelist[k]].end(), infoheader.begin(), infoheader.end());
								newAnimData[nodelist[k]][animDataHeader[nodelist[k]].back()].push_back("<!-- CLOSE -->");
							}
						}
					}
				}
			}
			else if (boost::iequals(behaviorfilelist[j], "animationsetdatasinglefile"))
			{
				if (newAnimDataSet.size() == 0)
				{
					ErrorMessage(3017, "nemesis_animationsetdatasinglefile.txt");
					return;
				}

				for (unsigned int k = 0; k < nodelist.size(); ++k)
				{
					boost::filesystem::path curPath(directory + modcode + "\\" + behaviorfilelist[j] + "\\" + nodelist[k]);

					if (boost::filesystem::is_directory(curPath) && wordFind(nodelist[k], "data") != NOT_FOUND)
					{
						bool newProject = false;
						string project = nodelist[k] + "\\" + nodelist[k].substr(0, wordFind(nodelist[k], "data", true)) + ".txt";

						if (newAnimDataSet.find(project) == newAnimDataSet.end())
						{
							projectList.push_back(project);
							newProject = true;
						}

						vecstr uniquecodelist;
						string filepath = directory + modcode + "\\" + behaviorfilelist[j] + "\\" + nodelist[k];
						read_directory(filepath, uniquecodelist);

						for (unsigned int i = 0; i < uniquecodelist.size(); ++i)
						{
							if (!AnimDataSetUpdate(modcode, behaviorfilelist[j], nodelist[k], project, filepath + "\\" + uniquecodelist[k], newAnimDataSet, projectList, newProject, oriASDH, lastUpdate))
							{
								if (error)
								{
									return;
								}
							}
						}

						if (newProject)
						{
							newAnimDataSet[project].begin()->second.insert(newAnimDataSet[project].begin()->second.begin(), "<!-- NEW *" + modcode + "* -->");
							newAnimDataSet[project].rbegin()->second.push_back("<!-- CLOSE -->");
						}
					}
				}
			}
			else
			{
				for (unsigned int k = 0; k < nodelist.size(); ++k)
				{
					if (!FunctionUpdate(modcode, behaviorfilelist[j], nodelist[k], newFile, lastUpdate))
					{
						if (error)
						{
							return;
						}
					}
				}
			}
		}
	}

	emit progressUp();
}

void UpdateFilesStart::JoiningEdits(string directory, unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader, unordered_map<string, map<string, vecstr, alphanum_less>>& newAnimDataSet, vecstr& projectList, unordered_map<string, set<string>>& oriADH, unordered_map<string, set<string>>& oriASDH, unordered_map<string, string>& lastUpdate)
{
	if (CreateDirectoryA(directory.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		vecstr filelist;
		vector<std::thread> threads;
		read_directory(directory, filelist);

		for (unsigned int i = 0; i < filelist.size(); ++i)
		{
			boost::filesystem::path curPath(directory + filelist[i]);

			if (boost::filesystem::is_directory(curPath))
			{
				vecstr filelist2;
				read_directory(directory + filelist[i] + "\\", filelist2);
				threads.emplace_back(&UpdateFilesStart::SeparateMod, this, directory, filelist[i], filelist2, ref(newFile), ref(newAnimData), ref(animDataChar), ref(animDataHeader), ref(newAnimDataSet), ref(projectList), ref(oriADH), ref(oriASDH), ref(lastUpdate));
			}
		}

		for (auto& th : threads)
		{
			th.join();
		}
	}

	ofstream lastmod("engine_update");

	if (lastmod.is_open())
	{
		FunctionWriter fwriter(&lastmod);

		for (auto& it : lastUpdate)
		{
			fwriter << it.first << " " << it.second << "\n";
		}

		lastmod.close();
	}
	else
	{
		ErrorMessage(2009, "engine_update");
		return;
	}
}

void UpdateFilesStart::CombiningFiles(unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader, unordered_map<string, map<string, vecstr, alphanum_less>>& newAnimDataSet, vecstr& projectList)
{
	vecstr fileline;
	string compilingfolder = "temp_behaviors\\";

	for (auto it = newFile.begin(); it != newFile.end(); ++it) // behavior file name
	{
		string rootID;
		bool isOpen = false;
		string OpeningMod;

		for (auto iter = it->second.begin(); iter != it->second.end(); ++iter) // behavior node ID
		{
			if (iter->first.find("$") != NOT_FOUND)
			{
				string modID = iter->first.substr(1, iter->first.find("$") - 1);

				if (OpeningMod != modID && isOpen)
				{
					fileline.push_back("<!-- CLOSE -->");
					isOpen = false;
				}

				if (!isOpen)
				{
					fileline.push_back("<!-- NEW *" + modID + "* -->");
					OpeningMod = modID;
					isOpen = true;
				}
			}

			vecstr storeline = iter->second;
			string line;

			if (storeline.size() != 0)
			{
				for (unsigned int l = 0; l < storeline.size(); ++l)
				{
					line = storeline[l];

					if (line.find("class=\"hkRootLevelContainer\" signature=\"0x2772c11e\">", 0) != NOT_FOUND)
					{
						rootID = "#" + boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
					}

					fileline.push_back(line);
				}
			}
			else
			{
				ErrorMessage(2008, it->first + "(" + iter->first + ")");
				return;
			}
		}

		if (isOpen)
		{
			fileline.push_back("<!-- CLOSE -->");
			isOpen = false;
		}

		if (CreateDirectoryA(compilingfolder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
		{
			string lowerBehaviorFile = boost::algorithm::to_lower_copy(it->first);
			string filename = compilingfolder + lowerBehaviorFile + ".txt";
			ofstream output(filename);

			if (output.is_open())
			{
				FunctionWriter writeoutput(&output);
				bool behaviorRef = false;

				writeoutput << "<?xml version=\"1.0\" encoding=\"ascii\"?>" << "\n";
				writeoutput << "<hkpackfile classversion=\"8\" contentsversion=\"hk_2010.2.0 - r1\" toplevelobject=\"" << rootID << "\">" << "\n";
				writeoutput << "\n";
				writeoutput << "	<hksection name=\"__data__\">" << "\n";
				writeoutput << "\n";

				for (unsigned int linecount = 0; linecount < fileline.size(); ++linecount)
				{
					writeoutput << fileline[linecount] << "\n";
					size_t pos = fileline[linecount].find("<hkobject name=\"");

					if (pos != NOT_FOUND && fileline[linecount].find("signature=\"", pos) != NOT_FOUND)
					{
						if (fileline[linecount].find("class=\"hkbBehaviorReferenceGenerator\" signature=\"", pos) != NOT_FOUND)
						{
							behaviorRef = true;
						}
						else
						{
							behaviorRef = false;
						}
					}

					if (behaviorRef && fileline[linecount].find("<hkparam name=\"behaviorName\">") != NOT_FOUND)
					{
						size_t nextpos = fileline[linecount].find("behaviorName\">") + 14;
						string behaviorName = GetFileName(fileline[linecount].substr(nextpos, fileline[linecount].find("</hkparam>", nextpos) - nextpos));
						boost::algorithm::to_lower(behaviorName);
						behaviorJoints[behaviorName].push_back(lowerBehaviorFile);
						behaviorRef = false;
					}
					else if (fileline[linecount].find("<hkparam name=\"behaviorFilename\">") != NOT_FOUND)
					{
						size_t nextpos = fileline[linecount].find("behaviorFilename\">") + 18;
						string behaviorName = fileline[linecount].substr(nextpos, fileline[linecount].find("</hkparam>", nextpos) - nextpos);
						behaviorName = GetFileName(behaviorName);
						boost::algorithm::to_lower(behaviorName);
						behaviorJoints[behaviorName].push_back(lowerBehaviorFile);
					}
				}

				writeoutput << "	</hksection>" << "\n";
				writeoutput << "\n";
				writeoutput << "</hkpackfile>" << "\n";
				fileline.clear();
				output.close();
			}
			else
			{
				ErrorMessage(2009, filename);
				return;
			}
		}
	}

	emit progressUp();
	behaviorJointsOutput();

	if (CreateDirectoryA(compilingfolder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		string filename = compilingfolder + "animationdatasinglefile.txt";
		ofstream output(filename);
		ofstream outputlist("animationdata_list");

		if (output.is_open())
		{
			if (outputlist.is_open())
			{
				FunctionWriter writeoutput(&output);
				FunctionWriter fwriter(&outputlist);

				for (unsigned int i = 0; i < animDataChar.size(); ++i)
				{
					string character = animDataChar[i];
					fwriter << character << "\n";		// character

					for (unsigned int j = 0; j < animDataHeader[character].size(); ++j)
					{
						string header = animDataHeader[character][j];
						fwriter << header << "\n";

						for (unsigned int k = 0; k < newAnimData[character][header].size(); ++k)
						{
							writeoutput << newAnimData[character][header][k] << "\n";
						}

						if (j != 0 && header != "$info header$" && character != "$end$" && newAnimData[character][header].back().length() != 0)
						{
							writeoutput << "\n";
						}
					}

					fwriter << "\n";
				}

				output.close();
				outputlist.close();
			}
			else
			{
				ErrorMessage(2009, "animationdata_list");
				return;
			}
		}
		else
		{
			ErrorMessage(2009, filename);
			return;
		}
	}

	emit progressUp();

	if (CreateDirectoryA(compilingfolder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		string filename = compilingfolder + "animationsetdatasinglefile.txt";
		ofstream output(filename);
		ofstream outputlist("animationsetdata_list");

		if (output.is_open())
		{
			if (outputlist.is_open())
			{
				FunctionWriter writeoutput(&output);
				FunctionWriter fwriter(&outputlist);

				for (unsigned int i = 0; i < projectList.size(); ++i)
				{
					string project = projectList[i];
					fwriter << project << "\n";		// character

					for (auto it = newAnimDataSet[project].begin(); it != newAnimDataSet[project].end(); ++it)
					{
						string header = it->first;
						fwriter << header << "\n";

						for (unsigned int k = 0; k < it->second.size(); ++k)
						{
							writeoutput << it->second[k] << "\n";
						}
					}

					fwriter << "\n";
				}

				output.close();
				outputlist.close();
			}
			else
			{
				ErrorMessage(2009, "animationsetdata_list");
				return;
			}
		}
		else
		{
			ErrorMessage(2009, filename);
			return;
		}
	}

	emit progressUp();
}

bool UpdateFilesStart::newAnimUpdate(string sourcefolder, unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, unordered_map<string, vecstr>>& newAnimData, vecstr& animDataChar, unordered_map<string, vecstr>& animDataHeader, unordered_map<string, map<string, vecstr, alphanum_less>>& newAnimDataSet, vecstr& projectList, unordered_map<string, set<string>>& oriADH, unordered_map<string, set<string>>& oriASDH, unordered_map<string, string>& lastUpdate)
{
	if (CreateDirectoryA(sourcefolder.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		vecstr codelist;
		read_directory(sourcefolder, codelist);

		for (unsigned int i = 0; i < codelist.size(); ++i)
		{
			string folderpath = sourcefolder + codelist[i];
			boost::filesystem::path codefile(folderpath);

			if (boost::filesystem::is_directory(codefile))
			{
				vecstr behaviorlist;
				read_directory(folderpath, behaviorlist);

				for (unsigned int j = 0; j < behaviorlist.size(); ++j)
				{
					boost::filesystem::path curfolder(folderpath + "\\" + behaviorlist[j]);

					if (boost::filesystem::is_directory(curfolder))
					{
						if (boost::iequals(behaviorlist[j], "animationdatasinglefile"))
						{
							vecstr characterlist;
							read_directory(folderpath + "\\" + behaviorlist[j], characterlist);

							for (unsigned int k = 0; k < characterlist.size(); ++k)
							{
								boost::filesystem::path characterfolder(folderpath + "\\" + behaviorlist[j] + "\\" + characterlist[k]);

								if (boost::filesystem::is_directory(characterfolder))
								{
									if (!newAnimDataUpdateExt(folderpath + "\\" + behaviorlist[j] + "\\" + characterlist[k], codelist[i], characterlist[k], newAnimData, animDataChar, animDataHeader, oriADH, lastUpdate))
									{
										return false;
									}

								}
							}
						}
						else if (boost::iequals(behaviorlist[j], "animationsetdatasinglefile"))
						{
							vecstr projectfile;
							read_directory(folderpath + "\\" + behaviorlist[j], projectfile);

							for (unsigned int k = 0; k < projectfile.size(); ++k)
							{
								boost::filesystem::path characterfolder(folderpath + "\\" + behaviorlist[j] + "\\" + projectfile[k]);

								if (boost::filesystem::is_directory(characterfolder) && wordFind(projectfile[k], "data") == projectfile[k].length() - 4)
								{
									if (!newAnimDataSetUpdateExt(folderpath + "\\" + behaviorlist[j] + "\\" + projectfile[k], codelist[i], projectfile[k], newAnimDataSet, projectList, oriASDH, lastUpdate))
									{
										return false;
									}

								}
							}
						}
						else
						{
							if (!newAnimUpdateExt(folderpath, codelist[i], behaviorlist[j], newFile, lastUpdate))
							{
								return false;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		ErrorMessage(2010, sourcefolder);
		return false;
	}

	return true;
}

void UpdateFilesStart::milestoneStart()
{
	filenum = 11;
	connectProcess(this);
}

void UpdateFilesStart::message(string input)
{
	emit incomingMessage(QString::fromStdString(input));
}

void UpdateFilesStart::unregisterProcess()
{
	if (!error)
	{
		interMsg("Engine update complete");
		emit enableCheck(false);
	}
	else
	{
		interMsg("Failed to update engine");
	}

	disconnectProcess();
	emit enable(false);
	emit hide(true);
	emit end();
}

void UpdateFilesStart::milestoneUp()
{
	if (!error)
	{
		std::lock_guard<std::mutex>locker(pLock);
		int old = progressPercentage * 100 / filenum;
		++progressPercentage;
		int result = progressPercentage * 100 / filenum;

		if (result > old)
		{
			if (result - old < 2)
			{
				emit progress(old + 1);
				Sleep(20);
			}
			else
			{
				for (int i = old + 1; i <= result; ++i)
				{
					if (error)
					{
						break;
					}

					emit progress(i);
					Sleep(rand() % 100 + 100);
				}
			}
		}
	}
}

IDCatcher::IDCatcher(int id, int curline)
{
	ID = id;
	line = curline;
}

int IDCatcher::getID()
{
	return ID;
}

int IDCatcher::getLine()
{
	return line;
}

BehaviorStart::BehaviorStart()
{
}

BehaviorStart::~BehaviorStart()
{
	if (error)
	{
		error = false;
	}
}

void BehaviorStart::addBehaviorPick(BehaviorStart* newProcess, vecstr behaviorOrder, unordered_map<string, bool> behaviorPick)
{
	behaviorPriority = behaviorOrder;
	chosenBehavior = behaviorPick;
	behaviorProcess = newProcess;
}

void BehaviorStart::message(string input)
{
	emit incomingMessage(QString::fromStdString(input));
}

void BehaviorStart::GenerateBehavior()
{
	milestoneStart();

	// Check the existence of required files
	if (!FileCheck())
	{
		ClearGlobal();
		unregisterProcess();
		return;
	}

	try
	{
		ClearGlobal();
		GetAnimData();
		characterHKX();
		GetBehaviorPath();
		GetBehaviorProject();
		behaviorActivateMod(behaviorPriority);
		ClearTempXml();
	}
	catch (int ex)
	{
		ClearGlobal();
		unregisterProcess();
		return;
	}

	if (error)
	{
		ClearGlobal();
		unregisterProcess();
		return;
	}

	// register animation & organize AE n Var
	string directory = "temp_behaviors\\";
	unordered_map<string, int> animationCount;												// animation type counter; use to determine how many of the that type of animation have been installed
	getTemplate BehaviorTemplate;															// get animation type
	vector<unique_ptr<registerAnimation>> animationList = move(openFile(BehaviorTemplate));	// get anim list installed by mods
	unordered_map<string, vector<shared_ptr<Furniture>>> newAnimation;						// record each of the animation created from registerAnimation
	mapSetString newAnimEvent;																// template code, 
	mapSetString newAnimVariable;
	unordered_map<string, var> AnimVar;
	unordered_map<string, vector<string>> modAnimBehavior;				// behavior directory, list of behavior files; use to get behavior reference
	unordered_map<string, unordered_map<int, bool>> ignoreFunction;		// behavior file, function ID, true/false; is the function part of animation template?

	if (error)
	{
		ClearGlobal();
		unregisterProcess();
		return;
	}

	// read each animation list file'
	for (unsigned int i = 0; i < animationList.size(); ++i)
	{
		set<string> coreRegistered;		// list of registered core behavior
		string modID = animationList[i]->modID;

		// read each animation in a group of the same type
		for (auto it = animationList[i]->templateType.begin(); it != animationList[i]->templateType.end(); ++it)
		{
			string coreModName = "";
			string firstAE;
			string templatecode = it->first;
			vector<shared_ptr<animationInfo>> animInfoGroup;
			unordered_map<string, vecstr>* functionlines = &BehaviorTemplate.behaviortemplate[templatecode];
			unordered_map<string, unordered_map<string, vecstr>>* animdatalines = &BehaviorTemplate.animdatatemplate[templatecode];
			unordered_map<string, map<string, vecstr, alphanum_less>>* asdlines = &BehaviorTemplate.asdtemplate[templatecode];
			newAnimation[templatecode].reserve(50 * memory);
			animationCount[templatecode]++;
			bool isGroup;
			bool ignoreGroup = BehaviorTemplate.optionlist[templatecode].ignoreGroup;

			// Create proxy new animation to link master behavior branch with sub-branch, which is used as a core
			if (BehaviorTemplate.coreTemplate[templatecode].length() > 0)
			{
				coreModName = "Nemesis_" + modID + "_";
				string corepath = behaviorPath[BehaviorTemplate.coreTemplate[templatecode]];
				string corename = GetFileName(corepath);
				corepath = corepath.substr(0, corepath.length() - corename.length());

				if (!isFileExist(corepath + "nemesis_" + corename + ".txt") && !isFileExist(corepath + "nemesis_" + corename + ".xml"))
				{
					WarningMessage(1004, BehaviorTemplate.optionlist[templatecode].coreBehavior, templatecode, corepath + "nemesis_" + corename + ".txt");
				}
				else
				{
					string corecode = BehaviorTemplate.coreBehaviorCode[BehaviorTemplate.coreTemplate[templatecode]];

					if (coreRegistered.find(corecode) == coreRegistered.end())
					{
						coreRegistered.insert(corecode);
						unordered_map<string, vecstr>* functionlines = &BehaviorTemplate.behaviortemplate[corecode];
						shared_ptr<animationInfo> dummy = make_shared<animationInfo>();

						if (isFileExist(corepath + "FNIS_" + modID + "_" + BehaviorTemplate.optionlist[corecode].coreBehavior + ".hkx"))
						{
							dummy->addFilename("FNIS_" + modID + "_" + BehaviorTemplate.optionlist[corecode].coreBehavior + ".hkx");
						}
						else
						{
							dummy->addFilename("Nemesis_" + modID + "_" + BehaviorTemplate.optionlist[corecode].coreBehavior + ".hkx");
							coreModList[boost::to_lower_copy(BehaviorTemplate.optionlist[corecode].coreBehavior) + ".txt"].push_back(coreModName);
						}

						dummy->ignoreGroup = false;
						dummy->optionPickedCount[corecode] = 1;
						animationCount[corecode]++;
						newAnimation[corecode].emplace_back(make_unique<Furniture>(*functionlines, corecode, animationCount[corecode], "", *dummy));
						vector<shared_ptr<animationInfo>> animInfoGroup = { dummy };
						newAnimation[corecode].back()->addGroupAnimInfo(animInfoGroup);
						newAnimation[corecode].back()->setLastOrder(0);
					}
				}
			}

			if (BehaviorTemplate.behaviortemplate.find(templatecode + "_group") != BehaviorTemplate.behaviortemplate.end() && BehaviorTemplate.behaviortemplate[templatecode + "_group"].size() != 0)
			{
				animInfoGroup.reserve(100);
				isGroup = true;
			}
			else
			{
				animInfoGroup.reserve(50 * memory);
				isGroup = false;
			}

			// read each line and generate lines categorized using template code (animation type)
			for (int j = 0; j < it->second; ++j)
			{
				int order = animationList[i]->isMulti[templatecode][j];
				int lastOrder = animationList[i]->last[templatecode][j];
				newAnimation[templatecode].emplace_back(make_unique<Furniture>(*functionlines, templatecode, animationCount[templatecode], modID + "\\", *animationList[i]->animInfo[templatecode][j]));
				newAnimation[templatecode].back()->addAnimData(*animdatalines);
				newAnimation[templatecode].back()->addAnimSetData(*asdlines);
				newAnimation[templatecode].back()->coreModID = coreModName;
				vecstr tempEventID = newAnimation[templatecode].back()->GetEventID();
				vecstr tempVariableID = newAnimation[templatecode].back()->GetVariableID();
				newAnimEvent[templatecode].insert(tempEventID.begin(), tempEventID.end());
				newAnimVariable[templatecode].insert(tempVariableID.begin(), tempVariableID.end());

				if (order != 0)
				{
					newAnimation[templatecode].back()->setOrder(order);
				}
				else
				{
					firstAE = newAnimation[templatecode].back()->mainAnimEvent;

					if (BehaviorTemplate.optionlist[templatecode].eleEventGroupF.size() != 0)
					{
						vector<vecstr>* elementList = &BehaviorTemplate.optionlist[templatecode].eleEventGroupF;
						vector<string>* elementListLine = &BehaviorTemplate.optionlist[templatecode].eleEventGroupFLine;
						ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
						unordered_map<string, unordered_map<string, vecstr>> groupAddOn = newAnimation[templatecode].back()->GetGroupAddition();

						for (unsigned int k = 0; k < elementList->size(); ++k)
						{
							vecstr* element = &(*elementList)[k];
							string elementLine = (*elementListLine)[k];
							vector<vecstr> groupAddOnElement;
							groupAddOnElement.reserve(memory / 10);

							for (unsigned int l = 0; l < element->size(); ++l)
							{
								string templine = (*element)[l];

								if (boost::iequals(templine, "main_anim_event"))
								{
									templine = newAnimation[templatecode].back()->mainAnimEvent;
									elementLine.replace(elementLine.find("$$"), 2, templine);
								}
								else
								{
									bool isDone = false;

									for (auto it = addOn.begin(); it != addOn.end(); ++it)
									{
										bool isBreak = false;

										for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
										{
											if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
											{
												elementLine.replace(elementLine.find("$$"), 2, iter->second);
												isBreak = true;
												isDone = true;
												break;
											}
										}

										if (isBreak)
										{
											break;
										}
									}

									if (!isDone)
									{
										for (auto it = groupAddOn.begin(); it != groupAddOn.end(); ++it)
										{
											bool isBreak = false;

											for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
											{
												if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
												{
													elementLine.replace(elementLine.find("$$"), 2, "##");
													isBreak = true;
													isDone = true;
													break;
												}

												groupAddOnElement.push_back(iter->second);
											}

											if (isBreak)
											{
												break;
											}
										}

									}
								}
							}

							if (groupAddOnElement.size() != 0)
							{
								vecstr animEvent = newAnimationElement(elementLine, groupAddOnElement, 0);

								for (unsigned int l = 0; l < animEvent.size(); ++l)
								{
									newAnimEvent[templatecode].insert(animEvent[l]);
								}
							}
							else
							{
								newAnimEvent[templatecode].insert(elementLine);
							}
						}
					}

					if (BehaviorTemplate.optionlist[templatecode].eleVarGroupF.size() != 0)
					{
						vector<vecstr>* elementList = &BehaviorTemplate.optionlist[templatecode].eleVarGroupF;
						vector<string>* elementListLine = &BehaviorTemplate.optionlist[templatecode].eleVarGroupFLine;
						ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
						unordered_map<string, unordered_map<string, vecstr>> groupAddOn = newAnimation[templatecode].back()->GetGroupAddition();

						for (unsigned int k = 0; k < elementList->size(); ++k)
						{
							vecstr* element = &(*elementList)[k];
							string elementLine = (*elementListLine)[k];
							vector<vecstr> groupAddOnElement;
							groupAddOnElement.reserve(memory / 10);

							for (unsigned int l = 0; l < element->size(); ++l)
							{
								string templine = (*element)[l];
								bool isBreak = false;

								for (auto it = addOn.begin(); it != addOn.end(); ++it)
								{
									for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
									{
										if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
										{
											elementLine.replace(elementLine.find("$$"), 2, iter->second);
											isBreak = true;
											break;
										}
									}

									if (isBreak)
									{
										break;
									}
								}

								if (!isBreak)
								{
									bool isDone = false;

									for (auto it = groupAddOn.begin(); it != groupAddOn.end(); ++it)
									{
										for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
										{
											if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
											{
												elementLine.replace(elementLine.find("$$"), 2, "##");
												isDone = true;
												break;
											}

											groupAddOnElement.push_back(iter->second);
										}

										if (isDone)
										{
											break;
										}
									}
								}
							}

							if (groupAddOnElement.size() != 0)
							{
								vecstr animVar = newAnimationElement(elementLine, groupAddOnElement, 0);

								for (unsigned int l = 0; l < animVar.size(); ++l)
								{
									newAnimVariable[templatecode].insert(animVar[l]);
								}
							}
							else
							{
								newAnimVariable[templatecode].insert(elementLine);
							}
						}
					}
				}

				newAnimation[templatecode].back()->setLastOrder(lastOrder);
				animInfoGroup.push_back(move(animationList[i]->animInfo[templatecode][j]));

				if (!ignoreGroup)
				{
					animationCount[templatecode]++;
				}

				if (lastOrder == order)
				{
					size_t animationSize = newAnimation[templatecode].size();
					animInfoGroup.shrink_to_fit();

					for (int k = 0; k < order + 1; ++k)
					{
						newAnimation[templatecode][animationSize - k - 1]->addGroupAnimInfo(animInfoGroup);

						if (error)
						{
							ClearGlobal();
							unregisterProcess();
							return;
						}
					}

					animInfoGroup.clear();
					animInfoGroup.reserve(100);

					if (ignoreGroup)
					{
						animationCount[templatecode]++;
					}
					else
					{
						if (BehaviorTemplate.optionlist[templatecode].eleEventGroupL.size() != 0)
						{
							vector<vecstr>* elementList = &BehaviorTemplate.optionlist[templatecode].eleEventGroupL;
							vector<string>* elementListLine = &BehaviorTemplate.optionlist[templatecode].eleEventGroupLLine;
							ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
							unordered_map<string, unordered_map<string, vecstr>> groupAddOn = newAnimation[templatecode].back()->GetGroupAddition();

							for (unsigned int k = 0; k < elementList->size(); ++k)
							{
								vecstr* element = &(*elementList)[k];
								string elementLine = (*elementListLine)[k];
								vector<vecstr> groupAddOnElement;
								groupAddOnElement.reserve(memory / 10);

								for (unsigned int l = 0; l < element->size(); ++l)
								{
									string templine = (*element)[l];

									if (boost::iequals(templine, "main_anim_event"))
									{
										templine = newAnimation[templatecode].back()->mainAnimEvent;
										elementLine.replace(elementLine.find("$$"), 2, templine);
									}
									else
									{
										bool isDone = false;

										for (auto it = addOn.begin(); it != addOn.end(); ++it)
										{
											bool isBreak = false;

											for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
											{
												if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
												{
													elementLine.replace(elementLine.find("$$"), 2, iter->second);
													isBreak = true;
													isDone = true;
													break;
												}
											}

											if (isBreak)
											{
												break;
											}
										}

										if (!isDone)
										{
											for (auto it = groupAddOn.begin(); it != groupAddOn.end(); ++it)
											{
												bool isBreak = false;

												for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
												{
													if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
													{
														elementLine.replace(elementLine.find("$$"), 2, "##");
														isBreak = true;
														isDone = true;
														break;
													}

													groupAddOnElement.push_back(iter->second);
												}

												if (isBreak)
												{
													break;
												}
											}

										}
									}
								}

								if (groupAddOnElement.size() != 0)
								{
									vecstr animEvent = newAnimationElement(elementLine, groupAddOnElement, 0);

									for (unsigned int l = 0; l < animEvent.size(); ++l)
									{
										newAnimEvent[templatecode].insert(animEvent[l]);
									}
								}
								else
								{
									newAnimEvent[templatecode].insert(elementLine);
								}
							}
						}

						if (BehaviorTemplate.optionlist[templatecode].eleVarGroupL.size() != 0)
						{
							vector<vecstr>* elementList = &BehaviorTemplate.optionlist[templatecode].eleVarGroupL;
							vector<string>* elementListLine = &BehaviorTemplate.optionlist[templatecode].eleVarGroupLLine;
							ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
							unordered_map<string, unordered_map<string, vecstr>> groupAddOn = newAnimation[templatecode].back()->GetGroupAddition();

							for (unsigned int k = 0; k < elementList->size(); ++k)
							{
								vecstr* element = &(*elementList)[k];
								string elementLine = (*elementListLine)[k];
								vector<vecstr> groupAddOnElement;
								groupAddOnElement.reserve(memory / 10);

								for (unsigned int l = 0; l < element->size(); ++l)
								{
									string templine = (*element)[l];
									bool isDone = false;

									for (auto it = addOn.begin(); it != addOn.end(); ++it)
									{
										bool isBreak = false;

										for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
										{
											if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
											{
												elementLine.replace(elementLine.find("$$"), 2, iter->second);
												isBreak = true;
												isDone = true;
												break;
											}
										}

										if (isBreak)
										{
											break;
										}
									}

									if (!isDone)
									{
										for (auto it = groupAddOn.begin(); it != groupAddOn.end(); ++it)
										{
											bool isBreak = false;

											for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
											{
												if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
												{
													elementLine.replace(elementLine.find("$$"), 2, "##");
													isBreak = true;
													isDone = true;
													break;
												}

												groupAddOnElement.push_back(iter->second);
											}

											if (isBreak)
											{
												break;
											}
										}

									}
								}

								if (groupAddOnElement.size() != 0)
								{
									vecstr animVar = newAnimationElement(elementLine, groupAddOnElement, 0);

									for (unsigned int l = 0; l < animVar.size(); ++l)
									{
										newAnimVariable[templatecode].insert(animVar[l]);
									}
								}
								else
								{
									newAnimVariable[templatecode].insert(elementLine);
								}
							}
						}
					}
				}

				if (BehaviorTemplate.optionlist[templatecode].eleEvent.size() != 0)
				{
					vector<vecstr>* elementList = &BehaviorTemplate.optionlist[templatecode].eleEvent;
					vector<string>* elementListLine = &BehaviorTemplate.optionlist[templatecode].eleEventLine;
					ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
					unordered_map<string, unordered_map<string, vecstr>> groupAddOn = newAnimation[templatecode].back()->GetGroupAddition();

					for (unsigned int k = 0; k < elementList->size(); ++k)
					{
						vecstr* element = &(*elementList)[k];
						string elementLine = (*elementListLine)[k];
						vector<vecstr> groupAddOnElement;
						groupAddOnElement.reserve(memory / 10);

						for (unsigned int l = 0; l < element->size(); ++l)
						{
							string templine = (*element)[l];

							if (boost::iequals(templine, "main_anim_event"))
							{
								templine = newAnimation[templatecode].back()->mainAnimEvent;
								elementLine.replace(elementLine.find("$$"), 2, templine);
							}
							else
							{
								bool isDone = false;

								for (auto it = addOn.begin(); it != addOn.end(); ++it)
								{
									bool isBreak = false;

									for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
									{
										if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
										{
											elementLine.replace(elementLine.find("$$"), 2, iter->second);
											isBreak = true;
											isDone = true;
											break;
										}
									}

									if (isBreak)
									{
										break;
									}
								}

								if (!isDone)
								{
									for (auto it = groupAddOn.begin(); it != groupAddOn.end(); ++it)
									{
										bool isBreak = false;

										for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
										{
											if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
											{
												elementLine.replace(elementLine.find("$$"), 2, "##");
												isBreak = true;
												isDone = true;
												break;
											}

											groupAddOnElement.push_back(iter->second);
										}

										if (isBreak)
										{
											break;
										}
									}

								}
							}
						}

						if (groupAddOnElement.size() != 0)
						{
							vecstr animEvent = newAnimationElement(elementLine, groupAddOnElement, 0);

							for (unsigned int l = 0; l < animEvent.size(); ++l)
							{
								newAnimEvent[templatecode].insert(animEvent[l]);
							}
						}
						else
						{
							newAnimEvent[templatecode].insert(elementLine);
						}
					}
				}

				if (BehaviorTemplate.optionlist[templatecode].eleVar.size() != 0)
				{
					vector<vecstr>* elementList = &BehaviorTemplate.optionlist[templatecode].eleVar;
					vector<string>* elementListLine = &BehaviorTemplate.optionlist[templatecode].eleVarLine;
					ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
					unordered_map<string, unordered_map<string, vecstr>> groupAddOn = newAnimation[templatecode].back()->GetGroupAddition();

					for (unsigned int k = 0; k < elementList->size(); ++k)
					{
						vecstr* element = &(*elementList)[k];
						string elementLine = (*elementListLine)[k];
						vector<vecstr> groupAddOnElement;
groupAddOnElement.reserve(memory / 10);

for (unsigned int l = 0; l < element->size(); ++l)
{
	string templine = (*element)[l];

	if (boost::iequals(templine, "main_anim_event"))
	{
		templine = newAnimation[templatecode].back()->mainAnimEvent;
		elementLine.replace(elementLine.find("$$"), 2, templine);
	}
	else
	{
		bool isDone = false;

		for (auto it = addOn.begin(); it != addOn.end(); ++it)
		{
			bool isBreak = false;

			for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
			{
				if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
				{
					elementLine.replace(elementLine.find("$$"), 2, iter->second);
					isBreak = true;
					isDone = true;
					break;
				}
			}

			if (isBreak)
			{
				break;
			}
		}

		if (!isDone)
		{
			for (auto it = groupAddOn.begin(); it != groupAddOn.end(); ++it)
			{
				bool isBreak = false;

				for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
				{
					if (boost::iequals(templine, it->first + "[" + iter->first + "]"))
					{
						elementLine.replace(elementLine.find("$$"), 2, "##");
						isBreak = true;
						isDone = true;
						break;
					}

					groupAddOnElement.push_back(iter->second);
				}

				if (isBreak)
				{
					break;
				}
			}

		}
	}
}

if (groupAddOnElement.size() != 0)
{
	vecstr animVar = newAnimationElement(elementLine, groupAddOnElement, 0);

	for (unsigned int l = 0; l < animVar.size(); ++l)
	{
		newAnimVariable[templatecode].insert(animVar[l]);
	}
}
else
{
	newAnimVariable[templatecode].insert(elementLine);
}
					}
				}

				for (unsigned int k = 0; k < tempVariableID.size(); ++k)
				{
					string name = tempVariableID[k];
					AnimVar[name] = animationList[i]->AnimVar[name];
				}

				if (error)
				{
					ErrorMessage(6000);
					ClearGlobal();
					unregisterProcess();
					return;
				}
			}

			newAnimation[templatecode].shrink_to_fit();
		}
	}

	{
		int multi = 0;
		
		for (auto& modlist : coreModList)
		{
			multi = int(modlist.second.size());
			--multi;
		}

		filenum += (multi * 10);
	}

	emit progressUp();

	if (error)
	{
		ClearGlobal();
		unregisterProcess();
		return;
	}

	// Ignore function in master branch
	for (auto it = BehaviorTemplate.grouplist.begin(); it != BehaviorTemplate.grouplist.end(); ++it)
	{
		vecstr templateGroup = it->second;

		for (unsigned int j = 0; j < templateGroup.size(); ++j)
		{
			// existing function
			for (unsigned int k = 0; k < BehaviorTemplate.existingFunctionID[templateGroup[j]][it->first].size(); ++k)
			{
				ignoreFunction[it->first + ".txt"][BehaviorTemplate.existingFunctionID[templateGroup[j]][it->first][k]] = true;
			}
		}
	}

	emit progressUp();

	vecstr filelist;
	read_directory(directory, filelist);

	if (AAGroup.size() > 0)
	{
		AAInstallation();		// install AA script
	}

	if (error)
	{
		ClearGlobal();
		unregisterProcess();
		return;
	}

	emit progressUp();
	vector<QThread*> behaviorThreads;

	for (unsigned int i = 0; i < filelist.size(); ++i)
	{
		if (!boost::filesystem::is_directory(directory + filelist[i]))
		{
			string modID = "";
			bool isCore = false;
			int repeatcount = 0;
			int repeat = 1;
			string lowerFileName = boost::to_lower_copy(filelist[i]);

			if (coreModList.find(lowerFileName) != coreModList.end())
			{
				repeat = int(coreModList[lowerFileName].size());
				isCore = true;
			}

			while (repeatcount < repeat)
			{
				if (isCore)
				{
					modID = coreModList[lowerFileName][repeatcount];
				}

				bool skip = false;
				string tempfilename = filelist[i].substr(0, filelist[i].find_last_of("."));
				boost::algorithm::to_lower(tempfilename);
				string temppath = behaviorPath[tempfilename];

				if (temppath.length() != 0)
				{
					size_t nextpos = 0;
					size_t lastpos = temppath.find_last_of("\\");

					while (temppath.find("\\", nextpos) != lastpos)
					{
						nextpos = temppath.find("\\", nextpos) + 1;
					}

					temppath = temppath.substr(nextpos, lastpos - nextpos);
				}

				QThread* thread = new QThread;
				BehaviorSub* worker = new BehaviorSub;
				worker->addInfo(directory, filelist, i, behaviorPriority, chosenBehavior, BehaviorTemplate, newAnimation, AnimVar, newAnimEvent, newAnimVariable, ignoreFunction, false, modID);
				connect(worker, SIGNAL(progressUp()), behaviorProcess, SLOT(milestoneUp()));
				connect(worker, SIGNAL(done()), behaviorProcess, SLOT(EndAttempt()));
				connect(worker, SIGNAL(done()), thread, SLOT(quit()));
				connect(worker, SIGNAL(done()), worker, SLOT(deleteLater()));

				connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

				if (boost::iequals(filelist[i], "animationdatasinglefile.txt"))
				{
					connect(thread, SIGNAL(started()), worker, SLOT(AnimDataCompilation()));
				}
				else if (boost::iequals(filelist[i], "animationsetdatasinglefile.txt"))
				{
					connect(thread, SIGNAL(started()), worker, SLOT(ASDCompilation()));
				}
				else if (temppath.find("characters") != 0)
				{
					connect(thread, SIGNAL(started()), worker, SLOT(BehaviorCompilation()));
				}
				else
				{
					worker->isCharacter = true;
					connect(thread, SIGNAL(started()), worker, SLOT(BehaviorCompilation()));
					connect(worker, SIGNAL(newAnim()), behaviorProcess, SLOT(increaseAnimCount()));
				}

				++repeatcount;
				++runningThread;
				worker->moveToThread(thread);
				behaviorThreads.push_back(thread);
			}
		}
	}

	for (auto& thread : behaviorThreads)
	{
		thread->start();
	}

	if (error)
	{
		ClearGlobal();
		unregisterProcess();
		return;
	}

	EndAttempt();
}

void BehaviorStart::milestoneStart()
{
	vecstr filelist;
	read_directory("temp_behaviors", filelist);
	int exclude = 2;

	if (isFileExist("temp_behaviors\\xml"))
	{
		++exclude;
	}

	filenum = (int(filelist.size() - exclude) * 10) + 21;
	connectProcess(this);
}

void BehaviorStart::milestoneUp()
{
	if (!error)
	{
		std::lock_guard<std::mutex>locker(pLock);
		int old = progressPercentage * 100 / filenum;
		++progressPercentage;
		int result = progressPercentage * 100 / filenum;

		if (result > old)
		{
			if (result - old < 2)
			{
				emit progress(old + 1);
				Sleep(20);
			}
			else
			{
				for (int i = old + 1; i <= result; ++i)
				{
					if (error)
					{
						break;
					}

					emit progress(i);
					Sleep(rand() % 100 + 100);
				}
			}
		}
	}
}

void BehaviorStart::unregisterProcess()
{
	if (!error)
	{
		interMsg("Behavior generation complete");
		emit enableCheck(false);
	}
	else
	{
		interMsg("Failed to generate behavior");
	}

	disconnectProcess();
	emit enable(false);
	emit hide(true);
	emit end();
}

void BehaviorStart::EndAttempt()
{
	--runningThread;

	if (runningThread == 0)
	{
		ClearGlobal(false);
		QTimer* timer = new QTimer;
		timer->setSingleShot(true);
		connect(timer, SIGNAL(timeout()), this, SLOT(unregisterProcess()));
		timer->start(1500);
	}
}

void BehaviorStart::increaseAnimCount()
{
	if (!error)
	{
		++animCount;
		emit totalAnim(animCount);
	}
}

void BehaviorSub::BehaviorCompilation()
{
	try
	{
		ImportContainer exportID;

		string filepath = directory + filelist[curList];
		string behaviorFile = filelist[curList].substr(0, filelist[curList].find_last_of("."));
		string lowerBehaviorFile = boost::algorithm::to_lower_copy(behaviorFile);

		int lastID = 0;
		int curID = 0;
		int firstID;

		unordered_map<string, bool> orievent;
		id eventid;

		unordered_map<string, bool> orivariable;
		id variableid;

		id attriid;
		id charid;

		unordered_map<int, unordered_map<string, vecstr>> clipAA;			// node id, original animation, list of alternative animations path
		unordered_map<string, vector<IDCatcher>> catcher;

		string ZeroEvent;
		SSMap IDExist;
		map<int, vecstr> catalystMap;

		// final output
		string outputdir;

#ifdef DEBUG
		if (modID.length() > 0)
		{
			outputdir = "new_behaviors\\" + GetFileDirectory(behaviorPath[lowerBehaviorFile].substr(behaviorPath[lowerBehaviorFile].find("\\") + 1)) + modID + lowerBehaviorFile;
		}
		else
		{
			outputdir = "new_behaviors\\" + behaviorPath[lowerBehaviorFile].substr(behaviorPath[lowerBehaviorFile].find("\\") + 1);
		}
#else
		if (modID.length() > 0)
		{
			outputdir = GetFileDirectory(behaviorPath[lowerBehaviorFile]) + modID + lowerBehaviorFile;
		}
		else
		{
			outputdir = behaviorPath[lowerBehaviorFile];
		}
#endif

		emit progressUp();

		if (modID.length() > 0 && isFileExist(outputdir + ".hkx"))
		{
			int i = 0;

			while (i < 9)
			{
				emit progressUp();
				++i;
			}

			emit done();
			return;
		}

		double duration;

		{
			bool eventOpen = false;
			bool varOpen = false;
			bool attriOpen = false;
			bool charOpen = false;
			bool animOpen = false;
			bool otherAnimOpen = false;
			bool norElement = false;
			bool isClip = false;
			bool hasAA = false;

			int oribone = -1;
			int bonenum = -1;
			int eventelements = -1;
			int variableelements = -1;
			int attributeelements = -1;
			int characterelements = -1;
			size_t elementLine = -1;
			size_t openRange = 0;

			unordered_map<int, string> eventName;
			unordered_map<int, string> varName;
			unordered_map<int, string> attriName;
			unordered_map<int, string> charName;

			vector<int> behaviorDataNode;
			set<string> AAGroupList;
			set<string> AAEventName;

			// read behavior file
			vecstr catalyst;
			GetFunctionLines(filepath, catalyst);

			// check for error
			if (error)
			{
				emit done();
				return;
			}

			if (alternateAnim.size() != 0)
			{
				hasAA = true;
			}

			if (isCharacter)
			{
				string rigfile = "<hkparam name=\"rigName\">";
				string bonemap = "<hkparam name=\"bonePairMap\" numelements=\"";
				bool found = false;

				for (string& line : catalyst)
				{
					if (line.find(bonemap) != NOT_FOUND)
					{
						size_t pos = line.find(bonemap) + bonemap.length();
						oribone = stoi(line.substr(pos, line.find("\">", pos) - pos));
					}
					else if (line.find(rigfile) != NOT_FOUND)
					{
						size_t pos = line.find(rigfile) + rigfile.length();
						rigfile = line.substr(pos, line.find("</hkparam>", pos) - pos);
						found = true;
						break;
					}
				}

				boost::filesystem::path curFile(GetFileDirectory(outputdir));
				rigfile = curFile.parent_path().parent_path().string() + "\\" + rigfile;

				if (found && isFileExist(rigfile) && !boost::filesystem::is_directory(rigfile))
				{
					vecstr storeline;
					hkxcmdXmlInput(rigfile.substr(0, rigfile.find_last_of(".")), storeline);

					if (error)
					{
						emit done();
						return;
					}

					bonemap = "<hkparam name=\"parentIndices\" numelements=\"";
					
					for (auto& line : storeline)
					{
						if (line.find(bonemap) != NOT_FOUND)
						{
							size_t pos = line.find(bonemap) + bonemap.length();
							int num = stoi(line.substr(pos, line.find("\">", pos) - pos));

							if (oribone < num)
							{
								bonenum = num - oribone;
								break;
							}
						}
					}
				}
			}

			for (unsigned int l = 0; l < catalyst.size(); ++l)
			{
				string line = catalyst[l];
				size_t pos = line.find("<hkobject name=\"");

				if (pos != NOT_FOUND && line.find("signature=\"", pos) != NOT_FOUND)
				{
					string templine = line.substr(0, line.find("class"));
					string tempID = boost::regex_replace(string(templine), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
					curID = stoi(tempID);

					if (tempID != templine && curID >= lastID)
					{
						lastID = curID + 1;
					}


					if (line.find("class=\"hkbClipGenerator\" signature=\"", pos) != NOT_FOUND)
					{
						isClip = true;
					}
					else
					{
						isClip = false;
					}
				}
				else if (isClip && line.find("<hkparam name=\"animationName\">") != NOT_FOUND)
				{
					size_t pos = line.find("animationName\">") + 15;
					string animPath = line.substr(pos, line.find("</hkparam>", pos) - pos);
					boost::algorithm::to_lower(animPath);
					addUsedAnim(lowerBehaviorFile, animPath);
					string animFile = GetFileName(animPath) + ".hkx";
					isClip = false;

					if (error)
					{
						emit done();
						return;
					}

					// Dynamic AA function without a template
					if (hasAA && alternateAnim.find(animFile) != alternateAnim.end())
					{
						clipAA[curID][animFile] = alternateAnim[animFile];
						AAGroupList.insert(AAGroup[animFile]);

						if (AAHasEvent.find(animFile) != AAHasEvent.end())
						{
							for (unsigned int j = 0; j < AAHasEvent[animFile].size(); ++j)
							{
								for (unsigned int k = 0; k < AAEvent[AAHasEvent[animFile][j]].size(); ++k)
								{
									if (k % 2 == 0)
									{
										AAEventName.insert(AAEvent[AAHasEvent[animFile][j]][k]);
									}
								}
							}
						}
					}
				}
				else if (line.find("<hkparam name=\"eventNames\" numelements=", 0) != NOT_FOUND)
				{
					if (line.find("</hkparam>") == NOT_FOUND)
					{
						eventOpen = true;
						string templine = line.substr(0, line.find("<hkparam name=\"", 0));
						openRange = count(templine.begin(), templine.end(), '\t');
					}
				}
				else if (eventOpen || varOpen)
				{
					if (line.find("<hkcstring>") != NOT_FOUND)
					{
						size_t nextpos = line.find("<hkcstring>") + 11;
						string name = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);

						if (eventOpen)
						{
							orievent[name] = true;
						}
						else if (varOpen)
						{
							orivariable[name] = true;
						}
					}
					else if (line.find("</hkparam>") != NOT_FOUND)
					{
						string templine = line.substr(0, line.find("</hkparam>"));
						size_t range = count(templine.begin(), templine.end(), '\t');

						if (openRange == range)
						{
							eventOpen = false;
							varOpen = false;
						}
					}
				}
				else if (line.find("<hkparam name=\"variableNames\" numelements=") != NOT_FOUND)
				{
					varOpen = true;
				}
			}

			if (error)
			{
				emit done();
				return;
			}

			if (clipAA.size() == 0 && !activatedBehavior[lowerBehaviorFile])
			{
				if (!isEdited(BehaviorTemplate, lowerBehaviorFile, newAnimation, isCharacter, modID))
				{
					// final output
#ifdef DEBUG
					outputdir = "new_behaviors\\" + behaviorPath[lowerBehaviorFile].substr(behaviorPath[lowerBehaviorFile].find("\\") + 1);
#else
					outputdir = behaviorPath[lowerBehaviorFile] + ".hkx";
#endif
					string cachedFile = "cached_behaviors\\" + lowerBehaviorFile + ".hkx";

					if (isFileExist(cachedFile) && !boost::filesystem::is_directory(cachedFile))
					{
						boost::filesystem::copy_file(cachedFile, outputdir, boost::filesystem::copy_option::overwrite_if_exists);
					}
					else if(isFileExist(outputdir) && !boost::filesystem::is_directory(outputdir))
					{
						if (!boost::filesystem::remove(outputdir))
						{
							WarningMessage(1005, outputdir);
						}
					}

					int i = 0;

					while (i < 9)
					{
						emit progressUp();
						++i;
					}

					emit done();
					return;
				}
			}

			emit progressUp();

			int newModID = -500000;
			int counter = 0;
			openRange = -1;

			{
				size_t pos = catalyst[1].find("toplevelobject=");

				if (pos == NOT_FOUND)
				{
					ErrorMessage(1095, filepath);
					emit done();
					return;
				}

				firstID = stoi(boost::regex_replace(string(catalyst[1].substr()), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
			}

			curID = firstID;
			bool negative = false;
			bool isOpen = true;
			bool special = false;
			string newMod;
			string curNum;
			unordered_map<string, string> chosenLines;
			unordered_map<string, bool> isAdded;

			// add picked behavior and remove not picked behavior
			for (unsigned int l = 0; l < catalyst.size(); ++l)
			{
				bool elementCatch = false;
				bool isNewID = false;
				string line = catalyst[l];
				bool skip = false;

				if (line.find("<!-- ", 0) != NOT_FOUND)
				{
					if (line.find("<!-- NEW *", 0) != NOT_FOUND)
					{
						size_t tempint = line.find("<!-- NEW *", 0) + 10;
						string modID = line.substr(tempint, line.find("* -->", tempint + 1) - tempint);

						if (!chosenBehavior[modID])
						{
							isOpen = false;
						}
						else
						{
							newMod = modID;
						}

						skip = true;
					}
					else if (line.find("<!-- NEW ^", 0) != NOT_FOUND)
					{
						special = true;
					}
					else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
					{
						isOpen = true;
						newMod.clear();

						if (!special)
						{
							skip = true;
						}
						else
						{
							special = false;
						}
					}
				}

				if (isOpen && !skip)
				{
					if (line == "	</hksection>")
					{
						break;
					}

					while (true)
					{
						if (line.find("<hkobject name=\"", 0) != NOT_FOUND && line.find("signature=\"", 0) != NOT_FOUND)
						{
							string tempID = boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

							if (line.find("\"#" + tempID + "\"") != NOT_FOUND)
							{
								if (catalystMap[curID].size() != 0)
								{
									if (catalystMap[curID].back().empty())
									{
										if (catalystMap[curID].size() > 1 && catalystMap[curID][catalystMap[curID].size() - 2].empty())
										{
											catalystMap[curID].pop_back();
										}
									}
									else
									{
										catalystMap[curID].push_back("");
									}
								}

								catalystMap[curID].shrink_to_fit();
								curID = stoi(tempID);
								catalystMap[curID].reserve(100 * memory);
							}
							else
							{
								catalystMap[curID].shrink_to_fit();
								isNewID = true;
							}

							if (line.find("class=\"hkbBehaviorGraphStringData\" signature=\"", 0) != NOT_FOUND || line.find("class=\"hkbVariableValueSet\" signature=\"", 0) != NOT_FOUND || line.find("class=\"hkbBehaviorGraphData\" signature=\"", 0) != NOT_FOUND)
							{
								behaviorDataNode.push_back(curID);
							}
						}

						if (line.find("<!-- *", 0) != NOT_FOUND)
						{
							size_t tempint = line.find("<!-- *") + 6;
							string modID = line.substr(tempint, line.find("* -->", tempint + 1) - tempint);
							chosenLines[modID] = line;
							break;
						}
						else if (line.find("<!-- original -->", 0) != NOT_FOUND)
						{
							if (chosenLines.size() != 0)
							{
								line = behaviorLineChooser(line, chosenLines, behaviorPriority);
								chosenLines.clear();
							}
							else
							{
								ErrorMessage(1165);
								emit done();
								return;
							}
						}

						// get event id from newAnimation
						if (line.find("<hkparam name=\"eventNames\" numelements=", 0) != NOT_FOUND || line.find("<hkparam name=\"eventInfos\" numelements=", 0) != NOT_FOUND)
						{
							if (eventelements == -1)
							{
								if (line.find("</hkparam>") == NOT_FOUND)
								{
									eventOpen = true;
									elementCatch = true;
									string templine = line.substr(0, line.find("<hkparam name=\"", 0));
									openRange = count(templine.begin(), templine.end(), '\t');
								}
							}
							else
							{
								eventOpen = true;
								string templine = line.substr(0, line.find("<hkparam name=\"", 0));
								openRange = count(templine.begin(), templine.end(), '\t');
								size_t position = line.find("numelements=\"") + 13;
								string element = line.substr(position, line.find("\">") - position);
								line.replace(line.find(element), element.length(), to_string(eventelements));
							}

							size_t position = line.find("<hkparam name=\"") + 15;
							curNum = line.substr(position, line.find("\" numelements=\"") - position);
						}
						else if (line.find("<hkparam name=\"attributeNames\" numelements=") != NOT_FOUND || line.find("<hkparam name=\"attributeDefaults\" numelements=") != NOT_FOUND)
						{
							if (attributeelements == -1)
							{
								if (line.find("</hkparam>") == NOT_FOUND)
								{
									attriOpen = true;
									elementCatch = true;
									string templine = line.substr(0, line.find("<hkparam name=\"", 0));
									openRange = count(templine.begin(), templine.end(), '\t');
								}
							}
							else
							{
								size_t position = line.find("numelements=\"") + 13;
								string element = line.substr(position, line.find("\">") - position);
								line.replace(line.find(element), element.length(), to_string(attributeelements));
							}

							size_t position = line.find("<hkparam name=\"") + 15;
							curNum = line.substr(position, line.find("\" numelements=\"") - position);
						}
						else if (line.find("<hkparam name=\"characterPropertyNames\" numelements=") != NOT_FOUND || line.find("<hkparam name=\"characterPropertyInfos\" numelements=") != NOT_FOUND)
						{
							if (characterelements == -1)
							{
								if (line.find("</hkparam>") == NOT_FOUND)
								{
									charOpen = true;
									elementCatch = true;
									string templine = line.substr(0, line.find("<hkparam name=\"", 0));
									openRange = count(templine.begin(), templine.end(), '\t');
								}
							}
							else
							{
								size_t position = line.find("numelements=\"") + 13;
								string element = line.substr(position, line.find("\">") - position);
								line.replace(line.find(element), element.length(), to_string(characterelements));
							}

							size_t position = line.find("<hkparam name=\"") + 15;
							curNum = line.substr(position, line.find("\" numelements=\"") - position);
						}
						else if (line.find("<hkparam name=\"variableNames\" numelements=") != NOT_FOUND || line.find("<hkparam name=\"wordVariableValues\" numelements=") != NOT_FOUND || line.find("<hkparam name=\"variableInfos\" numelements=") != NOT_FOUND)
						{
							if (variableelements == -1)
							{
								if (line.find("</hkparam>") == NOT_FOUND)
								{
									varOpen = true;
									elementCatch = true;
								}
							}
							else
							{
								varOpen = true;
								size_t position = line.find("numelements=\"") + 13;
								string element = line.substr(position, line.find("\">") - position);
								line.replace(line.find(element), element.length(), to_string(variableelements));
							}

							string templine = line.substr(0, line.find("<hkparam name=\"", 0));
							openRange = count(templine.begin(), templine.end(), '\t');
							size_t position = line.find("<hkparam name=\"") + 15;
							curNum = line.substr(position, line.find("\" numelements=\"") - position);
						}
						else if (line.find("<hkparam name=\"animationNames\" numelements=\"") != NOT_FOUND)
						{
							animOpen = true;
							elementCatch = true;
							string templine = line.substr(0, line.find("<hkparam name=\"", 0));
							openRange = count(templine.begin(), templine.end(), '\t');
							size_t pos = line.find("<hkparam name=\"animationNames\" numelements=\"") + 44;
							string number = line.substr(pos, line.find("\">", pos) - pos);

							if (!isOnlyNumber(number))
							{
								ErrorMessage(1174, filepath, l + 1, line.substr(line.find("<hkparam name=\"animationNames\" numelements=\"")));
								emit done();
								return;
							}

							base = stoi(number);
						}
						else if (line.find("<hkparam name=\"deformableSkinNames\" numelements=\"") != NOT_FOUND || line.find("<hkparam name=\"rigidSkinNames\" numelements=\"") != NOT_FOUND || line.find("<hkparam name=\"animationFilenames\" numelements=\"") != NOT_FOUND)
						{
							otherAnimOpen = true;
							elementCatch = true;
							string templine = line.substr(0, line.find("<hkparam name=\"", 0));
							openRange = count(templine.begin(), templine.end(), '\t');
						}
						else if (line.find("<hkparam name=\"") != NOT_FOUND && line.find("numelements=\"") != NOT_FOUND && line.find("</hkparam>") == NOT_FOUND)
						{
							norElement = true;
							elementCatch = true;
							string templine = line.substr(0, line.find("<hkparam name=\"", 0));
							openRange = count(templine.begin(), templine.end(), '\t');
						}
						else if (line.find("</hkparam>") != NOT_FOUND && (norElement || eventOpen || varOpen || attriOpen || charOpen || animOpen || otherAnimOpen))
						{
							string templine = line.substr(0, line.find("</hkparam>"));
							size_t range = count(templine.begin(), templine.end(), '\t');

							if (openRange == range)
							{
								unordered_map<string, bool> isExist;

								// reset
								if (norElement)
								{
									norElement = false;
								}
								else if (eventOpen)
								{
									for (auto it = AAEventName.begin(); it != AAEventName.end(); ++it)
									{
										if (curNum == "eventNames")
										{
											if (isExist[*it])
											{
												WarningMessage(1010, *it);
											}
											else if (!orievent[*it])
											{
												if (counter == 0)
												{
													ZeroEvent = *it;
												}

												eventid[*it] = counter;
												eventName[counter] = *it;
												catalystMap[curID].push_back("				<hkcstring>" + *it + "</hkcstring>");
												isExist[*it] = true;
												counter++;
											}
										}
										else if (curNum == "eventInfos")
										{
											if (!isExist[*it] && !orievent[*it])
											{
												catalystMap[curID].push_back("				<hkobject>");
												catalystMap[curID].push_back("					<hkparam name=\"flags\">0</hkparam>");
												catalystMap[curID].push_back("				</hkobject>");
												isExist[*it] = true;
												counter++;
											}
										}
										else
										{
											ErrorMessage(1031);
											emit done();
											return;
										}
									}

									vecstr codelist = BehaviorTemplate.grouplist[lowerBehaviorFile];

									for (unsigned int j = 0; j < codelist.size(); ++j)
									{
										for (auto it = newAnimEvent[codelist[j]].begin(); it != newAnimEvent[codelist[j]].end(); ++it)
										{
											if (curNum == "eventNames")
											{
												if (isExist[*it])
												{
													WarningMessage(1010, *it);
												}
												else if (!orievent[*it])
												{
													if (counter == 0)
													{
														ZeroEvent = *it;
													}

													eventid[*it] = counter;
													eventName[counter] = *it;
													catalystMap[curID].push_back("				<hkcstring>" + *it + "</hkcstring>");
													isExist[*it] = true;
													counter++;
												}
											}
											else if (curNum == "eventInfos")
											{
												if (!isExist[*it] && !orievent[*it])
												{
													catalystMap[curID].push_back("				<hkobject>");
													catalystMap[curID].push_back("					<hkparam name=\"flags\">0</hkparam>");
													catalystMap[curID].push_back("				</hkobject>");
													isExist[*it] = true;
													counter++;
												}
											}
											else
											{
												ErrorMessage(1031);
												emit done();
												return;
											}
										}
									}

									if (eventelements == -1)
									{
										eventelements = counter;
									}

									eventOpen = false;
								}
								else if (varOpen)
								{
									for (auto it = AAGroupList.begin(); it != AAGroupList.end(); ++it)
									{
										string variablename = "Nemesis_AA_" + *it;

										if (curNum == "variableNames")
										{
											if (isExist[variablename])
											{
												WarningMessage(1011, variablename);
											}
											else if (!orivariable[variablename])
											{
												string curline = "				<hkcstring>" + variablename + "</hkcstring>";
												varName[counter] = variablename;
												variableid[variablename] = counter;
												catalystMap[curID].push_back(curline);
												isExist[variablename] = true;
												counter++;
											}
										}
										else if (curNum == "wordVariableValues")
										{
											if (!isExist[variablename] && !orivariable[variablename])
											{
												catalystMap[curID].push_back("				<hkobject>");
												catalystMap[curID].push_back("					<hkparam name=\"value\">0</hkparam>");
												catalystMap[curID].push_back("				</hkobject>");
												isExist[variablename] = true;
												counter++;
											}
										}
										else if (curNum == "variableInfos")
										{
											if (!isExist[variablename] && !orivariable[variablename])
											{
												catalystMap[curID].push_back("				<hkobject>");
												catalystMap[curID].push_back("					<hkparam name=\"role\">");
												catalystMap[curID].push_back("						<hkobject>");
												catalystMap[curID].push_back("							<hkparam name=\"role\">ROLE_DEFAULT</hkparam>");
												catalystMap[curID].push_back("							<hkparam name=\"flags\">0</hkparam>");
												catalystMap[curID].push_back("						</hkobject>");
												catalystMap[curID].push_back("					</hkparam>");
												catalystMap[curID].push_back("					<hkparam name=\"type\">VARIABLE_TYPE_INT32</hkparam>");
												catalystMap[curID].push_back("				</hkobject>");
												isExist[variablename] = true;
												counter++;
											}
										}
										else
										{
											ErrorMessage(1032);
											emit done();
											return;
										}
									}

									vecstr codelist = BehaviorTemplate.grouplist[lowerBehaviorFile];

									for (unsigned int j = 0; j < codelist.size(); ++j)
									{
										for (auto it = newAnimVariable[codelist[j]].begin(); it != newAnimVariable[codelist[j]].end(); ++it)
										{
											string variablename = *it;

											if (curNum == "variableNames")
											{
												if (isExist[variablename])
												{
													WarningMessage(1011, variablename);
												}
												else if (!orivariable[variablename])
												{
													string curline = "				<hkcstring>" + variablename + "</hkcstring>";
													varName[counter] = variablename;
													variableid[variablename] = counter;
													catalystMap[curID].push_back(curline);
													isExist[variablename] = true;
													counter++;
												}
											}
											else if (curNum == "wordVariableValues")
											{
												if (!isExist[variablename] && !orivariable[variablename])
												{
													catalystMap[curID].push_back("				<hkobject>");
													catalystMap[curID].push_back("					<hkparam name=\"value\">" + AnimVar[variablename].init_value + "</hkparam>");
													catalystMap[curID].push_back("				</hkobject>");
													isExist[variablename] = true;
													counter++;
												}
											}
											else if (curNum == "variableInfos")
											{
												if (!isExist[variablename] && !orivariable[variablename])
												{
													catalystMap[curID].push_back("				<hkobject>");
													catalystMap[curID].push_back("					<hkparam name=\"role\">");
													catalystMap[curID].push_back("						<hkobject>");
													catalystMap[curID].push_back("							<hkparam name=\"role\">ROLE_DEFAULT</hkparam>");
													catalystMap[curID].push_back("							<hkparam name=\"flags\">0</hkparam>");
													catalystMap[curID].push_back("						</hkobject>");
													catalystMap[curID].push_back("					</hkparam>");
													catalystMap[curID].push_back("					<hkparam name=\"type\">VARIABLE_TYPE_" + AnimVar[variablename].var_type + "</hkparam>");
													catalystMap[curID].push_back("				</hkobject>");
													isExist[variablename] = true;
													counter++;
												}
											}
											else
											{
												ErrorMessage(1032);
												emit done();
												return;
											}
										}
									}

									if (variableelements == -1)
									{
										variableelements = counter;
									}

									varOpen = false;
								}
								else if (attriOpen)
								{
									if (attributeelements == -1)
									{
										attributeelements = counter;
									}

									attriOpen = false;
								}
								else if (charOpen)
								{
									if (characterelements == -1)
									{
										characterelements = counter;
									}

									charOpen = false;
								}
								else if (animOpen)		// adding new animation to character file
								{
									for (auto it = BehaviorTemplate.grouplist.begin(); it != BehaviorTemplate.grouplist.end(); ++it)
									{
										vecstr behaviorNames = behaviorJoints[it->first];

										for (unsigned int k = 0; k < behaviorNames.size(); ++k)
										{
											if (it->second.size() > 0 && lowerBehaviorFile == behaviorNames[k])
											{
												vecstr templateGroup = it->second;

												for (unsigned int j = 0; j < templateGroup.size(); ++j)
												{
													string templatecode = templateGroup[j];

													if (newAnimation[templatecode].size() != 0 && !BehaviorTemplate.optionlist[templatecode].core)
													{
														for (unsigned int k = 0; k < newAnimation[templatecode].size(); ++k)
														{
															if (!newAnimation[templatecode][k]->isKnown())
															{
																string animPath = "Animations\\" + newAnimation[templatecode][k]->GetFilePath();

																if (!isAdded[animPath])
																{
																	catalystMap[curID].push_back("				<hkcstring>" + animPath + "</hkcstring>");
																	isAdded[animPath] = true;
																	newMod = animPath.substr(10, animPath.find("\\", 10) - 10);
																	boost::algorithm::to_lower(animPath);
																	string animFile = GetFileName(animPath);
																	size_t matchSize = animModMatch[behaviorFile][animFile].size();
																	registeredAnim[lowerBehaviorFile][animFile] = true;
																	addAnimation();

																	if (matchSize == 0)
																	{
																		set<string> tempSetString;
																		tempSetString.insert(animPath);
																		animModMatch[behaviorFile][animFile].push_back(tempSetString);
																		tempSetString.clear();
																		tempSetString.insert(newMod);
																		animModMatch[behaviorFile][animFile].push_back(tempSetString);
																	}
																	else if (matchSize == 2)
																	{
																		animModMatch[behaviorFile][animFile][0].insert(animPath);
																		animModMatch[behaviorFile][animFile][1].insert(newMod);
																	}
																	else
																	{
																		ErrorMessage(1058);
																		emit done();
																		return;
																	}

																	counter++;
																}
															}
														}
													}
												}
											}
										}
									}

									for (auto& AA : alternateAnim)
									{
										for (auto& anim : AA.second)
										{
											if (anim != "x")
											{
												string animPath = "Animations\\" + anim;

												if (!isAdded[animPath])
												{
													catalystMap[curID].push_back("				<hkcstring>" + animPath + "</hkcstring>");
													isAdded[animPath] = true;
													newMod = animPath.substr(10, animPath.find("\\", 10) - 10);
													boost::algorithm::to_lower(animPath);
													string animFile = GetFileName(animPath);
													size_t matchSize = animModMatch[behaviorFile][animFile].size();
													registeredAnim[lowerBehaviorFile][animFile] = true;
													addAnimation();

													if (matchSize == 0)
													{
														set<string> tempSetString;
														tempSetString.insert(animPath);
														animModMatch[behaviorFile][animFile].push_back(tempSetString);
														tempSetString.clear();
														tempSetString.insert(newMod);
														animModMatch[behaviorFile][animFile].push_back(tempSetString);
													}
													else if (matchSize == 2)
													{
														animModMatch[behaviorFile][animFile][0].insert(animPath);
														animModMatch[behaviorFile][animFile][1].insert(newMod);
													}
													else
													{
														ErrorMessage(1058);
														emit done();
														return;
													}

													counter++;
												}
											}
										}
									}

									animOpen = false;
								}
								else if (otherAnimOpen)
								{
									otherAnimOpen = false;
								}

								if (elementLine != -1)
								{
									size_t position = catalystMap[curID][elementLine].find("numelements=\"") + 13;
									string oldElement = catalystMap[curID][elementLine].substr(position, catalystMap[curID][elementLine].find("\">", position) - position);

									if (oldElement != to_string(counter))
									{
										catalystMap[curID][elementLine].replace(catalystMap[curID][elementLine].find(oldElement), oldElement.length(), to_string(counter));
									}

									elementLine = -1;
								}

								curNum = "";
								counter = 0;
								openRange = 0;
							}
						}
						
						int repeatnum = 1;

						if (bonenum > 0 && oribone > 0)
						{
							size_t pos = line.find("<!-- Bone$N -->");

							if (pos != NOT_FOUND && pos == line.length() - 15)
							{
								repeatnum = bonenum;
							}
						}

						if (line.find("<hkobject name=\"", 0) != NOT_FOUND && line.find("signature=\"", 0) != NOT_FOUND)
						{
							size_t nextpos = line.find("<hkobject name=\"", 0) + 17;
							string funcID = line.substr(nextpos, line.find("class=\"", nextpos) - nextpos - 2);

							if (funcID.find("$", 0) != NOT_FOUND)
							{
								string modID = funcID.substr(0, funcID.find("$"));

								if (chosenBehavior[modID])
								{
									string ID = to_string(lastID);

									while (ID.length() < 4)
									{
										ID = "0" + ID;
									}

									line.replace(line.find(funcID), funcID.length(), ID);
									IDExist[funcID] = ID;

									if (isNewID)
									{
										curID = lastID;
										catalystMap[curID].reserve(100 * memory);
										isNewID = false;
										++lastID;

										if (lastID == 9216)
										{
											++lastID;
										}
									}
								}
								else if (modID == "MID")
								{
									ErrorMessage(1020);
									emit done();
									return;
								}
							}
						}
						else if (line.find("$", line.find("#")) != NOT_FOUND)
						{
							if (line.find(">#") != NOT_FOUND)
							{
								size_t reference = count(line.begin(), line.end(), '#');
								size_t nextpos = 0;

								for (size_t k = 0; k < reference; ++k)
								{
									nextpos = line.find("#", nextpos) + 1;
									string numID = boost::regex_replace(string(line.substr(nextpos)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
									string ID = line.substr(nextpos, line.find(numID) - nextpos + numID.length());

									if (line.find(ID, 0) != NOT_FOUND && ID.find("$") != NOT_FOUND)
									{
										if (IDExist[ID].length() != 0)
										{
											line.replace(nextpos, ID.length(), IDExist[ID]);
										}
										else if (!special)
										{
											IDCatcher catchingID(curID, int(catalystMap[curID].size()));
											catcher[ID].push_back(catchingID);
										}
									}
								}
							}
							else if (line.find("\t\t\t#") != NOT_FOUND)
							{
								stringstream sstream(line);
								istream_iterator<string> ssbegin(sstream);
								istream_iterator<string> ssend;
								vecstr generator(ssbegin, ssend);
								copy(generator.begin(), generator.end(), generator.begin());

								for (unsigned int p = 0; p < generator.size(); p++)
								{
									string ID = generator[p];
									string numID = boost::regex_replace(string(ID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

									if (ID.find("$") != NOT_FOUND)
									{
										string masterFormat = ID.substr(1, ID.find("$") - 1);

										if (ID == "#" + masterFormat + "$" + numID && line.find(ID, 0) != NOT_FOUND)
										{
											ID = ID.substr(1, ID.length() - 1);

											if (IDExist[ID].length() != 0)
											{
												line.replace(line.find(ID), ID.length(), IDExist[ID]);
											}
											else if (!special)
											{
												IDCatcher catchingID(curID, int(catalystMap[curID].size()));
												catcher[ID].push_back(catchingID);
											}
										}
									}
								}
							}
						}

						// counting for numelement
						if (eventOpen && line.find("<hkcstring>") != NOT_FOUND)
						{
							size_t nextpos = line.find("<hkcstring>") + 11;
							string name = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
							eventName[counter] = name;
							eventid[name] = counter;

							if (counter == 0)
							{
								ZeroEvent = name;
							}

							counter++;
						}
						else if (varOpen)
						{
							if (curNum == "variableNames" && line.find("<hkcstring>") != NOT_FOUND)
							{
								size_t nextpos = line.find("<hkcstring>") + 11;
								string name = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
								varName[counter] = name;
								variableid[name] = counter;
								counter++;
							}
							else if (curNum == "wordVariableValues" && line.find("<hkparam name=\"value\">") != NOT_FOUND)
							{
								counter++;
							}
							else if (curNum == "variableInfos" && line.find("<hkparam name=\"type\">") != NOT_FOUND)
							{
								counter++;
							}
						}
						else if (attriOpen && line.find("<hkcstring>") != NOT_FOUND)
						{
							size_t nextpos = line.find("<hkcstring>") + 11;
							string name = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
							attriName[counter] = name;
							attriid[name] = counter;
							counter++;
						}
						else if (charOpen && line.find("<hkcstring>") != NOT_FOUND)
						{
							size_t nextpos = line.find("<hkcstring>") + 11;
							string name = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
							charName[counter] = name;
							charid[name] = counter;
							counter++;
						}
						else if (animOpen && line.find("<hkcstring>") != NOT_FOUND)
						{
							size_t nextpos = line.find("<hkcstring>") + 11;
							string animPath = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
							isAdded[animPath] = true;
							boost::algorithm::to_lower(animPath);
							string animFile = GetFileName(animPath);
							registeredAnim[lowerBehaviorFile][animFile] = true;
							addAnimation();
							counter++;

							if (newMod.length() == 0)
							{
								newMod = "Skyrim";
							}

							size_t matchSize = animModMatch[behaviorFile][animFile].size();

							if (matchSize == 0)
							{
								set<string> tempSetString;
								tempSetString.insert(animPath);
								animModMatch[behaviorFile][animFile].push_back(tempSetString);
								tempSetString.clear();
								tempSetString.insert(newMod);
								animModMatch[behaviorFile][animFile].push_back(tempSetString);
							}
							else if (matchSize == 2)
							{
								animModMatch[behaviorFile][animFile][0].insert(animPath);
								animModMatch[behaviorFile][animFile][1].insert(newMod);
							}
							else
							{
								ErrorMessage(1058);
								emit done();
								return;
							}
						}
						else if (otherAnimOpen && line.find("<hkcstring>") != NOT_FOUND)
						{
							counter++;
						}
						else if (norElement)
						{
							string templine = catalyst[l];

							if (templine.find("<hkobject>") != NOT_FOUND)
							{
								templine = templine.substr(0, templine.find("<hkobject>"));
								size_t range = count(templine.begin(), templine.end(), '\t');

								if (range == openRange + 1)
								{
									counter++;
								}
							}
							else if (templine.find("\t\t\t#") != NOT_FOUND)
							{
								templine = templine.substr(0, templine.find("#", 0));
								size_t reference = count(templine.begin(), templine.end(), '\t');

								if (reference == openRange + 1)
								{
									int number = int(count(catalyst[l].begin(), catalyst[l].end(), '#'));
									counter += number;
								}
							}
						}

						if (isNewID)
						{
							curID = newModID;
							catalystMap[curID].reserve(100 * memory);
							newModID++;
						}

						if (line.find("$") != NOT_FOUND)
						{
							if (!ignoreFunction[filelist[curList]][curID])
							{
								__int64 funCounter = count(line.begin(), line.end(), '$') / 2;
								size_t curPos = 0;

								for (int i = 0; i < funCounter; ++i)
								{
									bool isChange = false;
									curPos = line.find("$", curPos + 1);
									string change = line.substr(curPos, line.find("$", curPos + 1) - curPos + 1);
									string oldChange = change;
									change = change.substr(1, change.length() - 2);
									curPos = line.find("$", curPos + 1);

									if (change.find("eventID[", 0) != NOT_FOUND &&  change.find("]", 0) != NOT_FOUND)
									{
										eventIDReplacer(change, "BASE", behaviorFile, eventid, ZeroEvent, l + 1);
										isChange = true;
									}

									if (error)
									{
										emit done();
										return;
									}

									if (change.find("variableID[", 0) != NOT_FOUND &&  change.find("]", 0) != NOT_FOUND)
									{
										variableIDReplacer(change, "BASE", behaviorFile, variableid, l + 1);
										isChange = true;
									}

									if (error)
									{
										emit done();
										return;
									}

									if (isChange)
									{
										line.replace(line.find(oldChange), oldChange.length(), change);
									}
								}
							}
						}

						if (line.find("<hkparam name=\"localTime\">-") != NOT_FOUND)
						{
							negative = true;
						}
						else if (negative && line.find("<hkparam name=\"relativeToEndOfClip\">") != NOT_FOUND)
						{
							if (line.find("<hkparam name=\"relativeToEndOfClip\">true</hkparam>") == NOT_FOUND)
							{
								size_t pos = line.find("<hkparam name=\"relativeToEndOfClip\">") + 36;
								line.replace(pos, line.find("</hkparam>", pos) - pos, "true");
							}

							negative = false;
						}

						int repeated = 0;

						while (repeated < repeatnum)
						{
							string newline = line;

							if (newline.find("%n") != NOT_FOUND)
							{
								while (newline.find("%n") != NOT_FOUND)
								{
									newline.replace(newline.find("%n"), 2, to_string(repeated + oribone));
								}
							}

							catalystMap[curID].push_back(newline);
							++repeated;
						}

						if (elementCatch)
						{
							elementLine = catalystMap[curID].size() - 1;
						}

						break;
					}
				}

				if (error)
				{
					emit done();
					return;
				}
			}
		}

		if (catalystMap[curID].size() != 0)
		{
			if (catalystMap[curID].back().empty())
			{
				if (catalystMap[curID].size() > 1 && catalystMap[curID][catalystMap[curID].size() - 2].empty())
				{
					catalystMap[curID].pop_back();
				}
			}
			else
			{
				catalystMap[curID].push_back("");
			}
		}

		catalystMap[curID].shrink_to_fit();

		emit progressUp();

		// newAnimation ID in existing function
		if (catcher.size() != 0)
		{
			for (auto it = catcher.begin(); it != catcher.end(); ++it)
			{
				for (unsigned int k = 0; k < it->second.size(); ++k)
				{
					int tempID = it->second[k].getID();
					int tempLine = it->second[k].getLine();
					string* line = &catalystMap[it->second[k].getID()][it->second[k].getLine()];

					if (IDExist[it->first].length() != 0)
					{
						line->replace(line->find(it->first), it->first.length(), IDExist[it->first]);
					}
					else
					{
						ErrorMessage(1026, filepath, it->first.substr(0, it->first.find("$")));
						emit done();
						return;
					}
				}
			}
		}

		if (error)
		{
			emit done();
			return;
		}

		emit progressUp();

		// add new animation
		vector<unique_ptr<vecstr>> allEditLines;
		unordered_map<string, bool> isCoreDone;
		unordered_map<int, shared_ptr<int>> functionState;

		// check for having newAnimation for the file
		if (BehaviorTemplate.grouplist.find(lowerBehaviorFile) != BehaviorTemplate.grouplist.end() && BehaviorTemplate.grouplist[lowerBehaviorFile].size() > 0)
		{
			vecstr templateGroup = BehaviorTemplate.grouplist[lowerBehaviorFile];

			for (unsigned int j = 0; j < templateGroup.size(); ++j)
			{
				string templateCode = templateGroup[j];
				vecstr opening;
				opening.push_back("<!-- ======================== NEMESIS " + templateCode + " TEMPLATE START ======================== -->");
				opening.push_back("");
				allEditLines.push_back(make_unique<vecstr>(opening));
				bool hasGroup = false;
				bool hasMaster = false;
				bool ignoreGroup = false;

				if (BehaviorTemplate.behaviortemplate.find(templateCode + "_group") != BehaviorTemplate.behaviortemplate.end())
				{
					if (BehaviorTemplate.behaviortemplate[templateCode + "_group"].find(lowerBehaviorFile) != BehaviorTemplate.behaviortemplate[templateCode + "_group"].end())
					{
						if (BehaviorTemplate.behaviortemplate[templateCode + "_group"][lowerBehaviorFile].size() > 0)
						{
							hasGroup = true;
						}
					}
				}

				if (BehaviorTemplate.behaviortemplate.find(templateCode + "_master") != BehaviorTemplate.behaviortemplate.end())
				{
					if (BehaviorTemplate.behaviortemplate[templateCode + "_master"].find(lowerBehaviorFile) != BehaviorTemplate.behaviortemplate[templateCode + "_master"].end())
					{
						if (BehaviorTemplate.behaviortemplate[templateCode + "_master"][lowerBehaviorFile].size() > 0)
						{
							hasMaster = true;
						}
					}
				}

				if (newAnimation.find(templateCode) != newAnimation.end() && newAnimation[templateCode].size() != 0)
				{
					while (true)
					{
						if (newAnimation[templateCode].size() == 0 || newAnimSkip(newAnimation[templateCode], modID))
						{
							break;
						}

						group subFunctionIDs;
						subFunctionIDs.singlelist.reserve(memory);
						master groupFunctionIDs;
						group newSubFunctionIDs;
						vector<shared_ptr<int>> stateID;
						vector<vector<shared_ptr<animationInfo>>> groupAnimInfo;
						int stateMultiplier = GetStateCount(BehaviorTemplate.behaviortemplate[templateCode][lowerBehaviorFile]);

						if (!hasGroup)
						{
							stateID = GetStateID(BehaviorTemplate.mainBehaviorJoint[templateCode][lowerBehaviorFile], catalystMap, functionState);

							if (newAnimation[templateCode][0]->GetGroupAnimInfo()[0]->ignoreGroup)
							{
								ignoreGroup = true;
								newSubFunctionIDs.singlelist.reserve(memory);
							}
						}
						else
						{
							stateID.push_back(make_shared<int>(0));
							groupFunctionIDs.grouplist.reserve(memory);
						}

						if (error)
						{
							emit done();
							return;
						}

						size_t newAnimCount = newAnimation[templateCode].size();
						shared_ptr<Furniture> dummyAnimation;

						// individual animation
						for (unsigned int k = 0; k < newAnimCount; ++k)
						{
							while (true)
							{
								if (BehaviorTemplate.optionlist[templateCode].core)
								{
									if (isCoreDone[newAnimation[templateCode][k]->GetGroupAnimInfo()[0]->filename])
									{
										break;
									}
									else
									{
										isCoreDone[newAnimation[templateCode][k]->GetGroupAnimInfo()[0]->filename] = true;
									}
								}

								if (newAnimation[templateCode][k]->coreModID != modID)
								{
									if (dummyAnimation != nullptr && !dummyAnimation->isLast())
									{
										ErrorMessage(1183);
										return;
									}

									break;
								}

								// getlines from newAnination
								dummyAnimation = newAnimation[templateCode][k];
								dummyAnimation->setZeroEvent(ZeroEvent);
								unique_ptr<vecstr> tempoLines = make_unique<vecstr>(dummyAnimation->GetFurnitureLine(lowerBehaviorFile, lastID, exportID, eventid, variableid, stateID, stateMultiplier, hasGroup, BehaviorTemplate.optionlist[templateCode].core, subFunctionIDs));

								if (error)
								{
									emit done();
									return;
								}

								if (tempoLines->size() != 0)
								{
									if (tempoLines->back().empty())
									{
										if (tempoLines->size() > 1 && (*tempoLines)[tempoLines->size() - 2].empty())
										{
											tempoLines->pop_back();
										}
									}
									else
									{
										tempoLines->push_back("");
									}
								}

								subFunctionIDs.singlelist.push_back(dummyAnimation->GetSingle());
								allEditLines.emplace_back(move(tempoLines));

								if (dummyAnimation->isLast())
								{
									if (hasGroup)
									{
										subFunctionIDs.singlelist.shrink_to_fit();
										groupFunctionIDs.grouplist.push_back(subFunctionIDs);
										group newFunctionIDs;
										subFunctionIDs = newFunctionIDs;
										subFunctionIDs.singlelist.reserve(memory);
										stateID = { make_shared<int>(0) };
									}
									else if (ignoreGroup)
									{
										newSubFunctionIDs.singlelist.push_back(subFunctionIDs.singlelist[0]);
										subFunctionIDs.singlelist.clear();
									}

									groupAnimInfo.push_back(dummyAnimation->GetGroupAnimInfo());
								}

								break;
							}
						}

						if (ignoreGroup)
						{
							newSubFunctionIDs.singlelist.shrink_to_fit();
							groupFunctionIDs.grouplist.push_back(newSubFunctionIDs);
							newSubFunctionIDs.singlelist.clear();
						}
						else if (!hasGroup)
						{
							subFunctionIDs.singlelist.shrink_to_fit();
							groupFunctionIDs.grouplist.push_back(subFunctionIDs);
							subFunctionIDs.singlelist.clear();
							vector<shared_ptr<animationInfo>> subGroupAnimInfo;
							vector<vector<shared_ptr<animationInfo>>> newGroupAnimInfo;

							for (unsigned int l = 0; l < groupAnimInfo.size(); ++l)
							{
								subGroupAnimInfo.push_back(groupAnimInfo[l][0]);
							}

							newGroupAnimInfo.push_back(subGroupAnimInfo);
							groupAnimInfo = newGroupAnimInfo;
						}

						// check error before initialization
						for (unsigned int curGroup = 0; curGroup < groupFunctionIDs.grouplist.size(); ++curGroup)
						{
							if (groupFunctionIDs.grouplist[curGroup].singlelist.size() != groupAnimInfo[curGroup].size())
							{
								ErrorMessage(1156, templateCode);
								return;
							}
						}

						// group animation
						if (hasGroup)
						{
							if (!hasMaster)
							{
								stateID = GetStateID(BehaviorTemplate.mainBehaviorJoint[templateCode][lowerBehaviorFile], catalystMap, functionState);

								if (error)
								{
									emit done();
									return;
								}
							}
							else
							{
								stateID = { make_shared<int>(0) };
							}

							for (unsigned int k = 0; k < groupFunctionIDs.grouplist.size(); ++k)
							{
								string filename = templateCode + "_group";
								groupTemplate group(BehaviorTemplate.behaviortemplate[filename][lowerBehaviorFile]);
								group.setZeroEvent(ZeroEvent);
								unique_ptr<vecstr> groupBehaviorTemplate = make_unique<vecstr>(group.getFunctionLines(lowerBehaviorFile, filename, stateID, groupFunctionIDs, groupAnimInfo, lastID, exportID, eventid, variableid, templateCode, k + 1));

								if (error)
								{
									emit done();
									return;
								}

								if (groupBehaviorTemplate->size() != 0)
								{
									if (groupBehaviorTemplate->back().empty())
									{
										if (groupBehaviorTemplate->size() > 1 && (*groupBehaviorTemplate)[groupBehaviorTemplate->size() - 2].empty())
										{
											groupBehaviorTemplate->pop_back();
										}
									}
									else
									{
										groupBehaviorTemplate->push_back("");
									}
								}

								allEditLines.emplace_back(move(groupBehaviorTemplate));
							}
						}

						// master animation
						if (hasMaster)
						{
							string filename = templateCode + "_master";
							stateID = GetStateID(BehaviorTemplate.mainBehaviorJoint[templateCode][lowerBehaviorFile], catalystMap, functionState);

							if (error)
							{
								emit done();
								return;
							}

							groupTemplate master(BehaviorTemplate.behaviortemplate[filename][lowerBehaviorFile]);
							master.setZeroEvent(ZeroEvent);
							unique_ptr<vecstr> masterBehaviorTemplate = make_unique<vecstr>(master.getFunctionLines(lowerBehaviorFile, filename, stateID, groupFunctionIDs, groupAnimInfo, lastID, exportID, eventid, variableid, templateCode, -1));

							if (error)
							{
								emit done();
								return;
							}

							if (masterBehaviorTemplate->size() != 0)
							{
								if (masterBehaviorTemplate->back().empty())
								{
									if (masterBehaviorTemplate->size() > 1 && (*masterBehaviorTemplate)[masterBehaviorTemplate->size() - 2].empty())
									{
										masterBehaviorTemplate->pop_back();
									}
								}
								else
								{
									masterBehaviorTemplate->push_back("");
								}
							}

							allEditLines.emplace_back(move(masterBehaviorTemplate));
						}

						// existing function
						for (unsigned int k = 0; k < BehaviorTemplate.existingFunctionID[templateCode][lowerBehaviorFile].size(); ++k)
						{
							ExistingFunction exFunction;
							exFunction.setZeroEvent(ZeroEvent);
							int functionID = BehaviorTemplate.existingFunctionID[templateCode][lowerBehaviorFile][k];
							catalystMap[functionID] = exFunction.groupExistingFunctionProcess(functionID, catalystMap[functionID], groupFunctionIDs, groupAnimInfo, templateCode, exportID, eventid, variableid, lastID, hasMaster, hasGroup, templateGroup);

							if (error)
							{
								emit done();
								return;
							}

							if (catalystMap[functionID].size() == 0)
							{
								ErrorMessage(1099);
								emit done();
								return;
							}

							if (catalystMap[functionID].back().empty())
							{
								if (catalystMap[functionID].size() > 1 && catalystMap[functionID][catalystMap[functionID].size() - 2].empty())
								{
									catalystMap[functionID].pop_back();
								}
							}
							else
							{
								catalystMap[functionID].push_back("");
							}
						}

						if (error)
						{
							emit done();
							return;
						}

						break;
					}
				}
				else
				{
					// existing function
					for (unsigned int k = 0; k < BehaviorTemplate.existingFunctionID[templateCode][lowerBehaviorFile].size(); ++k)
					{
						vector<vector<shared_ptr<animationInfo>>> animInfo;
						master subFunctionIDs;
						ExistingFunction exFunction;
						exFunction.setZeroEvent(ZeroEvent);
						int functionID = BehaviorTemplate.existingFunctionID[templateCode][lowerBehaviorFile][k];
						catalystMap[functionID] = exFunction.groupExistingFunctionProcess(functionID, catalystMap[functionID], subFunctionIDs, animInfo, templateCode, exportID, eventid, variableid, lastID, hasMaster, hasGroup, templateGroup);

						if (error)
						{
							emit done();
							return;
						}

						if (catalystMap[functionID].size() == 0)
						{
							ErrorMessage(1099);
							emit done();
							return;
						}

						if (catalystMap[functionID].back().empty())
						{
							if (catalystMap[functionID].size() > 1 && catalystMap[functionID][catalystMap[functionID].size() - 2].empty())
							{
								catalystMap[functionID].pop_back();
							}
						}
						else
						{
							catalystMap[functionID].push_back("");
						}
					}
				}

				vecstr closing;
				closing.push_back("<!-- ======================== NEMESIS " + templateCode + " TEMPLATE END ======================== -->");
				allEditLines.push_back(make_unique<vecstr>(closing));
			}
		}

		// check for error
		if (error)
		{
			emit done();
			return;
		}

		emit progressUp();

		// AA animation installation
		vecstr AAlines;

		if (clipAA.size() != 0)
		{
			unordered_map<string, int> replacerCount;

			for (auto iter = clipAA.begin(); iter != clipAA.end(); ++iter)
			{
				bool isChange = false;
				vecstr msglines;

				for (auto it = iter->second.begin(); it != iter->second.end(); ++it)
				{
					for (auto& animName : it->second)
					{
						if (animName != "x")
						{
							isChange = true;
							break;
						}
					}

					if (!isChange)
					{
						break;
					}

					vecstr children;
					string groupName = AAGroup[it->first];
					string importline = to_string(iter->first);

					while (importline.length() < 4)
					{
						importline = "0" + importline;
					}

					msglines.push_back("		<hkobject name=\"#" + importline + "\" class=\"hkbManualSelectorGenerator\" signature=\"0xd932fab8\">");
					importline = "variableID[Nemesis_AA_" + groupName + "]";
					variableIDReplacer(importline, "AA", behaviorFile, variableid, 0);

					if (error)
					{
						emit done();
						return;
					}

					if (exportID["variable_binding"]["selectedGeneratorIndex!~^!" + importline].length() == 0)
					{
						string tempID = to_string(lastID);

						while (tempID.length() < 4)
						{
							tempID = "0" + tempID;
						}

						exportID["variable_binding"]["selectedGeneratorIndex!~^!" + importline] = tempID;
						importline = "			<hkparam name=\"variableBindingSet\">#" + tempID + "</hkparam>";
						++lastID;

						if (lastID == 9216)
						{
							++lastID;
						}
					}
					else
					{
						importline = "			<hkparam name=\"variableBindingSet\">#" + exportID["variable_binding"]["selectedGeneratorIndex!~^!" + importline] + "</hkparam>";
					}

					msglines.push_back(importline);
					msglines.push_back("			<hkparam name=\"userData\">0</hkparam>");
					replacerCount[it->first]++;
					msglines.push_back("			<hkparam name=\"name\">Nemesis_" + to_string(replacerCount[it->first]) + "_" + groupName + "_MSG</hkparam>");
					msglines.push_back("			<hkparam name=\"generators\" numelements=\"" + to_string(it->second.size() + 1) + "\">");

					string baseID = to_string(lastID);

					while (baseID.length() < 4)
					{
						baseID = "0" + baseID;
					}

					msglines.push_back("				#" + baseID);
					++lastID;

					if (lastID == 9216)
					{
						++lastID;
					}

					for (unsigned int i = 0; i < it->second.size(); ++i)
					{
						if (it->second[i] != "x")
						{
							string tempID = to_string(lastID);

							while (tempID.length() < 4)
							{
								tempID = "0" + tempID;
							}

							msglines.push_back("				#" + tempID);
							children.push_back(tempID);
							++lastID;

							if (lastID == 9216)
							{
								++lastID;
							}
						}
						else
						{
							msglines.push_back("				#" + baseID);
						}
					}

					msglines.push_back("			</hkparam>");
					msglines.push_back("			<hkparam name=\"selectedGeneratorIndex\">0</hkparam>");
					msglines.push_back("			<hkparam name=\"currentGeneratorIndex\">0</hkparam>");
					msglines.push_back("		</hkobject>");
					msglines.push_back("");

					AAlines.push_back("		<hkobject name=\"#" + baseID + "\" class=\"hkbClipGenerator\" signature=\"0x333b85b9\">");

					unordered_map<string, vecstr> triggerID;

					for (unsigned int i = 1; i < catalystMap[iter->first].size(); ++i)
					{
						AAlines.push_back(catalystMap[iter->first][i]);
					}

					if (AAlines.back().length() != 0)
					{
						AAlines.push_back("");
					}

					int num = 0;

					for (unsigned int i = 0; i < children.size(); ++i)
					{
						AAlines.push_back("		<hkobject name=\"#" + children[i] + "\" class=\"hkbClipGenerator\" signature=\"0x333b85b9\">");
						AAlines.push_back(catalystMap[iter->first][1]);
						AAlines.push_back(catalystMap[iter->first][2]);
						AAlines.push_back(catalystMap[iter->first][3]);

						while (it->second[num] == "x")
						{
							++num;
						}

						AAlines.push_back("			<hkparam name=\"animationName\">Animations\\" + it->second[num] + "</hkparam>");
						string animFile = it->second[num].substr(it->second[num].find_last_of("\\") + 1);

						if (AAEvent.find(animFile) != AAEvent.end())
						{
							if (AAEvent[animFile].size() > 0)
							{
								string tempID = to_string(lastID);

								while (tempID.length() < 4)
								{
									tempID = "0" + tempID;
								}

								AAlines.push_back("			<hkparam name=\"triggers\">#" + tempID + "</hkparam>");
								triggerID[tempID] = AAEvent[animFile];
								++lastID;

								if (lastID == 9216)
								{
									++lastID;
								}
							}
							else
							{
								AAlines.push_back("			<hkparam name=\"triggers\">null</hkparam>");
							}
						}
						else
						{
							AAlines.push_back(catalystMap[iter->first][5]);
						}

						for (unsigned int j = 6; j < catalystMap[iter->first].size(); ++j)
						{
							AAlines.push_back(catalystMap[iter->first][j]);
						}

						if (AAlines.back().length() != 0)
						{
							AAlines.push_back("");
						}

						++num;
					}

					for (auto itera = triggerID.begin(); itera != triggerID.end(); ++itera)
					{
						AAlines.push_back("		<hkobject name=\"#" + itera->first + "\" class=\"hkbClipTriggerArray\" signature=\"0x59c23a0f\">");
						AAlines.push_back("			<hkparam name=\"triggers\" numelements=\"" + to_string(itera->second.size() / 2) + "\">");

						for (unsigned int i = 0; i < itera->second.size(); ++i)
						{
							bool negative = false;
							string time = itera->second[i + 1];

							if (time[0] == '-')
							{
								negative = true;
								string signTime = time.substr(1);

								if (isOnlyNumber(signTime) && stod(signTime) == 0)
								{
									time = signTime;
								}
							}

							AAlines.push_back("				<hkobject>");
							AAlines.push_back("					<hkparam name=\"localTime\">" + time + "</hkparam>");
							AAlines.push_back("					<hkparam name=\"event\">");
							AAlines.push_back("						<hkobject>");

							string eventID = "eventID[" + itera->second[i] + "]";
							eventIDReplacer(eventID, "AA", behaviorFile, eventid, ZeroEvent, 0);

							if (error)
							{
								emit done();
								return;
							}

							AAlines.push_back("							<hkparam name=\"id\">" + eventID + "</hkparam>");
							AAlines.push_back("							<hkparam name=\"payload\">null</hkparam>");
							AAlines.push_back("						</hkobject>");
							AAlines.push_back("					</hkparam>");

							if (negative)
							{
								AAlines.push_back("					<hkparam name=\"relativeToEndOfClip\">true</hkparam>");
							}
							else
							{
								AAlines.push_back("					<hkparam name=\"relativeToEndOfClip\">false</hkparam>");
							}

							AAlines.push_back("					<hkparam name=\"acyclic\">false</hkparam>");
							AAlines.push_back("					<hkparam name=\"isAnnotation\">false</hkparam>");
							AAlines.push_back("				</hkobject>");

							++i;
						}

						AAlines.push_back("			</hkparam>");
						AAlines.push_back("		</hkobject>");
						AAlines.push_back("");
					}
				}

				if (isChange)
				{
					catalystMap[iter->first] = msglines;
				}
			}
		}

		emit progressUp();

		size_t reserveSize = 0;
		vecstr behaviorlines;

		// output import functions
		// Must be in vector
		vector<ImportContainer> groupExportID;
		groupExportID.push_back(exportID);
		vecstr additionallines = importOutput(groupExportID, int(groupExportID.size() - 1), lastID, filelist[curList]);

		emit progressUp();

		for (unsigned int j = 0; j < allEditLines.size(); ++j)
		{
			reserveSize += allEditLines[j]->size();
		}

		for (auto it = catalystMap.begin(); it != catalystMap.end(); ++it)
		{
			reserveSize += it->second.size();
		}

		reserveSize = reserveSize + additionallines.size() + AAlines.size();
		behaviorlines.reserve(behaviorlines.size() + reserveSize + 7);

		for (int i = firstID; i < firstID + 4; ++i)
		{
			behaviorlines.insert(behaviorlines.end(), catalystMap[i].begin(), catalystMap[i].end());
		}

		behaviorlines.push_back("<!-- ======================== NEMESIS import TEMPLATE START ======================== -->");
		behaviorlines.push_back("");
		behaviorlines.insert(behaviorlines.end(), additionallines.begin(), additionallines.end());
		behaviorlines.push_back("<!-- ======================== NEMESIS import TEMPLATE END ======================== -->");

		behaviorlines.push_back("<!-- ======================== NEMESIS alternate animation TEMPLATE START ======================== -->");
		behaviorlines.push_back("");
		behaviorlines.insert(behaviorlines.end(), AAlines.begin(), AAlines.end());
		behaviorlines.push_back("<!-- ======================== NEMESIS alternate animation TEMPLATE END ======================== -->");

		for (unsigned int j = 0; j < allEditLines.size(); ++j)
		{
			behaviorlines.insert(behaviorlines.end(), allEditLines[j]->begin(), allEditLines[j]->end());
		}

		firstID = firstID + 3;

		for (auto it = catalystMap.begin(); it != catalystMap.end(); ++it)
		{
			if (it->first > firstID)
			{
				behaviorlines.insert(behaviorlines.end(), it->second.begin(), it->second.end());
			}
		}

		if (behaviorlines.back().length() != 0)
		{
			behaviorlines.push_back("");
		}

		behaviorlines.push_back("	</hksection>");
		behaviorlines.push_back("");
		behaviorlines.push_back("</hkpackfile>");

		if (error)
		{
			emit done();
			return;
		}

		emit progressUp();

		if (behaviorPath[lowerBehaviorFile].size() == 0)
		{
			ErrorMessage(1068, behaviorFile);
			emit done();
			return;
		}

		string filename = "temp_behaviors\\xml\\" + modID + lowerBehaviorFile;
		FolderCreate(filename.substr(0, filename.find_last_of(filename)));
		FolderCreate(outputdir.substr(0, outputdir.find_last_of(outputdir)));
		ofstream output(filename + ".xml");

		if (output.is_open())
		{
			FunctionWriter fwriter(&output);

			for (unsigned int i = 0; i < behaviorlines.size(); ++i)
			{
				fwriter << behaviorlines[i] << "\n";
			}

			output.close();
		}
		else
		{
			ErrorMessage(1025, filename);
			emit done();
			return;
		}

		emit progressUp();
		hkxcmdOutput(filename, outputdir);
	}
	catch (const exception& ex)
	{
		ErrorMessage(6002, ex.what());
	}

	emit done();
}

void BehaviorSub::AnimDataCompilation()
{
	try
	{
		string filepath = directory + filelist[curList];
		string behaviorFile = filelist[curList].substr(0, filelist[curList].find_last_of("."));
		string lowerBehaviorFile = boost::algorithm::to_lower_copy(behaviorFile);

		vecstr projectList;		// list of projects
		unordered_map<string, int> projectNameCount;	// count the occurance of the project name
		vector<AnimDataProject> ADProject;
		unordered_map<string, int> nextProject;

		string project;
		string header;

		emit progressUp();

		{
			unordered_map<string, int> uCode;		// project, highest unique code
			unordered_map<string, unordered_map<string, vecstr>> catalystMap;	// project, header, list of lines
			unordered_map<string, vecstr> animDataHeader;						// project, list of headers
			unordered_map<string, vecstr> animDataInfo;							// project, list of info headers

			{
				// read behavior file
				vecstr catalyst;
				GetFunctionLines(filepath, catalyst);
				vecstr newline;
				unordered_map<string, string> chosenLines;

				// check for error
				if (error)
				{
					emit done();
					return;
				}

				emit progressUp();

				int projectcounter = 0;
				bool isOpen = true;
				bool special = false;
				bool isInfo = false;
				int num = 0;
				projectList.reserve(500);

				for (int i = 1; i < catalyst.size(); ++i)
				{
					if (catalyst[i].find(".txt") == NOT_FOUND)
					{
						num = i;
						break;
					}

					projectList.push_back(catalyst[i]);
				}

				projectList.shrink_to_fit();
				project = projectList[0] + " " + to_string(++projectNameCount[projectList[0]]);
				header = "$header$";
				animDataHeader[project].push_back(header);
				newline.reserve(20);
				newline.clear();

				emit progressUp();

				// add picked behavior and remove not picked behavior 
				// separation of all items for easier access and better compatibility
				for (unsigned int l = num; l < catalyst.size(); ++l)
				{
					string line = catalyst[l];
					bool skip = false;

					if (line.find("<!-- ", 0) != NOT_FOUND)
					{
						if (line.find("<!-- NEW *", 0) != NOT_FOUND)
						{
							size_t tempint = line.find("<!-- NEW *", 0) + 10;
							string modID = line.substr(tempint, line.find("* -->", tempint + 1) - tempint);

							if (!chosenBehavior[modID])
							{
								isOpen = false;
							}

							skip = true;
						}
						else if (line.find("<!-- NEW ^", 0) != NOT_FOUND)
						{
							special = true;
						}
						else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
						{
							isOpen = true;

							if (!special)
							{
								skip = true;
							}
							else
							{
								special = false;
							}
						}
					}

					if (isOpen && !skip)
					{
						while (true)
						{
							if (line.find("<!-- *", 0) != NOT_FOUND)
							{
								size_t tempint = line.find("<!-- *") + 6;
								string modID = line.substr(tempint, line.find("* -->", tempint + 1) - tempint);
								chosenLines[modID] = line;
								break;
							}
							else if (line.find("<!-- original -->", 0) != NOT_FOUND)
							{
								if (chosenLines.size() != 0)
								{
									line = behaviorLineChooser(line, chosenLines, behaviorPriority);
									chosenLines.clear();
								}
								else
								{
									ErrorMessage(1165);
									emit done();
									return;
								}
							}

							if (l + 3 < catalyst.size() && l > 2)
							{
								if (catalyst[l - 1] == "")
								{
									bool out = false;

									if (isOnlyNumber(line))
									{
										int next = 1;

										if (l + next < catalyst.size() && catalyst[l + next].find("<!--") != NOT_FOUND)
										{
											++next;
										}

										if (l + next < catalyst.size() && isOnlyNumber(catalyst[l + next]))
										{
											++next;

											if (l + next < catalyst.size() && catalyst[l + next].find("<!--") != NOT_FOUND)
											{
												++next;
											}

											if (l + next < catalyst.size() && isOnlyNumber(catalyst[l + next]))
											{
												int nextnext = next + 1;

												if (l + next < catalyst.size() && catalyst[l + next].find("<!--") != NOT_FOUND)
												{
													++nextnext;
												}

												if (catalyst[l + next] == "0" || (l + nextnext < catalyst.size() && catalyst[l + nextnext].find("\\") != NOT_FOUND))
												{
													newline.shrink_to_fit();
													catalystMap[project][header] = newline;
													string newproject = projectList[++projectcounter];
													header = "$header$";
													project = newproject + " " + to_string(++projectNameCount[newproject]);
													animDataHeader[project].push_back(header);
													newline.reserve(20);
													newline.clear();
													isInfo = false;
													out = true;
												}
											}
										}
									}

									if (!out)
									{
										if (!isInfo)
										{
											if (hasAlpha(line))
											{
												if (isOnlyNumber(catalyst[l + 1]))
												{
													int number = stoi(catalyst[l + 1]);
													newline.shrink_to_fit();
													catalystMap[project][header] = newline;
													newline.reserve(20);
													newline.clear();
													header = line + " " + catalyst[l + 1];
													animDataHeader[project].push_back(header);

													if (number >= uCode[project])
													{
														uCode[project] = number;
													}
												}
											}
											else if (isOnlyNumber(line))
											{
												isInfo = true;
												newline.shrink_to_fit();
												catalystMap[project][header] = newline;;
												newline.reserve(20);
												newline.clear();
												header = catalyst[++l];
												line = catalyst[l];
												animDataInfo[project].push_back(header);
											}
										}
										else if (isOnlyNumber(line))
										{
											newline.shrink_to_fit();
											catalystMap[project][header] = newline;
											newline.reserve(20);
											newline.clear();
											header = line;
											animDataInfo[project].push_back(header);
										}
									}
								}
								else if (header == "$header$")
								{
									if (hasAlpha(line) && line.find("\\") == NOT_FOUND && l + 1 < catalyst.size())
									{
										if (isOnlyNumber(catalyst[l + 1]))	// if it is unique code
										{
											int number = stoi(catalyst[l + 1]);
											newline.shrink_to_fit();
											catalystMap[project][header] = newline;
											newline.reserve(20);
											newline.clear();
											header = line + " " + catalyst[l + 1];
											animDataHeader[project].push_back(header);

											if (number >= uCode[project])
											{
												uCode[project] = number;
											}
										}
									}
									else if (isOnlyNumber(catalyst[l - 1]) && catalyst[l - 1] == "0" && isOnlyNumber(line))
									{
										int next = 1;

										if (l + next < catalyst.size() && isOnlyNumber(catalyst[l + next]))
										{
											++next;

											if (l + next < catalyst.size() && catalyst[l + next].find("<!--") != NOT_FOUND)
											{
												++next;
											}

											if (l + next < catalyst.size() && isOnlyNumber(catalyst[l + next]))
											{
												int nextnext = next + 1;

												if (l + next < catalyst.size() && catalyst[l + next].find("<!--") != NOT_FOUND)
												{
													++nextnext;
												}

												if (catalyst[l + next] == "0" || (l + nextnext < catalyst.size() && catalyst[l + nextnext].find("\\") != NOT_FOUND))
												{
													newline.shrink_to_fit();
													catalystMap[project][header] = newline;
													string newproject = projectList[++projectcounter];
													project = newproject + " " + to_string(++projectNameCount[newproject]);
													animDataHeader[project].push_back(header);
													newline.reserve(20);
													newline.clear();
													isInfo = false;
												}
											}
										}
									}
								}
							}

							newline.push_back(line);
							break;
						}
					}

					if (error)
					{
						emit done();
						return;
					}
				}

				if (newline.size() != 0)
				{
					if (newline.back().length() == 0)
					{
						newline.pop_back();
					}

					newline.shrink_to_fit();
					catalystMap[project][header] = newline;
					newline.clear();
				}
			}

			emit progressUp();

			// check for having newAnimation for the file
			if (BehaviorTemplate.grouplist.find(lowerBehaviorFile) != BehaviorTemplate.grouplist.end() && BehaviorTemplate.grouplist[lowerBehaviorFile].size() > 0)
			{
				vecstr templateGroup = BehaviorTemplate.grouplist[lowerBehaviorFile];
				unordered_map<string, unordered_map<string, vector<map<int, vecstr>>>> editExtract;
				unordered_map<string, unordered_map<string, int>> ASDCount;

				for (unsigned int j = 0; j < templateGroup.size(); ++j)
				{
					string templateCode = templateGroup[j];
					bool hasGroup = false;
					bool hasMaster = false;
					bool ignoreGroup = false;

					if (newAnimation.find(templateCode) != newAnimation.end() && newAnimation[templateCode].size() != 0)
					{
						for (unsigned int k = 0; k < newAnimation[templateCode].size(); ++k)
						{
							unordered_map<string, unordered_map<string, vecstr>> generatedAnimData;
							newAnimation[templateCode][k]->GetAnimData(generatedAnimData);

							if (error)
							{
								emit done();
								return;
							}

							for (auto it = generatedAnimData.begin(); it != generatedAnimData.end(); ++it)
							{
								project = it->first;
								string projectplus;
								unordered_map<string, string> isExist;

								if (projectNameCount[project] == 0)
								{
									projectplus = project + to_string(++projectNameCount[project]);
									projectList.push_back(project);
								}

								for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
								{
									if (iter->first.find_last_of("$UC") != iter->first.length() - 1)
									{
										if (iter->first[0] == '$' && iter->first.back() == '$')
										{
											header = iter->first.substr(1, iter->first.length() - 2);
											header.append("~" + to_string(++ASDCount[projectplus][header]));
											string uniquecode;

											if (isExist[header].length() == 0)
											{
												uniquecode = to_string(++uCode[projectplus]);
												isExist[header] = uniquecode;
											}
											else
											{
												uniquecode = isExist[header];
											}

											iter->second[1] = uniquecode;
											animDataHeader[projectplus].push_back(header);
										}
										else
										{
											ErrorMessage(3003, iter->first + ".txt");
											emit done();
											return;
										}
									}
									else
									{
										if (iter->first[0] != '$')
										{
											ErrorMessage(3006, project, iter->first);
											emit done();
											return;
										}

										header = iter->first.substr(1, iter->first.length() - 4);

										if (isExist[header].length() == 0)
										{
											header = to_string(++uCode[projectplus]);
											isExist[header] = header;
											iter->second[0] = header;
											animDataInfo[projectplus].push_back(header);
										}
									}

									if (catalystMap[projectplus][header].size() > 0)
									{
										ErrorMessage(5012, templateCode, project, header);
										emit done();
										return;
									}

									if (projectNameCount[project] != 1)
									{
										WarningMessage(1020, project, header);

										for (int i = 1; i <= projectNameCount[project]; ++i)
										{
											catalystMap[project + to_string(i)][header] = iter->second;
										}
									}
									else
									{
										catalystMap[projectplus][header] = iter->second;
									}
								}
							}

							for (auto it = BehaviorTemplate.existingAnimDataHeader[templateCode].begin(); it != BehaviorTemplate.existingAnimDataHeader[templateCode].end(); ++it)
							{
								project = it->first;

								for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
								{
									header = *iter;

									if (catalystMap[project][header].size() == 0)
									{
										ErrorMessage(5011, templateCode, project, header);
										emit done();
										return;
									}

									map<int, vecstr> extract;
									newAnimation[templateCode][k]->existingASDProcess(catalystMap[project][header], extract);

									if (error)
									{
										emit done();
										return;
									}

									editExtract[project][header].push_back(extract);
								}
							}

							if (error)
							{
								emit done();
								return;
							}
						}
					}
				}

				emit progressUp();

				for (auto it = editExtract.begin(); it != editExtract.end(); ++it)
				{
					string project = it->first;

					for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
					{
						string header = iter->first;
						map<int, vecstr> combined;
						int totalline = 0;

						for (unsigned int i = 0; iter->second.size(); ++i)
						{
							vecstr newline;
							string line;

							for (auto itera = iter->second[i].begin(); itera != iter->second[i].end(); ++itera)
							{
								combined[itera->first].insert(combined[itera->first].end(), itera->second.begin(), itera->second.end());
								totalline += int(itera->second.size());
							}
						}

						if (totalline == 0)
						{
							ErrorMessage(5014, project, header);
							emit done();
							return;
						}

						for (int i = 1; i <= projectNameCount[project]; ++i)
						{
							combineExtraction(catalystMap[project + " " + to_string(i)][header], combined, project, header);
						}

						if (error)
						{
							emit done();
							return;
						}
					}
				}
			}
			else
			{
				emit progressUp();
			}

			emit progressUp();

			{
				// unsure of the function but is present in FNIS
				unordered_map<string, bool> isExist;

				for (auto& it : alternateAnim)
				{
					for (auto& iter : it.second)
					{
						header = GetFileName(iter);

						if (!isExist[header] && header != "x")
						{
							vecstr newlines;
							isExist[header] = true;
							project = "DefaultFemale.txt 1";

							newlines.push_back(header);
							newlines.push_back(to_string(++uCode[project]));
							newlines.push_back("1");
							newlines.push_back("0");
							newlines.push_back("0");
							newlines.push_back("0");
							newlines.push_back("");

							animDataHeader[project].push_back(header);
							catalystMap[project][header] = newlines;

							project = "DefaultMale.txt 1";
							newlines[1] = to_string(++uCode[project]);
							animDataHeader[project].push_back(header);
							catalystMap[project][header] = newlines;
						}
					}
				}
			}

			emit progressUp();

			try
			{
				for (unsigned int i = 0; i < projectList.size(); ++i)
				{
					if (projectNameCount[project] > 1)
					{
						project = projectList[i] + to_string(++nextProject[project]);
					}
					else
					{
						project = projectList[i] + " 1";
					}

					vecstr combined;

					for (unsigned int j = 0; j < animDataHeader[project].size(); ++j)
					{
						header = animDataHeader[project][j];
						combined.insert(combined.end(), catalystMap[project][header].begin(), catalystMap[project][header].end());
					}

					for (unsigned int j = 0; j < animDataInfo[project].size(); ++j)
					{
						header = animDataInfo[project][j];
						combined.insert(combined.end(), catalystMap[project][header].begin(), catalystMap[project][header].end());
					}

					if (combined.size() == 0)
					{
						ErrorMessage(5017, projectList[i]);
						emit done();
						return;
					}

					AnimDataProject newProject(combined, projectList[i], filepath);
					ADProject.push_back(newProject);
				}
			}
			catch (int)
			{
				emit done();
				return;
			}
		}

		emit progressUp();

		// final output	
#ifdef DEBUG
		string filename = "new_behaviors\\" + behaviorPath[lowerBehaviorFile].substr(behaviorPath[lowerBehaviorFile].find("\\") + 1);
#else
		string filename = behaviorPath[lowerBehaviorFile];
#endif

		FolderCreate(filename.substr(0, filename.find_last_of(filename)));
		ofstream output(filename + ".txt");

		if (output.is_open())
		{
			FunctionWriter fwriter(&output);
			fwriter << to_string(projectList.size()) << "\n";

			for (unsigned int i = 0; i < projectList.size(); ++i)
			{
				fwriter << projectList[i] << "\n";
			}

			for (unsigned int i = 0; i < projectList.size(); ++i)
			{
				project = projectList[i];
				fwriter << to_string(ADProject[i].GetAnimTotalLine()) << "\n";
				fwriter << ADProject[i].unknown1 << "\n";
				fwriter << to_string(ADProject[i].behaviorlist.size()) << "\n";

				for (unsigned int j = 0; j < ADProject[i].behaviorlist.size(); ++j)
				{
					fwriter << ADProject[i].behaviorlist[j] << "\n";
				}

				fwriter << ADProject[i].unknown2 << "\n";

				if (ADProject[i].unknown2 != "0")
				{
					for (unsigned int j = 0; j < ADProject[i].animdatalist.size(); ++j)
					{
						fwriter << ADProject[i].animdatalist[j].name << "\n";
						fwriter << ADProject[i].animdatalist[j].uniquecode << "\n";
						fwriter << ADProject[i].animdatalist[j].unknown1 << "\n";
						fwriter << ADProject[i].animdatalist[j].unknown2 << "\n";
						fwriter << ADProject[i].animdatalist[j].unknown3 << "\n";
						fwriter << to_string(ADProject[i].animdatalist[j].eventname.size()) << "\n";

						for (unsigned int k = 0; k < ADProject[i].animdatalist[j].eventname.size(); ++k)
						{
							fwriter << ADProject[i].animdatalist[j].eventname[k] << "\n";
						}

						fwriter << "\n";
					}

					fwriter << to_string(ADProject[i].GetInfoTotalLine()) << "\n";

					for (unsigned int j = 0; j < ADProject[i].infodatalist.size(); ++j)
					{
						fwriter << ADProject[i].infodatalist[j].uniquecode << "\n";
						fwriter << ADProject[i].infodatalist[j].duration << "\n";
						fwriter << to_string(ADProject[i].infodatalist[j].motiondata.size()) << "\n";

						for (unsigned int k = 0; k < ADProject[i].infodatalist[j].motiondata.size(); ++k)
						{
							fwriter << ADProject[i].infodatalist[j].motiondata[k] << "\n";
						}

						fwriter << to_string(ADProject[i].infodatalist[j].rotationdata.size()) << "\n";

						for (unsigned int k = 0; k < ADProject[i].infodatalist[j].rotationdata.size(); ++k)
						{
							fwriter << ADProject[i].infodatalist[j].rotationdata[k] << "\n";
						}

						fwriter << "\n";
					}
				}
			}

			output.close();
		}
		else
		{
			ErrorMessage(1025, filename);
			emit done();
			return;
		}

		emit progressUp();
	}
	catch (const exception& ex)
	{
		ErrorMessage(6002, ex.what());
	}

	emit done();
}

void BehaviorSub::ASDCompilation()
{
	try
	{
		string filepath = directory + filelist[curList];
		string behaviorFile = filelist[curList].substr(0, filelist[curList].find_last_of("."));
		string lowerBehaviorFile = boost::algorithm::to_lower_copy(behaviorFile);

		vecstr projectList;
		int projectcounter = 0;
		int headercounter = 0;
		bool isOpen = true;
		bool special = false;
		string newMod;
		string project = "	";
		string header = project;
		string line;
		unordered_map<string, string> chosenLines;
		unordered_map<string, map<string, vecstr, alphanum_less>> ASDPack;
		unordered_map<string, AnimationDataProject> ASDData;

		emit progressUp();

		{
			// read behavior file
			vecstr catalyst;
			GetFunctionLines(filepath, catalyst, false);
			vecstr newline;
			vecstr storeline;

			// check for error
			if (error)
			{
				emit done();
				return;
			}

			while (catalyst.back().length() == 0)
			{
				catalyst.pop_back();
			}

			emit progressUp();

			// add picked behavior and remove not picked behavior 
			// separation of all items for easier access and better compatibility
			for (unsigned int l = 0; l < catalyst.size(); ++l)
			{
				string line = catalyst[l];
				bool skip = false;

				if (line.find("<!-- ", 0) != NOT_FOUND)
				{
					if (line.find("<!-- NEW *", 0) != NOT_FOUND)
					{
						size_t tempint = line.find("<!-- NEW *", 0) + 10;
						string modID = line.substr(tempint, line.find("* -->", tempint + 1) - tempint);

						if (!chosenBehavior[modID])
						{
							isOpen = false;
						}
						else
						{
							newMod = modID;
						}

						skip = true;
					}
					else if (line.find("<!-- NEW ^", 0) != NOT_FOUND)
					{
						special = true;
					}
					else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
					{
						isOpen = true;
						newMod.clear();

						if (!special)
						{
							skip = true;
						}
						else
						{
							special = false;
						}
					}
				}

				if (isOpen && !skip)
				{
					while (true)
					{
						if (line.find("<!-- *", 0) != NOT_FOUND)
						{
							size_t tempint = line.find("<!-- *") + 6;
							string modID = line.substr(tempint, line.find("* -->", tempint + 1) - tempint);
							chosenLines[modID] = line;
							break;
						}
						else if (line.find("<!-- original -->", 0) != NOT_FOUND)
						{
							if (chosenLines.size() != 0)
							{
								line = behaviorLineChooser(line, chosenLines, behaviorPriority);
								chosenLines.clear();
							}
							else
							{
								ErrorMessage(1165);
								emit done();
								return;
							}
						}

						storeline.push_back(line);
						break;
					}
				}

				if (error)
				{
					emit done();
					return;
				}
			}

			for (int i = 1; i < catalyst.size(); ++i)
			{
				if (isOnlyNumber(catalyst[i]))
				{
					break;
				}

				projectList.push_back(catalyst[i]);
				newline.push_back(catalyst[i]);
			}

			unordered_map<string, vecstr> animDataSetHeader;
			animDataSetHeader[project].push_back(header);

			emit progressUp();

			int num;

			for (unsigned int i = projectList.size() + 1; i < storeline.size(); ++i)
			{
				line = storeline[i];

				if (i != storeline.size() - 1 && wordFind(storeline[i + 1], ".txt") != NOT_FOUND)
				{
					if (i != projectList.size() + 1)
					{
						num = i;
						break;
					}
					else
					{
						newline.reserve(100);
						newline.clear();
						project = projectList[projectcounter];
						++projectcounter;
						headercounter = 0;
						animDataSetHeader[project].push_back("	");
						newline.push_back(storeline[i]);
						++i;
					}

					if (animDataSetHeader[project].size() != 1)
					{
						ErrorMessage(5005, filepath, i + 1);
						emit done();
						return;
					}

					while (i < storeline.size())
					{
						if (wordFind(storeline[i], ".txt") != NOT_FOUND)
						{
							string curHeader = storeline[i];
							animDataSetHeader[project].push_back(curHeader);
						}
						else if (wordFind(storeline[i], "V3") != NOT_FOUND)
						{
							if (headercounter >= animDataSetHeader[project].size())
							{
								ErrorMessage(5015, "animationsetdatasinglefile.txt", i + 1);
								emit done();
								return;
							}

							header = animDataSetHeader[project][headercounter];
							++headercounter;

							if (header != "	" || project == "	")
							{
								newline.shrink_to_fit();
								ASDPack[project][header] = newline;
							}

							newline.reserve(100);
							newline.clear();
							break;
						}
						else
						{
							ErrorMessage(5001, filepath, i + 1);
							emit done();
							return;
						}

						newline.push_back(storeline[i]);
						++i;
					}
				}
				else if (wordFind(storeline[i], "V3") != NOT_FOUND)
				{
					if (headercounter >= animDataSetHeader[project].size())
					{
						ErrorMessage(5015, "animationsetdatasinglefile.txt", i + 1);
						emit done();
						return;
					}

					header = animDataSetHeader[project][headercounter];
					++headercounter;

					if (header != "	" || project == "	")
					{
						newline.shrink_to_fit();
						ASDPack[project][header] = newline;
					}

					newline.reserve(100);
					newline.clear();
				}

				newline.push_back(storeline[i]);
			}

			emit progressUp();

			for (int i = num; i < storeline.size(); ++i)
			{
				line = storeline[i];

				if (i != storeline.size() - 1 && wordFind(storeline[i + 1], ".txt") != NOT_FOUND)
				{
					header = animDataSetHeader[project][headercounter];

					if (header != "	" || project == "	")
					{
						newline.shrink_to_fit();
						ASDPack[project][header] = newline;
					}

					newline.reserve(100);
					newline.clear();
					project = projectList[projectcounter];
					++projectcounter;
					headercounter = 0;
					animDataSetHeader[project].push_back("	");
					newline.push_back(storeline[i]);
					++i;

					if (animDataSetHeader[project].size() != 1)
					{
						ErrorMessage(5005, filepath, i + 1);
						emit done();
						return;
					}

					while (i < storeline.size())
					{
						if (wordFind(storeline[i], ".txt") != NOT_FOUND)
						{
							string curHeader = storeline[i];
							animDataSetHeader[project].push_back(curHeader);
						}
						else if (wordFind(storeline[i], "V3") != NOT_FOUND)
						{
							if (headercounter >= animDataSetHeader[project].size())
							{
								ErrorMessage(5015, "animationsetdatasinglefile.txt", i + 1);
								emit done();
								return;
							}

							header = animDataSetHeader[project][headercounter];
							++headercounter;

							if (header != "	" || project == "	")
							{
								newline.shrink_to_fit();
								ASDPack[project][header] = newline;
							}

							newline.reserve(100);
							newline.clear();
							break;
						}
						else
						{
							ErrorMessage(5001, filepath, i + 1);
							emit done();
							return;
						}

						newline.push_back(storeline[i]);
						++i;
					}
				}
				else if (wordFind(storeline[i], "V3") != NOT_FOUND)
				{
					if (headercounter >= animDataSetHeader[project].size() - 1)
					{
						ErrorMessage(5015, "animationsetdatasinglefile.txt", i + 1);
						emit done();
						return;
					}

					header = animDataSetHeader[project][headercounter];
					++headercounter;

					if (header != "	" || project == "	")
					{
						newline.shrink_to_fit();
						ASDPack[project][header] = newline;
					}

					newline.reserve(100);
					newline.clear();
				}

				newline.push_back(storeline[i]);
			}

			if (error)
			{
				emit done();
				return;
			}

			if (newline.size() != 0)
			{
				header = animDataSetHeader[project][headercounter];

				if (header != "	" || project == "	")
				{
					newline.shrink_to_fit();
					ASDPack[project][header] = newline;
				}
			}

			emit progressUp();
		}

		// check for having newAnimation for the file
		if (BehaviorTemplate.grouplist.find(lowerBehaviorFile) != BehaviorTemplate.grouplist.end() && BehaviorTemplate.grouplist[lowerBehaviorFile].size() > 0)
		{
			vecstr templateGroup = BehaviorTemplate.grouplist[lowerBehaviorFile];
			unordered_map<string, unordered_map<string, vector<map<int, vecstr>>>> editExtract;	// project, header, list of extracts, startline, extractions; to get all edits done to master branch

			for (unsigned int j = 0; j < templateGroup.size(); ++j)
			{
				string templateCode = templateGroup[j];
				unordered_map<string, int> ASDCount;
				bool hasGroup = false;
				bool hasMaster = false;
				bool ignoreGroup = false;

				if (newAnimation.find(templateCode) != newAnimation.end() && newAnimation[templateCode].size() != 0)
				{
					for (unsigned int k = 0; k < newAnimation[templateCode].size(); ++k)
					{
						unordered_map<string, map<string, vecstr, alphanum_less>> generatedASD;
						newAnimation[templateCode][k]->GetAnimSetData(generatedASD);

						if (error)
						{
							emit done();
							return;
						}

						for (auto it = generatedASD.begin(); it != generatedASD.end(); ++it)
						{
							string project = it->first;

							if (ASDPack.find(project) == ASDPack.end())
							{
								projectList.push_back(project);
							}

							for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
							{
								string header = iter->first.substr(1, iter->first.length() - 2);
								header.append("_" + to_string(++ASDCount[header]) + ".txt");

								if (ASDPack[project][header].size() > 0)
								{
									ErrorMessage(5012, templateCode, project, header);
									emit done();
									return;
								}

								ASDPack[project][header] = iter->second;
							}
						}

						for (auto it = BehaviorTemplate.existingASDHeader[templateCode].begin(); it != BehaviorTemplate.existingASDHeader[templateCode].end(); ++it)
						{
							string project = it->first;

							for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
							{
								string header = *iter;

								if (ASDPack[project][header].size() == 0)
								{
									ErrorMessage(5011, templateCode, project, header);
									emit done();
									return;
								}

								map<int, vecstr> extract;
								newAnimation[templateCode][k]->existingASDProcess(ASDPack[project][header], extract, 1);

								if (error)
								{
									emit done();
									return;
								}

								editExtract[project][header].push_back(extract);
							}
						}

						if (error)
						{
							emit done();
							return;
						}
					}
				}
			}

			emit progressUp();

			for (auto it = editExtract.begin(); it != editExtract.end(); ++it)
			{
				string project = it->first;

				for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
				{
					string header = iter->first;
					map<int, vecstr> combined;
					int totalline = 0;

					for (unsigned int i = 0; iter->second.size(); ++i)
					{
						vecstr newline;
						string line;

						for (auto itera = iter->second[i].begin(); itera != iter->second[i].end(); ++itera)
						{
							combined[itera->first].insert(combined[itera->first].end(), itera->second.begin(), itera->second.end());
							totalline += int(itera->second.size());
						}
					}

					if (totalline == 0)
					{
						ErrorMessage(5014, project, header);
						emit done();
						return;
					}

					combineExtraction(ASDPack[project][header], combined, project, header);

					if (error)
					{
						emit done();
						return;
					}
				}
			}
		}
		else
		{
			emit progressUp();
		}

		emit progressUp();

		for (unsigned int i = 0; i < projectList.size(); ++i)
		{
			int startline = 0;
			vecstr projectline;
			projectline.push_back(to_string(ASDPack[projectList[i]].size()));

			for (auto it = ASDPack[projectList[i]].begin(); it != ASDPack[projectList[i]].end(); ++it)
			{
				projectline.push_back(it->first);
			}

			for (auto it = ASDPack[projectList[i]].begin(); it != ASDPack[projectList[i]].end(); ++it)
			{
				projectline.insert(projectline.end(), it->second.begin(), it->second.end());
			}

			AnimationDataProject newProject(startline, projectline, filepath);
			ASDData[projectList[i]] = newProject;

			if (error)
			{
				emit done();
				return;
			}
		}

		emit progressUp();

		// final output	
#ifdef DEBUG
		string filename = "new_behaviors\\" + behaviorPath[lowerBehaviorFile].substr(behaviorPath[lowerBehaviorFile].find("\\") + 1);
#else
		string filename = behaviorPath[lowerBehaviorFile];
#endif

		FolderCreate(filename.substr(0, filename.find_last_of(filename)));
		ofstream output(filename + ".txt");

		if (output.is_open())
		{
			FunctionWriter fwriter(&output);
			fwriter << to_string(projectList.size()) << "\n";

			for (unsigned int i = 0; i < projectList.size(); ++i)
			{
				fwriter << projectList[i] << "\n";
			}

			for (unsigned int i = 0; i < projectList.size(); ++i)
			{
				string curProject = projectList[i];
				fwriter << to_string(ASDData[curProject].datalist.size()) << "\n";

				for (auto it = ASDData[curProject].datalist.begin(); it != ASDData[curProject].datalist.end(); ++it)
				{
					fwriter << it->first << "\n";
				}

				for (auto it = ASDData[curProject].datalist.begin(); it != ASDData[curProject].datalist.end(); ++it)
				{
					fwriter << "V3" << "\n";
					fwriter << to_string(it->second.equiplist.size()) << "\n";

					for (unsigned int k = 0; k < it->second.equiplist.size(); ++k)
					{
						fwriter << it->second.equiplist[k].name << "\n";
					}

					fwriter << to_string(it->second.typelist.size()) << "\n";

					for (unsigned int k = 0; k < it->second.typelist.size(); ++k)
					{
						fwriter << it->second.typelist[k].name << "\n";
						fwriter << it->second.typelist[k].equiptype1 << "\n";
						fwriter << it->second.typelist[k].equiptype2 << "\n";
					}

					fwriter << to_string(it->second.animlist.size()) << "\n";

					for (unsigned int k = 0; k < it->second.animlist.size(); ++k)
					{
						fwriter << it->second.animlist[k].name << "\n";
						fwriter << it->second.animlist[k].unknown << "\n";
						fwriter << to_string(it->second.animlist[k].attack.size()) << "\n";

						for (unsigned int j = 0; j < it->second.animlist[k].attack.size(); ++j)
						{
							fwriter << it->second.animlist[k].attack[j].data << "\n";
						}
					}

					fwriter << to_string(it->second.crc32list.size()) << "\n";

					for (unsigned int k = 0; k < it->second.crc32list.size(); ++k)
					{
						fwriter << it->second.crc32list[k].filepath << "\n";
						fwriter << it->second.crc32list[k].filename << "\n";
						fwriter << it->second.crc32list[k].fileformat << "\n";
					}
				}
			}

			output.close();
		}
		else
		{
			ErrorMessage(1025, filename);
			emit done();
			return;
		}

		emit progressUp();
	}
	catch (const exception& ex)
	{
		ErrorMessage(6002, ex.what());
	}

	emit done();
}

void BehaviorSub::addInfo(string& newDirectory, vecstr& newfilelist, int newCurList, vecstr& newBehaviorPriority, unordered_map<string, bool>& newChosenBehavior, getTemplate& newBehaviorTemplate, unordered_map<string, vector<shared_ptr<Furniture>>>& addAnimation, unordered_map<string, var>& newAnimVar, mapSetString& addAnimEvent, mapSetString& addAnimVariable, unordered_map<string, unordered_map<int, bool>>& newIgnoreFunction, bool newIsCharacter, string newModID)
{
	directory = newDirectory;
	filelist = newfilelist;
	curList = newCurList;
	behaviorPriority = newBehaviorPriority;
	chosenBehavior = newChosenBehavior;
	BehaviorTemplate = newBehaviorTemplate;
	newAnimation = addAnimation;
	AnimVar = newAnimVar;
	newAnimEvent = addAnimEvent;
	newAnimVariable = addAnimVariable;
	ignoreFunction = newIgnoreFunction;
	isCharacter = newIsCharacter;
	modID = newModID;
}

void BehaviorSub::addAnimation()
{
	++animCounter;

	if (animCounter > base)
	{
		emit newAnim();
	}
}

void BehaviorSub::hkxcmdOutput(string xmlfile, string hkxfile)
{
	if (hkxcmdProcess(xmlfile, hkxfile))
	{
		emit progressUp();
	}
}

bool readMod(string& errormod)
{
	string folder = "mod\\";
	vecstr modlist;
	read_directory(folder, modlist);

	for (auto& modcode : modlist)
	{
		if (boost::filesystem::is_directory(folder + modcode))
		{
			if (!isFileExist(folder + modcode + "\\info.ini"))
			{
				return false;
			}

			vecstr filelist;
			read_directory(folder + modcode, filelist);

			for (auto& file : filelist)
			{
				string filename = folder + modcode + "\\" + file;

				if (!boost::filesystem::is_directory(filename))
				{
					if (boost::iequals(file, "info.ini"))
					{
						vecstr storeline;
						GetFunctionLines(filename, storeline, false);

						string name, author, site, automatic;

						for (auto& line : storeline)
						{
							while (true)
							{
								size_t pos = wordFind(line, "name=");

								if (pos == 0)
								{
									name = line.substr(line.find("=") + 1);
									break;
								}

								pos = wordFind(line, "author=");

								if (pos == 0)
								{
									author = line.substr(line.find("=") + 1);
									break;
								}

								pos = wordFind(line, "site=");

								if (pos == 0)
								{
									site = line.substr(line.find("=") + 1);
									break;
								}

								pos = wordFind(line, "auto=");

								if (pos == 0)
								{
									automatic = line.substr(line.find("=") + 1);
								}
								
								break;
							}
						}

						if (name.length() == 0 || author.length() == 0 || site.length() == 0 || automatic.length() == 0)
						{
							errormod = modcode;
							return false;
						}

						modinfo[modcode].push_back(name + " (" + site + ")");
						modinfo[modcode].push_back(author);
						modinfo[modcode].push_back(automatic);
					}
				}
			}
		}
	}

	return true;
}

void DummyLog::message(std::string input)
{
	emit incomingMessage(QString::fromStdString(input));
}
