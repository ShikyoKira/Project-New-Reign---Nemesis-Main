#include "generator_utility.h"

#pragma warning(disable:4503)

using namespace std;

int GetStateID(int mainJoint, map<int, vecstr> functionlist)
{
	int stateID = 0;
	vecstr storeID;
	bool open = false;
	bool rightFunction = false;

	for (unsigned int j = 0; j < functionlist[mainJoint].size(); j++)
	{
		string curline = functionlist[mainJoint][j];

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
			break;
		}
		else if (open)
		{
			if (curline.find("#") != string::npos)
			{
				size_t counter = count(curline.begin(), curline.end(), '#');
				size_t nextpos = 0;

				for (int k = 0; k < counter; k++) // multiple IDs in 1 line
				{
					nextpos = curline.find("#", nextpos) + 1;

					int ID = stoi(boost::regex_replace(string(curline.substr(nextpos)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

					for (unsigned int l = 0; l < functionlist[ID].size(); l++)
					{
						string line = functionlist[ID][l];

						if (line.find("<hkparam name=\"stateId\">", 0) != string::npos)
						{
							int tempStateID = stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

							if (tempStateID >= stateID)
							{
								stateID = tempStateID + 1;
							}

							break;
						}
					}
				}
			}
		}
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
	for (unsigned int i = 0; i < behaviorPriority.size(); i++)
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

	for (auto it = behaviortemplate.grouplist.begin(); it != behaviortemplate.grouplist.end(); ++it)
	{
		string path = behaviorPath[it->first];

		if (path.length() == 0)
		{
			cout << "ERROR(1050): Unregistered behavior path detected. Perform \"Update Patcher\" operation to fix this" << endl << "Behavior :" << it->first << endl << endl;
			error = true;
			return list;
		}

		size_t pos = wordFind(path, "/behaviors/", true);

		if (pos != -1)
		{
			animPath.insert(path.substr(0, pos) + "/");
		}
		else
		{
			cout << "WARNING: Behavior located in non-standard path detected. Following behavior will be ignored" << endl << "Behavior: " << it->first << endl << "Path: " << path << endl << endl;
		}
	}

	for (auto it = animPath.begin(); it != animPath.end(); ++it)
	{
#ifndef DEBUG
		string animationDirectory = *it + "animations/";
#else
		string animationDirectory = skyrimdataPath.GetDataPath() + *it + "animations/";
#endif

		vecstr filelist1;
		vecstr filelist2;
		unordered_map<string, vecstr> animFile;

		if (error)
		{
			return list;
		}

		read_directory(animationDirectory, filelist1);

		for (unsigned int l = 0; l < filelist1.size(); l++)
		{
			boost::filesystem::path FOF(animationDirectory + filelist1[l]);

			if (boost::filesystem::is_directory(FOF))
			{
				read_directory(animationDirectory + filelist1[l], filelist2);

				for (unsigned int k = 0; k < filelist2.size(); k++)
				{
					if (filelist2[k].find("_" + filelist1[l] + "_List.txt") != string::npos)
					{
						string fileToolName = filelist2[k].substr(0, filelist2[k].find("_", 0) + 1);
						string listword = filelist2[k].substr(filelist2[k].find_last_of("_"));

						if (fileToolName == "FNIS_" && listword == "_List.txt")
						{
							list.emplace_back(make_unique<registerAnimation>(animationDirectory + filelist1[l] + "/" + filelist2[k], filelist2[k], behaviortemplate));
						}
						else if (fileToolName == "Nemesis_" && listword == "_List.txt")
						{
							list.emplace_back(make_unique<registerAnimation>(animationDirectory + filelist1[l] + "/" + filelist2[k], filelist2[k], behaviortemplate, true));
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
				linecount++;

				if ((line[0] != '\'' || line[0] != '\n') && strlen(line) != 0)
				{
					stringstream sstream(line);
					istream_iterator<string> ssbegin(sstream);
					istream_iterator<string> ssend;
					vecstr path(ssbegin, ssend);
					copy(path.begin(), path.end(), path.begin());

					if (path.size() == 2)
					{
						behaviorPath[path[0]] = path[1];
					}
					else
					{
						cout << "ERROR(1067): Invalid input. Only 2 elements are acceptable with the first element being the file name and second element being the file path" << endl << "File: " << filename << "Line: " << linecount << endl << endl;
						error = true;
						return;
					}
				}
			}
		}
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
	size_t pos = curBehaviorPath.find("/") + 1;

#ifdef DEBUG
	string curFolder = curBehaviorPath.substr(0, pos);
#else
	string curFolder = curBehaviorPath.substr(0, pos);
#endif
	__int64 counter = sameWordCount(curBehaviorPath, "/");

	for (int i = 0; i < counter; ++i)
	{
		if (CreateDirectory((curFolder).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
		{
			pos = curBehaviorPath.find("/", pos) + 1;

			if (pos != 0)
			{
				curFolder = curBehaviorPath.substr(0, pos);
			}
		}
	}
}
