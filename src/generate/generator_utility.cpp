#include <thread>

#include <boost/atomic.hpp>

#include "version.h"
#include "debuglog.h"
#include "nemesisinfo.h"

#include "utilities/compute.h"
#include "utilities/lastupdate.h"
#include "utilities/atomiclock.h"
#include "utilities/stringsplit.h"
#include "utilities/readtextfile.h"

#include "generate/playerexclusive.h"
#include "generate/generator_utility.h"
#include "generate/animationdatatracker.h"

#include "generate/animation/templateinfo.h"
#include "generate/animation/registeranimation.h"

#pragma warning(disable:4503)

using namespace std;

extern unordered_map<string, string> crc32Cache;
extern VecStr warningMsges;

static bool* globalThrow;

VecStr fileCheckMsg;
VecStr hkxFiles;

atomic_flag animdata_lock{};

void readList(string directory, string animationDirectory, vector<unique_ptr<registerAnimation>>& list, TemplateInfo& behaviortemplate, bool firstP);
void fileArchitectureCheck(string hkxfile);
void checkFolder(string filepath);

std::vector<int> GetStateID(map<int, int> mainJoint, map<int, VecStr> functionlist, unordered_map<int, int>& functionState)
{
	std::vector<int> stateID;
	VecStr storeID;
	bool open = false;
	bool rightFunction = false;
	size_t jointsize = mainJoint.size();

	if (jointsize > 0)
	{
		for (auto it = mainJoint.begin(); it != mainJoint.end(); ++it)
		{
			if (functionState.find(it->second) == functionState.end())
			{
				int curState = 0;

				for (unsigned int j = 0; j < functionlist[it->second].size(); ++j)
				{
					string curline = functionlist[it->second][j];

					if (curline.find("class=\"hkbStateMachine\" signature=\"") != NOT_FOUND) rightFunction = true;
					else if (curline.find("<hkparam name=\"states\" numelements=\"") != NOT_FOUND) open = true;

					if (!rightFunction)
					{
						ErrorMessage(1077);
					}
					else if (open)
					{
						if (curline.find("#") != NOT_FOUND)
						{
							size_t counter = count(curline.begin(), curline.end(), '#');
							size_t nextpos = 0;
							VecStr generator;
							StringSplit(curline, generator);

							for (size_t k = 0; k < generator.size(); ++k) // multiple IDs in 1 line
							{
								curline = generator[k];

								if (curline.find("#") != 0 || !isOnlyNumber(curline.substr(1))) continue;

								int ID = stoi(curline.substr(1));

								for (unsigned int l = 0; l < functionlist[ID].size(); ++l)
								{
									string line = functionlist[ID][l];

									if (line.find("<hkparam name=\"stateId\">", 0) != NOT_FOUND)
									{
                                        int tempStateID
                                            = stoi(nemesis::regex_replace(string(line),
                                                                          nemesis::regex("[^0-9]*([0-9]+).*"),
                                                                          string("\\1")));

										if (tempStateID >= curState) curState = tempStateID + 1;

										break;
									}
								}
							}
						}
					}
				}

				functionState[it->second] = curState;
				stateID.push_back(curState);
			}
			else
			{
				stateID.push_back(functionState[it->second]);
			}
		}
	}
	else
	{
		ErrorMessage(1078);
	}
	
	return stateID;
}

bool GetStateCount(vector<int>& count, VecStr templatelines, string format, string filename, bool hasGroup)
{
	int counter = 1;

	for (auto& line : templatelines)
	{
		size_t pos = line.find("\t\t\t<hkparam name=\"stateId\">$(S");

		if (pos != NOT_FOUND && line.find(")$</hkparam>", pos) != NOT_FOUND)
		{
            string ID = nemesis::regex_replace(
                string(line),
                nemesis::regex(".*<hkparam name=\"stateId\">[$]\\(S([0-9]*)(.*)\\)[$]</hkparam>.*"),
                string("\\1"));
            string number = nemesis::regex_replace(
                string(line),
                nemesis::regex(".*<hkparam name=\"stateId\">[$]\\(S([0-9]*)(.*)\\)[$]</hkparam>.*"),
                string("\\2"));

			if (ID != line && number != line)
			{
				size_t IDSize;

				if (ID.empty()) IDSize = 0;
				else IDSize = static_cast<size_t>(stoi(ID)) - 1;

				if (IDSize >= count.size())
				{
					while (IDSize >= int(count.size()))
					{
						count.push_back(0);
					}
				}

				string equation = "0" + number;
				nemesis::calculate(equation, format, filename, counter);

				if (count[IDSize] <= stoi(equation)) count[IDSize] = stoi(equation) + 1;
			}
		}

		++counter;
	}

	return true;
}

