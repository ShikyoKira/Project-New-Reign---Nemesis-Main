#include <QtCore\QCoreApplication.h>

#include "version.h"
#include "filechecker.h"
#include "updateprocess.h"
#include "dataunification.h"

#include "functions\renew.h"

#include "utilities\Terminator.h"
#include "utilities\externalscript.h"

using namespace std;

extern bool processdone;
extern mutex processlock;
extern condition_variable cv;
extern Terminator* p_terminate;
extern atomic<int> m_RunningThread;
extern boost::atomic_flag atomic_lock;

void stateCheck(SSMap& c_parent, string parentID, string lowerbehaviorfile, string sID, SSSMap& stateID, SSMap& n_stateID, vecstr children, string filename, string ID,
	string modcode, unordered_map<string, map<string, unordered_map<string, set<string>>>>& duplicatedStateList);

class UpdateLock
{
	unordered_map<string, NodeU>& modUpdate;

public:
	UpdateLock(unordered_map<string, NodeU>& n_modUpdate) : modUpdate(n_modUpdate) {}

	NodeU& operator[](string key)
	{
		innerLock lock(atomic_lock);
		return modUpdate[key];
	}
};

struct arguPack
{
	arguPack(std::string n_directory, std::string n_modcode, vecstr n_behaviorfilelist, std::unordered_map<std::string, std::map<std::string, vecstr>>& n_newFile,
		MapChildState& n_childrenState, SSSMap& n_stateID, SSSMap& n_parent,
		StateIDList& n_modStateList, StateIDList& n_duplicatedStateList, MasterAnimData& n_animData, MasterAnimSetData& n_animSetData,
		std::unordered_map<std::string, std::string>& n_lastUpdate, std::shared_ptr<UpdateLock> n_modUpdate)
		: newFile(n_newFile), childrenState(n_childrenState), stateID(n_stateID), parent(n_parent), modStateList(n_modStateList), duplicatedStateList(n_duplicatedStateList),
		animData(n_animData), animSetData(n_animSetData), lastUpdate(n_lastUpdate)
	{
		directory = n_directory;
		modcode = n_modcode;
		behaviorfilelist = n_behaviorfilelist;
		modUpdate = n_modUpdate;
	}

	std::string directory;
	std::string modcode;
	vecstr behaviorfilelist;
	std::shared_ptr<UpdateLock> modUpdate;
	std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile;
	MapChildState& childrenState;
	SSSMap& stateID;
	SSSMap& parent;
	StateIDList& modStateList;
	StateIDList& duplicatedStateList;
	MasterAnimData& animData;
	MasterAnimSetData& animSetData;
	std::unordered_map<std::string, std::string>& lastUpdate;
};

UpdateFilesStart::UpdateFilesStart()
{
}

UpdateFilesStart::~UpdateFilesStart()
{
	if (!cmdline && error) error = false;
}

void UpdateFilesStart::UpdateFiles()
{
	try
	{
		try
		{
			string directory = "mod\\";
			string newAnimDirectory = "behavior templates\\";
			milestoneStart(directory);

			unordered_map<string, map<string, vecstr>> newFile;								// behavior file, node ID, node data lines; memory to access each node
			unordered_map<string, map<string, unordered_map<string, bool>>> childrenState;	// behavior file, node ID, children of state machine; ensure no state conflict

			SSSMap stateID;		// behavior file, children state machine info, state ID
			SSSMap parent;		// behavior file, children state machine info, parent state machine

			MasterAnimData animData;
			MasterAnimSetData animSetData;

			unordered_map<string, string> lastUpdate;

			ClearGlobal();

			// Check the existence of required files
			if (FileCheck(true))
			{
				RunScript("scripts\\update\\start\\");
				DebugLogging("External script run complete");

				// clear the temp_behaviors folder to prevent it from bloating
				ClearTempBehaviors();
				DebugLogging("Temp behavior clearance complete");

				// create "temp_behaviors" folder
				if (!isFileExist(directory)) boost::filesystem::create_directory(directory);

				emit progressUp();

				// copy latest vanilla into memory
				if (!error && VanillaUpdate(newFile, childrenState, stateID, parent, animData, animSetData))
				{
					if (!error)
					{
						DebugLogging("Data record complete");
						emit progressUp(); // 5

										   // check template for association with vanilla nodes from behavior template file
						if (newAnimUpdate(newAnimDirectory, newFile, animData, animSetData, lastUpdate))
						{
							DebugLogging("New Animations record complete");
							emit progressUp(); // 6

											   // comparing if different from mod file
							JoiningEdits(directory, newFile, childrenState, stateID, parent, animData, animSetData, lastUpdate);

							if (!error)
							{
								DebugLogging("Modification successfully extracted");
								emit progressUp();

								// compiling all behaviors in "data/meshes" to "temp_behaviors" folder
								CombiningFiles(newFile, animData, animSetData);

								emit progressUp();
							}
						}
					}
				}
			}

			ClearGlobal();

			if (!cmdline) this_thread::sleep_for(chrono::milliseconds(1500));
		}
		catch (exception& ex)
		{
			ErrorMessage(6001, ex.what());
		}
	}
	catch (nemesis::exception&)
	{
		// resolved exception
	}
	catch (...)
	{
		try
		{
			ErrorMessage(6001, "Unknown: Update Failed");
		}
		catch (nemesis::exception&)
		{
			// resolved exception
		}
	}

	unregisterProcess();
	p_terminate->exitSignal();
}

bool UpdateFilesStart::VanillaUpdate(unordered_map<string, map<string, vecstr>>& newFile, MapChildState& childrenState, SSSMap& stateID, SSSMap& parent,
	MasterAnimData& animData, MasterAnimSetData& animSetData)
{
#ifdef DEBUG
	string path = "data\\";
#else
	string path = nemesisInfo->GetDataPath() + "meshes";
#endif

	unordered_map<string, string> emptyPath;
	behaviorPath = emptyPath;

	if (!isFileExist(path))
	{
		boost::filesystem::create_directory(path);
		emit progressUp();
		emit progressUp();
		emit progressUp();		// 4
	}
	else
	{
		if (!GetPathLoop(path + "\\", &newFile, &childrenState, &stateID, &parent, &animData, &animSetData, false)) return false;

		emit progressUp();		// 4

		if (behaviorPath.size() != 0)
		{
			FileWriter output("cache\\behavior_path");

			if (output.is_open())
			{
				for (auto it = behaviorPath.begin(); it != behaviorPath.end(); ++it)
				{
					output << it->first + "=" + it->second + "\n";
				}
			}
			else
			{
				ErrorMessage(2009, "cache\\behavior_path");
			}
		}

		if (behaviorProject.size() != 0)
		{
			FileWriter output("cache\\behavior_project");

			if (output.is_open())
			{
				for (auto it = behaviorProject.begin(); it != behaviorProject.end(); ++it)
				{
					output << it->first + "\n";

					for (unsigned int i = 0; i < it->second.size(); ++i)
					{
						output << it->second[i] + "\n";
					}

					output << "\n";
				}
			}
			else
			{
				ErrorMessage(2009, "cache\\behavior_project");
			}
		}

		if (behaviorProjectPath.size() != 0)
		{
			FileWriter output("cache\\behavior_project_path");

			if (output.is_open())
			{
				for (auto it = behaviorProjectPath.begin(); it != behaviorProjectPath.end(); ++it)
				{
					output << it->first + "=" + it->second + "\n";
				}
			}
			else
			{
				ErrorMessage(2009, "cache\\behavior_project_path");
			}
		}
	}

	return true;
}

