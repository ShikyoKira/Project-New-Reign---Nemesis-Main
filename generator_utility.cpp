#include "generator_utility.h"

#pragma warning(disable:4503)

using namespace std;

vector<int> GetStateID(map<int, int> mainJoint, map<int, vecstr> functionlist)
{
	vector<int> stateID;
	vecstr storeID;
	bool open = false;
	bool rightFunction = false;
	size_t jointsize = mainJoint.size();

	if (jointsize > 0)
	{
		for (auto it = mainJoint.begin(); it != mainJoint.end(); ++it)
		{
			stateID.push_back(0);

			for (unsigned int j = 0; j < functionlist[it->second].size(); ++j)
			{
				string curline = functionlist[it->second][j];

				if (curline.find("class=\"hkbStateMachine\" signature=\"") != string::npos)
				{
					rightFunction = true;
				}
				else if (curline.find("<hkparam name=\"states\" numelements=\"") != string::npos)
				{
					open = true;
				}

				if (!rightFunction)
				{
					cout << ">> ERROR(1077): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << "Function ID: " << it->second << endl << endl;
					error = true;
					return stateID;
				}
				else if (open)
				{
					if (curline.find("#") != string::npos)
					{
						size_t counter = count(curline.begin(), curline.end(), '#');
						size_t nextpos = 0;

						for (size_t k = 0; k < counter; ++k) // multiple IDs in 1 line
						{
							nextpos = curline.find("#", nextpos) + 1;

							int ID = stoi(boost::regex_replace(string(curline.substr(nextpos)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

							for (unsigned int l = 0; l < functionlist[ID].size(); ++l)
							{
								string line = functionlist[ID][l];

								if (line.find("<hkparam name=\"stateId\">", 0) != string::npos)
								{
									int tempStateID = stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

									if (tempStateID >= stateID.back())
									{
										stateID.back() = tempStateID + 1;
									}

									break;
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		cout << ">> ERROR(1078): BUG FOUND!! Report to Nemesis' author immediately <<" << endl << endl;
		error = true;
		return stateID;
	}
	
	return stateID;
}

int GetStateCount(vecstr templatelines)
{
	int count = 0;

	for (unsigned int i = 0; i < templatelines.size(); ++i)
	{
		if (templatelines[i].find("\t\t\t<hkparam name=\"stateId\">$(S+") != string::npos && templatelines[i].find(")$</hkparam>", templatelines[i].find("\t\t\t<hkparam name=\"stateId\">$(S+")) != string::npos)
		{
			string number = boost::regex_replace(string(templatelines[i]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

			if (templatelines[i].find("\t\t\t<hkparam name=\"stateId\">$(S+" + number + ")$</hkparam>") != string::npos)
			{
				if (count <= stoi(number))
				{
					count = stoi(number) + 1;
				}
			}
		}
	}

	return count;
}

vecstr newAnimationElement(string line, vector<vecstr> element, int curNumber)
{
	vecstr animElement;

	for (unsigned int j = 0; j < element[curNumber].size(); ++j)
	{
		string templine = line;
		templine.replace(templine.find("##"), 2, element[curNumber][j]);

		if (templine.find("##") != string::npos)
		{
			vecstr tempAnimEvent = newAnimationElement(templine, element, curNumber + 1);
			animElement.reserve(animElement.size() + tempAnimEvent.size());
			animElement.insert(animElement.end(), tempAnimEvent.begin(), tempAnimEvent.end());
		}
		else
		{
			animElement.push_back(templine);
		}
	}

	return animElement;
}

string behaviorLineChooser(string originalline, unordered_map<string, string> chosenLines, vecstr behaviorPriority)
{
	for (unsigned int i = 0; i < behaviorPriority.size(); ++i)
	{
		if (chosenLines[behaviorPriority[i]].length() != 0)
		{
			return chosenLines[behaviorPriority[i]];
		}
	}

	return originalline;
}

vector<unique_ptr<registerAnimation>> openFile(getTemplate behaviortemplate)
{
	vector<unique_ptr<registerAnimation>> list;
	set<string> animPath;
	AAInitialize("alternate animation");

	for (auto it = behaviortemplate.grouplist.begin(); it != behaviortemplate.grouplist.end(); ++it)
	{
		string path = behaviorPath[it->first];

		if (path.length() == 0)
		{
			cout << "ERROR(1050): Unregistered behavior path detected. Perform \"Update Patcher\" operation to fix this" << endl << "Behavior :" << it->first << endl << endl;
			error = true;
			return list;
		}

		size_t pos = wordFind(path, "\\behaviors\\", true);

		if (pos != -1)
		{
			animPath.insert(path.substr(0, pos) + "\\");
		}
		else
		{
			cout << "WARNING: Behavior located in non-standard path detected. Following behavior will be ignored" << endl << "Behavior: " << it->first << endl << "Path: " << path << endl << endl;
		}
	}

	for (auto it = animPath.begin(); it != animPath.end(); ++it)
	{
#ifndef DEBUG
		string directory = *it;
#else
		string directory = skyrimdataPath.GetDataPath() + *it;
#endif

		string animationDirectory = directory + "animations\\";
		vecstr filelist1;
		vecstr filelist2;
		unordered_map<string, vecstr> animFile;

		if (error)
		{
			return list;
		}

		read_directory(animationDirectory, filelist1);

		for (unsigned int l = 0; l < filelist1.size(); ++l)
		{
			boost::filesystem::path FOF(animationDirectory + filelist1[l]);

			if (boost::filesystem::is_directory(FOF))
			{
				read_directory(animationDirectory + filelist1[l], filelist2);

				for (unsigned int k = 0; k < filelist2.size(); ++k)
				{
					if (filelist2[k].find("_" + filelist1[l] + "_List.txt") != string::npos)
					{
						string fileToolName = filelist2[k].substr(0, filelist2[k].find("_", 0) + 1);
						string listword = filelist2[k].substr(filelist2[k].find_last_of("_"));
						string behaviorfile = fileToolName + filelist1[l] + "_Behavior.hkx";
						string modBehavior = directory + "Behaviors\\" + behaviorfile;

						if (!isFileExist(modBehavior))
						{
							cout << "ERROR(1082): " << behaviorfile << " not found. Please contact the mod author" << endl << "File Path: " << modBehavior << endl << endl;
							error = true;
							return list;
						}

						if (fileToolName == "FNIS_" && listword == "_List.txt")
						{
							list.emplace_back(make_unique<registerAnimation>(animationDirectory + filelist1[l] + "\\", filelist2[k], behaviortemplate, modBehavior, behaviorfile));
						}
						else if (fileToolName == "Nemesis_" && listword == "_List.txt")
						{
							list.emplace_back(make_unique<registerAnimation>(animationDirectory + filelist1[l] + "\\", filelist2[k], behaviortemplate, modBehavior, behaviorfile, true));
						}

						if (error)
						{
							return list;
						}
					}
				}

				filelist2.clear();
			}
		}
	}

	return list;
}

void GetBehaviorPath()
{
	string filename = "behavior_path.txt";

	if (isFileExist(filename))
	{
		int linecount = 0;
		char line[2000];
		FILE* pathFile;
		fopen_s(&pathFile, filename.c_str(), "r");

		if (pathFile)
		{
			while (fgets(line, 2000, pathFile))
			{
				++linecount;

				if ((line[0] != '\'' || line[0] != '\n') && strlen(line) != 0)
				{
					stringstream sstream(line);
					istream_iterator<string> ssbegin(sstream);
					istream_iterator<string> ssend;
					vecstr path(ssbegin, ssend);
					copy(path.begin(), path.end(), path.begin());

					for (unsigned int i = 0; i < path.size(); ++i)
					{
						boost::algorithm::to_lower(path[i]);
					}

					if (path.size() == 2)
					{
						behaviorPath[path[0]] = path[1];
					}
					else
					{
						if (path.size() > 2)
						{
							string pathline = "";

							for (unsigned int i = 1; i < path.size(); ++i)
							{
								if (path[i].find("\\") != string::npos)
								{
									pathline = pathline + path[i] + " ";
								}
								else
								{
									cout << "ERROR(1067): Invalid input. Only 2 elements are acceptable with the first element being the file name and second element being the file path" << endl << "File: " << filename << "Line: " << linecount << endl << endl;
									error = true;
									fclose(pathFile);
									return;
								}
							}

							pathline.pop_back();
							behaviorPath[path[0]] = pathline;
						}
						else
						{
							cout << "ERROR(1067): Invalid input. Only 2 elements are acceptable with the first element being the file name and second element being the file path" << endl << "File: " << filename << "Line: " << linecount << endl << endl;
							error = true;
							fclose(pathFile);
							return;
						}
					}
				}
			}
		}
		else
		{
			cout << "ERROR(2602): Unable to open file" << endl << "File: " << filename << endl << endl;
			error = true;
			return;
		}

		fclose(pathFile);
	}
	else
	{
		cout << "ERROR(1068): Missing \"" << filename << "\" file. Perform \"Update Patcher\" operation to fix this" << endl << "File :" << filename << endl << endl;
		error = true;
		return;
	}
}

void GetAnimData()
{
	string filename = "animationdata_list.txt";

	if (isFileExist(filename))
	{
		int linecount = 0;
		char charline[2000];
		FILE* pathFile;
		fopen_s(&pathFile, filename.c_str(), "r");
		bool newCharacter = false;
		string character;
		string line;

		if (pathFile)
		{
			while (fgets(charline, 2000, pathFile))
			{
				line = charline;

				if (line.back() == '\n')
				{
					line.pop_back();
				}

				if (!newCharacter)
				{
					if (line.length() == 0)
					{
						cout << "ERROR(3009): Invalid format detected in animationdata_list.txt. Please re-run Update Pathcer to fix it" << endl;
						error = true;
						return;
					}

					character = line;
					newCharacter = true;

					if (characterHeaders.find(character) != characterHeaders.end())
					{
						cout << "ERROR(3010): Duplicated character detected in animationdata_list.txt. Please re-run Update Pathcer to fix it" << endl << "Character: " << character << endl << endl;
						error = true;
						return;
					}
				}
				else if (line.length() == 0)
				{
					newCharacter = false;
				}
				else if (newCharacter)
				{
					if (characterHeaders[character].find(line) != characterHeaders[character].end())
					{
						cout << "ERROR(3008): Duplicated header detected in animationdata_list.txt. Please re-run Update Pathcer to fix it" << endl << "Character: " << character << endl << endl;
						error = true;
						return;
					}
					
					characterHeaders[character].insert(line);
				}
			}
		}
		else
		{
			cout << "ERROR(2602): Unable to open file" << endl << "File: " << filename << endl << endl;
			error = true;
			return;
		}

		fclose(pathFile);
	}
	else
	{
		cout << "ERROR(1068): Missing \"" << filename << "\" file. Perform \"Update Patcher\" operation to fix this" << endl << "File :" << filename << endl << endl;
		error = true;
		return;
	}
}

void FolderCreate(string curBehaviorPath)
{
	size_t pos = curBehaviorPath.find("\\") + 1;
	string curFolder = curBehaviorPath.substr(0, pos);
	__int64 counter = sameWordCount(curBehaviorPath, "\\");

	for (int i = 0; i < counter; ++i)
	{
		if (CreateDirectory((curFolder).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
		{
			pos = curBehaviorPath.find("\\", pos) + 1;

			if (pos != 0)
			{
				curFolder = curBehaviorPath.substr(0, pos);
			}
		}
	}
}

void characterHKX(string directory, string filename)
{
	char charline[2000];
	string line;
	FILE* file;
	fopen_s(&file, (directory + filename).c_str(), "r");

	if (file)
	{
		while (fgets(charline, 2000, file))
		{
			line = charline;

			if (line.find("<hkparam name=\"behaviorFilename\">") != string::npos)
			{
				size_t nextpos = line.find("behaviorFilename\">") + 18;
				string behaviorName = line.substr(nextpos, line.find("</hkparam>", nextpos) - nextpos);
				behaviorName = GetFileName(behaviorName);
				boost::algorithm::to_lower(behaviorName);
				string lowerBehaviorFile = boost::algorithm::to_lower_copy(filename.substr(0, filename.find_last_of(".")));
				behaviorJoints[behaviorName].push_back(lowerBehaviorFile);
			}
		}
		
		fclose(file);
	}
	else
	{
		cout << "ERROR(3002): Failed to open behavior template" << endl << "File: " << directory << filename << endl << endl;
		error = true;
		return;
	}
}

string GetFileName(string filepath)
{
	string filename;
	size_t nextpos;

	if (filepath.find("/") != string::npos)
	{
		if (filepath.find("\\") != string::npos)
		{
			if (filepath.find_last_of("/") < filepath.find_last_of("\\"))
			{
				nextpos = filepath.find_last_of("\\") + 1;
			}
			else
			{
				nextpos = filepath.find_last_of("/") + 1;
			}
		}
		else
		{
			nextpos = filepath.find_last_of("/") + 1;
		}
	}
	else
	{
		nextpos = filepath.find_last_of("\\") + 1;
	}

	filename = filepath.substr(nextpos, filepath.find_last_of(".") - nextpos);
	return filename;
}

inline bool isEdited(getTemplate& BehaviorTemplate, string& lowerBehaviorFile, unordered_map<string, vector<shared_ptr<Furniture>>>& newAnimation, bool isCharacter)
{
	if (BehaviorTemplate.grouplist.find(lowerBehaviorFile) != BehaviorTemplate.grouplist.end() && BehaviorTemplate.grouplist[lowerBehaviorFile].size() > 0)
	{
		vecstr templateGroup = BehaviorTemplate.grouplist[lowerBehaviorFile];

		for (unsigned int j = 0; j < templateGroup.size(); ++j)
		{
			if (newAnimation.find(templateGroup[j]) != newAnimation.end() && newAnimation[templateGroup[j]].size() != 0)
			{
				return true;
			}
		}
	}

	if (isCharacter)
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
									return true;
								}
							}
						}
					}
				}
			}
		}
	}

	return false;
}
