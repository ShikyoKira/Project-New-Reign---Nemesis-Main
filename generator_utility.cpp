#include "generator_utility.h"
#include "readtextfile.h"
#include "add animation\playerexclusive.h"
#include <boost\thread.hpp>

#pragma warning(disable:4503)

using namespace std;

static bool* globalThrow;

std::vector<int> GetStateID(map<int, int> mainJoint, map<int, vecstr> functionlist, unordered_map<int, int>& functionState)
{
	std::vector<int> stateID;
	vecstr storeID;
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

					if (curline.find("class=\"hkbStateMachine\" signature=\"") != NOT_FOUND)
					{
						rightFunction = true;
					}
					else if (curline.find("<hkparam name=\"states\" numelements=\"") != NOT_FOUND)
					{
						open = true;
					}

					if (!rightFunction)
					{
						ErrorMessage(1077);
						return stateID;
					}
					else if (open)
					{
						if (curline.find("#") != NOT_FOUND)
						{
							size_t counter = count(curline.begin(), curline.end(), '#');
							size_t nextpos = 0;

							stringstream sstream(curline);
							istream_iterator<string> ssbegin(sstream);
							istream_iterator<string> ssend;
							vecstr generator(ssbegin, ssend);
							copy(generator.begin(), generator.end(), generator.begin());

							for (size_t k = 0; k < generator.size(); ++k) // multiple IDs in 1 line
							{
								curline = generator[k];

								if (curline.find("#") == 0 && isOnlyNumber(curline.substr(1)))
								{
									int ID = stoi(curline.substr(1));

									for (unsigned int l = 0; l < functionlist[ID].size(); ++l)
									{
										string line = functionlist[ID][l];

										if (line.find("<hkparam name=\"stateId\">", 0) != NOT_FOUND)
										{
											int tempStateID = stoi(boost::regex_replace(string(line), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));

											if (tempStateID >= curState)
											{
												curState = tempStateID + 1;
											}

											break;
										}
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
		return stateID;
	}
	
	return stateID;
}

bool GetStateCount(vector<int>& count, vecstr templatelines, string format, string filename, bool hasGroup)
{
	int counter = 1;

	for (auto& line : templatelines)
	{
		size_t pos = line.find("\t\t\t<hkparam name=\"stateId\">$(S");

		if (pos != NOT_FOUND && line.find(")$</hkparam>", pos) != NOT_FOUND)
		{
			string ID = boost::regex_replace(string(line), boost::regex(".*<hkparam name=\"stateId\">[$]\\(S([0-9]*)(.*)\\)[$]</hkparam>.*"), string("\\1"));
			string number = boost::regex_replace(string(line), boost::regex(".*<hkparam name=\"stateId\">[$]\\(S([0-9]*)(.*)\\)[$]</hkparam>.*"), string("\\2"));

			if (ID != line && number != line)
			{
				size_t IDSize;

				if (ID.empty())
				{
					IDSize = 0;
				}
				else
				{
					IDSize = static_cast<size_t>(stoi(ID)) - 1;
				}

				if (IDSize >= count.size())
				{
					while (IDSize >= int(count.size()))
					{
						count.push_back(0);
					}
				}

				string equation = "0" + number;

				if (!calculate(equation, format, filename, counter))
				{
					return false;
				}

				if (count[IDSize] <= stoi(equation))
				{
					count[IDSize] = stoi(equation) + 1;
				}
			}
		}

		++counter;
	}

	return true;
}

vecstr newAnimationElement(string line, vector<vecstr> element, int curNumber)
{
	vecstr animElement;

	for (unsigned int j = 0; j < element[curNumber].size(); ++j)
	{
		string templine = line;
		templine.replace(templine.find("##"), 2, element[curNumber][j]);

		if (templine.find("##") != NOT_FOUND)
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
	int chosen = -1;

	for (unsigned int i = 0; i < behaviorPriority.size(); ++i)
	{
		if (chosenLines[behaviorPriority[i]].length() != 0)
		{
			if (chosen == -1)
			{
				chosen = i;
			}

			string line = boost::regex_replace(string(chosenLines[behaviorPriority[i]]), boost::regex("[\t]+([^\t]+).*"), string("\\1"));
			string line2 = boost::regex_replace(string(line), boost::regex("[^ ]+[ ]([^ ]+)[ ][^ ]+"), string("\\1"));

			if (line2 != line && line.find("<!-- ") == 0)
			{
				string out = chosenLines[behaviorPriority[i]];

				if (out.find("<!-- ") != NOT_FOUND)
				{
					out = boost::regex_replace(string(chosenLines[behaviorPriority[i]]), boost::regex("[^\t]+([\t]+<!-- [^ ]+ -->).*"), string("\\1"));
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
			out = boost::regex_replace(string(chosenLines[behaviorPriority[chosen]]), boost::regex("[^\t]+([\t]+<!-- [^ ]+ -->).*"), string("\\1"));
			out = chosenLines[behaviorPriority[chosen]].substr(0, chosenLines[behaviorPriority[chosen]].find(out));
		}

		return out;
	}

	string out = originalline;

	if (out.find("<!-- ") != NOT_FOUND)
	{
		out = boost::regex_replace(string(originalline), boost::regex("[^\t]+([\t]+<!-- [^ ]+ -->).*"), string("\\1"));
		out = originalline.substr(0, originalline.find(out));
	}

	return out;
}

vector<unique_ptr<registerAnimation>> openFile(getTemplate behaviortemplate)
{
	vector<unique_ptr<registerAnimation>> list;
	set<string> animPath;
	AAInitialize("alternate animation");
	DebugLogging("Reading new animations...");

	for (auto it = behaviortemplate.grouplist.begin(); it != behaviortemplate.grouplist.end(); ++it)
	{
		string path = behaviorPath[it->first];

		if (path.length() == 0)
		{
			ErrorMessage(1050, it->first);
			return list;
		}

		size_t pos = wordFind(path, "\\behaviors\\", true);
		size_t nextpos = wordFind(path, "\\data\\") + 5;

		if (pos != NOT_FOUND && nextpos != NOT_FOUND && nextpos < pos)
		{
			animPath.insert(path.substr(nextpos, pos - nextpos) + "\\");
		}
		else if (it->first != "animationdatasinglefile" && it->first != "animationsetdatasinglefile")
		{
			WarningMessage(1007, it->first, path);
		}
	}

	for (auto it = animPath.begin(); it != animPath.end(); ++it)
	{
#ifdef DEBUG
		string directory = "data\\" + *it;
#else
		string directory = skyrimDataPath->GetDataPath() + *it;
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

		for(auto& file1 : filelist1)
		{
			boost::filesystem::path FOF(animationDirectory + file1);

			if (boost::filesystem::is_directory(FOF))
			{
				read_directory(animationDirectory + file1, filelist2);

				for (auto& file2 : filelist2)
				{
					if (wordFind(file2, "_" + file1 + "_List.txt") != NOT_FOUND)
					{
						string fileToolName = file2.substr(0, file2.find("_", 0) + 1);
						string listword = file2.substr(file2.find_last_of("_"));
						string behaviorfile = fileToolName + file1 + "_Behavior.hkx";
						string modBehavior = directory + "Behaviors\\" + behaviorfile;
						DebugLogging("Reading animation mod: " + file1);

						if (fileToolName == "FNIS_" && listword == "_List.txt")
						{
							list.emplace_back(make_unique<registerAnimation>(animationDirectory + file1 + "\\", file2, behaviortemplate, modBehavior, behaviorfile));
						}
						else if (fileToolName == "Nemesis_" && listword == "_List.txt")
						{
							list.emplace_back(make_unique<registerAnimation>(animationDirectory + file1 + "\\", file2, behaviortemplate, modBehavior, behaviorfile, true));
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

	DebugLogging("Reading new animations complete");
	return list;
}

void newFileCheck(string directory, unordered_map<string, bool>& isChecked)
{
	vecstr filelist;
	boost::thread_group multithreads;
	read_directory(directory, filelist);

	try
	{
		for (auto& file : filelist)
		{
			string path = directory + "\\" + file;
			boost::to_lower(path);
			boost::filesystem::path curfile(path);

			if (boost::filesystem::is_directory(curfile))
			{
				multithreads.create_thread(boost::bind(newFileCheck, path, boost::ref(isChecked)));
			}
			else if (curfile.extension().string() == ".txt")
			{
				if (directory.find("animationdatasinglefile") != NOT_FOUND)
				{
					size_t pos = directory.find("\\") + 1;
					string modcode = directory.substr(pos, directory.find("\\", pos) - pos);

					if (file.find(modcode + "$") == NOT_FOUND)
					{
						if (!isChecked[path])
						{
							throw false;
						}
					}
				}
				else if (directory.find("animationsetdatasinglefile") != NOT_FOUND)
				{					
					if (boost::filesystem::path(directory).stem().string().find("~") != NOT_FOUND && file.length() > 0 && file[0] != '$')
					{
						if (!isChecked[path])
						{
							throw false;
						}
					}
				}
				else
				{
					if (isOnlyNumber(boost::regex_replace(string(file), boost::regex("#([^.txt]+).txt"), string("\\1"))))
					{
						if (!isChecked[path])
						{
							throw false;
						}
					}
				}
			}

			if (globalThrow)
			{
				break;
			}
		}
	}
	catch (bool& ex)
	{
		globalThrow = &ex;
	}

	multithreads.join_all();
}

bool isEngineUpdated()
{
	string directory = "temp_behaviors";
	vecstr filelist;

	read_directory(directory, filelist);

	if (filelist.size() < 3)
	{
		ErrorMessage(6006);
		return false;
	}

	vecstr storeline;
	string filename = "engine_update";
	unordered_map<string, bool> isChecked;

	if (!isFileExist(filename))
	{
		return false;
	}

	if (!GetFunctionLines(filename, storeline, false))
	{
		return false;
	}

	for (auto& line : storeline)
	{
		if (line.length() > 0)
		{
			if (line.find(" ") == NOT_FOUND)
			{
				ErrorMessage(2021);
				return false;
			}

			stringstream sstream(line);
			istream_iterator<string> ssbegin(sstream);
			istream_iterator<string> ssend;
			vecstr path(ssbegin, ssend);
			copy(path.begin(), path.end(), path.begin());

			if (path.size() == 3)
			{
				if (!isFileExist(path[0]))
				{
					return false;
				}
				else if (GetLastModified(path[0]) != path[1] + " " + path[2])
				{
					return false;
				}

				isChecked[path[0]] = true;
			}
			else if (path.size() > 3)
			{
				string pathline = "";

				for (unsigned int i = 0; i < path.size() - 2; ++i)
				{
					pathline = pathline + path[i] + " ";
				}

				pathline.pop_back();

				if (!isFileExist(pathline))
				{
					return false;
				}
				else if (GetLastModified(pathline) != path[path.size() - 2] + " " + path.back())
				{
					return false;
				}

				isChecked[pathline] = true;
			}
			else
			{
				ErrorMessage(2021);
				return false;
			}
		}
	}

	globalThrow = nullptr;
	boost::thread t1(boost::bind(newFileCheck, "mod", boost::ref(isChecked)));
	boost::thread t2(boost::bind(newFileCheck, "behavior templates", boost::ref(isChecked)));

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
	string filename = "behavior_path";

	if (isFileExist(filename))
	{
		string line;
		int linecount = 0;
		char charline[2000];
		shared_ptr<TextFile> pathFile = make_shared<TextFile>(filename);

		if (pathFile->GetFile())
		{
			while (fgets(charline, 2000, pathFile->GetFile()))
			{
				++linecount;
				line = charline;

				if (line.length() > 0 && line.back() == '\n')
				{
					line.pop_back();
				}

				if (line.find("=") != NOT_FOUND)
				{
					size_t pos = line.find("=");
					string file = line.substr(0, pos);
					string path = line.substr(pos + 1);
					behaviorPath[file] = path;
				}
				else
				{
					ErrorMessage(1067, filename, linecount);
					throw 1;
				}
			}
		}
		else
		{
			ErrorMessage(2000, filename);
			throw 1;
		}
	}
	else
	{
		ErrorMessage(1068, filename);
		throw 1;
	}
}

void GetBehaviorProject()
{
	string filename = "behavior_project";

	if (isFileExist(filename))
	{
		string characterfile;
		bool newChar = true;
		char charline[2000];
		shared_ptr<TextFile> pathFile = make_shared<TextFile>(filename);

		if (pathFile->GetFile())
		{
			while (fgets(charline, 2000, pathFile->GetFile()))
			{
				string line = charline;

				if (line.back() == '\n')
				{
					line.pop_back();
				}

				if (line.length() == 0)
				{
					newChar = true;
				}
				else
				{
					if (newChar)
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
		}
		else
		{
			ErrorMessage(2000, filename);
			throw 1;
		}
	}
	else
	{
		ErrorMessage(1068, filename);
		throw 1;
	}
}

void GetBehaviorProjectPath()
{
	string filename = "behavior_project_path";

	if (isFileExist(filename))
	{
		string line;
		int linecount = 0;
		char charline[2000];
		shared_ptr<TextFile> pathFile = make_shared<TextFile>(filename);

		if (pathFile->GetFile())
		{
			while (fgets(charline, 2000, pathFile->GetFile()))
			{
				++linecount;
				line = charline;

				if (line.length() > 0 && line.back() == '\n')
				{
					line.pop_back();
				}

				if (line.find("=") != NOT_FOUND)
				{
					size_t pos = line.find("=");
					string file = line.substr(0, pos);
					string path = line.substr(pos + 1);
					behaviorProjectPath[file] = path;
				}
				else
				{
					ErrorMessage(1067, filename, linecount);
					throw 1;
				}
			}
		}
		else
		{
			ErrorMessage(2000, filename);
			throw 1;
		}
	}
	else
	{
		ErrorMessage(1068, filename);
		throw 1;
	}
}

void GetAnimData()
{
	string filename = "animationdata_list";
	unordered_map<string, set<string>> characterHeaders;

	if (isFileExist(filename))
	{
		int linecount = 0;
		char charline[2000];
		shared_ptr<TextFile> pathFile = make_shared<TextFile>(filename);
		bool newCharacter = false;
		string character;
		string line;

		if (pathFile->GetFile())
		{
			while (fgets(charline, 2000, pathFile->GetFile()))
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
						ErrorMessage(3019);
						throw 1;
					}

					character = line;
					newCharacter = true;

					if (characterHeaders.find(character) != characterHeaders.end())
					{
						ErrorMessage(3010, character);
						throw 1;
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
						ErrorMessage(3008, character);
						throw 1;
					}
					
					characterHeaders[character].insert(line);
				}
			}
		}
		else
		{
			ErrorMessage(2000, filename);
			throw 1;
		}
	}
	else
	{
		ErrorMessage(1068, filename);
		throw 1;
	}
}

void characterHKX()
{
	string filename = "behavior_joints";

	if (isFileExist(filename))
	{
		bool open = false;
		char charline[2000];
		string line;
		string header;
		shared_ptr<TextFile> file = make_shared<TextFile>(filename);

		if (file->GetFile())
		{
			while (fgets(charline, 2000, file->GetFile()))
			{
				line = charline;

				if (line.back() == '\n')
				{
					line.pop_back();
				}

				if (line.length() != 0)
				{
					if (!open)
					{
						open = true;
						header = line;
					}
					else
					{
						if (header.length() == 0)
						{
							ErrorMessage(1094);
							return;
						}

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
			throw 1;
		}
	}
	else
	{
		ErrorMessage(1068, filename);
		throw 1;
	}
}

string GetFileName(string filepath)
{
	if (!isFileExist(filepath))
	{
		size_t nextpos;
		size_t lastpos;

		if (filepath.find("/") != NOT_FOUND)
		{
			if (filepath.find("\\") != NOT_FOUND)
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

		lastpos = filepath.find_last_of(".");

		if (lastpos == NOT_FOUND)
		{
			return filepath.substr(nextpos);
		}
		else
		{
			return filepath.substr(nextpos, lastpos - nextpos);
		}
	}
	else
	{
		return boost::filesystem::path(filepath).stem().string();
	}
}

string GetFileDirectory(string filepath)
{
	size_t nextpos;

	if (filepath.find("/") != NOT_FOUND)
	{
		if (filepath.find("\\") != NOT_FOUND)
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

	return filepath.substr(0, nextpos);
}

int getTemplateNextID(vecstr& templatelines)
{
	unordered_map<int, bool> taken;
	int IDUsed = 0;

	for (auto& line : templatelines)
	{
		if (line.find("<hkobject name=\"#MID$") != NOT_FOUND)
		{
			string number = boost::regex_replace(string(line), boost::regex(".*<hkobject name=\"#MID[$]([0-9]+)\" class=\".*"), string("\\1"));

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

			if (pos != NOT_FOUND && line.find("]", pos) != NOT_FOUND)
			{
				++IDUsed;
			}
		}
	}

	return IDUsed;
}

bool isEdited(getTemplate& BehaviorTemplate, string& lowerBehaviorFile, unordered_map<string, vector<shared_ptr<Furniture>>>& newAnimation, bool isCharacter, string modID)
{
	if (BehaviorTemplate.grouplist.find(lowerBehaviorFile) != BehaviorTemplate.grouplist.end() && BehaviorTemplate.grouplist[lowerBehaviorFile].size() > 0)
	{
		for (auto& templatecode : BehaviorTemplate.grouplist[lowerBehaviorFile])
		{
			if (newAnimation.find(templatecode) != newAnimation.end())
			{
				for (auto& curAnim : newAnimation[templatecode])
				{
					if (modID == curAnim->coreModID)
					{
						return true;
					}
				}
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
					for (auto& templatecode : it->second)
					{
						if (newAnimation.find(templatecode) != newAnimation.end() && !BehaviorTemplate.optionlist[templatecode].core)
						{
							for (auto& curAnim : newAnimation[templatecode])
							{
								if (!curAnim->isKnown())
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

bool newAnimSkip(vector<shared_ptr<Furniture>> newAnim, string modID)
{
	for (auto& anim : newAnim)
	{
		if (anim->coreModID == modID)
		{
			return false;
		}
	}

	return true;
}

void ClearGlobal(bool all)
{
	unordered_map<string, vecstr> emptySVSMap;

	if (all)
	{
		DebugLogging("Global reset all: TRUE");
		unordered_map<string, set<string>> emptySSSMap;
		usedAnim = emptySSSMap;

		unordered_map<string, unordered_map<string, bool>> emptySSBMap;
		registeredAnim = emptySSBMap;

		unordered_map<string, unordered_map<string, vector<set<string>>>> emptySSVSMap;
		animModMatch = emptySSVSMap;
		behaviorJoints = emptySVSMap;
	}
	else
	{
		DebugLogging("Global reset all: FALSE");
	}

	unordered_map<string, string> emptySSMap;
	behaviorProjectPath = emptySSMap;
	behaviorPath = emptySSMap;
	AAGroup = emptySSMap;
	
	behaviorProject = emptySVSMap;
	alternateAnim = emptySVSMap;
	groupAA = emptySVSMap;
	groupAAPrefix = emptySVSMap;
	AAEvent = emptySVSMap;
	AAHasEvent = emptySVSMap;

	vector<PCEA> emptyPCEAlist;
	pcealist = emptyPCEAlist;

	unordered_map<string, vector<PCEAData>> emptyPCEAData;
	animReplaced = emptyPCEAData;

	unordered_map<string, bool> emptySBMap;
	activatedBehavior = emptySBMap;
	
	unordered_map<string, unordered_map<string, int>> emptySSIMap;
	AAGroupCount = emptySSIMap;

	set<string> emptyVS;
	groupNameList = emptyVS;
}