void UpdateFilesStart::GetFileLoop(string path)
{
	vecstr filelist;
	read_directory(path, filelist);

	for (auto& file : filelist)
	{
		string newPath = path + file;
		boost::filesystem::path curfile(newPath);

		if (!boost::filesystem::is_directory(curfile))
		{
			if (boost::iequals(curfile.extension().string(), ".xml") || boost::iequals(curfile.extension().string(), ".txt"))
			{
				string curFileName = curfile.stem().string();

				if ((boost::iequals(curFileName, "nemesis_animationdatasinglefile") || boost::iequals(curFileName, "nemesis_animationsetdatasinglefile")) ||
					(wordFind(curFileName, "Nemesis_") == 0 && wordFind(curFileName, "_List") == NOT_FOUND && wordFind(curFileName, "_Project") == NOT_FOUND) ||
					(wordFind(curFileName, "Nemesis_") == 0 && wordFind(curFileName, "_Project") + 8 == curFileName.length())) ++filenum;
			}
		}
		else
		{
			GetFileLoop(newPath + "\\");
		}
	}
}

bool UpdateFilesStart::GetPathLoop(string path, unordered_map<string, map<string, vecstr>>* newFile, MapChildState* childrenState, SSSMap* stateID, SSSMap* parent,
	MasterAnimData* animData, MasterAnimSetData* animSetData, bool isFirstPerson)
{
	vector<unique_ptr<std::thread>> multiThreads;

	try
	{
		try
		{
			try
			{
				vecstr filelist;
				read_directory(path, filelist);

				for (auto& file : filelist)
				{
					string newPath = path + file;
					boost::filesystem::path curfile(newPath);

					if (!boost::filesystem::is_directory(curfile))
					{
						if (boost::iequals(curfile.extension().string(), ".xml") || boost::iequals(curfile.extension().string(), ".txt"))
						{
							string curFileName = curfile.stem().string();

							if (boost::iequals(curFileName, "nemesis_animationdatasinglefile"))
							{
								curFileName = curFileName.substr(8);
								DebugLogging("AnimData Disassemble start (File: " + newPath + ")");

								if (!AnimDataDisassemble(newPath, *animData)) return false;

								DebugLogging("AnimData Disassemble complete (File: " + newPath + ")");
								string parent = curfile.parent_path().filename().string();
								newPath = path + file.substr(8);
								boost::algorithm::to_lower(parent);
								boost::algorithm::to_lower(newPath);
								boost::algorithm::to_lower(curFileName);
								behaviorPath[curFileName] = newPath.substr(0, newPath.find_last_of("."));
								emit progressUp();
							}
							else if (boost::iequals(curFileName, "nemesis_animationsetdatasinglefile"))
							{
								curFileName = curFileName.substr(8);
								DebugLogging("AnimSetData Disassemble start (File: " + newPath + ")");

								if (!AnimSetDataDisassemble(newPath, *animSetData)) return false;

								DebugLogging("AnimSetData Disassemble complete (File: " + newPath + ")");
								string parent = curfile.parent_path().filename().string();
								newPath = path + file.substr(8);
								boost::algorithm::to_lower(parent);
								boost::algorithm::to_lower(newPath);
								boost::algorithm::to_lower(curFileName);
								behaviorPath[curFileName] = newPath.substr(0, newPath.find_last_of("."));
								emit progressUp();
							}
							else if (wordFind(curFileName, "Nemesis_") == 0 && wordFind(curFileName, "_List") == NOT_FOUND && wordFind(curFileName, "_Project") == NOT_FOUND)
							{
								string firstperson = "";

								if (isFirstPerson) firstperson = "_1stperson\\";

								curFileName = firstperson + curFileName.substr(8);
								string smallfile = boost::to_lower_copy(curFileName);
								DebugLogging("Behavior Disassemble start (File: " + newPath + ")");

								if (!VanillaDisassemble(newPath, smallfile, &(*newFile)[smallfile], *childrenState, (*stateID)[smallfile], (*parent)[smallfile])) return false;

								DebugLogging("Behavior Disassemble complete (File: " + newPath + ")");
								string parent = curfile.parent_path().filename().string();
								newPath = path + file.substr(8);
								boost::algorithm::to_lower(parent);
								boost::algorithm::to_lower(newPath);
								boost::algorithm::to_lower(curFileName);
								behaviorPath[curFileName] = newPath.substr(0, newPath.find_last_of("."));
								emit progressUp();

								if (parent.find("characters") == 0)
								{
									unordered_map<string, bool> empty;
									registeredAnim[boost::algorithm::to_lower_copy(curFileName)] = empty;
								}
							}
							else if (wordFind(curFileName, "Nemesis_") == 0 && wordFind(curFileName, "_Project") + 8 == curFileName.length())
							{
								string firstperson = "";

								if (isFirstPerson) firstperson = "_1stperson\\";

								string curPath = boost::to_lower_copy(path);
								curPath = curPath.substr(curPath.find("\\meshes\\") + 1);
								curPath.pop_back();
								curFileName = firstperson + curFileName.substr(8, curFileName.length() - 16);
								behaviorProjectPath[boost::to_lower_copy(curFileName)] = curPath;
								vecstr storeline;
								bool record = false;
								DebugLogging("Nemesis Project Record start (File: " + newPath + ")");

								if (!GetFunctionLines(newPath, storeline)) return false;

								for (unsigned int j = 0; j < storeline.size(); ++j)
								{
									string line = storeline[j];

									if (record && line.find("</hkparam>") != NOT_FOUND) break;

									if (record)
									{
										if (line.find("<hkcstring>") == NOT_FOUND) ErrorMessage(1093, newPath, j + 1);

										int pos = line.find("<hkcstring>") + 11;
										string characterfile = boost::to_lower_copy(line.substr(pos, line.find("</hkcstring>", pos) - pos));
										characterfile = GetFileName(characterfile);
										behaviorProject[characterfile].push_back(boost::to_lower_copy(curFileName));
									}

									if (line.find("<hkparam name=\"characterFilenames\" numelements=\"") != NOT_FOUND && line.find("</hkparam>") == NOT_FOUND) record = true;
								}

								emit progressUp();
								DebugLogging("Nemesis Project Record complete (File: " + newPath + ")");
							}
						}
					}
					else
					{
						if (boost::iequals(file, "_1stperson"))
						{
							// look deeper into the folder for behavior file
							multiThreads.emplace_back(make_unique<std::thread>(&UpdateFilesStart::GetPathLoop, this, newPath + "\\", newFile, childrenState, stateID, parent,
								animData, animSetData, true));
						}
						else
						{
							// look deeper into the folder for behavior file
							multiThreads.emplace_back(make_unique<std::thread>(&UpdateFilesStart::GetPathLoop, this, newPath + "\\", newFile, childrenState, stateID, parent,
								animData, animSetData, isFirstPerson));
						}
					}

					if (error) throw nemesis::exception();
				}
			}
			catch (exception& ex)
			{
				ErrorMessage(6001, ex.what());
			}
		}
		catch (nemesis::exception&)
		{
			// resolved exception
		}
	}
	catch (...)
	{
		try
		{
			ErrorMessage(6001, "Unknown: GetPathLoop");
		}
		catch (nemesis::exception&)
		{
			// resolved exception
		}
	}

	if (multiThreads.size() > 0)
	{
		for (unique_ptr<std::thread>& thrd : multiThreads)
		{
			thrd->join();
		}
	}

	return true;
}

