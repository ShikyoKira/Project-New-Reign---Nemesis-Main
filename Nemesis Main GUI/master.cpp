#include "master.h"
#include "NemesisMainGUI.h"
#include "readtextfile.h"
#include "version.h"
#include "utilities\addanims.h"
#include "utilities\addevents.h"
#include "utilities\addvariables.h"
#include "add animation\animationthread.h"
#include "src\DebugLog.h"
#include <QThread>
#include <atomic>

#pragma warning(disable:4503)

using namespace std;

unordered_map<string, vecstr> modinfo;
mutex processlock;
condition_variable cv;
bool processdone = false;

atomic<int> m_RunningThread;
vecstr hiddenMods;
atomic_flag atomic_lock = ATOMIC_FLAG_INIT;

void groupThreadStart(newGroupArgs args);
void animThreadStart(newAnimArgs args);
void stateCheck(SSMap& c_parent, string parentID, string lowerbehaviorfile, string sID, SSSMap& stateID, SSMap& n_stateID, vecstr children, string filename,
	string ID, string modcode, unordered_map<string, map<string, unordered_map<string, set<string>>>>& duplicatedStateList);

UpdateFilesStart::UpdateFilesStart()
{
}

UpdateFilesStart::~UpdateFilesStart()
{
	if (!cmdline && error)
	{
		error = false;
	}
}

void UpdateFilesStart::UpdateFiles()
{
	string directory = "mod\\";
	string newAnimDirectory = "behavior templates\\";
	milestoneStart(directory);

	if (!error)
	{
		unordered_map<string, map<string, vecstr>> newFile;								// behavior file, node ID, node data lines; memory to access each node
		unordered_map<string, map<string, unordered_map<string, bool>>> childrenState;	// behavior file, node ID, children of state machine; ensure no state conflict

		SSSMap stateID;		// behavior file, children state machine info, state ID
		SSSMap parent;		// behavior file, children state machine info, parent state machine

		MasterAnimData animData;
		MasterAnimSetData animSetData;

		unordered_map<string, string> lastUpdate;

		ClearGlobal();

		try
		{
			// Check the existence of required files
			if (FileCheck(true))
			{
				RunScript("scripts\\update\\start\\");
				UpdateDebug("External script run complete");

				// clear the temp_behaviors folder to prevent it from bloating
				ClearTempBehaviors();
				UpdateDebug("Temp behavior clearance complete");

				// create "temp_behaviors" folder
				if (!isFileExist(directory))
				{
					boost::filesystem::create_directory(directory);
				}

				emit progressUp();

				// copy latest vanilla into memory
				if (!error && VanillaUpdate(newFile, childrenState, stateID, parent, animData, animSetData))
				{
					if (!error)
					{
						UpdateDebug("Data record complete");
						emit progressUp(); // 5

						// check template for association with vanilla nodes from behavior template file
						if (newAnimUpdate(newAnimDirectory, newFile, animData, animSetData, lastUpdate))
						{
							UpdateDebug("New Animations record complete");
							emit progressUp(); // 6

							// comparing if different from mod file
							JoiningEdits(directory, newFile, childrenState, stateID, parent, animData, animSetData, lastUpdate);

							if (!error)
							{
								UpdateDebug("Modification successfully extracted");
								emit progressUp();

								// compiling all behaviors in "data/meshes" to "temp_behaviors" folder
								CombiningFiles(newFile, animData, animSetData);

								emit progressUp();
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
	}

	ClearGlobal();

	if (!cmdline)
	{
		Sleep(1500);
	}

	unregisterProcess();
}

bool UpdateFilesStart::VanillaUpdate(unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, map<string, unordered_map<string,
	bool>>>& childrenState, SSSMap& stateID, SSSMap& parent, MasterAnimData& animData, MasterAnimSetData& animSetData)
{
#ifdef DEBUG
	string path = "data\\";
#else
	string path = skyrimDataPath->GetDataPath() + "meshes";
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
		if (!GetPathLoop(path + "\\", newFile, childrenState, stateID, parent, animData, animSetData, false))
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

		if (behaviorProjectPath.size() != 0)
		{
			ofstream output("behavior_project_path");

			if (output.is_open())
			{
				FunctionWriter fwriter(&output);

				for (auto it = behaviorProjectPath.begin(); it != behaviorProjectPath.end(); ++it)
				{
					fwriter << it->first << "=" << it->second << "\n";
				}

				output.close();
			}
			else
			{
				ErrorMessage(2009, "behavior_project_path");
				return false;
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

				if (boost::iequals(curFileName, "nemesis_animationdatasinglefile") || boost::iequals(curFileName, "nemesis_animationsetdatasinglefile"))
				{
					++filenum;
				}
				else if (wordFind(curFileName, "Nemesis_") == 0 && wordFind(curFileName, "_List") == NOT_FOUND && wordFind(curFileName, "_Project") == NOT_FOUND)
				{
					++filenum;
				}
				else if (wordFind(curFileName, "Nemesis_") == 0 && wordFind(curFileName, "_Project") + 8 == curFileName.length())
				{
					++filenum;
				}
			}
		}
		else
		{
			GetFileLoop(newPath + "\\");
		}
	}
}

bool UpdateFilesStart::GetPathLoop(string path, unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, map<string,
	unordered_map<string, bool>>>& childrenState, SSSMap& stateID, SSSMap& parent, MasterAnimData& animData, MasterAnimSetData& animSetData, bool isFirstPerson)
{
	boost::thread_group multithreads;

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
						UpdateDebug("AnimData Disassemble start (File: " + newPath + ")");

						if (!AnimDataDisassemble(newPath, animData))
						{
							return false;
						}

						UpdateDebug("AnimData Disassemble complete (File: " + newPath + ")");
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
						UpdateDebug("AnimSetData Disassemble start (File: " + newPath + ")");

						if (!AnimSetDataDisassemble(newPath, animSetData))
						{
							return false;
						}

						UpdateDebug("AnimSetData Disassemble complete (File: " + newPath + ")");
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

						if (isFirstPerson)
						{
							firstperson = "_1stperson\\";
						}

						curFileName = firstperson + curFileName.substr(8);
						string smallfile = boost::to_lower_copy(curFileName);
						UpdateDebug("Behavior Disassemble start (File: " + newPath + ")");

						if (!VanillaDisassemble(newPath, smallfile, newFile, childrenState, stateID[smallfile], parent[smallfile]))
						{
							return false;
						}

						UpdateDebug("Behavior Disassemble complete (File: " + newPath + ")");
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

						if (isFirstPerson)
						{
							firstperson = "_1stperson\\";
						}

						string curPath = boost::to_lower_copy(path);
						curPath = curPath.substr(curPath.find("\\meshes\\") + 1);
						curPath.pop_back();
						curFileName = firstperson + curFileName.substr(8, curFileName.length() - 16);
						behaviorProjectPath[boost::to_lower_copy(curFileName)] = curPath;
						vecstr storeline;
						bool record = false;
						UpdateDebug("Nemesis Project Record start (File: " + newPath + ")");

						if (!GetFunctionLines(newPath, storeline))
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

						emit progressUp();
						UpdateDebug("Nemesis Project Record complete (File: " + newPath + ")");
					}
				}
			}
			else
			{
				if (boost::iequals(file, "_1stperson"))
				{
					// look deeper into the folder for behavior file
					multithreads.create_thread(boost::bind(&UpdateFilesStart::GetPathLoop, this, newPath + "\\", boost::ref(newFile), boost::ref(childrenState),
						boost::ref(stateID), boost::ref(parent), boost::ref(animData), boost::ref(animSetData), true));
				}
				else
				{
					// look deeper into the folder for behavior file
					multithreads.create_thread(boost::bind(&UpdateFilesStart::GetPathLoop, this, newPath + "\\", boost::ref(newFile), boost::ref(childrenState),
						boost::ref(stateID), boost::ref(parent), boost::ref(animData), boost::ref(animSetData), isFirstPerson));
				}
			}
		}
	}
	catch (exception& ex)
	{
		ErrorMessage(6002, ex.what());
	}
	
	multithreads.join_all();
	return true;
}