VecStr newAnimationElement(string line, vector<VecStr> element, int curNumber)
{
	VecStr animElement;

	for (unsigned int j = 0; j < element[curNumber].size(); ++j)
	{
		string templine = line;
		templine.replace(templine.find("##"), 2, element[curNumber][j]);

		if (templine.find("##") != NOT_FOUND)
		{
			VecStr tempAnimEvent = newAnimationElement(templine, element, curNumber + 1);
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

string behaviorLineChooser(string originalline, unordered_map<string, string> chosenLines, VecStr behaviorPriority)
{
	int chosen = -1;

	for (unsigned int i = 0; i < behaviorPriority.size(); ++i)
	{
		if (chosenLines[behaviorPriority[i]].length() != 0)
		{
			if (chosen == -1) chosen = i;

			string line = nemesis::regex_replace(
                string(chosenLines[behaviorPriority[i]]), nemesis::regex("[\t]+([^\t]+).*"), string("\\1"));
            string line2 = nemesis::regex_replace(
                string(line), nemesis::regex("[^ ]+[ ]([^ ]+)[ ][^ ]+"), string("\\1"));

			if (line2 != line && line.find("<!-- ") == 0)
			{
				string out = chosenLines[behaviorPriority[i]];

				if (out.find("<!-- ") != NOT_FOUND)
				{
                    out = nemesis::regex_replace(string(chosenLines[behaviorPriority[i]]),
                                                 nemesis::regex("[^\t]+([\t]+<!-- [^ ]+ -->).*"),
                                                 string("\\1"));
					out = chosenLines[behaviorPriority[i]].substr(0, chosenLines[behaviorPriority[i]].find(out));
				}

				return out;
			}
		}
	}

	if (chosen != -1)
	{
		string out = chosenLines[behaviorPriority[chosen]];

		if (out.find("<!-- ") != NOT_FOUND)
		{
            out = nemesis::regex_replace(string(chosenLines[behaviorPriority[chosen]]),
                                       nemesis::regex("[^\t]+([\t]+<!-- [^ ]+ -->).*"),
                                       string("\\1"));
			out = chosenLines[behaviorPriority[chosen]].substr(0, chosenLines[behaviorPriority[chosen]].find(out));
		}

		return out;
	}

	string out = originalline;

	if (out.find("<!-- ") != NOT_FOUND)
	{
        out = nemesis::regex_replace(
            string(originalline), nemesis::regex("[^\t]+([\t]+<!-- [^ ]+ -->).*"), string("\\1"));
		out = originalline.substr(0, originalline.find(out));
	}

	return out;
}

void readList(string directory, string animationDirectory, vector<unique_ptr<registerAnimation>>& list, TemplateInfo& behaviortemplate, bool firstP)
{
	VecStr filelist;

	if (error) throw nemesis::exception();

	if (!isFileExist(animationDirectory)) return;

	read_directory(animationDirectory, filelist);

	for (auto& file1 : filelist)
	{
		if (std::filesystem::is_directory(animationDirectory + file1))
		{
			string targetfile = "FNIS_" + file1 + "_List.txt";
			string behaviorfile = "FNIS_" + file1 + "_Behavior.hkx";
			string targetdir = animationDirectory + file1 + "\\";
			string modBehavior = directory + "Behaviors\\" + behaviorfile;

			if (isFileExist(animationDirectory + file1 + "\\" + targetfile))
			{
				list.emplace_back(make_unique<registerAnimation>(targetdir, targetfile, behaviortemplate, modBehavior, behaviorfile, firstP));
			}

			targetfile = "Nemesis_" + file1 + "_List.txt";
			behaviorfile = "Nemesis_" + file1 + "_Behavior.hkx";

			if (isFileExist(animationDirectory + file1 + "\\" + targetfile))
			{
				list.emplace_back(make_unique<registerAnimation>(targetdir, targetfile, behaviortemplate, modBehavior, behaviorfile, firstP, true));
			}
		}

		if (error) throw nemesis::exception();
	}
}

vector<unique_ptr<registerAnimation>> openFile(TemplateInfo* behaviortemplate, const NemesisInfo* nemesisInfo)
{
	vector<unique_ptr<registerAnimation>> list;
	set<string> animPath;
	AAInitialize("alternate animation");
	DebugLogging("Reading new animations...");

	for (auto& behaviorGroup : behaviortemplate->grouplist)
	{
		string path = behaviorPath[behaviorGroup.first];

		if (path.length() == 0) ErrorMessage(1050, behaviorGroup.first);

		size_t pos = wordFind(path, "\\behaviors\\", true);
		size_t nextpos = wordFind(path, "\\data\\") + 6;

		if (pos != NOT_FOUND && nextpos != NOT_FOUND && nextpos < pos) animPath.insert(string(path.substr(nextpos, pos - nextpos)) + '\\');
		else if (behaviorGroup.first != "animationdatasinglefile" && behaviorGroup.first != "animationsetdatasinglefile") WarningMessage(1007, behaviorGroup.first, path);

		if (error) throw nemesis::exception();
	}

	for (string path : animPath)
	{
#ifdef DEBUG
		string directory = "data\\" + path;
#else
		string directory = nemesisInfo->GetDataPath() + path;
#endif

		readList(directory, directory + "animations\\", list, *behaviortemplate, false);
		readList(directory, directory + "_1stperson\\animations\\", list, *behaviortemplate, true);
	}

	DebugLogging("Reading new animations complete");
	return list;
}

void newFileCheck(string directory, unordered_map<string, bool>* isChecked)
{
	VecStr filelist;
	read_directory(directory, filelist);

	try
	{
		for (auto& file : filelist)
		{
			string path = directory + "\\" + file;
			nemesis::to_lower(path);
			std::filesystem::path curfile(path);

			if (std::filesystem::is_directory(curfile))
			{
				newFileCheck(path, isChecked);
			}
			else if (nemesis::iequals(curfile.extension().string(), ".txt"))
			{
				if (directory.find("animationdatasinglefile") != NOT_FOUND)
				{
					size_t pos = directory.find("\\") + 1;
					string modcode = directory.substr(pos, directory.find("\\", pos) - pos);

					if (file.find(modcode + "$") == NOT_FOUND)
					{
						if (isChecked->find(path) == isChecked->end()) throw false;
					}
				}
				else if (directory.find("animationsetdatasinglefile") != NOT_FOUND)
				{					
					if (std::filesystem::path(directory).stem().string().find("~") != NOT_FOUND && file.length() > 0 && file[0] != '$')
					{
						if (isChecked->find(path) == isChecked->end()) throw false;
					}
				}
				else if (!nemesis::iequals(file, "option_list.txt"))
				{
					if (isChecked->find(path) == isChecked->end()) throw false;
				}
			}

			if (globalThrow) break;
		}
	}
	catch (bool& ex)
	{
		globalThrow = &ex;
	}
}

bool isEngineUpdated(string& versionCode)
{
	string directory = "temp_behaviors";
	VecStr filelist;

	read_directory(directory, filelist);

	if (filelist.size() < 3) ErrorMessage(6006);

	VecStr storeline;
	string filename = "cache\\engine_update";
	unordered_map<string, bool> isChecked;

	if (!isFileExist(filename)) return false;

	if (!GetFunctionLines(filename, storeline, false)) return false;

	if (storeline.size() > 0)
	{
		if (GetNemesisVersion() != storeline[0]) return false;

		storeline.erase(storeline.begin());
	}

	if (storeline.size() > 0)
	{
		versionCode = storeline[0];
		storeline.erase(storeline.begin());
	}

	for (auto& line : storeline)
	{
		if (line.length() > 0)
		{
			size_t pos = line.find(">>");

			if (pos == NOT_FOUND) ErrorMessage(2021);

			string part1 = line.substr(0, pos);
			string part2 = line.substr(pos + 2);

			if (!isFileExist(part1)) return false;
			else if (GetLastModified(part1) != part2) return false;

			isChecked[part1] = true;
		}
	}

	globalThrow = nullptr;
	std::thread t1(newFileCheck, "mod", &isChecked);
	std::thread t2(newFileCheck, "behavior templates", &isChecked);

	t1.join();
	t2.join();

	if (globalThrow)
	{
		globalThrow = nullptr;
		return false;
	}

	globalThrow = nullptr;
	return true;
}

void GetBehaviorPath()
{
	string filename = "cache\\behavior_path";

	if (isFileExist(filename))
	{
		int linecount = 0;
		FileReader pathFile(filename);

		if (pathFile.GetFile())
		{
			string line;

			while (pathFile.GetLines(line))
			{
				++linecount;
				size_t pos = line.find("=");

				if (pos == NOT_FOUND) ErrorMessage(1067, filename, linecount);

				behaviorPath[line.substr(0, pos)] = line.substr(pos + 1);
			}
		}
		else
		{
			ErrorMessage(2000, filename);
		}
	}
	else
	{
		ErrorMessage(1068, filename);
	}
}

void GetBehaviorProject()
{
	string filename = "cache\\behavior_project";

	if (isFileExist(filename))
	{
		string characterfile;
		bool newChar = true;
		FileReader pathFile(filename);

		if (pathFile.GetFile())
		{
			string line;

			while (pathFile.GetLines(line))
			{
				if (line.length() == 0)
				{
					newChar = true;
				}
				else if (newChar)
				{
					characterfile = line;
					newChar = false;
				}
				else
				{
					behaviorProject[characterfile].push_back(line);
				}
			}
		}
		else
		{
			ErrorMessage(2000, filename);
		}
	}
	else
	{
		ErrorMessage(1068, filename);
	}
}

void GetBehaviorProjectPath()
{
	string filename = "cache\\behavior_project_path";

	if (isFileExist(filename))
	{
		int linecount = 0;
		FileReader pathFile(filename);

		if (pathFile.GetFile())
		{
			string line;

			while (pathFile.GetLines(line))
			{
				++linecount;
				size_t pos = line.find("=");

				if (pos == NOT_FOUND) ErrorMessage(1067, filename, linecount);

				behaviorProjectPath[line.substr(0, pos)] = line.substr(pos + 1);
			}
		}
		else
		{
			ErrorMessage(2000, filename);
		}
	}
	else
	{
		ErrorMessage(1068, filename);
	}
}

void GetAnimData()
{
	string filename = "cache\\animationdata_list";
	unordered_map<string, set<string>> characterHeaders;

	if (isFileExist(filename))
	{
		int linecount = 0;
		FileReader pathFile(filename);
		bool newCharacter = false;
		string character;

		if (pathFile.GetFile())
		{
			string line;

			while (pathFile.GetLines(line))
			{
				if (!newCharacter)
				{
					if (line.length() == 0) ErrorMessage(3019);

					character = line;
					newCharacter = true;

					if (characterHeaders.find(character) != characterHeaders.end()) ErrorMessage(3010, character);
				}
				else if (line.length() == 0)
				{
					newCharacter = false;
				}
				else if (newCharacter)
				{
					if (characterHeaders[character].find(line) != characterHeaders[character].end()) ErrorMessage(3008, character);
					
					characterHeaders[character].insert(line);
				}
			}
		}
		else
		{
			ErrorMessage(2000, filename);
		}
	}
	else
	{
		ErrorMessage(1068, filename);
	}
}

void characterHKX()
{
	string filename = "cache\\behavior_joints";

	if (isFileExist(filename))
	{
		bool open = false;
		FileReader file(filename);

		if (file.GetFile())
		{
			string line;
			string header;

			while (file.GetLines(line))
			{
				if (line.length() != 0)
				{
					if (!open)
					{
						open = true;
						header = line;
					}
					else
					{
						if (header.length() == 0) ErrorMessage(1094);

						behaviorJoints[header].push_back(line);
					}
				}
				else
				{
					open = false;
				}
			}
		}
		else
		{
			ErrorMessage(3002, filename);
		}
	}
	else
	{
		ErrorMessage(1068, filename);
	}
}

string GetFileName(string filepath)
{
	return std::filesystem::path(filepath).stem().string();
}
string GetFileName(string_view filepath)
{
	return std::filesystem::path(filepath).stem().string();
}

wstring GetFileName(wstring filepath)
{
	return std::filesystem::path(filepath).stem().wstring();
}

wstring GetFileName(wstring_view filepath)
{
	return std::filesystem::path(filepath).stem().wstring();
}

string GetFileDirectory(string filepath)
{
	string dir = std::filesystem::path(filepath).parent_path().string();
	return filepath.substr(0, dir.length() + 1);
}

wstring GetFileDirectory(wstring filepath)
{
	wstring dir = std::filesystem::path(filepath).parent_path().wstring();
	return filepath.substr(0, dir.length() + 1);
}

int getTemplateNextID(VecStr& templatelines)
{
	unordered_map<int, bool> taken;
	int IDUsed = 0;

	for (auto& line : templatelines)
	{
		if (line.find("<hkobject name=\"#MID$") != NOT_FOUND)
		{
            string number
                = nemesis::regex_replace(string(line),
                                       nemesis::regex(".*<hkobject name=\"#MID[$]([0-9]+)\" class=\".*"),
                                       string("\\1"));

			if (number != line && isOnlyNumber(number))
			{
				int num = stoi(number);

				if (!taken[num])
				{
					taken[num] = true;
					++IDUsed;
				}
			}
		}
		else
		{
			size_t pos = line.find("import[");

			if (pos != NOT_FOUND && line.find("]", pos) != NOT_FOUND) ++IDUsed;
		}
	}

	return IDUsed;
}

bool isEdited(TemplateInfo* BehaviorTemplate, string& lowerBehaviorFile, unordered_map<string, vector<shared_ptr<NewAnimation>>>& newAnimation, bool isCharacter, string modID)
{
	if (BehaviorTemplate->grouplist.find(lowerBehaviorFile) != BehaviorTemplate->grouplist.end() && BehaviorTemplate->grouplist[lowerBehaviorFile].size() > 0)
	{
		for (auto& templatecode : BehaviorTemplate->grouplist[lowerBehaviorFile])
		{
			if (newAnimation.find(templatecode) == newAnimation.end()) continue;

			for (auto& curAnim : newAnimation[templatecode])
			{
				if (modID == curAnim->coreModID) return true;
			}
		}
	}

	if (isCharacter)
	{
		if ((lowerBehaviorFile == "defaultfemale" || lowerBehaviorFile == "defaultmale") && animReplaced.size() > 0) return true;

		for (auto templist : BehaviorTemplate->grouplist)
		{
			VecStr behaviorNames = behaviorJoints[templist.first];

			for (auto name : behaviorNames)
			{
				if (templist.second.size() == 0 || lowerBehaviorFile != name) continue;

				for (auto& templatecode : templist.second)
				{
					if (newAnimation.find(templatecode) == newAnimation.end() || BehaviorTemplate->optionlist[templatecode].core) continue;

					for (auto& curAnim : newAnimation[templatecode])
					{
						if (!curAnim->isKnown()) return true;
					}
				}
			}
		}
	}

	return false;
}

bool newAnimSkip(vector<shared_ptr<NewAnimation>> newAnim, string modID)
{
	for (auto& anim : newAnim)
	{
		if (anim->coreModID == modID) return false;
	}

	return true;
}

void checkClipAnimData(string& line, VecStr& characterFiles, string& clipName, bool& isClip)
{
	if (!isClip)
	{
		if (line.find("class=\"hkbClipGenerator\" signature=\"0x333b85b9\">") != NOT_FOUND) isClip = true;
	}
	else
	{
		size_t pos = line.find("<hkparam name=\"animationName\">");

		if (pos != NOT_FOUND)
		{
			isClip = false;
			pos += 30;
			string animFile = std::filesystem::path(line.substr(pos, line.find("</hkparam>", pos) - pos)).filename().string();
			nemesis::to_lower(animFile);

			for (auto file : characterFiles)
			{
                Lockless locker(animdata_lock);
				shared_ptr<AnimationDataTracker>& animDataPtr = charAnimDataInfo[file][animFile];

				if (animDataPtr == nullptr)
				{
					animDataPtr = make_shared<AnimationDataTracker>();
					animDataPtr->filename = animFile;
				}

				animDataPtr->cliplist.insert(clipName);
				vector<shared_ptr<AnimationDataTracker>>& listAnimData = clipPtrAnimData[file][clipName];

				if (listAnimData.size() > 0)
				{
					bool same = false;

					for (auto& animData : listAnimData)
					{
						if (animData->filename == animFile)
						{
							same = true;
							break;
						}
					}

					if (!same)
					{
						clipPtrAnimData[file][clipName].push_back(animDataPtr);
					}
				}
				else
				{
					clipPtrAnimData[file][clipName].push_back(animDataPtr);
				}
			}
		}
		else
		{
			pos = line.find("<hkparam name=\"name\">");

			if (pos != NOT_FOUND)
			{
				pos += 21;
				clipName = line.substr(pos, line.find("</hkparam>", pos) - pos);
			}
		}
	}
}

void fileArchitectureCheck(string hkxfile)
{
	FILE* f;
	fopen_s(&f, hkxfile.c_str(), "r+b");

	if (f)
	{
		fseek(f, 0x10, SEEK_SET);
		unsigned char charcode;
		fread(&charcode, sizeof(charcode), 1, f);
		fclose(f);
		
		if (SSE)
		{
			if (charcode == 0x4)
			{
				WarningMessage(1027, "32bit", hkxfile);
				fileCheckMsg.push_back(warningMsges.back());
			}
		}
		else if (charcode == 0x8)
		{
			WarningMessage(1027, "64bit", hkxfile);
			fileCheckMsg.push_back(warningMsges.back());
		}
	}
}

void checkAllStoredHKX()
{
	for (auto& file : hkxFiles)
	{
		if (isFileExist(file))
		{
			fileArchitectureCheck(file);
		}
	}

	hkxFiles.clear();
}

void checkFolder(string filepath)
{
	VecStr list;
	read_directory(filepath, list);

	for (auto& each : list)
	{
		std::filesystem::path file(filepath + "\\" + each);

		if (std::filesystem::is_directory(file))
		{
			checkFolder(file.string());
		}
		else if (nemesis::iequals(file.extension().string(), ".hkx"))
		{
			hkxFiles.push_back(file.string());
		}

		if (error) throw nemesis::exception();
	}
}

void checkAllFiles(string filepath)
{
	try
	{
		try
		{
			try
			{
				DebugLogging("Background hkx file architecture check: INITIALIZED");
				hkxFiles.clear();
				checkFolder(filepath);
				DebugLogging("Background hkx file architecture check: COMPLETED");
			}
			catch (const exception& ex)
			{
				ErrorMessage(6002, "None", ex.what());
			}
		}
		catch (const nemesis::exception&)
		{
		}
	}
	catch (...)
	{
		try
		{
			ErrorMessage(6002, "None", "Unknown");
		}
		catch (nemesis::exception&)
		{
			// resolved exception
		}
	}
}

void ClearGlobal(bool all)
{
	if (all)
	{
		DebugLogging("Global reset all: TRUE");

		usedAnim = unordered_map<string, SetStr>();

		registeredAnim = unordered_map<string, unordered_map<string, bool>>();

		animModMatch = unordered_map<string, unordered_map<string, vector<SetStr>>>();

		behaviorJoints = unordered_map<string, VecStr>();

		warningMsges = VecStr();
	}
	else
	{
		DebugLogging("Global reset all: FALSE");
	}

	clipPtrAnimData = map<string, map<string, vector<shared_ptr<AnimationDataTracker>>>>();
	charAnimDataInfo = map<string, map<string, shared_ptr<AnimationDataTracker>>>();

	behaviorProjectPath = unordered_map<string, string>();
	behaviorPath = unordered_map<string, string>();
	AAGroup = unordered_map<string, string>();
	crc32Cache = unordered_map<string, string>();
	
	behaviorProject = unordered_map<string, VecStr>();
	alternateAnim = unordered_map<string, VecStr>();
	groupAA = unordered_map<string, VecStr>();
	groupAAPrefix = unordered_map<string, VecStr>();
	AAEvent = unordered_map<string, VecStr>();
	AAHasEvent = unordered_map<string, VecStr>();

	pcealist = vector<PCEA>();

	animReplaced = unordered_map<string, vector<PCEAData>>();

	activatedBehavior = unordered_map<string, bool>();
	
	AAGroupCount = unordered_map<string, unordered_map<string, int>>();

	AAgroup_Counter = unordered_map<string, int>();

	groupNameList = set<string>();

	fileCheckMsg = VecStr();
}