bool UpdateFilesStart::VanillaDisassemble(string path, string filename, map<string, vecstr>* newFile, MapChildState& childrenState, SSMap& stateID, SSMap& parent)
{
	vecstr storeline;
	storeline.reserve(2000);

	FileReader vanillafile(path);
	string curID;

	unordered_map<string, vecstr> statelist;		// parent ID, list of children

	if (vanillafile.GetFile())
	{
		bool skip = true;
		bool start = false;
		bool isSM = false;
		string curline;

		while (vanillafile.GetLines(curline))
		{
			if (curline.find("	</hksection>") != NOT_FOUND) break;

			if (!skip)
			{
				if (curline.find("SERIALIZE_IGNORED") == NOT_FOUND)
				{
					bool isVector4 = false;

					if (storeline.size() > 0 && storeline.back().find("numelements=\"") != NOT_FOUND &&
						storeline.back().find("</hkparam>", storeline.back().find("numelements=\"")) == NOT_FOUND)
					{
						if (curline.find("			#") != NOT_FOUND)
						{
							start = true;
						}
						else
						{
							bool check = true;

							for (auto& each : curline)
							{
								if (each != '\t' && each != '.' && each != '-' && each != ' ' && !isdigit(each))
								{
									check = false;
									break;
								}
							}

							if (check) start = true;
						}
					}
					else
					{
						size_t pos = curline.find("\">(");

						if (pos != NOT_FOUND && curline.find("<hkparam name=\"", 0) != NOT_FOUND)
						{
							pos += 3;
							char curchr = curline[pos];
							size_t pos2 = curline.find(")</hkparam>", pos);

							if ((isdigit(curchr) || curchr == '-') && pos2 != NOT_FOUND)
							{
								bool check = true;
								string test = curline.substr(pos, pos2 - pos);

								for (auto& each : test)
								{
									if (each != '\t' && each != '.' && each != '-' && each != ' ' && !isdigit(each))
									{
										check = false;
										break;
									}
								}

								if (check)
								{
									isVector4 = true;
									start = true;
								}
							}
						}
						else if (start && curline.find("</hkparam>") != NOT_FOUND)
						{
							start = false;
						}
					}

					if (curline.find("<hkparam name=\"stateId\">") != NOT_FOUND)
					{
						string stateIDStr = boost::regex_replace(string(curline), boost::regex(".*<hkparam name=\"stateId\">([0-9]+)</hkparam>.*"), string("\\1"));

						if (stateIDStr != curline) stateID[curID] = stateIDStr;
					}

					if (start)
					{
						if (curline.find("			#") != NOT_FOUND)
						{
							vecstr curElements;
							string temp = curline.substr(0, curline.find("#"));
							string spaces = string(count(temp.begin(), temp.end(), '\t'), '\t');
							StringSplit(curline, curElements);

							if (isSM)
							{
								for (auto& element : curElements)
								{
									storeline.push_back(spaces + element);
									statelist[curID].push_back(element);
									parent[element] = curID;
								}
							}
							else
							{
								for (auto& element : curElements)
								{
									storeline.push_back(spaces + element);
								}
							}
						}
						else
						{
							bool bone = false;
							boost::regex vector4("\\(((?:-|)[0-9]+\\.[0-9]+) ((?:-|)[0-9]+\\.[0-9]+) ((?:-|)[0-9]+\\.[0-9]+) ((?:-|)[0-9]+\\.[0-9]+)\\)");
							boost::smatch match;

							if (!boost::regex_search(curline, match, vector4))
							{
								string spaces;

								for (auto& ch : curline)
								{
									if (ch == '\t') spaces.push_back(ch);
									else break;
								}

								if (curline.find("<!-- Bone$N -->") == NOT_FOUND)
								{
									for (auto& it = boost::sregex_iterator(curline.begin(), curline.end(), boost::regex("([0-9]+(\\.[0-9]+)?)"));
										it != boost::sregex_iterator(); ++it)
									{
										storeline.push_back(spaces + it->str(1));
										bone = true;
									}
								}
							}
							else if (isVector4)
							{
								bone = true;
								string spaces = "\t";

								for (auto& ch : curline)
								{
									if (ch == '\t') spaces.push_back(ch);
									else break;
								}

								storeline.push_back(curline.substr(0, match.position()));

								for (auto& it = boost::sregex_iterator(curline.begin(), curline.end(), vector4); it != boost::sregex_iterator(); ++it)
								{
									storeline.push_back(spaces + it->str(1));
									storeline.push_back(spaces + it->str(2));
									storeline.push_back(spaces + it->str(3));
									storeline.push_back(spaces + it->str(4));
								}

								spaces.pop_back();
								storeline.push_back(spaces + curline.substr(match.position() + match.str().length()));
								start = false;
							}
							else
							{
								bone = true;
								string spaces;

								for (auto& ch : curline)
								{
									if (ch == '\t') spaces.push_back(ch);
									else break;
								}

								for (auto& it = boost::sregex_iterator(curline.begin(), curline.end(), vector4); it != boost::sregex_iterator(); ++it)
								{
									storeline.push_back(spaces + it->str(1));
									storeline.push_back(spaces + it->str(2));
									storeline.push_back(spaces + it->str(3));
									storeline.push_back(spaces + it->str(4));
								}
							}

							if (!bone) storeline.push_back(curline);
						}
					}
					else
					{
						if (curline.find("<hkobject name=\"", 0) != NOT_FOUND && curline.find("signature=\"", curline.find("<hkobject name=\"")) != NOT_FOUND)
						{
							if (curline.find("class=\"hkbStateMachine\" signature=\"") != NOT_FOUND) isSM = true;
							else isSM = false;

							if (storeline.size() != 0 && curID.length() != 0)
							{
								storeline.shrink_to_fit();

								while (atomic_lock.test_and_set(boost::memory_order_acquire));

								try
								{
									(*newFile)[curID] = storeline;
								}
								catch (exception& ex)
								{
									atomic_lock.clear(boost::memory_order_release);
									throw ex;
								}

								atomic_lock.clear(boost::memory_order_release);

								storeline.reserve(2000);
								storeline.clear();
							}

							size_t pos = curline.find("<hkobject name=\"#") + 16;
							curID = curline.substr(pos, curline.find("\" class=\"", curline.find("<hkobject name=\"")) - pos);
						}

						storeline.push_back(curline);
					}
				}
			}
			else if (curline.find("<hkobject name=\"#") != NOT_FOUND)
			{
				skip = false;
				size_t pos = curline.find("<hkobject name=\"#") + 16;
				curID = curline.substr(pos, curline.find("\" class=\"", curline.find("<hkobject name=\"")) - pos);
				storeline.push_back(curline);
			}
		}

		if (storeline.size() != 0 && curID.length() != 0)
		{
			storeline.shrink_to_fit();

			while (atomic_lock.test_and_set(boost::memory_order_acquire));

			try
			{
				(*newFile)[curID] = storeline;
			}
			catch (exception& ex)
			{
				atomic_lock.clear(boost::memory_order_release);
				throw ex;
			}

			atomic_lock.clear(boost::memory_order_release);
		}
	}
	else
	{
		ErrorMessage(2000, path);
	}

	for (auto& state : statelist)
	{
		for (auto& ID : state.second)
		{
			if (stateID[ID].empty()) ErrorMessage(1187);

			childrenState[filename][state.first][stateID[ID]] = true;
		}
	}

	return true;
}