bool UpdateFilesStart::VanillaDisassemble(string path, string filename, unordered_map<string, map<string, vecstr>>& newFile,
	unordered_map<string, map<string, unordered_map<string, bool>>>& childrenState, SSMap& stateID, SSMap& parent)
{
	vecstr storeline;
	storeline.reserve(2000);
	char line[2000];

	shared_ptr<TextFile> vanillafile = make_shared<TextFile>(path);
	string curID;

	unordered_map<string, vecstr> statelist;		// parent ID, list of children

	if (vanillafile->GetFile())
	{
		bool skip = true;
		bool start = false;
		bool isSM = false;

		while (fgets(line, 2000, vanillafile->GetFile()))
		{
			string curline = line;

			if (curline.length() > 0 && curline.back() == '\n')
			{
				curline.pop_back();
			}

			if (curline.find("	</hksection>") != NOT_FOUND)
			{
				break;
			}

			if (!skip)
			{
				if (curline.find("SERIALIZE_IGNORED") == NOT_FOUND)
				{
					if (curline.find("			#") != NOT_FOUND && storeline.back().find("numelements=\"", 0) != NOT_FOUND)
					{
						start = true;
					}
					else if (start && curline.find("</hkparam>") != NOT_FOUND)
					{
						start = false;
					}

					if (curline.find("<hkparam name=\"stateId\">") != NOT_FOUND)
					{
						string stateIDStr = boost::regex_replace(string(curline), boost::regex(".*<hkparam name=\"stateId\">([0-9]+)</hkparam>.*"), string("\\1"));

						if (stateIDStr != curline)
						{
							stateID[curID] = stateIDStr;
						}
					}

					if (start)
					{
						if (curline.find("			#") != NOT_FOUND)
						{
							stringstream sstream(curline);
							istream_iterator<string> ssbegin(sstream);
							istream_iterator<string> ssend;
							vector<string> curElements(ssbegin, ssend);
							copy(curElements.begin(), curElements.end(), curElements.begin());

							if (isSM)
							{
								for (auto& element : curElements)
								{
									storeline.push_back("				" + element);
									statelist[curID].push_back(element);
									parent[element] = curID;
								}
							}
							else
							{
								for (auto& element : curElements)
								{
									storeline.push_back("				" + element);
								}
							}
						}
						else
						{
							storeline.push_back(curline);
						}
					}
					else
					{
						if (curline.find("<hkobject name=\"", 0) != NOT_FOUND && curline.find("signature=\"", curline.find("<hkobject name=\"")) != NOT_FOUND)
						{
							if (curline.find("class=\"hkbStateMachine\" signature=\"") != NOT_FOUND)
							{
								isSM = true;
							}
							else
							{
								isSM = false;
							}

							if (storeline.size() != 0 && curID.length() != 0)
							{
								storeline.shrink_to_fit();

								while (atomic_lock.test_and_set(memory_order_acquire));
								newFile[filename][curID] = storeline;
								atomic_lock.clear(memory_order_release);

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
			newFile[filename][curID] = storeline;
		}
	}
	else
	{
		ErrorMessage(2000, path);
		return false;
	}

	for (auto& state : statelist)
	{
		for (auto& ID : state.second)
		{
			if (stateID[ID].empty())
			{
				ErrorMessage(1187);
				return false;
			}

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

	if (!GetFunctionLines(path, storeline))
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
				if (isOnlyNumber(line))
				{
					if (isOnlyNumber(storeline[i + 1]))
					{
						if (isOnlyNumber(storeline[i + 2]))
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
					if (isOnlyNumber(storeline[i + 1]))
					{
						if (isOnlyNumber(storeline[i + 2]))
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
		}

		if (!out)
		{
			newline.push_back(line);
		}

		out = false;
	}

	if (newline.size() != 0)
	{
		if (newline.back().length() == 0)
		{
			newline.pop_back();
		}

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

	if (!GetFunctionLines(path, storeline))
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
			animSetData.newAnimSetData[project][header] = newline;
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
		animSetData.newAnimSetData[project][header] = newline;
	}

	return true;
}

void UpdateFilesStart::SeparateMod(arguPack& pack)
{
	string directory = pack.directory, modcode = pack.modcode;
	vecstr behaviorfilelist = pack.behaviorfilelist;
	unordered_map<string, map<string, vecstr>>& newFile(pack.newFile);
	unordered_map<string, map<string, unordered_map<string, bool>>>& childrenState(pack.childrenState);
	SSSMap& stateID(pack.stateID);
	SSSMap& parent(pack.parent);
	StateIDList& modStateList(pack.modStateList);
	StateIDList& duplicatedStateList(pack.duplicatedStateList);
	MasterAnimData& animData(pack.animData);
	MasterAnimSetData& animSetData(pack.animSetData);
	unordered_map<string, string>& lastUpdate(pack.lastUpdate);

	directory = pack.directory;
	modcode = pack.modcode;
	UpdateDebug("Installing Mod: " + modcode);

	for (unsigned int j = 0; j < behaviorfilelist.size(); ++j)
	{
		boost::filesystem::path curPath(directory + modcode + "\\" + behaviorfilelist[j]);

		if (boost::filesystem::is_directory(curPath))
		{
			vecstr nodelist;
			read_directory(curPath.string(), nodelist);
			emit progressUp();

			if (boost::iequals(behaviorfilelist[j], "animationdatasinglefile"))
			{
				if (animData.newAnimData.size() == 0)
				{
					ErrorMessage(3017, "nemesis_animationdatasinglefile.txt");
					return;
				}

				for (string& node : nodelist)
				{
					boost::filesystem::path curPath(directory + modcode + "\\" + behaviorfilelist[j] + "\\" + node);

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
						string filepath = directory + modcode + "\\" + behaviorfilelist[j] + "\\" + node;
						read_directory(filepath, uniquecodelist);

						for (string& uniquecode : uniquecodelist)
						{
							if (!boost::filesystem::is_directory(filepath + "\\" + uniquecode))
							{
								if (!AnimDataUpdate(modcode, behaviorfilelist[j], projectname, filepath + "\\" + uniquecode, ref(animData), ref(newChar),
									ref(lastUpdate), ref(openAnim), ref(openInfo)))
								{
									if (error)
									{
										return;
									}
								}
							}
						}

						if (openAnim)
						{
							animData.newAnimData[projectname][animData.animDataHeader[projectname].back()].push_back("<!-- CLOSE -->");
						}

						if (openInfo)
						{
							animData.newAnimData[projectname][animData.animDataInfo[projectname].back()].push_back("<!-- CLOSE -->");
						}

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
			else if (boost::iequals(behaviorfilelist[j], "animationsetdatasinglefile"))
			{
				if (animSetData.newAnimSetData.size() == 0)
				{
					ErrorMessage(3017, "nemesis_animationsetdatasinglefile.txt");
					return;
				}

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
							if (!AnimSetDataUpdate(modcode, behaviorfilelist[j], node, lowerproject, filepath + "\\" + uniquecode, ref(animSetData), newProject,
								ref(lastUpdate)))
							{
								if (error)
								{
									return;
								}
							}
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
			else
			{
				if (boost::iequals(behaviorfilelist[j], "_1stperson"))
				{
					for (string& node : nodelist)
					{
						SSMap n_stateID;
						unordered_map<string, vecstr> statelist;
						unordered_map<string, bool> skipped;
						vecstr fpnodelist;
						read_directory(curPath.string() + "\\" + node, fpnodelist);
						string lowerbehaviorfile = boost::to_lower_copy(behaviorfilelist[j] + "\\" + node);
						SSMap& c_parent = parent[lowerbehaviorfile];

						for (auto& fpnode : fpnodelist)
						{
							if (!FunctionUpdate(modcode, lowerbehaviorfile, fpnode, ref(newFile), n_stateID, c_parent, statelist, ref(lastUpdate)))
							{
								if (error)
								{
									return;
								}
							}
						}

						for (auto& state : statelist)
						{
							for (string& ID : state.second)		// state machine info node ID
							{
								string filename = curPath.string() + "\\" + node + "\\" + ID + ".txt";

								if (n_stateID.find(ID) != n_stateID.end())
								{
									string sID = n_stateID[ID];
									skipped[ID] = true;

									if (sID.empty())
									{
										ErrorMessage(1188, modcode, filename);
										return;
									}

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
									return;
								}
							}
						}

						for (auto& ID : n_stateID)
						{
							if (!skipped[ID.first])
							{
								bool skip = false;
								string parentID = c_parent[ID.first];
								string filename = curPath.string() + "\\" + node + "\\" + ID.first + ".txt";

								if (parentID.empty())
								{
									if (ID.first.find("$") != NOT_FOUND)
									{
										skip = true;
									}
									else
									{
										ErrorMessage(1191, modcode, ID.first);
										return;
									}
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
				else
				{
					SSMap n_stateID;
					unordered_map<string, vecstr> statelist;
					unordered_map<string, bool> skipped;
					string lowerbehaviorfile = boost::to_lower_copy(behaviorfilelist[j]);
					SSMap& c_parent = parent[lowerbehaviorfile];

					for (string& node : nodelist)
					{
						if (!FunctionUpdate(modcode, lowerbehaviorfile, node, ref(newFile), n_stateID, c_parent, statelist, ref(lastUpdate)))
						{
							if (error)
							{
								return;
							}
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

								if (sID.empty())
								{
									ErrorMessage(1188, modcode, filename);
									return;
								}

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
								return;
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
								if (ID.first.find("$") != NOT_FOUND)
								{
									skip = true;
								}
								else
								{
									ErrorMessage(1191, modcode, ID.first);
									return;
								}
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
	}

	if (!error)
	{
		UpdateDebug("Mod Installed sucessfully: " + modcode);
	}
}

void UpdateFilesStart::JoiningEdits(string directory, unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string,
	map<string, unordered_map<string, bool>>>& childrenState,  SSSMap& stateID, SSSMap& parent, MasterAnimData& animData, MasterAnimSetData& animSetData,
	unordered_map<string, string>& lastUpdate)
{
	if (isFileExist(directory))
	{
		vecstr filelist;
		boost::thread_group threads;
		read_directory(directory, filelist);

		StateIDList modStateList;				// behavior file, SM ID, state ID, list of mods
		StateIDList duplicatedStateList;		// behavior file, SM ID, state ID, list of conflicting mods

		for (unsigned int i = 0; i < filelist.size(); ++i)
		{
			boost::filesystem::path curPath(directory + filelist[i]);

			if (boost::filesystem::is_directory(curPath))
			{
				vecstr filelist2;
				read_directory(directory + filelist[i] + "\\", filelist2);
				arguPack pack(directory, filelist[i], filelist2, newFile, childrenState, stateID, parent, modStateList, duplicatedStateList, animData, animSetData, lastUpdate);
				threads.create_thread(boost::bind(&UpdateFilesStart::SeparateMod, this, pack));
			}
		}

		emit progressUp();

		try
		{
			threads.join_all();

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

						if (mods.length() > 0)
						{
							ErrorMessage(1189, duplicates.first, modlist.first, mods);
						}
					}
				}
			}

			if (error)
			{
				return;
			}
		}
		catch (exception& ex)
		{
			ErrorMessage(6002, ex.what());
			return;
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

	emit progressUp();
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
				return;
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

			if (lowerBehaviorFile.find("_1stperson\\") == 0)
			{
				boost::filesystem::create_directory(boost::filesystem::path(compilingfolder + "_1stperson\\"));
			}

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

						if (lowerBehaviorFile.find("_1stperson") != NOT_FOUND)
						{
							behaviorName = "_1stperson\\" + behaviorName;
						}

						behaviorJoints[behaviorName].push_back(lowerBehaviorFile);
						behaviorRef = false;
					}
					else if (fileline[linecount].find("<hkparam name=\"behaviorFilename\">") != NOT_FOUND)
					{
						size_t nextpos = fileline[linecount].find("behaviorFilename\">") + 18;
						string behaviorName = fileline[linecount].substr(nextpos, fileline[linecount].find("</hkparam>", nextpos) - nextpos);
						behaviorName = GetFileName(behaviorName);
						boost::algorithm::to_lower(behaviorName);

						if (lowerBehaviorFile.find("_1stperson") != NOT_FOUND)
						{
							behaviorName = "_1stperson\\" + behaviorName;
						}

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

	if (CreateFolder(compilingfolder))
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

				writeoutput << to_string(animData.animDataChar.size() - 1) << "\n";
				
				for (unsigned int i = 0; i < animData.newAnimData["$header$"]["$header$"].size(); ++i)
				{
					writeoutput << animData.newAnimData["$header$"]["$header$"][i] << "\n";
				}

				for (unsigned int i = 1; i < animData.animDataChar.size(); ++i)
				{	// character
					string project = animData.animDataChar[i];
					fwriter << project << "\n";
					size_t animsize = 0;
					size_t infosize = 0;

					for (auto& header: animData.animDataHeader[project])
					{
						animsize += animData.newAnimData[project][header].size();
					}

					if (animsize > 0)
					{
						writeoutput << to_string(animsize) << "\n";

						for (unsigned int j = 0; j < animData.animDataHeader[project].size(); ++j)
						{
							string header = animData.animDataHeader[project][j];
							fwriter << header << "\n";

							for (unsigned int k = 0; k < animData.newAnimData[project][header].size(); ++k)
							{
								writeoutput << animData.newAnimData[project][header][k] << "\n";
							}
						}
					}

					for (auto& header : animData.animDataInfo[project])
					{
						infosize += animData.newAnimData[project][header].size();
					}

					if (infosize > 0)
					{
						writeoutput << to_string(infosize) << "\n";

						for (unsigned int j = 0; j < animData.animDataInfo[project].size(); ++j)
						{
							string header = animData.animDataInfo[project][j];
							fwriter << header << "\n";

							for (unsigned int k = 0; k < animData.newAnimData[project][header].size(); ++k)
							{
								writeoutput << animData.newAnimData[project][header][k] << "\n";
							}
						}

						fwriter << "\n";
					}
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

	if (CreateFolder(compilingfolder))
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
				
				writeoutput << to_string(animSetData.projectList.size() - 1) << "\n";

				for (unsigned int i = 0; i < animSetData.newAnimSetData["$header$"]["$header$"].size(); ++i)
				{
					writeoutput << animSetData.newAnimSetData["$header$"]["$header$"][i] << "\n";
				}

				for (unsigned int i = 1; i < animSetData.projectList.size(); ++i)
				{
					string project = animSetData.projectList[i];
					fwriter << project << "\n";		// character
					fwriter << "$header$" << "\n";

					for (auto& line : animSetData.newAnimSetData[project]["$header$"])
					{
						writeoutput << line << "\n";
					}

					for (auto it = animSetData.newAnimSetData[project].begin(); it != animSetData.newAnimSetData[project].end(); ++it)
					{
						string header = it->first;

						if (header != "$header$")
						{
							fwriter << header << "\n";

							for (unsigned int k = 0; k < it->second.size(); ++k)
							{
								writeoutput << it->second[k] << "\n";
							}
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

bool UpdateFilesStart::newAnimUpdate(string sourcefolder, unordered_map<string, map<string, vecstr>>& newFile, MasterAnimData& animData, MasterAnimSetData& animSetData,
	unordered_map<string, string>& lastUpdate)
{
	if(CreateFolder(sourcefolder))
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
							UpdateDebug("New Animations extraction start (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");

							for (unsigned int k = 0; k < characterlist.size(); ++k)
							{
								boost::filesystem::path characterfolder(folderpath + "\\" + behaviorlist[j] + "\\" + characterlist[k]);

								if (boost::filesystem::is_directory(characterfolder))
								{
									if (!newAnimDataUpdateExt(folderpath + "\\" + behaviorlist[j] + "\\" + characterlist[k], codelist[i], characterlist[k],
										animData, lastUpdate))
									{
										return false;
									}
								}
								else if (characterfolder.stem().string() == "$header$")
								{
									if (!animDataHeaderUpdate(folderpath + "\\" + behaviorlist[j] + "\\" + characterlist[k], codelist[i], animData, lastUpdate))
									{
										return false;
									}
								}
							}

							UpdateDebug("New Animations extraction complete (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");
						}
						else if (boost::iequals(behaviorlist[j], "animationsetdatasinglefile"))
						{
							vecstr projectfile;
							read_directory(folderpath + "\\" + behaviorlist[j], projectfile);
							UpdateDebug("New Animations extraction start (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");

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

									if (!newAnimDataSetUpdateExt(folderpath + "\\" + behaviorlist[j] + "\\" + projectfile[k], codelist[i],
										projectname + ".txt", animSetData, lastUpdate))
									{
										return false;
									}

								}
							}

							UpdateDebug("New Animations extraction complete (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");
						}
						else
						{
							if (boost::iequals(behaviorlist[j], "_1stperson"))
							{
								ErrorMessage(6004, folderpath + "\\" + behaviorlist[j]);
								return false;
							}

							UpdateDebug("New Animations extraction start (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");

							if (!newAnimUpdateExt(folderpath, codelist[i], boost::to_lower_copy(behaviorlist[j]), newFile, lastUpdate))
							{
								return false;
							}

							UpdateDebug("New Animations extraction complete (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");
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

void UpdateFilesStart::milestoneStart(string directory)
{
	UpdateReset();

	try
	{
		UpdateDebug("Nemesis Behavior Version: v" + GetNemesisVersion());
	}
	catch (int)
	{
		return;
	}

	UpdateDebug("Current Directory: " + boost::filesystem::current_path().string());
	UpdateDebug("Data Directory: " + skyrimDataPath->GetDataPath());

	if (SSE)
	{
		UpdateDebug("Skyrim Special Edition: TRUE");
	}
	else
	{
		UpdateDebug("Skyrim Special Edition: FALSE");
	}

	filenum = 11;

#ifdef DEBUG
	string path = "data\\";
#else
	string path = skyrimDataPath->GetDataPath() + "meshes";
#endif

	GetFileLoop(path + "\\");

	if (isFileExist(directory) || boost::filesystem::create_directory(directory))
	{
		vecstr filelist;
		read_directory(directory, filelist);

		for (auto& file1 : filelist)
		{
			boost::filesystem::path curPath(directory + file1);

			if (boost::filesystem::is_directory(curPath))
			{
				string newPath = directory + file1 + "\\";
				vecstr filelist2;
				read_directory(newPath, filelist2);

				for (auto& file2 : filelist2)
				{
					boost::filesystem::path curPath(newPath + file2);

					if (boost::filesystem::is_directory(curPath))
					{
						++filenum;
					}
				}
			}
		}
	}

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
			interMsg("Failed to update engine");
			UpdateDebug("Failed to update engine");
		}
		else
		{
			interMsg("Engine update complete");
			UpdateDebug("Engine update complete");
			emit enableCheck(false);
		}
	}
	else
	{
		interMsg("Failed to update engine");
		UpdateDebug("Failed to update engine");
	}

	UpdateDebugOutput();
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

	emit end();
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
	if (!cmdline && error)
	{
		error = false;
	}
}

void BehaviorStart::addBehaviorPick(BehaviorStart* newProcess, NemesisMainGUI* newWidget, vecstr behaviorOrder, unordered_map<string, bool> behaviorPick)
{
	behaviorPriority = behaviorOrder;
	chosenBehavior = behaviorPick;
	behaviorProcess = newProcess;
	widget = newWidget;
}

void BehaviorStart::addBehaviorPick(BehaviorStart* newProcess, vecstr behaviorOrder, unordered_map<string, bool> behaviorPick)
{
	behaviorPriority = behaviorOrder;
	chosenBehavior = behaviorPick;
	behaviorProcess = newProcess;
	cmdline = true;
}

void BehaviorStart::message(string input)
{
	emit incomingMessage(QString::fromStdString(input));
}

void BehaviorStart::GenerateBehavior()
{
	try
	{
		try
		{
			milestoneStart();

			if (error)
			{
				ClearGlobal();
				unregisterProcess();
				return;
			}

			if (!isEngineUpdated())
			{
				if (!error)
				{
					interMsg(TextBoxMessage(1000));
				}

				ClearGlobal();
				unregisterProcess();
				return;
			}

			interMsg(TextBoxMessage(1003));
			interMsg("");
			interMsg(TextBoxMessage(1004));
			interMsg("");

			// Check the existence of required files
			if (!FileCheck())
			{
				ClearGlobal();
				unregisterProcess();
				return;
			}

			try
			{
				RunScript("scripts\\launcher\\start\\");
				ClearGlobal();
				characterHKX();
				GetBehaviorPath();
				GetBehaviorProject();
				GetBehaviorProjectPath();
				behaviorActivateMod(behaviorPriority);
				ClearTempXml();
			}
			catch (int)
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
			unordered_map<string, int> animationCount;		// animation type counter; use to determine how many of the that type of animation have been installed
			getTemplate BehaviorTemplate;					// get animation type

			vector<unique_ptr<registerAnimation>> animationList = move(openFile(BehaviorTemplate));	// get anim list installed by mods
			unordered_map<string, vector<shared_ptr<Furniture>>> newAnimation;						// record each of the animation created from registerAnimation

			mapSetString newAnimEvent;				// template code, list of events
			mapSetString newAnimVariable;			// template code, list of variables

			unordered_map<string, var> AnimVar;
			unordered_map<string, vector<string>> modAnimBehavior;				// behavior directory, list of behavior files; use to get behavior reference
			unordered_map<string, unordered_map<int, bool>> ignoreFunction;		// behavior file, function ID, true/false; is the function part of animation template?

			if (PCEACheck())
			{
				ReadPCEA();
			}

			if (error)
			{
				ClearGlobal();
				unregisterProcess();
				return;
			}

			if (animationList.size() > 0 || pcealist.size() > 0)
			{
				interMsg("");
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
						PatchDebug(corename);
						PatchDebug(corepath);

						if (!isFileExist(corepath + "nemesis_" + corename + ".txt") && !isFileExist(corepath + "nemesis_" + corename + ".xml"))
						{
							WarningMessage(1004, BehaviorTemplate.optionlist[templatecode].coreBehavior, templatecode, corepath + "nemesis_" + corename + ".txt");
						}
						else
						{
							string corecode = BehaviorTemplate.coreBehaviorCode[BehaviorTemplate.coreTemplate[templatecode]];

							if (coreRegistered.find(corecode) == coreRegistered.end())
							{
								PatchDebug("Core Registration: " + corecode);
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

					if (BehaviorTemplate.behaviortemplate.find(templatecode + "_group") != BehaviorTemplate.behaviortemplate.end() &&
						BehaviorTemplate.behaviortemplate[templatecode + "_group"].size() != 0)
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
						newAnimation[templatecode].emplace_back(make_unique<Furniture>(*functionlines, templatecode, animationCount[templatecode], modID +
							"\\", *animationList[i]->animInfo[templatecode][j]));
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
				emit progressMax(filenum);
			}

			PatchDebug("New animations registration complete");
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
				for (auto& templatecode : it->second)
				{
					// existing function
					for (unsigned int k = 0; k < BehaviorTemplate.existingFunctionID[templatecode][it->first].size(); ++k)
					{
						ignoreFunction[it->first + ".txt"][BehaviorTemplate.existingFunctionID[templatecode][it->first][k]] = true;
					}
				}
			}

			emit progressUp();

			if (pcealist.size() > 0 || alternateAnim.size() > 0)
			{
				QThread* ScriptThread = new QThread;
				InstallScripts* ScriptWorker = new InstallScripts;		// install PCEA & AA script
				connect(ScriptThread, SIGNAL(started()), ScriptWorker, SLOT(Run()));
				connect(ScriptWorker, SIGNAL(end()), ScriptThread, SLOT(quit()));
				connect(ScriptWorker, SIGNAL(end()), ScriptWorker, SLOT(deleteLater()));
				connect(ScriptThread, SIGNAL(finished()), ScriptThread, SLOT(deleteLater()));
				ScriptWorker->moveToThread(ScriptThread);
				ScriptThread->start();
			}

			vecstr filelist;
			read_directory(directory, filelist);

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
						worker->addInfo(directory, filelist, i, behaviorPriority, chosenBehavior, BehaviorTemplate, newAnimation, AnimVar, newAnimEvent, newAnimVariable,
							ignoreFunction, false, modID);

						if (!cmdline)
						{
							connect(worker, SIGNAL(progressAdd()), widget, SLOT(setProgressBarValue()));
						}

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
						++m_RunningThread;
						worker->moveToThread(thread);
						behaviorThreads.push_back(thread);
					}
				}
				else if (wordFind(filelist[i], "_1stperson") != NOT_FOUND)
				{
					vecstr fpfilelist;
					read_directory(directory + filelist[i], fpfilelist);

					for (unsigned int j = 0; j < fpfilelist.size(); ++j)
					{
						fpfilelist[j] = filelist[i] + "\\" + fpfilelist[j];
					}

					for (unsigned int j = 0; j < fpfilelist.size(); ++j)
					{
						if (!boost::filesystem::is_directory(directory + fpfilelist[j]))
						{
							string modID = "";
							bool isCore = false;
							int repeatcount = 0;
							int repeat = 1;
							string lowerFileName = boost::to_lower_copy(fpfilelist[j]);

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
								string tempfilename = fpfilelist[j].substr(0, fpfilelist[j].find_last_of("."));
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
								worker->addInfo(directory, fpfilelist, j, behaviorPriority, chosenBehavior, BehaviorTemplate, newAnimation, AnimVar, newAnimEvent,
									newAnimVariable, ignoreFunction, false, modID);

								if (!cmdline)
								{
									connect(worker, SIGNAL(progressAdd()), widget, SLOT(setProgressBarValue()));
								}

								connect(worker, SIGNAL(done()), behaviorProcess, SLOT(EndAttempt()));
								connect(worker, SIGNAL(done()), thread, SLOT(quit()));
								connect(worker, SIGNAL(done()), worker, SLOT(deleteLater()));

								connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

								if (boost::iequals(fpfilelist[j], "animationdatasinglefile.txt"))
								{
									connect(thread, SIGNAL(started()), worker, SLOT(AnimDataCompilation()));
								}
								else if (boost::iequals(fpfilelist[j], "animationsetdatasinglefile.txt"))
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
								++m_RunningThread;
								worker->moveToThread(thread);
								behaviorThreads.push_back(thread);
							}
						}
					}
				}

				if (error)
				{
					for (auto& thread : behaviorThreads)
					{
						thread->start();
					}

					ClearGlobal();
					unregisterProcess();
					return;
				}
			}

			for (auto& thread : behaviorThreads)
			{
				thread->start();
			}

			EndAttempt();
		}
		catch (exception& ex)
		{
			ErrorMessage(6002, ex.what());
		}
	}
	catch (...)
	{
		ErrorMessage(6002, "Unknown");
	}
}

void BehaviorStart::milestoneStart()
{
	isPatch = true;
	PatchReset();

	try
	{
		PatchDebug("Nemesis Behavior Version: v" + GetNemesisVersion());
	}
	catch (int)
	{
		return;
	}

	PatchDebug("Current Directory: " + boost::filesystem::current_path().string());
	PatchDebug("Data Directory: " + skyrimDataPath->GetDataPath());

	if (SSE)
	{
		PatchDebug("Skyrim Special Edition: TRUE");
	}
	else
	{
		PatchDebug("Skyrim Special Edition: FALSE");
	}

	int counter = 0;

	for (auto& mod : behaviorPriority)
	{
		PatchDebug("Mod " + to_string(++counter) + ": " + mod);
	}

	m_RunningThread = 1;
	connectProcess(this);
	string directory = "temp_behaviors";
	string fpdirectory = directory + "\\_1stperson";
	vecstr filelist;
	int include = 0;
	int add = 3;

	if (!isFileExist(directory))
	{
		boost::filesystem::create_directory(boost::filesystem::path(directory));
		ErrorMessage(6006);
		return;
	}
	else
	{
		read_directory(directory, filelist);

		if (isFileExist(directory + "\\xml"))
		{
			--include;
		}

		if (isFileExist(directory + "\\animationdatasinglefile.txt"))
		{
			--add;
		}

		if (isFileExist(directory + "\\animationsetdatasinglefile.txt"))
		{
			--add;
		}

		if (isFileExist(fpdirectory) && boost::filesystem::is_directory(fpdirectory))
		{
			--include;
			vecstr fpfilelist;
			read_directory(fpdirectory, fpfilelist);
			include += int(fpfilelist.size());
		}
	}

	filenum = (int(filelist.size() + include) * 10) + add;
}

void BehaviorStart::unregisterProcess()
{
	if (!error)
	{
		RunScript("scripts\\launcher\\end\\");

		if (error)
		{
			interMsg("Failed to generate behavior");
			PatchDebug("Failed to generate behavior");
		}
		else
		{
			PatchDebug("Number of animations: " + to_string(animCount));
			interMsg("Behavior generation complete");
			PatchDebug("Behavior generate complete");
			emit enableCheck(false);
		}
	}
	else
	{
		interMsg("Failed to generate behavior");
		PatchDebug("Failed to generate behavior");
	}

	isPatch = false;
	PatchDebugOutput();
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

	emit end();
}

void BehaviorStart::EndAttempt()
{
	--m_RunningThread;

	if (m_RunningThread == 0)
	{
		if (error)
		{
			ClearGlobal();
		}
		else
		{
			ClearGlobal(false);

			if (!cmdline)
			{
				Sleep(1500);
			}
		}

		unregisterProcess();
	}
}

void BehaviorStart::increaseAnimCount()
{
	if (!error)
	{
		++animCount;
		emit totalAnim(animCount);
		boost::this_thread::sleep_for(boost::chrono::milliseconds(2));
	}
}

void BehaviorStart::newMilestone()
{
	if (!error)
	{
		emit progressUp();
	}
}

void BehaviorSub::BehaviorCompilation()
{
	try
	{
		try
		{
			ImportContainer exportID;

			string filepath = directory + filelist[curList];
			string behaviorFile = filelist[curList].substr(0, filelist[curList].find_last_of("."));
			string lowerBehaviorFile = boost::algorithm::to_lower_copy(behaviorFile);

			int lastID = 0;
			int curID = 0;
			int firstID = -1;

			unordered_map<string, bool> orievent;
			unordered_map<string, bool> orivariable;

			id eventid;
			id variableid;
			id attriid;
			id charid;

			unordered_map<int, unordered_map<string, vecstr>> clipAA;			// node id, original animation, list of alternative animations path
			unordered_map<string, vector<IDCatcher>> catcher;

			SSMap IDExist;
			map<int, vecstr> catalystMap;

			set<string> pceaMod;
			unordered_map<int, vector<PCEAData>*> pceaID;	// node ID, list of mods

															// final output
			string outputdir;
			string ZeroEvent;
			string ZeroVariable;

#ifdef DEBUG
			if (modID.length() > 0)
			{
				outputdir = "new_behaviors\\" + GetFileDirectory(behaviorPath[lowerBehaviorFile].substr(behaviorPath[lowerBehaviorFile].find("\\") + 1)) + modID +
					lowerBehaviorFile;
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
			if (error)
			{
				emit done();
				return;
			}

			PatchDebug("Processing behavior: " + filepath);
			PatchDebug("Behavior output path: " + outputdir);
			emit progressAdd();

			if (modID.length() > 0 && isFileExist(outputdir + ".hkx"))
			{
				int i = 0;

				while (i < 9)
				{
					emit progressAdd();
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
				bool fp_animOpen = false;
				bool otherAnimOpen = false;
				bool norElement = false;
				bool hasAA = false;
				bool characterAA = false;
				bool newBone = false;
				bool isFirstPerson = false;
				bool isOpen = true;
				bool special = false;

				int counter = 0;
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

				unordered_map<string, string> chosenLines;

				string newMod;

				set<string> AAGroupList;
				set<string> AAEventName;

				if (lowerBehaviorFile.find("_1stperson") == NOT_FOUND)
				{
					if (alternateAnim.size() != 0)
					{
						hasAA = true;
					}
				}
				else
				{
					isFirstPerson = true;
				}

				// read behavior file
				vecstr catalyst;

				{
					if (!boost::filesystem::is_directory(filepath))
					{
						catalyst.reserve(fileLineCount(filepath));

						if (error)
						{
							emit done();
							return;
						}

						string line;
						char charline[2000];
						shared_ptr<TextFile> BehaviorFormat = make_shared<TextFile>(filepath);

						if (BehaviorFormat->GetFile())
						{
							while (fgets(charline, 2000, BehaviorFormat->GetFile()))
							{
								bool skip = false;
								line = charline;

								if (line.back() == '\n')
								{
									line.pop_back();
								}

								if (line.find("<!-- ", 0) != NOT_FOUND)
								{
									if (line.find("<!-- NEW *", 0) != NOT_FOUND)
									{
										size_t tempint = line.find("<!-- NEW *", 0) + 10;
										string mod = line.substr(tempint, line.find("* -->", tempint + 1) - tempint);

										if (!chosenBehavior[mod])
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
										if (!special)
										{
											skip = true;
										}

										isOpen = true;
										special = false;
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
										if (line.find("<!-- *", 0) != NOT_FOUND)
										{
											size_t tempint = line.find("<!-- *") + 6;
											string mod = line.substr(tempint, line.find("* -->", tempint + 1) - tempint);
											chosenLines[mod] = line;
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

										size_t pos = line.find("<hkobject name=\"");

										if (pos != NOT_FOUND && line.find("signature=\"", pos) != NOT_FOUND)
										{
											string ID = boost::regex_replace(string(line),
												boost::regex(".*<hkobject name=\"#([0-9]+)\" class=\"[a-zA-Z]+\" signature=\".*\">.*"), string("\\1"));

											if (ID != line)
											{
												curID = stoi(ID);

												if (curID >= lastID)
												{
													lastID = curID + 1;
												}
											}
										}

										catalyst.push_back(line);
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
						else
						{
							ErrorMessage(3002, filepath);
							emit done();
							return;
						}
					}
					else
					{
						ErrorMessage(3001, filepath);
						emit done();
						return;
					}

					if (catalyst.size() != 0 && catalyst.back().length() != 0 && catalyst.back().find("<!-- CONDITION END -->") == NOT_FOUND &&
						catalyst.back().find("<!-- CLOSE -->") == NOT_FOUND)
					{
						catalyst.push_back("");
					}
				}

				PatchDebug("Processing behavior: " + filepath + " (Check point 1, File extraction & mod selection complete)");

				if (isCharacter)
				{
					PatchDebug("Processing behavior: " + filepath + " (IsCharater: TRUE)");
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
						if (SSE)
						{
							FILE* bonefile;
							fopen_s(&bonefile, rigfile.c_str(), "r+b");

							if (bonefile)
							{
								int num = 0;
								vector<char> chlist;
								bool start = false;
								bool startCount = false;

								while (!feof(bonefile))
								{
									chlist.push_back(fgetc(bonefile));
								}

								fclose(bonefile);

								for (unsigned int i = 0; i < chlist.size(); ++i)
								{
									if (i % 16 == 0 && chlist[i] == unsigned char(0x4E) && chlist[i + 1] == unsigned char(0x50) && chlist[i + 2] == unsigned char(0x43)
										&& chlist[i + 3] == unsigned char(0x20) && chlist[i + 4] == unsigned char(0x52) && chlist[i + 5] == unsigned char(0x6F)
										&& chlist[i + 6] == unsigned char(0x6F) && chlist[i + 7] == unsigned char(0x74) && chlist[i + 8] == unsigned char(0x20)
										&& chlist[i + 9] == unsigned char(0x5B) && chlist[i + 10] == unsigned char(0x52) && chlist[i + 11] == unsigned char(0x6F)
										&& chlist[i + 12] == unsigned char(0x6F) && chlist[i + 13] == unsigned char(0x74) && chlist[i + 14] == unsigned char(0x5D)
										&& chlist[i + 15] == unsigned char(0x0) && chlist[i + 16] == 'ÿ' && chlist[i + 17] == 'ÿ')
									{
										start = true;
									}
									else if (start && i % 16 == 0)
									{
										bool pass = true;

										for (unsigned int j = i; j < i + 16; ++j)
										{
											if (chlist[j] != unsigned char(0x0) && chlist[j] != unsigned char(0x1))
											{
												pass = false;
												break;
											}
										}

										if (pass)
										{
											startCount = true;
											start = false;
											i += 15;
											++num;
										}
									}
									else if (startCount)
									{
										if (chlist[i] == unsigned char(0x0))
										{
											i += 15;
											++num;
										}
										else
										{
											break;
										}
									}

									if (error)
									{
										emit done();
										return;
									}
								}

								if (oribone < num)
								{
									bonenum = num - oribone;
									newBone = true;
								}
							}
							else
							{
								ErrorMessage(3002, rigfile);
								emit done();
								return;
							}
						}
						else
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
										newBone = true;
										break;
									}
								}
							}
						}
					}

					PatchDebug("Processing behavior: " + filepath + " (Check point 1.5, Character bone identification complete)");
				}
				else
				{
					PatchDebug("Processing behavior: " + filepath + " (IsCharater: FALSE)");
				}

				if (error)
				{
					emit done();
					return;
				}

				emit progressAdd();

				bool isClip = false;
				bool negative = false;
				string curNum;
				unordered_map<string, int> stateStrID;					// parent node ID, new state ID counter; counter for state ID
				unordered_map<string, string> registeredStateID;		// parent node ID & base ID & modcode & ID, existing state ID
				unordered_map<string, bool> isAdded;

				for (auto& line : catalyst)
				{
					if (line.find("<hkobject name=\"", 0) != NOT_FOUND && line.find("signature=\"", 0) != NOT_FOUND)
					{
						string nodeID = boost::regex_replace(string(line), boost::regex(".*<hkobject name=\"#(.*)\" class=\"[a-zA-Z]+\" signature=\".*\">.*"),
							string("\\1"));

						if (isOnlyNumber(nodeID))
						{
							curID = stoi(nodeID);
						}
						else
						{
							if (nodeID.find("$", 0) != NOT_FOUND)
							{
								string mod = nodeID.substr(0, nodeID.find("$"));

								if (chosenBehavior[mod])
								{
									string ID = to_string(lastID);

									while (ID.length() < 4)
									{
										ID = "0" + ID;
									}

									line.replace(line.find(nodeID), nodeID.length(), ID);
									IDExist[nodeID] = ID;
									curID = lastID;
									++lastID;

									if (lastID == 9216)
									{
										++lastID;
									}
								}
								else if (mod == "MID")
								{
									ErrorMessage(1020);
									emit done();
									return;
								}
							}
						}

						if (line.find("class=\"hkbClipGenerator\" signature=\"") != NOT_FOUND)
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
						string animPath = boost::regex_replace(string(line), boost::regex(".*<hkparam name=\"animationName\">(.*)</hkparam>.*"), string("\\1"));
						boost::algorithm::to_lower(animPath);
						addUsedAnim(lowerBehaviorFile, animPath);
						string animFile = GetFileName(animPath) + ".hkx";

						if (!isFirstPerson && animReplaced.find(animFile) != animReplaced.end() && animReplaced[animFile].size() > 0)
						{
							pceaID.insert(make_pair(curID, &animReplaced[animFile]));

							for (auto& data : animReplaced[animFile])
							{
								pceaMod.insert(data.modFile);
							}
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

					if (error)
					{
						emit done();
						return;
					}
				}

				PatchDebug("Processing behavior: " + filepath + " (Check point 2, ID replacement & PCEA record complete)");

				{
					size_t pos = catalyst[1].find("toplevelobject=");

					if (catalyst[1].find("toplevelobject=") == NOT_FOUND)
					{
						for (auto& line : catalyst)
						{
							pos = line.find("toplevelobject=");

							if (pos != NOT_FOUND)
							{
								firstID = stoi(boost::regex_replace(string(line.substr(pos)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
								break;
							}
						}

						if (firstID == -1)
						{
							ErrorMessage(1095, filepath);
							emit done();
							return;
						}
					}
					else
					{
						firstID = stoi(boost::regex_replace(string(catalyst[1].substr(pos)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
					}
				}

				curID = firstID;

				// add picked behavior and remove not picked behavior
				for (unsigned int l = 0; l < catalyst.size(); ++l)
				{
					bool elementCatch = false;
					string line = catalyst[l];

					if (line.find("<!-- ", 0) != NOT_FOUND)
					{
						if (line.find("<!-- NEW *", 0) != NOT_FOUND)
						{
							size_t tempint = line.find("<!-- NEW *", 0) + 10;
							string mod = line.substr(tempint, line.find("* -->", tempint + 1) - tempint);

							if (chosenBehavior[mod])
							{
								newMod = mod;
							}
						}
						else if (line.find("<!-- NEW ^", 0) != NOT_FOUND)
						{
							special = true;
						}
						else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
						{
							newMod.clear();
							special = false;
						}
					}

					if (line.find("<hkobject name=\"", 0) != NOT_FOUND && line.find("signature=\"", 0) != NOT_FOUND)
					{
						curID = stoi(boost::regex_replace(string(line), boost::regex(".*<hkobject name=\"#(.*)\" class=\"[a-zA-Z]+\" signature=\".*\">.*"),
							string("\\1")));
					}

					if (line.find("$", line.find("#")) != NOT_FOUND)
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
										boost::smatch match;
										boost::regex_match(line, match, boost::regex(".*#(" + ID.substr(0, ID.find("$")) + "[$]" + numID + ")[^0-9]*.*"));
										nextpos = match.position(1);
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
							line.append("%");

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
											boost::smatch match;
											boost::regex_match(line, match, boost::regex(".*#(" + masterFormat + "[$]" + numID + ")[^0-9]+.*"));
											line.replace(match.position(1), ID.length(), IDExist[ID]);
										}
										else if (!special)
										{
											IDCatcher catchingID(curID, int(catalystMap[curID].size()));
											catcher[ID].push_back(catchingID);
										}
									}
								}
							}

							line.pop_back();
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
					else if (line.find("<hkparam name=\"attributeNames\" numelements=") != NOT_FOUND ||
						line.find("<hkparam name=\"attributeDefaults\" numelements=") != NOT_FOUND)
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
					else if (line.find("<hkparam name=\"characterPropertyNames\" numelements=") != NOT_FOUND ||
						line.find("<hkparam name=\"characterPropertyInfos\" numelements=") != NOT_FOUND)
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
					else if (line.find("<hkparam name=\"variableNames\" numelements=") != NOT_FOUND ||
						line.find("<hkparam name=\"wordVariableValues\" numelements=") != NOT_FOUND ||
						line.find("<hkparam name=\"variableInfos\" numelements=") != NOT_FOUND)
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
						if (filelist[curList].find("_1stperson\\") != NOT_FOUND)
						{
							fp_animOpen = true;
						}

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
					else if (line.find("<hkparam name=\"deformableSkinNames\" numelements=\"") != NOT_FOUND ||
						line.find("<hkparam name=\"rigidSkinNames\" numelements=\"") != NOT_FOUND ||
						line.find("<hkparam name=\"animationFilenames\" numelements=\"") != NOT_FOUND)
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
					else if (line.find("</hkparam>") != NOT_FOUND && (norElement || eventOpen || varOpen || attriOpen || charOpen ||
						animOpen || otherAnimOpen))
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
									if (!AddEvents(curNum, catalystMap[curID], *it, orievent, isExist, counter, ZeroEvent, eventid, eventName))
									{
										emit done();
										return;
									}
								}

								setstr codelist = BehaviorTemplate.grouplist[lowerBehaviorFile];

								for (auto& templatecode : codelist)
								{
									for (auto it = newAnimEvent[templatecode].begin(); it != newAnimEvent[templatecode].end(); ++it)
									{
										if (!AddEvents(curNum, catalystMap[curID], *it, orievent, isExist, counter, ZeroEvent, eventid, eventName))
										{
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
								if (behaviorFile == "magicbehavior")
									behaviorFile = behaviorFile;

								for (auto it = AAGroupList.begin(); it != AAGroupList.end(); ++it)
								{
									string variablename = "Nemesis_AA" + *it;

									if (!AddVariables(curNum, catalystMap[curID], variablename, orivariable, isExist, counter, ZeroVariable,
										variableid, varName))
									{
										emit done();
										return;
									}

									variablename = "Nemesis_AA_Priority" + *it;

									if (!AddVariables(curNum, catalystMap[curID], variablename, orivariable, isExist, counter, ZeroVariable,
										variableid, varName))
									{
										emit done();
										return;
									}

									for (int k = 0; k < AAgroupCount[*it]; ++k)
									{
										variablename = "Nemesis_AA_Priority" + *it + "_" + to_string(k);

										if (!AddVariables(curNum, catalystMap[curID], variablename, orivariable, isExist, counter, ZeroVariable,
											variableid, varName))
										{
											emit done();
											return;
										}
									}
								}

								setstr codelist = BehaviorTemplate.grouplist[lowerBehaviorFile];

								for (auto& templatecode : codelist)
								{
									for (auto it = newAnimVariable[templatecode].begin(); it != newAnimVariable[templatecode].end(); ++it)
									{
										string variablename = *it;

										if (!AddVariables(curNum, catalystMap[curID], variablename, orivariable, isExist, counter, ZeroVariable,
											variableid, varName))
										{
											emit done();
											return;
										}
									}
								}

								for (auto& modname : pceaMod)
								{
									if (!AddVariables(curNum, catalystMap[curID], modname, orivariable, isExist, counter, ZeroVariable,
										variableid, varName))
									{
										emit done();
										return;
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
											for (auto& templatecode : it->second)
											{
												if (newAnimation[templatecode].size() != 0 && !BehaviorTemplate.optionlist[templatecode].core)
												{
													for (unsigned int k = 0; k < newAnimation[templatecode].size(); ++k)
													{
														if (!newAnimation[templatecode][k]->isKnown())
														{
															bool addAnim = false;
															string animPath = "Animations\\" + newAnimation[templatecode][k]->GetFilePath();

															if (!AddAnims(line, animPath, outputdir, behaviorFile, lowerBehaviorFile, newMod,
																catalystMap[curID], counter, isAdded, addAnim))
															{
																emit done();
																return;
															}

															if (addAnim)
															{
																addAnimation();
															}
														}
													}
												}
											}
										}
									}
								}

								if (!isFirstPerson)
								{
									for (auto& AA : alternateAnim)
									{
										for (auto& anim : AA.second)
										{
											if (anim != "x")
											{
												bool addAnim = false;
												string animPath = "Animations\\" + anim;

												if (!AddAnims(line, animPath, outputdir, behaviorFile, lowerBehaviorFile, newMod, catalystMap[curID],
													counter, isAdded, addAnim))
												{
													emit done();
													return;
												}

												if (addAnim)
												{
													addAnimation();
												}
											}
										}
									}

									for (auto& pcea : pcealist)
									{
										for (auto& animPath : pcea.animPathList)
										{
											bool addAnim = false;

											if (!AddAnims(line, animPath.second, outputdir, behaviorFile, lowerBehaviorFile, newMod, catalystMap[curID],
												counter, isAdded, addAnim))
											{
												emit done();
												return;
											}

											if (addAnim)
											{
												addAnimation();
											}
										}
									}
								}

								animOpen = false;
								fp_animOpen = false;
							}
							else if (otherAnimOpen)
							{
								otherAnimOpen = false;
							}

							if (elementLine != -1)
							{
								size_t position = catalystMap[curID][elementLine].find("numelements=\"") + 13;
								string oldElement = catalystMap[curID][elementLine].substr(position, catalystMap[curID][elementLine].
									find("\">", position) - position);

								if (oldElement != to_string(counter))
								{
									catalystMap[curID][elementLine].replace(catalystMap[curID][elementLine].find(oldElement), oldElement.length(),
										to_string(counter));
								}

								elementLine = -1;
							}

							curNum = "";
							counter = 0;
							openRange = 0;
						}
					}

					// counting for numelement
					if (line.find("<hkcstring>") != NOT_FOUND)
					{
						if (eventOpen)
						{
							size_t nextpos = line.find("<hkcstring>") + 11;
							string name = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
							eventName[counter] = name;
							eventid[name] = counter;
							orievent[name] = true;

							if (counter == 0)
							{
								ZeroEvent = name;
							}

							++counter;
						}
						else if (varOpen && curNum == "variableNames")
						{
							size_t nextpos = line.find("<hkcstring>") + 11;
							string name = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
							varName[counter] = name;
							variableid[name] = counter;
							orivariable[name] = true;

							if (counter == 0)
							{
								ZeroVariable = name;
							}

							++counter;
						}
						else if (attriOpen)
						{
							size_t nextpos = line.find("<hkcstring>") + 11;
							string name = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
							attriName[counter] = name;
							attriid[name] = counter;
							++counter;
						}
						else if (charOpen)
						{
							size_t nextpos = line.find("<hkcstring>") + 11;
							string name = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
							charName[counter] = name;
							charid[name] = counter;
							++counter;
						}
						else if (animOpen)
						{
							size_t nextpos = line.find("<hkcstring>") + 11;
							string animPath = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
							string animFile = GetFileName(animPath);

							if (!fp_animOpen && !characterAA && alternateAnim.find(boost::to_lower_copy(GetFileName(animPath)) + ".hkx") != alternateAnim.end())
							{
								if (!isCharacter)
								{
									ErrorMessage(1184, behaviorFile);
									emit done();
									return;
								}

								characterAA = true;
							}

							if (activatedBehavior["gender"])
							{
								if (lowerBehaviorFile == "defaultfemale" && !boost::iequals(animPath, "Animations\\female\\" + animFile))
								{
									boost::filesystem::path animation(GetFileDirectory(outputdir));

									if (isFileExist(animation.parent_path().parent_path().string() + "\\Animations\\female\\" + animFile))
									{
										nextpos = line.find(animPath);
										line.replace(nextpos, animPath.length(), "Animations\\female\\" + animFile);
									}
									else if (boost::iequals(animPath, "Animations\\male\\" + animFile))
									{
										if (isFileExist(animation.parent_path().parent_path().string() + "\\Animations\\" + animFile))
										{
											nextpos = line.find(animPath);
											line.replace(nextpos, animPath.length(), "Animations\\" + animFile);
										}
									}
								}
								else if (lowerBehaviorFile == "defaultmale" && !boost::iequals(animPath, "Animations\\male\\" + animFile))
								{
									boost::filesystem::path animation(GetFileDirectory(outputdir));

									if (isFileExist(animation.parent_path().parent_path().string() + "\\Animations\\male\\" + animFile))
									{
										nextpos = line.find(animPath);
										line.replace(nextpos, animPath.length(), "Animations\\male\\" + animFile);
									}
									else if (boost::iequals(animPath, "Animations\\female\\" + animFile))
									{
										if (isFileExist(animation.parent_path().parent_path().string() + "\\Animations\\" + animFile))
										{
											nextpos = line.find(animPath);
											line.replace(nextpos, animPath.length(), "Animations\\" + animFile);
										}
									}
								}
							}

							boost::to_lower(animPath);
							boost::to_lower(animFile);
							isAdded[animPath] = true;
							registeredAnim[lowerBehaviorFile][animFile] = true;
							addAnimation();
							++counter;


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
						else if (otherAnimOpen)
						{
							++counter;
						}
					}

					if (varOpen)
					{
						if (curNum == "wordVariableValues" && line.find("<hkparam name=\"value\">") != NOT_FOUND)
						{
							++counter;
						}
						else if (curNum == "variableInfos" && line.find("<hkparam name=\"type\">") != NOT_FOUND)
						{
							++counter;
						}
					}
					else if (norElement)
					{
						string templine = line;

						if (templine.find("<hkobject>") != NOT_FOUND)
						{
							templine = templine.substr(0, templine.find("<hkobject>"));
							size_t range = count(templine.begin(), templine.end(), '\t');

							if (range == openRange + 1)
							{
								++counter;
							}
						}
						else if (templine.find("\t\t\t#") != NOT_FOUND)
						{
							templine = templine.substr(0, templine.find("#", 0));
							size_t reference = count(templine.begin(), templine.end(), '\t');

							if (reference == openRange + 1)
							{
								int number = int(count(line.begin(), line.end(), '#'));
								counter += number;
							}
						}
					}

					int repeatnum = 1;

					if (line.find("$") != NOT_FOUND)
					{
						size_t pos = line.find("<!-- Bone$N -->");

						if (pos != NOT_FOUND && pos == line.length() - 15)
						{
							if (newBone)
							{
								repeatnum = bonenum;
							}
							else
							{
								repeatnum = 0;
							}
						}

						if (!ignoreFunction[filelist[curList]][curID])
						{
							if (line.find("$eventID[", 0) != NOT_FOUND &&  line.find("]$", 0) != NOT_FOUND)
							{
								string change = boost::regex_replace(string(line), boost::regex(".*[$](eventID[[].*[]])[$].*"), string("\\1"));

								if (change != line)
								{
									string oldChange = "$" + change + "$";
									eventIDReplacer(change, "BASE", behaviorFile, eventid, ZeroEvent, l + 1);
									line.replace(line.find(oldChange), oldChange.length(), change);
								}

								if (error)
								{
									emit done();
									return;
								}
							}

							if (line.find("$variableID[", 0) != NOT_FOUND)
							{
								string change = boost::regex_replace(string(line), boost::regex(".*[$](variableID[[].*[]])[$].*"), string("\\1"));

								if (change != line)
								{
									string oldChange = "$" + change + "$";
									variableIDReplacer(change, "BASE", behaviorFile, variableid, ZeroVariable, l + 1);
									line.replace(line.find(oldChange), oldChange.length(), change);
								}

								if (error)
								{
									emit done();
									return;
								}
							}
						}

						if (line.find("$stateID[", 0) != NOT_FOUND)
						{
							string change = boost::regex_replace(string(line), boost::regex(".*[$](stateID[[].*[]][[][0-9]+[]][[].*[]][[][0-9]+[]])[$].*"), string("\\1"));

							if (change != line)
							{
								string oldChange = "$" + change + "$";

								if (registeredStateID[change].length() > 0)
								{
									line.replace(line.find(oldChange), oldChange.length(), registeredStateID[change]);
								}
								else
								{
									string node = boost::regex_replace(string(line), boost::regex(".*[$]stateID[[](.*)[]][[]([0-9]+)[]][[].*[]][[][0-9]+[]][$].*"),
										string("\\1"));
									string base = boost::regex_replace(string(line), boost::regex(".*[$]stateID[[](.*)[]][[]([0-9]+)[]][[].*[]][[][0-9]+[]][$].*"),
										string("\\2"));

									if (stateStrID[node + base] == 0)
									{
										stateStrID[node + base] = stoi(base) + 1;
									}
									else
									{
										base = to_string(stateStrID[node + base]++);
									}

									line.replace(line.find(oldChange), oldChange.length(), base);
									registeredStateID[change] = base;
								}
							}
							else
							{
								ErrorMessage(1186, filepath, l + 1, change);
								emit done();
								return;
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

					if (error)
					{
						emit done();
						return;
					}
				}

				PatchDebug("Processing behavior: " + filepath + " (Check point 3, Behavior general processing complete)");

				if (clipAA.size() == 0 && pceaID.size() == 0 && !activatedBehavior[lowerBehaviorFile] && !characterAA && !newBone)
				{
					if (!isEdited(BehaviorTemplate, lowerBehaviorFile, newAnimation, isCharacter, modID))
					{
						// final output
#ifdef DEBUG
						outputdir = "new_behaviors\\" + behaviorPath[lowerBehaviorFile].substr(behaviorPath[lowerBehaviorFile].find("\\") + 1);
#else
						outputdir = behaviorPath[lowerBehaviorFile] + ".hkx";
#endif
						if (SSE)
						{
							lowerBehaviorFile = "SSE\\" + lowerBehaviorFile;
						}

						string cachedFile = "cached_behaviors\\" + lowerBehaviorFile + ".hkx";

						if (isFileExist(cachedFile) && !boost::filesystem::is_directory(cachedFile))
						{
							boost::filesystem::copy_file(cachedFile, outputdir, boost::filesystem::copy_option::overwrite_if_exists);
						}
						else if (isFileExist(outputdir) && !boost::filesystem::is_directory(outputdir))
						{
							if (!boost::filesystem::remove(outputdir))
							{
								WarningMessage(1005, outputdir);
							}
						}

						int i = 0;

						while (i < 8)
						{
							emit progressAdd();
							++i;
						}

						PatchDebug("Processing behavior: " + filepath + " (Check point 3.4, No changes detected)");
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
			emit progressAdd();

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
							line->append("%");
							string masterFormat = it->first.substr(0, it->first.find("$"));
							string numID = it->first.substr(it->first.find("$") + 1);
							boost::smatch match;
							boost::regex_match(*line, match, boost::regex(".*(" + masterFormat + "[$]" + numID + ")[^0-9]+.*"));
							line->replace(match.position(1), it->first.length(), IDExist[it->first]);
							line->pop_back();
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

			emit progressAdd();

			// add new animation
			vector<shared_ptr<vecstr>> allEditLines;
			unordered_map<string, bool> isCoreDone;
			unordered_map<int, int> functionState;

			// check for having newAnimation for the file
			if (BehaviorTemplate.grouplist.find(lowerBehaviorFile) != BehaviorTemplate.grouplist.end() && BehaviorTemplate.grouplist[lowerBehaviorFile].size() > 0)
			{
				for (auto& templateCode : BehaviorTemplate.grouplist[lowerBehaviorFile])
				{
					vecstr opening;
					opening.push_back("<!-- ======================== NEMESIS " + templateCode + " TEMPLATE START ======================== -->");
					allEditLines.emplace_back(make_shared<vecstr>(opening));
					bool hasGroup = false;
					bool hasMaster = false;
					bool ignoreGroup = false;

					if (BehaviorTemplate.behaviortemplate.find(templateCode + "_group") != BehaviorTemplate.behaviortemplate.end())
					{
						if (BehaviorTemplate.behaviortemplate[templateCode + "_group"].find(lowerBehaviorFile) != BehaviorTemplate.behaviortemplate[templateCode +
							"_group"].end())
						{
							if (BehaviorTemplate.behaviortemplate[templateCode + "_group"][lowerBehaviorFile].size() > 0)
							{
								hasGroup = true;
							}
						}
					}

					if (BehaviorTemplate.behaviortemplate.find(templateCode + "_master") != BehaviorTemplate.behaviortemplate.end())
					{
						if (BehaviorTemplate.behaviortemplate[templateCode + "_master"].find(lowerBehaviorFile) != BehaviorTemplate.behaviortemplate[templateCode +
							"_master"].end())
						{
							if (BehaviorTemplate.behaviortemplate[templateCode + "_master"][lowerBehaviorFile].size() > 0)
							{
								hasMaster = true;
							}
						}
					}

					if (newAnimation.find(templateCode) != newAnimation.end() && newAnimation[templateCode].size() != 0)
					{
						{
							vecstr space{ "" };
							allEditLines.emplace_back(make_shared<vecstr>(space));
						}

						while (true)
						{
							if (newAnimation[templateCode].size() == 0 || newAnimSkip(newAnimation[templateCode], modID))
							{
								break;
							}

							shared_ptr<group> subFunctionIDs = make_shared<group>();
							subFunctionIDs->singlelist.reserve(memory);
							shared_ptr<master> groupFunctionIDs = make_shared<master>();
							group newSubFunctionIDs;
							vector<vector<shared_ptr<animationInfo>>> groupAnimInfo;
							vector<int> stateID;
							vector<int> stateMultiplier = { 0 };

							if (!GetStateCount(stateMultiplier, BehaviorTemplate.behaviortemplate[templateCode][lowerBehaviorFile], templateCode, behaviorFile, hasGroup))
							{
								emit done();
								return;
							}

							if (!hasGroup)
							{
								stateID = GetStateID(BehaviorTemplate.mainBehaviorJoint[templateCode][lowerBehaviorFile], catalystMap, functionState);

								if (newAnimation[templateCode][0]->GetGroupAnimInfo()[0]->ignoreGroup)
								{
									ignoreGroup = true;
									// newSubFunctionIDs.singlelist.reserve(memory);
									groupFunctionIDs->grouplist.reserve(memory);
								}
							}
							else
							{
								stateID.push_back(0);
								groupFunctionIDs->grouplist.reserve(memory);
							}

							if (error)
							{
								emit done();
								return;
							}

							if (stateMultiplier.size() != stateID.size() || stateMultiplier.size() == 0)
							{
								ErrorMessage(1192);
								emit done();
								return;
							}

							size_t newAnimCount = newAnimation[templateCode].size();

							if (newAnimCount > 0)
							{
								PatchDebug("Processing behavior: " + filepath + " (Check point 3.8, Mod code: " + templateCode + ", Animation count" +
									to_string(newAnimCount) + ")");
								shared_ptr<Furniture> dummyAnimation;
								int IDMultiplier = newAnimation[templateCode][0]->getNextID(lowerBehaviorFile);
								newAnimLock animLock;

								// individual animation
								if (hasGroup)
								{
									size_t threadCount = 0;

									for (unsigned int k = 0; k < newAnimCount; ++k)
									{
										boost::thread_group multiThreads;

										for (k; k < newAnimCount; ++k)
										{
											if (newAnimation[templateCode][k]->coreModID != modID)
											{
												if (dummyAnimation != nullptr && !dummyAnimation->isLast())
												{
													ErrorMessage(1183);
													emit done();
													return;
												}
											}
											else
											{
												subFunctionIDs->singlelist.emplace_back(make_shared<single>());
												subFunctionIDs->singlelist.back()->format["Nemesis" + modID + lowerBehaviorFile + to_string(k)] = to_string(k);
												allEditLines.emplace_back(make_shared<vecstr>());
												dummyAnimation = newAnimation[templateCode][k];

												newAnimArgs args(modID, lowerBehaviorFile, lastID, BehaviorTemplate.optionlist[templateCode].core, newAnimation[templateCode][k],
													allEditLines.back(), isCoreDone[newAnimation[templateCode][k]->GetGroupAnimInfo()[0]->filename], functionState,
													exportID, eventid, variableid, animLock, ZeroEvent, ZeroVariable, hasGroup, stateID, stateMultiplier,
													subFunctionIDs->singlelist.back(), subFunctionIDs, groupFunctionIDs, false, groupAnimInfo);

												if (newAnimation[templateCode][k]->isLast())
												{
													subFunctionIDs->singlelist.shrink_to_fit();
													groupFunctionIDs->grouplist.push_back(subFunctionIDs);
													subFunctionIDs = make_shared<group>();
													subFunctionIDs->singlelist.reserve(memory);
													groupAnimInfo.push_back(newAnimation[templateCode][k]->GetGroupAnimInfo());

													for (unsigned int statenum = 0; statenum < stateID.size(); ++statenum)
													{
														stateID[statenum] = 0;
													}
												}
												else
												{
													for (unsigned int statenum = 0; statenum < stateID.size(); ++statenum)
													{
														stateID[statenum] += stateMultiplier[statenum];
													}
												}

												lastID += IDMultiplier;
												multiThreads.create_thread(boost::bind(animThreadStart, args));
												++threadCount;
											}

											if (threadCount % boost::thread::hardware_concurrency() == boost::thread::hardware_concurrency() - 1 || k + 1 == newAnimCount)
											{
												multiThreads.join_all();
												threadCount = 0;
												break;
											}
										}
									}
								}
								else
								{
									size_t threadCount = 0;

									for (unsigned int k = 0; k < newAnimCount; ++k)
									{
										boost::thread_group multiThreads;

										for (k; k < newAnimCount; ++k)
										{
											if (newAnimation[templateCode][k]->coreModID != modID)
											{
												if (dummyAnimation != nullptr && !dummyAnimation->isLast())
												{
													ErrorMessage(1183);
													emit done();
													return;
												}
											}
											else
											{
												subFunctionIDs->singlelist.emplace_back(make_shared<single>());
												subFunctionIDs->singlelist.back()->format["Nemesis" + modID + lowerBehaviorFile + to_string(k)] = to_string(k);
												allEditLines.emplace_back(make_shared<vecstr>());
												dummyAnimation = newAnimation[templateCode][k];

												newAnimArgs args(modID, lowerBehaviorFile, lastID, BehaviorTemplate.optionlist[templateCode].core, newAnimation[templateCode][k],
													allEditLines.back(), isCoreDone[newAnimation[templateCode][k]->GetGroupAnimInfo()[0]->filename], functionState,
													exportID, eventid, variableid, animLock, ZeroEvent, ZeroVariable, hasGroup, stateID, stateMultiplier,
													subFunctionIDs->singlelist.back(), subFunctionIDs, groupFunctionIDs, ignoreGroup, groupAnimInfo);

												if (ignoreGroup)
												{
													if (newAnimation[templateCode][k]->isLast())
													{
														subFunctionIDs->singlelist.shrink_to_fit();
														groupFunctionIDs->grouplist.push_back(subFunctionIDs);
														subFunctionIDs = make_shared<group>();
														subFunctionIDs->singlelist.reserve(memory);
														// newSubFunctionIDs.singlelist.push_back(subFunctionIDs.singlelist[0]);
														// subFunctionIDs.singlelist.clear();
														groupAnimInfo.push_back(newAnimation[templateCode][k]->GetGroupAnimInfo());

														for (unsigned int statenum = 0; statenum < stateID.size(); ++statenum)
														{
															stateID[statenum] += stateMultiplier[statenum];
														}
													}
												}
												else
												{
													groupAnimInfo.push_back(newAnimation[templateCode][k]->GetGroupAnimInfo());

													for (unsigned int statenum = 0; statenum < stateID.size(); ++statenum)
													{
														stateID[statenum] += stateMultiplier[statenum];
													}
												}

												lastID += IDMultiplier;
												multiThreads.create_thread(boost::bind(animThreadStart, args));
												++threadCount;
											}

											if (threadCount % boost::thread::hardware_concurrency() == boost::thread::hardware_concurrency() - 1 || k + 1 == newAnimCount)
											{
												multiThreads.join_all();
												threadCount = 0;
												break;
											}
										}
									}
								}

								if (ignoreGroup)
								{
									// newSubFunctionIDs.singlelist.shrink_to_fit();
									// groupFunctionIDs.grouplist.push_back(newSubFunctionIDs);
									// newSubFunctionIDs.singlelist.clear();
								}
								else if (!hasGroup)
								{
									subFunctionIDs->singlelist.shrink_to_fit();
									groupFunctionIDs->grouplist.push_back(subFunctionIDs);
									subFunctionIDs = make_shared<group>();
									vector<shared_ptr<animationInfo>> subGroupAnimInfo;
									vector<vector<shared_ptr<animationInfo>>> newGroupAnimInfo;

									for (unsigned int l = 0; l < groupAnimInfo.size(); ++l)
									{
										subGroupAnimInfo.push_back(groupAnimInfo[l][0]);
									}

									newGroupAnimInfo.push_back(subGroupAnimInfo);
									groupAnimInfo = newGroupAnimInfo;
								}

								// if (!ignoreGroup)
								{
									// check error before initialization
									for (unsigned int curGroup = 0; curGroup < groupFunctionIDs->grouplist.size(); ++curGroup)
									{
										if (groupFunctionIDs->grouplist[curGroup]->singlelist.size() != groupAnimInfo[curGroup].size())
										{
											ErrorMessage(1156, templateCode);
											emit done();
											return;
										}
									}
								}

								// group animation
								if (hasGroup && groupFunctionIDs->grouplist.size() > 0)
								{
									string filename = templateCode + "_group";
									IDMultiplier = getTemplateNextID(BehaviorTemplate.behaviortemplate[filename][lowerBehaviorFile]);

									{
										vector<int> newMultiplier;
										stateMultiplier = newMultiplier;
									}

									if (!GetStateCount(stateMultiplier, BehaviorTemplate.behaviortemplate[filename][lowerBehaviorFile], templateCode, behaviorFile, hasGroup))
									{
										emit done();
										return;
									}

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
										stateID = { 0 };
									}

									newAnimCount = groupFunctionIDs->grouplist.size();

									for (unsigned int k = 0; k < newAnimCount; ++k)
									{
										boost::thread_group multiThreads;

										for (k; k < newAnimCount; ++k)
										{
											allEditLines.emplace_back(make_shared<vecstr>());
											shared_ptr<groupTemplate> groupTemp = make_shared<groupTemplate>(BehaviorTemplate.behaviortemplate[filename][lowerBehaviorFile]);
											newGroupArgs args(templateCode, lowerBehaviorFile, filename, lastID, k + 1, stateID, groupTemp, allEditLines.back(), exportID, eventid,
												variableid, animLock, ZeroEvent, ZeroVariable, groupFunctionIDs, groupAnimInfo);

											if (error)
											{
												emit done();
												return;
											}

											for (unsigned int statenum = 0; statenum < stateID.size(); ++statenum)
											{
												stateID[statenum] += stateMultiplier[statenum];
											}

											lastID += IDMultiplier;
											multiThreads.create_thread(boost::bind(groupThreadStart, args));

											if (k % boost::thread::hardware_concurrency() == boost::thread::hardware_concurrency() - 1 || k + 1 == newAnimCount)
											{
												multiThreads.join_all();
												break;
											}
										}
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

									shared_ptr<groupTemplate> masterTemp = make_shared<groupTemplate>(BehaviorTemplate.behaviortemplate[filename][lowerBehaviorFile]);
									masterTemp->setZeroEvent(ZeroEvent);
									masterTemp->setZeroVariable(ZeroVariable);
									allEditLines.emplace_back(make_shared<vecstr>());
									masterTemp->getFunctionLines(allEditLines.back(), lowerBehaviorFile, filename, stateID, groupFunctionIDs,
										groupAnimInfo, lastID, exportID, eventid, variableid, templateCode, animLock, -1);

									lastID += getTemplateNextID(BehaviorTemplate.behaviortemplate[filename][lowerBehaviorFile]);

									if (error)
									{
										emit done();
										return;
									}
								}

								// existing function
								for (unsigned int k = 0; k < BehaviorTemplate.existingFunctionID[templateCode][lowerBehaviorFile].size(); ++k)
								{
									ExistingFunction exFunction;
									exFunction.setZeroEvent(ZeroEvent);
									exFunction.setZeroVariable(ZeroVariable);
									int functionID = BehaviorTemplate.existingFunctionID[templateCode][lowerBehaviorFile][k];
									catalystMap[functionID] = exFunction.groupExistingFunctionProcess(functionID, catalystMap[functionID], groupFunctionIDs,
										groupAnimInfo, templateCode, exportID, eventid, variableid, lastID, hasMaster, hasGroup,
										BehaviorTemplate.grouplist[lowerBehaviorFile], ignoreGroup);

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

								PatchDebug("Processing behavior: " + filepath + " (Check point 3.8, Mod code: " + templateCode + ", Animation count" +
									to_string(newAnimCount) + " COMPLETE)");
							}

							break;
						}
					}
					else
					{
						PatchDebug("Processing behavior: " + filepath + " (Check point 3.8, Mod code: " + templateCode + ", Existing ID count" +
							to_string(BehaviorTemplate.existingFunctionID[templateCode][lowerBehaviorFile].size()) + ")");

						// existing function
						for (unsigned int k = 0; k < BehaviorTemplate.existingFunctionID[templateCode][lowerBehaviorFile].size(); ++k)
						{
							vector<vector<shared_ptr<animationInfo>>> animInfo;
							ExistingFunction exFunction;
							exFunction.setZeroEvent(ZeroEvent);
							exFunction.setZeroVariable(ZeroVariable);
							int functionID = BehaviorTemplate.existingFunctionID[templateCode][lowerBehaviorFile][k];
							catalystMap[functionID] = exFunction.groupExistingFunctionProcess(functionID, catalystMap[functionID], make_shared<master>(),
								animInfo, templateCode, exportID, eventid, variableid, lastID, hasMaster, hasGroup, BehaviorTemplate.grouplist[lowerBehaviorFile],
								ignoreGroup);

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

						PatchDebug("Processing behavior: " + filepath + " (Check point 3.8, Mod code: " + templateCode + ", Existing ID count" +
							to_string(BehaviorTemplate.existingFunctionID[templateCode][lowerBehaviorFile].size()) + " COMPLETE)");
					}

					vecstr closing;
					closing.push_back("<!-- ======================== NEMESIS " + templateCode + " TEMPLATE END ======================== -->");
					closing.push_back("");
					allEditLines.emplace_back(make_unique<vecstr>(closing));
				}
			}

			// check for error
			if (error)
			{
				emit done();
				return;
			}

			PatchDebug("Processing behavior: " + filepath + " (Check point 4, New animation inclusion complete)");
			emit progressAdd();

			// AA animation installation
			vecstr AAlines;

			if (clipAA.size() != 0)
			{
				PatchDebug("Processing behavior: " + filepath + " (Check point 4.2, AA count " + to_string(clipAA.size()) + ")");
				unordered_map<string, int> replacerCount;

				for (auto iter = clipAA.begin(); iter != clipAA.end(); ++iter)
				{
					bool isChange = false;
					vecstr msglines;

					if (error)
					{
						emit done();
						return;
					}

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
						importline = "variableID[Nemesis_AA" + groupName + "]";
						variableIDReplacer(importline, "AA", behaviorFile, variableid, ZeroVariable, 0);

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
							importline = "			<hkparam name=\"variableBindingSet\">#" + exportID["variable_binding"]
								["selectedGeneratorIndex!~^!" + importline] + "</hkparam>";
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

						unordered_map<string, vecstr> triggerID;

						{
							int i_baseID = stoi(baseID);
							auto& pceaBaseIter = pceaID.find(iter->first);
							catalystMap[i_baseID].reserve(catalystMap[iter->first].size());
							catalystMap[i_baseID].push_back("		<hkobject name=\"#" + baseID + "\" class=\"hkbClipGenerator\" signature=\"0x333b85b9\">");

							for (unsigned int i = 1; i < catalystMap[iter->first].size(); ++i)
							{
								catalystMap[i_baseID].push_back(catalystMap[iter->first][i]);
							}

							if (catalystMap[i_baseID].back().length() != 0)
							{
								catalystMap[i_baseID].push_back("");
							}

							if (pceaBaseIter != pceaID.end())
							{
								pceaID[i_baseID] = pceaBaseIter->second;
								pceaID.erase(pceaBaseIter);
							}
						}

						int num = 0;
						AAlines.reserve(catalystMap[iter->first].size() * children.size());

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

							if (error)
							{
								emit done();
								return;
							}

							++num;
						}

						for (auto itera = triggerID.begin(); itera != triggerID.end(); ++itera)
						{
							AAlines.reserve(AAlines.size() + (itera->second.size() * 12) + 5);
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

							if (error)
							{
								emit done();
								return;
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

				PatchDebug("Processing behavior: " + filepath + " (Check point 4.4, AA count " + to_string(clipAA.size()) + " COMPLETE)");
			}

			vecstr PCEALines;

			if (pceaID.size() > 0)
			{
				PatchDebug("Processing behavior: " + filepath + " (Check point 4.6, PCEA count " + to_string(pceaID.size()) + ")");
				unordered_map<string, int> replacerCount;

				for (auto& datalist : pceaID)
				{
					vector<vecstr> lineRe;
					string importline = to_string(datalist.first);

					if (error)
					{
						emit done();
						return;
					}

					for (auto& data = datalist.second->rbegin(); data != datalist.second->rend(); ++data)
					{
						while (importline.length() < 4)
						{
							importline = "0" + importline;
						}

						// populating manual selector generator
						vecstr msglines;
						msglines.push_back("		<hkobject name=\"#" + importline + "\" class=\"hkbManualSelectorGenerator\" signature=\"0xd932fab8\">");
						importline = "variableID[" + data->modFile + "]";
						variableIDReplacer(importline, "PCEA", behaviorFile, variableid, ZeroVariable, 0);

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
							importline = "			<hkparam name=\"variableBindingSet\">#" + exportID["variable_binding"]
								["selectedGeneratorIndex!~^!" + importline] + "</hkparam>";
						}

						msglines.push_back(importline);
						msglines.push_back("			<hkparam name=\"userData\">0</hkparam>");
						++replacerCount[data->modFile];
						msglines.push_back("			<hkparam name=\"name\">Nemesis_PCEA_" + data->modFile + "_MSG</hkparam>");
						msglines.push_back("			<hkparam name=\"generators\" numelements=\"2\">");

						importline = to_string(lastID);

						while (importline.length() < 4)
						{
							importline = "0" + importline;
						}

						msglines.push_back("				#" + importline);
						++lastID;

						if (lastID == 9216)
						{
							++lastID;
						}

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

						msglines.push_back("			</hkparam>");
						msglines.push_back("			<hkparam name=\"selectedGeneratorIndex\">0</hkparam>");
						msglines.push_back("			<hkparam name=\"currentGeneratorIndex\">0</hkparam>");
						msglines.push_back("		</hkobject>");
						msglines.push_back("");

						lineRe.push_back(msglines);
						msglines.clear();

						// populating clip generator
						msglines.push_back("		<hkobject name=\"#" + baseID + "\" class=\"hkbClipGenerator\" signature=\"0x333b85b9\">");
						msglines.push_back(catalystMap[datalist.first][1]);
						msglines.push_back(catalystMap[datalist.first][2]);
						msglines.push_back(catalystMap[datalist.first][3]);
						msglines.push_back("			<hkparam name=\"animationName\">" + data->animPath + "</hkparam>");

						for (unsigned int j = 5; j < catalystMap[datalist.first].size(); ++j)
						{
							msglines.push_back(catalystMap[datalist.first][j]);
						}

						if (msglines.back().length() != 0)
						{
							msglines.push_back("");
						}

						lineRe.push_back(msglines);
					}

					vecstr msglines;
					msglines.push_back("		<hkobject name=\"#" + importline + "\" class=\"hkbClipGenerator\" signature=\"0x333b85b9\">");
					msglines.insert(msglines.end(), catalystMap[datalist.first].begin() + 1, catalystMap[datalist.first].end());

					if (msglines.back().length() != 0)
					{
						msglines.push_back("");
					}

					lineRe.push_back(msglines);

					for (auto& it = lineRe.rbegin(); it != lineRe.rend(); ++it)
					{
						PCEALines.insert(PCEALines.end(), it->begin(), it->end());
					}

					if (error)
					{
						emit done();
						return;
					}

					catalystMap.erase(catalystMap.find(datalist.first));
				}

				PatchDebug("Processing behavior: " + filepath + " (Check point 4.8, PCEA count " + to_string(pceaID.size()) + " COMPLETE)");
			}

			emit progressAdd();

			size_t reserveSize = 0;
			vecstr behaviorlines;

			// output import functions
			// Must be in vector
			vector<ImportContainer> groupExportID;
			groupExportID.push_back(exportID);
			vecstr additionallines = importOutput(groupExportID, int(groupExportID.size() - 1), lastID, filelist[curList]);

			emit progressAdd();

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

				if (error)
				{
					emit done();
					return;
				}
			}

			behaviorlines.push_back("<!-- ======================== NEMESIS import TEMPLATE START ======================== -->");

			if (additionallines.size() > 0)
			{
				behaviorlines.push_back("");
				behaviorlines.insert(behaviorlines.end(), additionallines.begin(), additionallines.end());

				if (error)
				{
					emit done();
					return;
				}
			}

			behaviorlines.push_back("<!-- ======================== NEMESIS import TEMPLATE END ======================== -->");
			behaviorlines.push_back("");
			behaviorlines.push_back("<!-- ======================== NEMESIS PCEA TEMPLATE START ======================== -->");

			if (PCEALines.size() > 0)
			{
				behaviorlines.push_back("");
				behaviorlines.insert(behaviorlines.end(), PCEALines.begin(), PCEALines.end());

				if (error)
				{
					emit done();
					return;
				}
			}

			behaviorlines.push_back("<!-- ======================== NEMESIS PCEA TEMPLATE END ======================== -->");
			behaviorlines.push_back("");
			behaviorlines.push_back("<!-- ======================== NEMESIS alternate animation TEMPLATE START ======================== -->");

			if (AAlines.size() > 0)
			{
				behaviorlines.push_back("");
				behaviorlines.insert(behaviorlines.end(), AAlines.begin(), AAlines.end());

				if (error)
				{
					emit done();
					return;
				}
			}

			behaviorlines.push_back("<!-- ======================== NEMESIS alternate animation TEMPLATE END ======================== -->");

			for (unsigned int j = 0; j < allEditLines.size(); ++j)
			{
				behaviorlines.insert(behaviorlines.end(), allEditLines[j]->begin(), allEditLines[j]->end());

				if (error)
				{
					emit done();
					return;
				}
			}

			firstID = firstID + 3;

			for (auto it = catalystMap.begin(); it != catalystMap.end(); ++it)
			{
				if (it->first > firstID)
				{
					behaviorlines.insert(behaviorlines.end(), it->second.begin(), it->second.end());

					if (error)
					{
						emit done();
						return;
					}
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

			PatchDebug("Processing behavior: " + filepath + " (Check point 5, Behavior combine complete)");
			emit progressAdd();

			if (behaviorPath[lowerBehaviorFile].size() == 0)
			{
				ErrorMessage(1068, behaviorFile);
				emit done();
				return;
			}

			string filename = "temp_behaviors\\xml\\" + modID + lowerBehaviorFile;

			if (!FolderCreate(GetFileDirectory(filename)))
			{
				emit done();
				return;
			}
			else
			{
				if (!FolderCreate(GetFileDirectory(outputdir)))
				{
					emit done();
					return;
				}
			}

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

			PatchDebug("Processing behavior: " + filepath + " (Check point 6, Behavior output complete)");
			emit progressAdd();

			if (hkxcmdProcess(filename, outputdir))
			{
				PatchDebug("Processing behavior: " + filepath + " (Check point 7, Behavior compile complete)");
				emit progressAdd();
			}
		}
		catch (exception& ex)
		{
			ErrorMessage(6002, ex.what());
		}
	}
	catch (...)
	{
		ErrorMessage(6002, "Unknown");
	}

	emit done();
}

void BehaviorSub::AnimDataCompilation()
{
	try
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

			emit progressAdd();

			{
				unordered_map<string, int> uCode;		// project, highest unique code
				unordered_map<string, unordered_map<string, string>> uniqueModCode;	// project, mod code, unique code
				unordered_map<string, unordered_map<string, vecstr>> catalystMap;	// project, header, list of lines
				unordered_map<string, vecstr> animDataHeader;						// project, list of headers
				unordered_map<string, vecstr> animDataInfo;							// project, list of info headers

				{
					// read behavior file
					vecstr catalyst;
					vecstr newline;
					unordered_map<string, string> chosenLines;

					if (!GetFunctionLines(filepath, catalyst))
					{
						emit done();
						return;
					}

					PatchDebug("Processing behavior: " + filepath + " (Check point 1, File extraction complete)");
					emit progressAdd();

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

					emit progressAdd();

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

								if (line.find("//* delete this line *//") != NOT_FOUND)
								{
									break;
								}

								if (l + 3 < catalyst.size() && l > 2)
								{
									bool empty = false;

									if (catalyst[l - 1] == "")
									{
										empty = true;
									}
									else
									{
										int next = -1;

										while (l + next >= 0 && catalyst[l + next].find("<!--") != NOT_FOUND)
										{
											--next;
										}

										if (catalyst[l + next] == "")
										{
											empty = true;
										}
									}

									if (empty)
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

													if (catalyst[l + next] == "0" || (l + nextnext < catalyst.size() &&
														catalyst[l + nextnext].find("\\") != NOT_FOUND))
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
													if (isOnlyNumber(catalyst[l + 1]))		// next anim header
													{
														int number = stoi(catalyst[l + 1]);
														newline.shrink_to_fit();
														catalystMap[project][header] = newline;
														newline.reserve(20);
														newline.clear();
														header = line + " " + catalyst[l + 1];
														animDataHeader[project].push_back(header);

														if (number > uCode[project])
														{
															uCode[project] = number;
														}
													}
													else		// new anim header added by mod
													{
														string number = boost::regex_replace(string(catalyst[l + 1]),
															boost::regex("[a-zA-Z]+[$]([0-9]+)"), string("\\1"));

														if (number != catalyst[l + 1] && isOnlyNumber(number))
														{
															string modcode = catalyst[l + 1];
															catalyst[l + 1] = to_string(++uCode[project]);
															uniqueModCode[project][modcode] = catalyst[l + 1];
															newline.shrink_to_fit();
															catalystMap[project][header] = newline;
															newline.reserve(20);
															newline.clear();
															header = line + " " + catalyst[l + 1];
															animDataHeader[project].push_back(header);
														}
													}
												}
												else if (isOnlyNumber(line))	// is info
												{
													isInfo = true;
													newline.shrink_to_fit();
													catalystMap[project][header] = newline;;
													newline.reserve(20);
													newline.clear();
													string number = boost::regex_replace(string(catalyst[++l]), boost::regex("[a-zA-Z]+[$]([0-9]+)"), string("\\1"));

													if (number != catalyst[l] && isOnlyNumber(number))
													{
														if (uniqueModCode[project].find(catalyst[l]) != uniqueModCode[project].end())
														{
															catalyst[l] = uniqueModCode[project][catalyst[l]];
														}
														else
														{
															WarningMessage(1024, catalyst[l]);
															uniqueModCode[project][catalyst[l]] = to_string(++uCode[project]);
															catalyst[l] = to_string(uCode[project]);
														}
													}

													line = catalyst[l];
													header = line;
													animDataInfo[project].push_back(header);
												}
											}
											else if (isOnlyNumber(line))	// next info
											{
												newline.shrink_to_fit();
												catalystMap[project][header] = newline;
												newline.reserve(20);
												newline.clear();
												header = line;
												animDataInfo[project].push_back(header);
											}
											else		// new info added by mod
											{
												string number = boost::regex_replace(string(line), boost::regex("[a-zA-Z]+[$]([0-9]+)"), string("\\1"));

												if (number != line && isOnlyNumber(number))
												{
													if (uniqueModCode[project].find(line) != uniqueModCode[project].end())
													{
														line = uniqueModCode[project][line];
													}
													else
													{
														WarningMessage(1024, line);
														uniqueModCode[project][line] = to_string(++uCode[project]);
														line = to_string(uCode[project]);
													}

													newline.shrink_to_fit();
													catalystMap[project][header] = newline;
													newline.reserve(20);
													newline.clear();
													header = line;
													animDataInfo[project].push_back(header);
												}
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

												if (number > uCode[project])
												{
													uCode[project] = number;
												}
											}
											else
											{
												string number = boost::regex_replace(string(catalyst[l + 1]), boost::regex("[a-zA-Z]+[$]([0-9]+)"), string("\\1"));

												if (number != catalyst[l + 1] && isOnlyNumber(number))
												{
													string modcode = catalyst[l + 1];
													catalyst[l + 1] = to_string(++uCode[project]);
													uniqueModCode[project][modcode] = catalyst[l + 1];
													newline.shrink_to_fit();
													catalystMap[project][header] = newline;
													newline.reserve(20);
													newline.clear();
													header = line + " " + catalyst[l + 1];
													animDataHeader[project].push_back(header);
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

													if (catalyst[l + next] == "0" || (l + nextnext < catalyst.size() &&
														catalyst[l + nextnext].find("\\") != NOT_FOUND))	// next project
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

							if (error)
							{
								emit done();
								return;
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

				PatchDebug("Processing behavior: " + filepath + " (Check point 2, AnimData general processing complete)");
				emit progressAdd();

				// check for having newAnimation for the file
				if (BehaviorTemplate.grouplist.find(lowerBehaviorFile) != BehaviorTemplate.grouplist.end() &&
					BehaviorTemplate.grouplist[lowerBehaviorFile].size() > 0)
				{
					unordered_map<string, unordered_map<string, vector<map<int, vecstr>>>> editExtract;
					unordered_map<string, unordered_map<string, int>> ASDCount;

					for (auto& templateCode : BehaviorTemplate.grouplist[lowerBehaviorFile])
					{
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
										projectplus = project + " " + to_string(++projectNameCount[project]);
										projectList.push_back(project);
									}
									else
									{
										projectplus = project + " " + to_string(projectNameCount[project]);
									}

									for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
									{
										size_t size = iter->second.size();

										if (size > 0)
										{
											if (size > 2 && iter->first[size - 3] == '$' && iter->first[size - 2] == 'U' && iter->first.back() == 'C')		// info data
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
											else	// anim data
											{
												if (iter->first[0] == '$' && iter->first.back() == '$')
												{
													header = iter->second[0];
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
													catalystMap[project + " " + to_string(i)][header] = iter->second;
												}
											}
											else
											{
												catalystMap[projectplus][header] = iter->second;
											}
										}
									}
								}

								for (auto it = BehaviorTemplate.existingAnimDataHeader[templateCode].begin();
									it != BehaviorTemplate.existingAnimDataHeader[templateCode].end(); ++it)
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
						else
						{
							for (auto it = BehaviorTemplate.existingAnimDataHeader[templateCode].begin();
								it != BehaviorTemplate.existingAnimDataHeader[templateCode].end(); ++it)
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

									editExtract[project][header].push_back(*new map<int, vecstr>());

									if (error)
									{
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
					}

					emit progressAdd();

					for (auto& project : editExtract)
					{
						for (auto& header : project.second)
						{
							map<int, vecstr> combined;
							int totalline = 0;

							for (auto& list : header.second)
							{
								vecstr newline;
								string line;

								for (auto& data : list)
								{
									combined[data.first].insert(combined[data.first].end(), data.second.begin(), data.second.end());
									totalline += int(data.second.size());
								}
							}

							if (totalline == 0 && header.second.size() > 0 && header.second[0].size() > 0)
							{
								ErrorMessage(5014, project.first, header.first);
								emit done();
								return;
							}

							for (int i = 1; i <= projectNameCount[project.first]; ++i)
							{
								combineExtraction(catalystMap[project.first + " " + to_string(i)][header.first], combined, project.first, header.first);
							}

							if (error)
							{
								emit done();
								return;
							}
						}

						if (error)
						{
							emit done();
							return;
						}
					}
				}
				else
				{
					emit progressAdd();
				}

				PatchDebug("Processing behavior: " + filepath + " (Check point 3, AnimData general new animations complete)");
				emit progressAdd();

				if (alternateAnim.size() > 0)
				{
					PatchDebug("Processing behavior: " + filepath + " (Check point 3.4, AnimData general new animations complete)");

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

							if (error)
							{
								emit done();
								return;
							}
						}

						if (error)
						{
							emit done();
							return;
						}
					}

					PatchDebug("Processing behavior: " + filepath + " (Check point 3.5, AnimData AA complete)");
				}

				emit progressAdd();

				try
				{
					for (unsigned int i = 0; i < projectList.size(); ++i)
					{
						if (projectNameCount[projectList[i]] > 1)
						{
							project = projectList[i] + " " + to_string(++nextProject[projectList[i]]);
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

						if (error)
						{
							emit done();
							return;
						}

						try
						{
							AnimDataProject newProject(combined, projectList[i], filepath);
							ADProject.push_back(newProject);
						}
						catch (int)
						{
							emit done();
							return;
						}

						if (error)
						{
							emit done();
							return;
						}
					}
				}
				catch (int)
				{
					emit done();
					return;
				}
			}

			PatchDebug("Processing behavior: " + filepath + " (Check point 4, AnimData format check complete)");
			emit progressAdd();

			// final output	
#ifdef DEBUG
			filepath = "new_behaviors\\" + behaviorPath[lowerBehaviorFile].substr(behaviorPath[lowerBehaviorFile].find("\\") + 1);
#else
			filepath = behaviorPath[lowerBehaviorFile];
#endif

			{
				if (!FolderCreate(GetFileDirectory(filepath)))
				{
					emit done();
					return;
				}
			}

			ofstream output(filepath + ".txt");

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

							if (error)
							{
								emit done();
								return;
							}
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

							if (error)
							{
								emit done();
								return;
							}
						}
					}

					if (error)
					{
						emit done();
						return;
					}
				}

				output.close();
			}
			else
			{
				ErrorMessage(1025, filepath);
				emit done();
				return;
			}

			PatchDebug("Processing behavior: " + filepath + " (Check point 5, AnimData output complete)");
			emit progressAdd();
		}
		catch (exception& ex)
		{
			ErrorMessage(6002, ex.what());
		}
	}
	catch (...)
	{
		ErrorMessage(6002, "New animation: Unknown");
	}

	emit done();
}

void BehaviorSub::ASDCompilation()
{
	try
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

			emit progressAdd();

			{
				// read behavior file
				vecstr catalyst;
				vecstr newline;
				vecstr storeline;

				if (!GetFunctionLines(filepath, catalyst, false))
				{
					emit done();
					return;
				}

				while (catalyst.back().length() == 0)
				{
					catalyst.pop_back();
				}

				PatchDebug("Processing behavior: " + filepath + " (Check point 1, File extraction complete)");
				emit progressAdd();

				// add picked behavior and remove not picked behavior 
				// separation of all items for easier access and better compatibility
				for (unsigned int l = 0; l < catalyst.size(); ++l)
				{
					line = catalyst[l];
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

							if (line.find("//* delete this line *//") != NOT_FOUND)
							{
								break;
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

					if (error)
					{
						emit done();
						return;
					}
				}

				PatchDebug("Processing behavior: " + filepath + " (Check point 2, AnimSetData mod selection complete)");
				unordered_map<string, vecstr> animDataSetHeader;
				animDataSetHeader[project].push_back(header);

				emit progressAdd();

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
									ErrorMessage(5015, filepath, i + 1);
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
								ErrorMessage(5020, filepath, i + 1);
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
							ErrorMessage(5015, filepath, i + 1);
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

					if (error)
					{
						emit done();
						return;
					}

					newline.push_back(storeline[i]);
				}

				emit progressAdd();

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
									ErrorMessage(5015, filepath, i + 1);
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
								ErrorMessage(5020, filepath, i + 1);
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
							ErrorMessage(5015, filepath, i + 1);
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

					if (error)
					{
						emit done();
						return;
					}

					newline.push_back(storeline[i]);
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

				PatchDebug("Processing behavior: " + filepath + " (Check point 3, AnimSetData general processing complete)");
				emit progressAdd();
			}

			// check for having newAnimation for the file
			if (BehaviorTemplate.grouplist.find(lowerBehaviorFile) != BehaviorTemplate.grouplist.end() && BehaviorTemplate.grouplist[lowerBehaviorFile].size() > 0)
			{
				unordered_map<string, unordered_map<string, vector<map<int, vecstr>>>> editExtract;	// project, header, list of extracts, startline, extractions;
																									// to get all edits done to master branch

				for (auto& templateCode : BehaviorTemplate.grouplist[lowerBehaviorFile])
				{
					unordered_map<string, unordered_map<string, int>> ASDCount;
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
								string interproject = it->first;

								if (ASDPack.find(interproject) == ASDPack.end())
								{
									projectList.push_back(interproject);
								}

								for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
								{
									string interheader = iter->first.substr(1, iter->first.length() - 2);
									interheader.append("_" + to_string(++ASDCount[interproject][interheader]) + ".txt");

									if (ASDPack[interproject][interheader].size() > 0)
									{
										ErrorMessage(5012, templateCode, interproject, interheader);
										emit done();
										return;
									}

									if (error)
									{
										emit done();
										return;
									}

									ASDPack[interproject][interheader] = iter->second;
								}
							}

							PatchDebug("Processing behavior: " + filepath + " (Check point 3.5, Mod code: " + templateCode + ", Existing AnimData" +
								to_string(BehaviorTemplate.existingFunctionID[templateCode][lowerBehaviorFile].size()) + ")");

							for (auto it = BehaviorTemplate.existingASDHeader[templateCode].begin(); it != BehaviorTemplate.existingASDHeader[templateCode].end(); ++it)
							{
								string interproject = it->first;

								for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
								{
									string interheader = *iter;

									if (ASDPack[interproject][interheader].size() == 0)
									{
										ErrorMessage(5011, templateCode, interproject, interheader);
										emit done();
										return;
									}

									map<int, vecstr> extract;
									newAnimation[templateCode][k]->existingASDProcess(ASDPack[interproject][interheader], extract, vector<int>(1));

									if (error)
									{
										emit done();
										return;
									}

									editExtract[interproject][interheader].push_back(extract);
								}
							}

							if (error)
							{
								emit done();
								return;
							}

							PatchDebug("Processing behavior: " + filepath + " (Check point 3.5, Mod code: " + templateCode + ", Existing AnimData" +
								to_string(BehaviorTemplate.existingFunctionID[templateCode][lowerBehaviorFile].size()) + " COMPLETE)");
						}
					}
					else
					{
						for (auto it = BehaviorTemplate.existingASDHeader[templateCode].begin(); it != BehaviorTemplate.existingASDHeader[templateCode].end(); ++it)
						{
							string interproject = it->first;

							for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
							{
								string interheader = *iter;

								if (ASDPack[interproject][interheader].size() == 0)
								{
									ErrorMessage(5011, templateCode, interproject, interheader);
									emit done();
									return;
								}

								editExtract[interproject][interheader].push_back(*new map<int, vecstr>());

								if (error)
								{
									emit done();
									return;
								}
							}
						}

					}
				}

				emit progressAdd();

				for (auto& project : editExtract)
				{
					for (auto& header : project.second)
					{
						map<int, vecstr> combined;
						int totalline = 0;

						for (auto& list : header.second)
						{
							vecstr newline;
							string line;

							for (auto& data : list)
							{
								combined[data.first].insert(combined[data.first].end(), data.second.begin(), data.second.end());
								totalline += int(data.second.size());
							}
						}

						if (totalline == 0 && header.second.size() > 0 && header.second[0].size() > 0)
						{
							ErrorMessage(5014, project.first, header.first);
							emit done();
							return;
						}

						combineExtraction(ASDPack[project.first][header.first], combined, project.first, header.first);

						if (error)
						{
							emit done();
							return;
						}
					}

					if (error)
					{
						emit done();
						return;
					}
				}
			}
			else
			{
				emit progressAdd();
			}

			PatchDebug("Processing behavior: " + filepath + " (Check point 4, AnimSetData new animations complete)");
			emit progressAdd();

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

				if (error)
				{
					emit done();
					return;
				}

				size_t pos = projectList[i].find_last_of("\\") + 1;
				string projectFile = projectList[i].substr(pos, projectList[i].find_last_of(".") - pos);
				boost::to_lower(projectFile);

				try
				{
					AnimationDataProject newProject(startline, projectline, filepath, projectFile);
					ASDData[projectList[i]] = newProject;
				}
				catch (int)
				{
					emit done();
					return;
				}

				if (error)
				{
					emit done();
					return;
				}
			}

			PatchDebug("Processing behavior: " + filepath + " (Check point 5, AnimSetData format check complete)");
			emit progressAdd();

			// final output	
#ifdef DEBUG
			filepath = "new_behaviors\\" + behaviorPath[lowerBehaviorFile].substr(behaviorPath[lowerBehaviorFile].find("\\") + 1);
#else
			filepath = behaviorPath[lowerBehaviorFile];
#endif

			{
				if (!FolderCreate(GetFileDirectory(filepath)))
				{
					emit done();
					return;
				}
			}

			ofstream output(filepath + ".txt");

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

					for (auto& data : ASDData[curProject].datalist)
					{
						fwriter << data.first << "\n";
					}

					for (auto& data : ASDData[curProject].datalist)
					{
						fwriter << "V3" << "\n";
						fwriter << to_string(data.second.equiplist.size()) << "\n";

						for (auto& equip : data.second.equiplist)
						{
							fwriter << equip.name << "\n";
						}

						fwriter << to_string(data.second.typelist.size()) << "\n";

						for (auto& type : data.second.typelist)
						{
							fwriter << type.name << "\n";
							fwriter << type.equiptype1 << "\n";
							fwriter << type.equiptype2 << "\n";
						}

						fwriter << to_string(data.second.animlist.size()) << "\n";

						for (auto& anim : data.second.animlist)
						{
							fwriter << anim.name << "\n";
							fwriter << anim.unknown << "\n";
							fwriter << to_string(anim.attack.size()) << "\n";

							for (auto& atk : anim.attack)
							{
								fwriter << atk.data << "\n";
							}
						}

						fwriter << to_string(data.second.crc32list.size()) << "\n";

						for (auto& crc32 : data.second.crc32list)
						{
							fwriter << crc32.filepath << "\n";
							fwriter << crc32.filename << "\n";
							fwriter << crc32.fileformat << "\n";
						}

						if (error)
						{
							emit done();
							return;
						}
					}

					if (error)
					{
						emit done();
						return;
					}
				}

				output.close();
			}
			else
			{
				ErrorMessage(1025, filepath);
				emit done();
				return;
			}

			PatchDebug("Processing behavior: " + filepath + " (Check point 6, AnimSetData output complete)");
			emit progressAdd();
		}
		catch (exception& ex)
		{
			ErrorMessage(6002, ex.what());
		}
	}
	catch (...)
	{
		ErrorMessage(6002, "New animation: Unknown");
	}

	emit done();
}

void BehaviorSub::addInfo(string& newDirectory, vecstr& newfilelist, int newCurList, vecstr& newBehaviorPriority, unordered_map<string,
	bool>& newChosenBehavior, getTemplate& newBehaviorTemplate, unordered_map<string, vector<shared_ptr<Furniture>>>& addAnimation,
	unordered_map<string, var>& newAnimVar, mapSetString& addAnimEvent, mapSetString& addAnimVariable, unordered_map<string, unordered_map<int,
	bool>>& newIgnoreFunction, bool newIsCharacter, string newModID)
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
							if (n_stateID[match] == sID && ID == match)
							{
								skip = true;
								break;
							}
							else if (ID != match)
							{
								skip = true;
								break;
							}
						}
					}
				}
			}
		}

		if (error)
		{
			return;
		}
	}

	if (!skip)
	{
		duplicatedStateList[filename][ID][sID].insert(modcode);
		duplicatedStateList[filename][ID][sID].insert("Vanilla");
	}
}

bool readMod(string& errormod, string& errormsg)
{
	string folder = "mod\\";
	vecstr modlist;
	read_directory(folder, modlist);

	for (auto& modcode : modlist)
	{
		if (boost::filesystem::is_directory(folder + modcode) && isFileExist(folder + modcode + "\\info.ini"))
		{
			string filename = folder + modcode + "\\info.ini";
			vecstr storeline;
			string name, author, site, automatic, hide;
			bool hidden = false;

			if (!GetFunctionLines(filename, storeline, false))
			{
				return false;
			}

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

					pos = wordFind(line, "hidden=");

					if (pos == 0)
					{
						hide = line.substr(line.find("=") + 1);
						boost::to_lower(hide);
						istringstream stream(hide);
						stream >> boolalpha >> hidden;
					}

					break;
				}
			}

			if (name.length() == 0 || author.length() == 0 || site.length() == 0)
			{
				errormsg = "Missing info in \"info.ini\" file. Please contact the mod author\nMod: ";
				errormod = modcode;
				return false;
			}

			if (!hidden)
			{
				modinfo[modcode].push_back(name + " (" + site + ")");
				modinfo[modcode].push_back(author);
				modinfo[modcode].push_back(automatic);
			}
			else
			{
				hiddenMods.push_back(modcode);
			}
		}
	}

	return true;
}

void groupThreadStart(newGroupArgs args)
{
	try
	{
		try
		{
			args.groupTemp->setZeroEvent(args.ZeroEvent);
			args.groupTemp->setZeroVariable(args.ZeroVariable);
			args.groupTemp->getFunctionLines(args.allEditLines, args.lowerBehaviorFile, args.filename, args.stateID, args.groupFunctionIDs, args.groupAnimInfo,
				args.lastID, args.exportID, args.eventid, args.variableid, args.templateCode, args.atomicLock, args.groupCount);
		}
		catch (const exception& ex)
		{
			ErrorMessage(6002, ex.what());
		}
	}
	catch (...)
	{
		ErrorMessage(6002, "New animation: Unknown");
	}
}

void animThreadStart(newAnimArgs args)
{
	try
	{
		try
		{
			if (args.core)
			{
				args.atomicLock.lockCore();

				if (args.isCoreDone)
				{
					return;
				}
				else
				{
					args.isCoreDone = true;
				}

				args.atomicLock.releaseCore();
			}

			// getlines from newAnination
			args.dummyAnimation->setZeroEvent(args.ZeroEvent);
			args.dummyAnimation->setZeroVariable(args.ZeroVariable);
			args.dummyAnimation->GetFurnitureLine(args.allEditLines, args.lowerBehaviorFile, args.lastID, args.exportID, args.eventid, args.variableid, args.stateID,
				args.stateMultiplier, args.hasGroup, args.core, args.subFunctionIDs, args.singleFunctionIDs, args.atomicLock);
		}
		catch (exception& ex)
		{
			ErrorMessage(6002, ex.what());
		}
	}
	catch (...)
	{
		ErrorMessage(6002, "New animation: Unknown");
	}
}

vecstr getHiddenMods()
{
	return hiddenMods;
}

void DummyLog::message(std::string input)
{
	emit incomingMessage(QString::fromStdString(input));
}

void InstallScripts::Run()
{
	try
	{
		try
		{
			if (alternateAnim.size() > 0)
			{
				if (!AAInstallation())
				{
					emit end();
					return;
				}
			}
		}
		catch (exception& ex)
		{
			ErrorMessage(6002, ex.what());
		}
	}
	catch (...)
	{
		ErrorMessage(6002, "AA animation: Unknown");
	}

	if (error)
	{
		emit end();
		return;
	}

	try
	{
		try
		{
			if (pcealist.size() > 0)
			{
				PCEAInstallation();
			}
		}
		catch (exception& ex)
		{
			ErrorMessage(6002, ex.what());
		}
	}
	catch (...)
	{
		ErrorMessage(6002, "PCEA animation: Unknown");
	}

	emit end();
}
