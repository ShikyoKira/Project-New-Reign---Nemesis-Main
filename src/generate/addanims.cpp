#include "Global.h"

#include "generate/addanims.h"
#include "generate/generator_utility.h"
#include "generate/animationdatatracker.h"
#include "generate/generator_utility.h"
#include "utilities/atomiclock.h"

using namespace std;

extern atomic_flag animdata_lock;

bool AddAnims(string& line,
              string animPath,
              string outputdir,
              string behaviorFile,
              string lowerBehaviorFile,
              string& newMod,
              VecStr& storeline,
              int& counter,
              unordered_map<string, bool>& isAdded,
              bool& addAnim)
{
	if (!isAdded[nemesis::to_lower_copy(animPath)])
	{
		string animFile = GetFileName(animPath) + ".hkx";

		if (activatedBehavior["gender*"])
		{
			if (lowerBehaviorFile == "defaultfemale")
			{
				if (!nemesis::iequals(animPath, "Animations\\female\\" + animFile))
				{
					std::filesystem::path animation(GetFileDirectory(outputdir));

					if (isFileExist(animation.parent_path().parent_path().string() + "\\Animations\\female\\" + animFile))
					{
						animPath = "Animations\\female\\" + animFile;
					}
					else if (nemesis::iequals(animPath, "Animations\\male\\" + animFile))
					{
						if (isFileExist(animation.parent_path().parent_path().string() + "\\Animations\\" + animFile))
						{
							animPath = "Animations\\" + animFile;
						}
					}
				}
			}
			else if (lowerBehaviorFile == "defaultmale")
			{
				if (!nemesis::iequals(animPath, "Animations\\male\\" + animFile))
				{
					std::filesystem::path animation(GetFileDirectory(outputdir));

					if (isFileExist(animation.parent_path().parent_path().string() + "\\Animations\\male\\" + animFile))
					{
						animPath = "Animations\\male\\" + animFile;
					}
					else if (nemesis::iequals(animPath, "Animations\\female\\" + animFile))
					{
						if (isFileExist(animation.parent_path().parent_path().string() + "\\Animations\\" + animFile))
						{
							animPath = "Animations\\" + animFile;
						}
					}
				}
			}
		}

		storeline.push_back("				<hkcstring>" + animPath + "</hkcstring>");
		nemesis::to_lower(animPath);
		nemesis::to_lower(animFile);
		isAdded[animPath] = true;

        {
            Lockless lock(animdata_lock);
            shared_ptr<AnimationDataTracker>& animData = charAnimDataInfo[lowerBehaviorFile][animFile];

			if (animData == nullptr)
			{
				animData = make_shared<AnimationDataTracker>(counter, animFile);
			}
			else
			{
				animData->SetOrder(counter);
			}
		}

        newMod                                      = animPath.substr(11, animPath.find("\\", 11) - 11);
        vector<set<string>>* match_ptr              = &animModMatch[lowerBehaviorFile][animFile];
        size_t matchSize                            = match_ptr->size();
        registeredAnim[lowerBehaviorFile][animFile] = true;
        addAnim                                     = true;

		if (matchSize == 0)
		{
			match_ptr->push_back(set<string>{animPath});
			match_ptr->push_back(set<string>{newMod});
		}
		else if (matchSize == 2)
		{
			match_ptr->at(0).insert(animPath);
			match_ptr->at(1).insert(newMod);
		}
		else
		{
			ErrorMessage(1058);
		}

		++counter;
	}

	return true;
}