bool UpdateFilesStart::AnimDataDisassemble(string path, MasterAnimData& animData)
{
	int num;
	vecstr newline;
	vecstr storeline;
	unordered_map<string, int> projectNameCount;

	int projectcounter = 1;

	string project;
	string header;

	bool special = false;
	bool isInfo = false;
	bool end = false;
	bool out = true;

	if (!GetFunctionLines(path, storeline)) return false;

	{
		string strnum = boost::regex_replace(string(storeline[0]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

		if (!isOnlyNumber(strnum) || stoi(strnum) < 10) ErrorMessage(3014);

		num = stoi(strnum) + 1;
	}

	header = "$header$";

	{
		vecstr prjlist;
		vecstr charlist;
		charlist.push_back(header);

		for (int i = 1; i < num; ++i)
		{
			prjlist.push_back(storeline[i]);
			charlist.push_back(storeline[i] + "~" + to_string(++projectNameCount[storeline[i]]));
		}

		animData.animDataChar = charlist;
		animData.newAnimData[header][header] = prjlist;
	}

	project = animData.animDataChar[1];
	animData.animDataHeader[header].push_back(header);
	animData.animDataHeader[project].push_back(header);
	newline.reserve(20);
	newline.clear();

	for (unsigned int i = num; i < storeline.size(); ++i)
	{
		string line = storeline[i];

		if (i + 3 < storeline.size() && i > 2)
		{
			if (storeline[i - 1] == "")
			{
				if (isOnlyNumber(line) && isOnlyNumber(storeline[i + 1]) && isOnlyNumber(storeline[i + 2]))
				{
					if (storeline[i + 2] == "0" || (i + 3 < storeline.size() && storeline[i + 3].find("\\") != NOT_FOUND))
					{
						newline.shrink_to_fit();
						animData.newAnimData[project][header] = newline;
						project = animData.animDataChar[++projectcounter];
						header = "$header$";
						animData.animDataHeader[project].push_back(header);
						newline.reserve(20);
						newline.clear();
						isInfo = false;
						out = true;
					}
				}

				if (!out)
				{
					if (!isInfo)
					{
						if (hasAlpha(line))
						{
							if (isOnlyNumber(storeline[i + 1]))
							{
								newline.shrink_to_fit();
								animData.newAnimData[project][header] = newline;
								newline.reserve(20);
								newline.clear();
								header = line + "~" + storeline[i + 1];
								animData.animDataHeader[project].push_back(header);
							}
						}
						else if (isOnlyNumber(line))
						{
							isInfo = true;
							newline.shrink_to_fit();
							animData.newAnimData[project][header] = newline;;
							newline.reserve(20);
							newline.clear();
							header = storeline[++i];
							line = storeline[i];
							animData.animDataInfo[project].push_back(header);
						}
					}
					else if (isOnlyNumber(line))
					{
						newline.shrink_to_fit();
						animData.newAnimData[project][header] = newline;
						newline.reserve(20);
						newline.clear();
						header = line;
						animData.animDataInfo[project].push_back(header);
					}
				}
			}
			else if (header == "$header$")
			{
				if (hasAlpha(line) && line.find("\\") == NOT_FOUND && i + 1 < storeline.size())
				{
					if (isOnlyNumber(storeline[i + 1]))	// if it is unique code
					{
						newline.shrink_to_fit();
						animData.newAnimData[project][header] = newline;
						newline.reserve(20);
						newline.clear();
						header = line + "~" + storeline[i + 1];
						animData.animDataHeader[project].push_back(header);
					}
				}
				else if (isOnlyNumber(storeline[i - 1]) && storeline[i - 1] == "0" && isOnlyNumber(line) && i + 3 < storeline.size())
				{
					if (isOnlyNumber(storeline[i + 1]) && isOnlyNumber(storeline[i + 2]))
					{
						if (storeline[i + 2] == "0" || storeline[i + 3].find("\\") != NOT_FOUND)
						{
							newline.shrink_to_fit();
							animData.newAnimData[project][header] = newline;
							project = animData.animDataChar[++projectcounter];
							animData.animDataHeader[project].push_back(header);
							newline.reserve(20);
							newline.clear();
							isInfo = false;
							out = true;
						}
					}
				}
			}
		}

		if (!out) newline.push_back(line);

		out = false;
	}

	if (newline.size() != 0)
	{
		if (newline.back().length() == 0) newline.pop_back();

		newline.shrink_to_fit();
		animData.newAnimData[project][header] = newline;
	}

	return true;
}

bool UpdateFilesStart::AnimSetDataDisassemble(string path, MasterAnimSetData& animSetData)
{
	vecstr storeline;
	int num;
	vecstr newline;
	newline.reserve(500);

	if (!GetFunctionLines(path, storeline)) return false;

	{
		string strnum = boost::regex_replace(string(storeline[0]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

		if (!isOnlyNumber(strnum) || stoi(strnum) < 10) ErrorMessage(3014);

		num = stoi(strnum) + 1;
	}

	unordered_map<string, vecstr> animDataSetHeader;
	string project = "$header$";
	string header = project;
	int projectcounter = 1;
	int headercounter = 0;
	bool special = false;
	bool isInfo = false;
	animSetData.projectList.push_back(project);
	animDataSetHeader[project].push_back(header);

	for (int i = 1; i < num; ++i)
	{
		newline.push_back(storeline[i]);
		animSetData.projectList.push_back(boost::to_lower_copy(storeline[i]));
	}

	for (unsigned int i = num; i < storeline.size(); ++i)
	{
		if (i != storeline.size() - 1 && wordFind(storeline[i + 1], ".txt") != NOT_FOUND)
		{
			header = animDataSetHeader[project][headercounter];
			newline.shrink_to_fit();
			animSetData.newAnimSetData[project][header] = newline;
			newline.reserve(100);
			newline.clear();
			project = animSetData.projectList[projectcounter];
			++projectcounter;
			headercounter = 0;
			animDataSetHeader[project].push_back("$header$");
			newline.push_back(storeline[i]);
			++i;

			if (animDataSetHeader[project].size() != 1) ErrorMessage(5005, path, i + 1);

			while (i < storeline.size())
			{
				if (wordFind(storeline[i], ".txt") != NOT_FOUND)
				{
					animDataSetHeader[project].push_back(GetFileName(boost::algorithm::to_lower_copy(storeline[i])));
				}
				else if (wordFind(storeline[i], "V3") != NOT_FOUND)
				{
					header = boost::to_lower_copy(animDataSetHeader[project][headercounter]);
					++headercounter;
					newline.shrink_to_fit();
					animSetData.newAnimSetData[project][header] = newline;
					newline.reserve(100);
					newline.clear();
					break;
				}
				else
				{
					ErrorMessage(5020, path, i + 1);
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
			animSetData.newAnimSetData[project][header] = newline;
			newline.reserve(100);
			newline.clear();
		}

		newline.push_back(storeline[i]);
	}

	if (newline.size() != 0)
	{
		if (newline.back().length() == 0) newline.pop_back();

		header = animDataSetHeader[project][headercounter];
		newline.shrink_to_fit();
		animSetData.newAnimSetData[project][header] = newline;
	}

	return true;
}

void UpdateFilesStart::SeparateMod(unique_ptr<arguPack> pack)
{
	try
	{
		try
		{
			string directory = pack->directory, modcode = pack->modcode;
			vecstr behaviorfilelist = pack->behaviorfilelist;
			unordered_map<string, map<string, vecstr>>& newFile(pack->newFile);
			unordered_map<string, map<string, unordered_map<string, bool>>>& childrenState(pack->childrenState);
			SSSMap& stateID(pack->stateID);
			SSSMap& parent(pack->parent);
			StateIDList& modStateList(pack->modStateList);
			StateIDList& duplicatedStateList(pack->duplicatedStateList);
			MasterAnimData& animData(pack->animData);
			MasterAnimSetData& animSetData(pack->animSetData);
			unordered_map<string, string>& lastUpdate(pack->lastUpdate);
			shared_ptr<UpdateLock> modUpdate(pack->modUpdate);

			directory = pack->directory;
			modcode = pack->modcode;
			DebugLogging("Installing Mod: " + modcode);

			for (auto& behaviorfile : behaviorfilelist)
			{
				boost::filesystem::path curPath(directory + modcode + "\\" + behaviorfile);

				if (boost::filesystem::is_directory(curPath))
				{
					vecstr nodelist;
					read_directory(curPath.string(), nodelist);
					emit progressUp();

					if (boost::iequals(behaviorfile, "animationdatasinglefile"))
					{
						if (animData.newAnimData.size() == 0) ErrorMessage(3017, "nemesis_animationdatasinglefile.txt");

						for (string& node : nodelist)
						{
							boost::filesystem::path curPath(directory + modcode + "\\" + behaviorfile + "\\" + node);

							if (boost::filesystem::is_directory(curPath))
							{
								bool newChar = false;
								bool openAnim = false;
								bool openInfo = false;
								string projectname = node;
								projectname.replace(projectname.find_last_of("~"), 0, ".txt");

								if (animData.newAnimData.find(projectname) == animData.newAnimData.end())
								{
									animData.animDataChar.push_back(projectname);
									newChar = true;
								}

								vecstr uniquecodelist;
								string filepath = directory + modcode + "\\" + behaviorfile + "\\" + node;
								read_directory(filepath, uniquecodelist);

								for (string& uniquecode : uniquecodelist)
								{
									if (!boost::filesystem::is_directory(filepath + "\\" + uniquecode))
									{
										AnimDataUpdate(modcode, behaviorfile, projectname, filepath + "\\" + uniquecode, ref(animData), ref(newChar), ref(lastUpdate),
											ref(openAnim), ref(openInfo));

										if (error) throw nemesis::exception();
									}
								}

								if (openAnim) animData.newAnimData[projectname][animData.animDataHeader[projectname].back()].push_back("<!-- CLOSE -->");

								if (openInfo) animData.newAnimData[projectname][animData.animDataInfo[projectname].back()].push_back("<!-- CLOSE -->");

								if (newChar)
								{
									if (animData.animDataHeader[projectname].size() == 0)
									{
										animData.animDataChar.pop_back();
										animData.newAnimData.erase(projectname);
										animData.animDataHeader.erase(projectname);
										animData.animDataInfo.erase(projectname);
									}
									else
									{
										vecstr header;
										vecstr infoheader;

										for (auto& it : animData.newAnimData[projectname])
										{
											if (!boost::iequals(it.first, "$header$"))
											{
												if (isOnlyNumber(it.first)) infoheader.push_back(it.first);
												else header.push_back(it.first);
											}
										}

										animData.newAnimData[projectname]["$header$"].insert(animData.newAnimData[projectname]["$header$"].begin(),
											"<!-- NEW *" + modcode + "* -->");
										animData.animDataHeader[projectname].clear();

										if (header.size() > 0)
										{
											animData.animDataHeader[projectname].reserve(header.size());
											animData.animDataHeader[projectname].insert(animData.animDataHeader[projectname].end(), header.begin(), header.end());
										}
										else
										{
											animData.newAnimData[projectname]["$header$"].push_back("<!-- CLOSE -->");
										}

										if (infoheader.size() > 0)
										{
											animData.animDataInfo[projectname].reserve(infoheader.size());
											animData.animDataInfo[projectname].insert(animData.animDataInfo[projectname].end(), infoheader.begin(), infoheader.end());
											animData.newAnimData[projectname][animData.animDataInfo[projectname].back()].push_back("<!-- CLOSE -->");
										}
										else
										{
											animData.newAnimData[projectname][animData.animDataHeader[projectname].back()].push_back("<!-- CLOSE -->");
										}
									}
								}
							}
						}
					}
					else if (boost::iequals(behaviorfile, "animationsetdatasinglefile"))
					{
						if (animSetData.newAnimSetData.size() == 0) ErrorMessage(3017, "nemesis_animationsetdatasinglefile.txt");

						for (string& node : nodelist)
						{
							boost::filesystem::path curPath2(curPath.string() + "\\" + node);

							if (boost::filesystem::is_directory(curPath2) && node.find("~") != NOT_FOUND)
							{
								bool newProject = false;
								string lowerproject = node + ".txt";
								boost::to_lower(lowerproject);

								while (lowerproject.find("~") != NOT_FOUND)
								{
									lowerproject.replace(lowerproject.find("~"), 1, "\\");
								}

								if (animSetData.newAnimSetData.find(lowerproject) == animSetData.newAnimSetData.end())
								{
									animSetData.projectList.push_back(lowerproject);
									newProject = true;
								}

								vecstr uniquecodelist;
								string filepath = curPath2.string();
								read_directory(filepath, uniquecodelist);

								for (string& uniquecode : uniquecodelist)
								{
									AnimSetDataUpdate(modcode, behaviorfile, node, lowerproject, filepath + "\\" + uniquecode, ref(animSetData), newProject, ref(lastUpdate));

									if (error) throw nemesis::exception();
								}

								if (newProject)
								{
									animSetData.newAnimSetData[lowerproject].begin()->second.insert(animSetData.newAnimSetData[lowerproject].begin()->second.begin(),
										"<!-- NEW *" + modcode + "* -->");
									animSetData.newAnimSetData[lowerproject].rbegin()->second.push_back("<!-- CLOSE -->");
								}
							}
						}
					}
					else if (boost::iequals(behaviorfile, "_1stperson"))
					{
						for (string& fpbehavior : nodelist)
						{
							SSMap n_stateID;
							unordered_map<string, vecstr> statelist;
							unordered_map<string, bool> skipped;
							vecstr fpnodelist;
							read_directory(curPath.string() + "\\" + fpbehavior, fpnodelist);
							string lowerbehaviorfile = boost::to_lower_copy(behaviorfile + "\\" + fpbehavior);
							SSMap& c_parent = parent[lowerbehaviorfile];
							int num = 1;
							string cursize = to_string(fpnodelist.size());

							for (auto& fpnode : fpnodelist)
							{
								(*modUpdate)[lowerbehaviorfile + fpnode.substr(0, fpnode.find_last_of("."))].FunctionUpdate(modcode, lowerbehaviorfile, fpnode, ref(newFile),
									n_stateID, c_parent, statelist, ref(lastUpdate));

								if (error) throw nemesis::exception();
							}

							for (auto& state : statelist)
							{
								for (string& ID : state.second)		// state machine info node ID
								{
									string filename = curPath.string() + "\\" + fpbehavior + "\\" + ID + ".txt";

									if (n_stateID.find(ID) != n_stateID.end())
									{
										string sID = n_stateID[ID];
										skipped[ID] = true;

										if (sID.empty()) ErrorMessage(1188, modcode, filename);

										if (stateID[lowerbehaviorfile][ID] != sID)
										{
											if (childrenState[lowerbehaviorfile][state.first][sID])
											{
												stateCheck(c_parent, state.first, lowerbehaviorfile, sID, stateID, n_stateID, state.second, filename, ID, modcode,
													duplicatedStateList);
											}
											else if (modStateList[lowerbehaviorfile][state.first][sID].size() > 0)
											{
												for (auto& modname : modStateList[lowerbehaviorfile][state.first][sID])
												{
													duplicatedStateList[filename][ID][sID].insert(modname);
												}

												duplicatedStateList[filename][ID][sID].insert(modcode);
												modStateList[lowerbehaviorfile][state.first][sID].insert(modcode);
											}
											else
											{
												modStateList[lowerbehaviorfile][state.first][sID].insert(modcode);
											}
										}
									}
									else if (ID.find("$") != NOT_FOUND)
									{
										ErrorMessage(1190, modcode, ID);
									}
								}

								if (error) throw nemesis::exception();
							}

							for (auto& ID : n_stateID)
							{
								if (!skipped[ID.first])
								{
									bool skip = false;
									string parentID = c_parent[ID.first];
									string filename = curPath.string() + "\\" + fpbehavior + "\\" + ID.first + ".txt";

									if (parentID.empty())
									{
										if (ID.first.find("$") != NOT_FOUND) skip = true;
										else ErrorMessage(1191, modcode, ID.first);
									}

									if (!skip)
									{
										if (stateID[lowerbehaviorfile][ID.first] != ID.second)
										{
											if (childrenState[lowerbehaviorfile][parentID][ID.second])
											{
												stateCheck(c_parent, parentID, lowerbehaviorfile, ID.second, stateID, n_stateID, statelist[parentID], filename, ID.first,
													modcode, duplicatedStateList);
											}
											else if (modStateList[lowerbehaviorfile][parentID][ID.second].size() > 0)
											{
												for (auto& modname : modStateList[lowerbehaviorfile][parentID][ID.second])
												{
													duplicatedStateList[filename][ID.first][ID.second].insert(modname);
												}

												duplicatedStateList[filename][ID.first][ID.second].insert(modcode);
												modStateList[lowerbehaviorfile][parentID][ID.second].insert(modcode);
											}
											else
											{
												modStateList[lowerbehaviorfile][parentID][ID.second].insert(modcode);
											}
										}
									}
								}

								if (error) throw nemesis::exception();
							}
						}
					}
					else
					{
						SSMap n_stateID;
						unordered_map<string, vecstr> statelist;
						unordered_map<string, bool> skipped;
						string lowerbehaviorfile = boost::to_lower_copy(behaviorfile);
						SSMap& c_parent = parent[lowerbehaviorfile];
						int num = 1;
						string cursize = to_string(nodelist.size());

						for (string& node : nodelist)
						{
							if (!(*modUpdate)[lowerbehaviorfile + node.substr(0, node.find_last_of("."))].FunctionUpdate(modcode, lowerbehaviorfile, node, ref(newFile),
								n_stateID, c_parent, statelist, ref(lastUpdate)))
							{
								if (error) throw nemesis::exception();
							}
						}

						for (auto& state : statelist)
						{
							for (string& ID : state.second)		// state machine info node ID
							{
								string filename = curPath.string() + "\\" + ID + ".txt";

								if (n_stateID.find(ID) != n_stateID.end())
								{
									string sID = n_stateID[ID];
									skipped[ID] = true;

									if (sID.empty()) ErrorMessage(1188, modcode, filename);

									if (childrenState[lowerbehaviorfile][state.first][sID])
									{
										stateCheck(c_parent, state.first, lowerbehaviorfile, sID, stateID, n_stateID, state.second, filename, ID, modcode,
											duplicatedStateList);
									}
									else if (modStateList[lowerbehaviorfile][state.first][sID].size() > 0)
									{
										for (auto& modname : modStateList[lowerbehaviorfile][state.first][sID])
										{
											duplicatedStateList[filename][ID][sID].insert(modname);
										}

										duplicatedStateList[filename][ID][sID].insert(modcode);
										modStateList[lowerbehaviorfile][state.first][sID].insert(modcode);
									}
									else
									{
										modStateList[lowerbehaviorfile][state.first][sID].insert(modcode);
									}
								}
								else if (ID.find("$") != NOT_FOUND)
								{
									ErrorMessage(1190, modcode, ID);
								}
							}
						}

						for (auto& ID : n_stateID)
						{
							if (!skipped[ID.first])
							{
								bool skip = false;
								string parentID = c_parent[ID.first];
								string filename = curPath.string() + "\\" + ID.first + ".txt";

								if (parentID.empty())
								{
									if (ID.first.find("$") != NOT_FOUND) skip = true;
									else ErrorMessage(1191, modcode, ID.first);
								}

								if (!skip)
								{
									if (stateID[lowerbehaviorfile][ID.first] != ID.second)
									{
										if (childrenState[lowerbehaviorfile][parentID][ID.second])
										{
											stateCheck(c_parent, parentID, lowerbehaviorfile, ID.second, stateID, n_stateID, statelist[parentID], filename, ID.first,
												modcode, duplicatedStateList);
										}
										else if (modStateList[lowerbehaviorfile][parentID][ID.second].size() > 0)
										{
											for (auto& modname : modStateList[lowerbehaviorfile][parentID][ID.second])
											{
												duplicatedStateList[filename][ID.first][ID.second].insert(modname);
											}

											duplicatedStateList[filename][ID.first][ID.second].insert(modcode);
											modStateList[lowerbehaviorfile][parentID][ID.second].insert(modcode);
										}
										else
										{
											modStateList[lowerbehaviorfile][parentID][ID.second].insert(modcode);
										}
									}
								}
							}
						}
					}
				}
			}

			DebugLogging("Mod Installed sucessfully: " + modcode);
		}
		catch (exception& ex)
		{
			ErrorMessage(6001, ex.what());
		}
	}
	catch (nemesis::exception&) {}
}

void UpdateFilesStart::JoiningEdits(string directory, unordered_map<string, map<string, vecstr>>& newFile, MapChildState& childrenState, SSSMap& stateID, SSSMap& parent,
	MasterAnimData& animData, MasterAnimSetData& animSetData, unordered_map<string, string>& lastUpdate)
{
	try
	{
		try
		{
			if (isFileExist(directory))
			{
				vecstr filelist;
				vector<unique_ptr<std::thread>> multiThreads;
				read_directory(directory, filelist);

				StateIDList modStateList;				// behavior file, SM ID, state ID, list of mods
				StateIDList duplicatedStateList;		// behavior file, SM ID, state ID, list of conflicting mods
				unordered_map<string, NodeU> modUpdate;
				shared_ptr<UpdateLock> modUpPtr = make_shared<UpdateLock>(modUpdate);

				for (unsigned int i = 0; i < filelist.size(); ++i)
				{
					boost::filesystem::path curPath(directory + filelist[i]);

					if (boost::filesystem::is_directory(curPath))
					{
						vecstr filelist2;
						read_directory(directory + filelist[i] + "\\", filelist2);
						multiThreads.emplace_back(make_unique<std::thread>(&UpdateFilesStart::SeparateMod, this, make_unique<arguPack>(directory, filelist[i], filelist2, newFile,
							childrenState, stateID, parent, modStateList, duplicatedStateList, animData, animSetData, lastUpdate, modUpPtr)));
					}
				}

				emit progressUp();

				if (multiThreads.size() > 0)
				{
					for (unique_ptr<std::thread>& thrd : multiThreads)
					{
						thrd->join();
					}
				}

				for (auto& duplicates : duplicatedStateList)
				{
					for (auto& IDs : duplicates.second)
					{
						for (auto& modlist : IDs.second)
						{
							string mods;

							for (auto& mod : modlist.second)
							{
								mods.append(mod + ", ");
							}

							mods.pop_back();
							mods.pop_back();

							if (mods.length() > 0) ErrorMessage(1189, duplicates.first, modlist.first, mods);
						}
					}
				}
			}

			FileWriter lastmod("cache\\engine_update");

			if (lastmod.is_open())
			{
				for (auto& it : lastUpdate)
				{
					lastmod << it.first + ">>" + it.second + "\n";
				}
			}
			else
			{
				ErrorMessage(2009, "cache\\engine_update");
			}

			emit progressUp();
		}
		catch (nemesis::exception&)
		{
			// resolved exception
		}
	}
	catch (exception& ex)
	{
		try
		{
			ErrorMessage(6001, ex.what());
		}
		catch (nemesis::exception&)
		{
			// resolved exception
		}
	}
}

void UpdateFilesStart::CombiningFiles(unordered_map<string, map<string, vecstr>>& newFile, MasterAnimData& animData, MasterAnimSetData& animSetData)
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
				ErrorMessage(2008, it->first + " (" + iter->first + ")");
			}
		}

		if (isOpen)
		{
			fileline.push_back("<!-- CLOSE -->");
			isOpen = false;
		}

		if (CreateFolder(compilingfolder))
		{
			string lowerBehaviorFile = it->first;
			string filename = compilingfolder + lowerBehaviorFile + ".txt";

			if (lowerBehaviorFile.find("_1stperson\\") == 0) boost::filesystem::create_directory(boost::filesystem::path(compilingfolder + "_1stperson\\"));

			FileWriter output(filename);

			if (output.is_open())
			{
				bool behaviorRef = false;

				output << "<?xml version=\"1.0\" encoding=\"ascii\"?>\n";
				output << "<hkpackfile classversion=\"8\" contentsversion=\"hk_2010.2.0 - r1\" toplevelobject=\"" + rootID + "\">\n\n";
				output << "	<hksection name=\"__data__\">\n\n";

				for (unsigned int linecount = 0; linecount < fileline.size(); ++linecount)
				{
					output << fileline[linecount] + "\n";
					size_t pos = fileline[linecount].find("<hkobject name=\"");

					if (pos != NOT_FOUND && fileline[linecount].find("signature=\"", pos) != NOT_FOUND)
					{
						behaviorRef = fileline[linecount].find("class=\"hkbBehaviorReferenceGenerator\" signature=\"", pos) != NOT_FOUND;
					}

					if (behaviorRef && fileline[linecount].find("<hkparam name=\"behaviorName\">") != NOT_FOUND)
					{
						size_t nextpos = fileline[linecount].find("behaviorName\">") + 14;
						string behaviorName = GetFileName(fileline[linecount].substr(nextpos, fileline[linecount].find("</hkparam>", nextpos) - nextpos));
						boost::algorithm::to_lower(behaviorName);

						if (lowerBehaviorFile.find("_1stperson") != NOT_FOUND) behaviorName = "_1stperson\\" + behaviorName;

						behaviorJoints[behaviorName].push_back(lowerBehaviorFile);
						behaviorRef = false;
					}
					else if (fileline[linecount].find("<hkparam name=\"behaviorFilename\">") != NOT_FOUND)
					{
						size_t nextpos = fileline[linecount].find("behaviorFilename\">") + 18;
						string behaviorName = fileline[linecount].substr(nextpos, fileline[linecount].find("</hkparam>", nextpos) - nextpos);
						behaviorName = GetFileName(behaviorName);
						boost::algorithm::to_lower(behaviorName);

						if (lowerBehaviorFile.find("_1stperson") != NOT_FOUND) behaviorName = "_1stperson\\" + behaviorName;

						behaviorJoints[behaviorName].push_back(lowerBehaviorFile);
					}
				}

				output << "	</hksection>\n\n";
				output << "</hkpackfile>\n";
				fileline.clear();
			}
			else
			{
				ErrorMessage(2009, filename);
			}
		}
	}

	emit progressUp();
	behaviorJointsOutput();

	if (CreateFolder(compilingfolder))
	{
		string filename = compilingfolder + "animationdatasinglefile.txt";
		FileWriter output(filename);
		FileWriter outputlist("cache\\animationdata_list");

		if (output.is_open())
		{
			if (outputlist.is_open())
			{
				output << to_string(animData.animDataChar.size() - 1) + "\n";

				for (unsigned int i = 0; i < animData.newAnimData["$header$"]["$header$"].size(); ++i)
				{
					output << animData.newAnimData["$header$"]["$header$"][i] + "\n";
				}

				for (unsigned int i = 1; i < animData.animDataChar.size(); ++i)
				{	// character
					string project = animData.animDataChar[i];
					outputlist << project + "\n";
					size_t animsize = 0;
					size_t infosize = 0;

					for (auto& header : animData.animDataHeader[project])
					{
						animsize += animData.newAnimData[project][header].size();
					}

					if (animsize > 0)
					{
						output << to_string(animsize) + "\n";

						for (unsigned int j = 0; j < animData.animDataHeader[project].size(); ++j)
						{
							string header = animData.animDataHeader[project][j];
							outputlist << header + "\n";

							for (unsigned int k = 0; k < animData.newAnimData[project][header].size(); ++k)
							{
								output << animData.newAnimData[project][header][k] + "\n";
							}
						}
					}

					for (auto& header : animData.animDataInfo[project])
					{
						infosize += animData.newAnimData[project][header].size();
					}

					if (infosize > 0)
					{
						output << to_string(infosize) + "\n";

						for (unsigned int j = 0; j < animData.animDataInfo[project].size(); ++j)
						{
							string header = animData.animDataInfo[project][j];
							outputlist << header + "\n";

							for (unsigned int k = 0; k < animData.newAnimData[project][header].size(); ++k)
							{
								output << animData.newAnimData[project][header][k] + "\n";
							}
						}

						outputlist << "\n";
					}
				}
			}
			else
			{
				ErrorMessage(2009, "cache\\animationdata_list");
			}
		}
		else
		{
			ErrorMessage(2009, filename);
		}
	}

	emit progressUp();

	if (CreateFolder(compilingfolder))
	{
		string filename = compilingfolder + "animationsetdatasinglefile.txt";
		FileWriter output(filename);
		FileWriter outputlist("cache\\animationsetdata_list");

		if (output.is_open())
		{
			if (outputlist.is_open())
			{
				output << to_string(animSetData.projectList.size() - 1) + "\n";

				for (string& header : animSetData.newAnimSetData["$header$"]["$header$"])
				{
					output << header + "\n";
				}

				for (string& project : animSetData.projectList)
				{
					outputlist << project + "\n";		// character
					outputlist << "$header$\n";

					for (string& line : animSetData.newAnimSetData[project]["$header$"])
					{
						output << line + "\n";
					}

					for (auto it = animSetData.newAnimSetData[project].begin(); it != animSetData.newAnimSetData[project].end(); ++it)
					{
						string header = it->first;

						if (header != "$header$")
						{
							outputlist << header + "\n";

							for (unsigned int k = 0; k < it->second.size(); ++k)
							{
								output << it->second[k] + "\n";
							}
						}
					}

					outputlist << "\n";
				}
			}
			else
			{
				ErrorMessage(2009, "cache\\animationsetdata_list");
			}
		}
		else
		{
			ErrorMessage(2009, filename);
		}
	}

	emit progressUp();
}

bool UpdateFilesStart::newAnimUpdate(string sourcefolder, unordered_map<string, map<string, vecstr>>& newFile, MasterAnimData& animData, MasterAnimSetData& animSetData,
	unordered_map<string, string>& lastUpdate)
{
	if (CreateFolder(sourcefolder))
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
							DebugLogging("New Animations extraction start (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");

							for (unsigned int k = 0; k < characterlist.size(); ++k)
							{
								boost::filesystem::path characterfolder(folderpath + "\\" + behaviorlist[j] + "\\" + characterlist[k]);

								if (boost::filesystem::is_directory(characterfolder))
								{
									if (!newAnimDataUpdateExt(folderpath + "\\" + behaviorlist[j] + "\\" + characterlist[k], codelist[i], characterlist[k], animData,
										lastUpdate)) return false;
								}
								else
								{
									string stemTemp = characterfolder.stem().string();

									if (stemTemp == "$header$")
									{
										if (!animDataHeaderUpdate(folderpath + "\\" + behaviorlist[j] + "\\" + characterlist[k], codelist[i], animData, lastUpdate)) return false;
									}
									else if (boost::regex_match(stemTemp, boost::regex("^\\$(?!" + codelist[i] + ").+\\$(?:UC|)$")))
									{
										ErrorMessage(3023, "$" + codelist[i] + "$" + (stemTemp.find("$UC") != NOT_FOUND ? "UC" : ""));
									}
								}
							}

							DebugLogging("New Animations extraction complete (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");
						}
						else if (boost::iequals(behaviorlist[j], "animationsetdatasinglefile"))
						{
							vecstr projectfile;
							read_directory(folderpath + "\\" + behaviorlist[j], projectfile);
							DebugLogging("New Animations extraction start (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");

							for (unsigned int k = 0; k < projectfile.size(); ++k)
							{
								if (boost::filesystem::is_directory(folderpath + "\\" + behaviorlist[j] + "\\" + projectfile[k]) &&
									projectfile[k].find("~") != NOT_FOUND)
								{
									string projectname = projectfile[k];
									boost::to_lower(projectname);

									while (projectname.find("~") != NOT_FOUND)
									{
										projectname.replace(projectname.find("~"), 1, "\\");
									}

									if (!newAnimDataSetUpdateExt(folderpath + "\\" + behaviorlist[j] + "\\" + projectfile[k], codelist[i], projectname + ".txt",
										animSetData, lastUpdate)) return false;
								}
							}

							DebugLogging("New Animations extraction complete (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");
						}
						else
						{
							if (boost::iequals(behaviorlist[j], "_1stperson")) ErrorMessage(6004, folderpath + "\\" + behaviorlist[j]);

							DebugLogging("New Animations extraction start (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");

							if (!newAnimUpdateExt(folderpath, codelist[i], boost::to_lower_copy(behaviorlist[j]), newFile[boost::to_lower_copy(behaviorlist[j])], lastUpdate))
								return false;

							DebugLogging("New Animations extraction complete (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");
						}
					}
				}
			}
		}
	}
	else
	{
		ErrorMessage(2010, sourcefolder);
	}

	return true;
}

