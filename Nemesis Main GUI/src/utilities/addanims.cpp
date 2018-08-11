#include "addanims.h"
#include "generator_utility.h"

using namespace std;

bool AddAnims(string& line, string animPath, string outputdir,string behaviorFile, string lowerBehaviorFile, string& newMod, vecstr& storeline, int& counter,
	unordered_map<string, bool>& isAdded, bool& addAnim)
{
	string animFile = GetFileName(animPath);

	if (!isAdded[animPath])
	{
		if (activatedBehavior["gender"])
		{
			if (lowerBehaviorFile == "defaultfemale")
			{
				if (!boost::iequals(animPath, "Animations\\female\\" + animFile))
				{
					boost::filesystem::path animation(GetFileDirectory(outputdir));

					if (isFileExist(animation.parent_path().parent_path().string() + "\\Animations\\female\\" + animFile))
					{
						size_t nextpos = line.find(animPath);
						animPath = "Animations\\female\\" + animFile;
					}
					else if (boost::iequals(animPath, "Animations\\male\\" + animFile))
					{
						if (isFileExist(animation.parent_path().parent_path().string() + "\\Animations\\" + animFile))
						{
							size_t nextpos = line.find(animPath);
							animPath = "Animations\\" + animFile;
						}
					}
				}
			}
			else if (lowerBehaviorFile == "defaultmale")
			{
				if (!boost::iequals(animPath, "Animations\\male\\" + animFile))
				{
					boost::filesystem::path animation(GetFileDirectory(outputdir));

					if (isFileExist(animation.parent_path().parent_path().string() + "\\Animations\\male\\" + animFile))
					{
						size_t nextpos = line.find(animPath);
						animPath = "Animations\\male\\" + animFile;
					}
					else if (boost::iequals(animPath, "Animations\\female\\" + animFile))
					{
						if (isFileExist(animation.parent_path().parent_path().string() + "\\Animations\\" + animFile))
						{
							size_t nextpos = line.find(animPath);
							animPath = "Animations\\" + animFile;
						}
					}
				}
			}
		}

		storeline.push_back("				<hkcstring>" + animPath + "</hkcstring>");
		boost::to_lower(animPath);
		boost::to_lower(animFile);
		isAdded[animPath] = true;
		newMod = animPath.substr(10, animPath.find("\\", 10) - 10);
		size_t matchSize = animModMatch[behaviorFile][animFile].size();
		registeredAnim[lowerBehaviorFile][animFile] = true;
		addAnim = true;

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
			return false;
		}

		++counter;
	}

	return true;
}
