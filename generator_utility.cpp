#include "generator_utility.h"

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
	// string animationDirectory = GetDataPath() + "/meshes/actors/character/animations/";
	string animationDirectory = "data/meshes/actors/character/animations/"; // need update

	vecstr filelist1;
	vecstr filelist2;

	vector<unique_ptr<registerAnimation>> list;

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
				}
			}
		}
	}

	return list;
}

void getBehaviorPath(SSMap& behaviorPath)
{
	string dir = "vanilla_behaviors";
}

void FolderCreate(string behaviorPath, DataPath skyrimDataPath)
{
	size_t pos = behaviorPath.find("/") + 1;
	string curFolder = skyrimDataPath.GetDataPath() + behaviorPath.substr(0, pos);
	__int64 counter = sameWordCount(behaviorPath, "/");

	for (int i = 0; i < counter; ++i)
	{
		if (CreateDirectory((curFolder).c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
		{
			pos = behaviorPath.find("/", pos) + 1;

			if (pos != 0)
			{
				curFolder = skyrimDataPath.GetDataPath() + behaviorPath.substr(0, pos);
			}
		}
	}
}