void UpdateFilesStart::milestoneStart(string directory)
{
	m_RunningThread = 1;
	UpdateReset();
	start_time = boost::posix_time::microsec_clock::local_time();
	namespace bf = boost::filesystem;

	try
	{
		DebugLogging("Nemesis Behavior Version: v" + GetNemesisVersion());
	}
	catch (int)
	{
		DebugLogging("Nemesis Behavior Version: Failed to get tool version");
		return;
	}

	string curdir = QCoreApplication::applicationDirPath().toStdString();
	replace(curdir.begin(), curdir.end(), '/', '\\');
	DebugLogging("Current Directory: " + curdir);
	DebugLogging("Data Directory: " + nemesisInfo->GetDataPath());
	DebugLogging("Skyrim Special Edition: " + string(SSE ? "TRUE" : "FALSE"));
	filenum = 11;

#ifdef DEBUG
	string path = "data\\";
#else
	string path = nemesisInfo->GetDataPath() + "meshes";
#endif

	DebugLogging("Detecting processes...");
	GetFileLoop(path + "\\");

	if (isFileExist(directory) || bf::create_directory(directory))
	{
		vecstr filelist;
		read_directory(directory, filelist);

		for (auto& file1 : filelist)
		{
			bf::path curPath(directory + file1);

			if (bf::is_directory(curPath))
			{
				string newPath = directory + file1 + "\\";
				vecstr filelist2;
				read_directory(newPath, filelist2);

				for (auto& file2 : filelist2)
				{
					bf::path curPath(newPath + file2);

					if (bf::is_directory(curPath)) ++filenum;
				}
			}
		}
	}

	DebugLogging("Process count: " + to_string(filenum));
	emit progressMax(filenum);
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
		RunScript("scripts\\update\\end\\");

		if (error)
		{
			string msg = TextBoxMessage(1008);
			interMsg(msg);
			DebugLogging(msg);
		}
		else
		{
			string msg;
			bool ms = false;
			boost::posix_time::time_duration diff = boost::posix_time::microsec_clock::local_time() - start_time;

			if (ms)
			{
				size_t second = diff.total_milliseconds();

				if (second > 1000)
				{
					string milli = to_string(second % 1000);

					while (milli.length() < 3)
					{
						milli.insert(0, "0");
					}

					msg = TextBoxMessage(1007) + ": " + to_string(second / 1000) + "," + milli + " " + TextBoxMessage(1011);
				}
				else
				{
					msg = TextBoxMessage(1007) + ": " + to_string(second) + " " + TextBoxMessage(1011);
				}
			}
			else
			{
				msg = TextBoxMessage(1007) + ": " + to_string(diff.total_seconds()) + " " + TextBoxMessage(1012);
			}

			interMsg(msg);
			DebugLogging(msg);
			emit enableCheck(false);
		}
	}
	else
	{
		string msg = TextBoxMessage(1008);
		interMsg(msg);
		DebugLogging(msg);
	}

	DebugOutput();
	disconnectProcess();

	if (cmdline)
	{
		lock_guard<mutex>lock(processlock);
		processdone = true;
		cv.notify_one();
	}
	else
	{
		emit enable(false);
		emit hide(true);
	}

	m_RunningThread = 0;
	emit end();
}

void stateCheck(SSMap& c_parent, string parentID, string lowerbehaviorfile, string sID, SSSMap& stateID, SSMap& n_stateID, vecstr children,
	string filename, string ID, string modcode, unordered_map<string, map<string, unordered_map<string, set<string>>>>& duplicatedStateList)
{
	bool skip = false;

	for (auto& child : c_parent)
	{
		if (parentID == child.second)
		{
			if (stateID[lowerbehaviorfile][child.first] == sID)
			{
				for (string& match : children)
				{
					if (match == child.first)
					{
						if (n_stateID.find(match) != n_stateID.end())
						{
							if (ID != match || (n_stateID[match] == sID && ID == match))
							{
								skip = true;
								break;
							}
						}
					}
				}
			}
		}

		if (error) throw nemesis::exception();
	}

	if (!skip)
	{
		duplicatedStateList[filename][ID][sID].insert(modcode);
		duplicatedStateList[filename][ID][sID].insert("Vanilla");
	}
}
