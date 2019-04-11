#include <boost/atomic.hpp>
#include "master.h"
#include "NemesisMainGUI.h"
#include "functions\readtextfile.h"
#include "version.h"
#include "utilities\addanims.h"
#include "utilities\addevents.h"
#include "utilities\addvariables.h"
#include "add animation\templatetree.h"
#include "add animation\singletemplate.h"
#include "add animation\animationthread.h"
#include "functions\atomiclock.h"
#include "src\DebugLog.h"
#include <QThread>
#include <QtCore\QCoreApplication.h>
#include <QtConcurrent\qtconcurrentrun.h>

#pragma warning(disable:4503)

using namespace std;

unordered_map<string, vecstr> modinfo;
mutex processlock;
condition_variable cv;
bool processdone = false;
Terminator* p_terminate = new Terminator;
atomic<int> m_RunningThread;
atomic<int> extraCore = 0;
vecstr hiddenMods;
boost::atomic_flag atomic_lock = BOOST_ATOMIC_FLAG_INIT;

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

void groupThreadStart(newGroupArgs args);
void animThreadStart(newAnimArgs args);
void stateCheck(SSMap& c_parent, string parentID, string lowerbehaviorfile, string sID, SSSMap& stateID, SSMap& n_stateID, vecstr children, string filename,
	string ID, string modcode, unordered_map<string, map<string, unordered_map<string, set<string>>>>& duplicatedStateList);
void elementUpdate(size_t& elementLine, int& counter, int& curID, map<int, vecstr>& catalystMap);

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
	try
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
					if (!isFileExist(directory))
					{
						boost::filesystem::create_directory(directory);
					}

					emit progressUp();

					// copy latest vanilla into memory
					if (!error && VanillaUpdate(newFile, childrenState, stateID, parent, animData, animSetData))
					{
						if (newFile.size() == 0) ErrorMessage(2024);

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

				if (!cmdline)
				{
					Sleep(1500);
				}
			}
			catch (exception& ex)
			{
				ErrorMessage(6001, ex.what());
			}
		}
		catch (nemesis::exception) {}
	}
	catch (...)
	{
		try
		{
			ErrorMessage(6001, "Unknown: Update Failed");
		}
		catch (nemesis::exception) {}
	}

	unregisterProcess();
	p_terminate->exitSignal();
}

bool UpdateFilesStart::VanillaUpdate(unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string, map<string, unordered_map<string,
	bool>>>& childrenState, SSSMap& stateID, SSSMap& parent, MasterAnimData& animData, MasterAnimSetData& animSetData)
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
		if (!GetPathLoop(path + "\\", newFile, childrenState, stateID, parent, animData, animSetData, false))
		{
			return false;
		}

		emit progressUp();		// 4

		if (behaviorPath.size() != 0)
		{
			FileWriter output("behavior_path");

			if (output.is_open())
			{
				for (auto it = behaviorPath.begin(); it != behaviorPath.end(); ++it)
				{
					output << it->first << "=" << it->second << "\n";
				}
			}
			else
			{
				ErrorMessage(2009, "behavior_path");
			}
		}

		if (behaviorProject.size() != 0)
		{
			FileWriter output("behavior_project");

			if (output.is_open())
			{
				for (auto it = behaviorProject.begin(); it != behaviorProject.end(); ++it)
				{
					output << it->first << "\n";

					for (unsigned int i = 0; i < it->second.size(); ++i)
					{
						output << it->second[i] << "\n";
					}

					output << "\n";
				}
			}
			else
			{
				ErrorMessage(2009, "behavior_project");
			}
		}

		if (behaviorProjectPath.size() != 0)
		{
			FileWriter output("behavior_project_path");

			if (output.is_open())
			{
				for (auto it = behaviorProjectPath.begin(); it != behaviorProjectPath.end(); ++it)
				{
					output << it->first << "=" << it->second << "\n";
				}
			}
			else
			{
				ErrorMessage(2009, "behavior_project_path");
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

								if (!AnimDataDisassemble(newPath, animData)) return false;

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

								if (!AnimSetDataDisassemble(newPath, animSetData)) return false;

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

								if (!VanillaDisassemble(newPath, smallfile, newFile[smallfile], childrenState, stateID[smallfile], parent[smallfile])) return false;

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
							try
							{
								multithreads.create_thread(boost::bind(&UpdateFilesStart::GetPathLoop, this, newPath + "\\", boost::ref(newFile), boost::ref(childrenState),
									boost::ref(stateID), boost::ref(parent), boost::ref(animData), boost::ref(animSetData), true));
							}
							catch (boost::thread_exception&)
							{
								GetPathLoop(newPath + "\\", newFile, childrenState, stateID, parent, animData, animSetData, true);
							}
						}
						else
						{
							// look deeper into the folder for behavior file
							try
							{
								multithreads.create_thread(boost::bind(&UpdateFilesStart::GetPathLoop, this, newPath + "\\", boost::ref(newFile), boost::ref(childrenState),
									boost::ref(stateID), boost::ref(parent), boost::ref(animData), boost::ref(animSetData), isFirstPerson));
							}
							catch (boost::thread_exception&)
							{
								GetPathLoop(newPath + "\\", newFile, childrenState, stateID, parent, animData, animSetData, isFirstPerson);
							}
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
		catch (nemesis::exception) {}
	}
	catch (...)
	{
		try
		{
			ErrorMessage(6001, "Unknown: GetPathLoop");
		}
		catch (nemesis::exception) {}
	}
	
	multithreads.join_all();
	return true;
}

bool UpdateFilesStart::VanillaDisassemble(string path, string filename, map<string, vecstr>& newFile,
	unordered_map<string, map<string, unordered_map<string, bool>>>& childrenState, SSMap& stateID, SSMap& parent)
{
	vecstr storeline;
	storeline.reserve(2000);
	char line[2000];

	shared_ptr<FileReader> vanillafile = make_shared<FileReader>(path);
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

			if (curline.length() > 0 && curline.back() == '\n') curline.pop_back();

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
									for (auto& it = boost::sregex_iterator(curline.begin(), curline.end(), boost::regex("([0-9]+(\\.[0-9]+)?)")); it != boost::sregex_iterator(); ++it)
									{
										storeline.push_back(spaces + it->str(1));
										bone = true;
									}
								}
							}
							else
							{
								if (isVector4)
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
								newFile[curID] = storeline;
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
			newFile[curID] = storeline;
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

	if (!GetFunctionLines(path, storeline))
	{
		return false;
	}
	
	{
		string strnum = boost::regex_replace(string(storeline[0]), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));

		if (!isOnlyNumber(strnum) || stoi(strnum) < 10)
		{
			ErrorMessage(3014);
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
	try
	{
		try
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
			shared_ptr<UpdateLock> modUpdate(pack.modUpdate);

			directory = pack.directory;
			modcode = pack.modcode;
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
						if (animData.newAnimData.size() == 0)
						{
							ErrorMessage(3017, "nemesis_animationdatasinglefile.txt");
						}

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
					else if (boost::iequals(behaviorfile, "animationsetdatasinglefile"))
					{
						if (animSetData.newAnimSetData.size() == 0)
						{
							ErrorMessage(3017, "nemesis_animationsetdatasinglefile.txt");
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

										if (sID.empty())
										{
											ErrorMessage(1188, modcode, filename);
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
										if (ID.first.find("$") != NOT_FOUND)
										{
											skip = true;
										}
										else
										{
											ErrorMessage(1191, modcode, ID.first);
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
							if (!(*modUpdate)[lowerbehaviorfile + node.substr(0, node.find_last_of("."))].FunctionUpdate(modcode, lowerbehaviorfile, node, ref(newFile), n_stateID,
								c_parent, statelist, ref(lastUpdate)))
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

									if (sID.empty())
									{
										ErrorMessage(1188, modcode, filename);
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

			DebugLogging("Mod Installed sucessfully: " + modcode);
		}
		catch (exception& ex)
		{
			ErrorMessage(6001, ex.what());
		}
	}
	catch (nemesis::exception&) {}
}

void UpdateFilesStart::JoiningEdits(string directory, unordered_map<string, map<string, vecstr>>& newFile, unordered_map<string,
	map<string, unordered_map<string, bool>>>& childrenState,  SSSMap& stateID, SSSMap& parent, MasterAnimData& animData, MasterAnimSetData& animSetData,
	unordered_map<string, string>& lastUpdate)
{
	try
	{
		try
		{
			if (isFileExist(directory))
			{
				vecstr filelist;
				boost::thread_group threads;
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
						arguPack pack(directory, filelist[i], filelist2, newFile, childrenState, stateID, parent, modStateList, duplicatedStateList, animData, animSetData, lastUpdate,
							modUpPtr);
						threads.create_thread(boost::bind(&UpdateFilesStart::SeparateMod, this, pack));
					}
				}

				emit progressUp();

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
			}

			FileWriter lastmod("engine_update");

			if (lastmod.is_open())
			{
				for (auto& it : lastUpdate)
				{
					lastmod << it.first << ">>" << it.second << "\n";
				}
			}
			else
			{
				ErrorMessage(2009, "engine_update");
			}

			emit progressUp();
		}
		catch (nemesis::exception) {}
	}
	catch (exception& ex)
	{
		try
		{
			ErrorMessage(6001, ex.what());
		}
		catch (nemesis::exception) {}
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

			if (lowerBehaviorFile.find("_1stperson\\") == 0)
			{
				boost::filesystem::create_directory(boost::filesystem::path(compilingfolder + "_1stperson\\"));
			}

			FileWriter output(filename);

			if (output.is_open())
			{
				bool behaviorRef = false;

				output << "<?xml version=\"1.0\" encoding=\"ascii\"?>" << "\n";
				output << "<hkpackfile classversion=\"8\" contentsversion=\"hk_2010.2.0 - r1\" toplevelobject=\"" << rootID << "\">" << "\n";
				output << "\n";
				output << "	<hksection name=\"__data__\">" << "\n";
				output << "\n";

				for (unsigned int linecount = 0; linecount < fileline.size(); ++linecount)
				{
					output << fileline[linecount] << "\n";
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

				output << "	</hksection>" << "\n";
				output << "\n";
				output << "</hkpackfile>" << "\n";
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
		FileWriter outputlist("animationdata_list");

		if (output.is_open())
		{
			if (outputlist.is_open())
			{
				output << to_string(animData.animDataChar.size() - 1) << "\n";
				
				for (unsigned int i = 0; i < animData.newAnimData["$header$"]["$header$"].size(); ++i)
				{
					output << animData.newAnimData["$header$"]["$header$"][i] << "\n";
				}

				for (unsigned int i = 1; i < animData.animDataChar.size(); ++i)
				{	// character
					string project = animData.animDataChar[i];
					outputlist << project << "\n";
					size_t animsize = 0;
					size_t infosize = 0;

					for (auto& header: animData.animDataHeader[project])
					{
						animsize += animData.newAnimData[project][header].size();
					}

					if (animsize > 0)
					{
						output << to_string(animsize) << "\n";

						for (unsigned int j = 0; j < animData.animDataHeader[project].size(); ++j)
						{
							string header = animData.animDataHeader[project][j];
							outputlist << header << "\n";

							for (unsigned int k = 0; k < animData.newAnimData[project][header].size(); ++k)
							{
								output << animData.newAnimData[project][header][k] << "\n";
							}
						}
					}

					for (auto& header : animData.animDataInfo[project])
					{
						infosize += animData.newAnimData[project][header].size();
					}

					if (infosize > 0)
					{
						output << to_string(infosize) << "\n";

						for (unsigned int j = 0; j < animData.animDataInfo[project].size(); ++j)
						{
							string header = animData.animDataInfo[project][j];
							outputlist << header << "\n";

							for (unsigned int k = 0; k < animData.newAnimData[project][header].size(); ++k)
							{
								output << animData.newAnimData[project][header][k] << "\n";
							}
						}

						outputlist << "\n";
					}
				}
			}
			else
			{
				ErrorMessage(2009, "animationdata_list");
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
		FileWriter outputlist("animationsetdata_list");

		if (output.is_open())
		{
			if (outputlist.is_open())
			{
				output << to_string(animSetData.projectList.size() - 1) << "\n";

				for (unsigned int i = 0; i < animSetData.newAnimSetData["$header$"]["$header$"].size(); ++i)
				{
					output << animSetData.newAnimSetData["$header$"]["$header$"][i] << "\n";
				}

				for (unsigned int i = 1; i < animSetData.projectList.size(); ++i)
				{
					string project = animSetData.projectList[i];
					outputlist << project << "\n";		// character
					outputlist << "$header$" << "\n";

					for (auto& line : animSetData.newAnimSetData[project]["$header$"])
					{
						output << line << "\n";
					}

					for (auto it = animSetData.newAnimSetData[project].begin(); it != animSetData.newAnimSetData[project].end(); ++it)
					{
						string header = it->first;

						if (header != "$header$")
						{
							outputlist << header << "\n";

							for (unsigned int k = 0; k < it->second.size(); ++k)
							{
								output << it->second[k] << "\n";
							}
						}
					}

					outputlist << "\n";
				}
			}
			else
			{
				ErrorMessage(2009, "animationsetdata_list");
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
							DebugLogging("New Animations extraction start (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");

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

									if (!newAnimDataSetUpdateExt(folderpath + "\\" + behaviorlist[j] + "\\" + projectfile[k], codelist[i],
										projectname + ".txt", animSetData, lastUpdate))
									{
										return false;
									}
								}
							}

							DebugLogging("New Animations extraction complete (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");
						}
						else
						{
							if (boost::iequals(behaviorlist[j], "_1stperson"))
							{
								ErrorMessage(6004, folderpath + "\\" + behaviorlist[j]);
							}

							DebugLogging("New Animations extraction start (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");

							if (!newAnimUpdateExt(folderpath, codelist[i], boost::to_lower_copy(behaviorlist[j]), newFile[boost::to_lower_copy(behaviorlist[j])], lastUpdate))
							{
								return false;
							}

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

	DebugLogging("Current Directory: " + QCoreApplication::applicationDirPath().toStdString());
	DebugLogging("Data Directory: " + nemesisInfo->GetDataPath());
	DebugLogging("Skyrim Special Edition: " + SSE ? "TRUE" : "FALSE");
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

					if (bf::is_directory(curPath))
					{
						++filenum;
					}
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
			try
			{
				milestoneStart();

				if (!isEngineUpdated())
				{
					interMsg(TextBoxMessage(1000));
					ClearGlobal();
					unregisterProcess(true);
					--m_RunningThread;
					return;
				}

				interMsg(TextBoxMessage(1003));
				interMsg("");
				interMsg(TextBoxMessage(1004));
				interMsg("");

				// Check the existence of required files
				FileCheck();

				// Script Run
				RunScript("scripts\\launcher\\start\\");
				ClearGlobal();
				characterHKX();
				GetBehaviorPath();
				GetBehaviorProject();
				GetBehaviorProjectPath();
				behaviorActivateMod(behaviorPriority);
				ClearTempXml();
				
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

				if (animationList.size() > 0 || pcealist.size() > 0)
				{
					interMsg("");
				}

				DebugLogging("Registering new animations...");

				// read each animation list file'
				for (unsigned int i = 0; i < animationList.size(); ++i)
				{
					set<string> coreRegistered;		// list of registered core behavior
					string modID = animationList[i]->modID;

					// read each animation in a group of the same type
					for (auto it = animationList[i]->templateType.begin(); it != animationList[i]->templateType.end(); ++it)
					{
						string firstAE;
						string coreModName = "";
						string templatecode = it->first;
						DebugLogging("Registering " + templatecode + " animations");
						vector<shared_ptr<animationInfo>> animInfoGroup;
						shared_ptr<unordered_map<string, AnimTemplate>> animTemplate = make_shared<unordered_map<string, AnimTemplate>>();	// behavior file, template
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
							DebugLogging(corename);
							DebugLogging(corepath);

							if (!isFileExist(corepath + "nemesis_" + corename + ".txt") && !isFileExist(corepath + "nemesis_" + corename + ".xml"))
							{
								WarningMessage(1004, BehaviorTemplate.optionlist[templatecode].coreBehavior, templatecode, corepath + "nemesis_" + corename + ".txt");
							}
							else
							{
								string corecode = BehaviorTemplate.coreBehaviorCode[BehaviorTemplate.coreTemplate[templatecode]];

								if (coreRegistered.find(corecode) == coreRegistered.end())
								{
									DebugLogging("Core Registration: " + corecode);
									coreRegistered.insert(corecode);
									unordered_map<string, vecstr>* c_functionlines = &BehaviorTemplate.behaviortemplate[corecode];
									shared_ptr<animationInfo> dummy = make_shared<animationInfo>();

									if (isFileExist(corepath + "FNIS_" + modID + "_" + BehaviorTemplate.optionlist[corecode].coreBehavior + ".hkx"))
									{
										dummy->addFilename("FNIS_" + modID + "_" + BehaviorTemplate.optionlist[corecode].coreBehavior + ".hkx");
									}
									else
									{
										dummy->addFilename(coreModName + BehaviorTemplate.optionlist[corecode].coreBehavior + ".hkx");
										coreModList[boost::to_lower_copy(BehaviorTemplate.optionlist[corecode].coreBehavior) + ".txt"].push_back(coreModName);
									}

									dummy->ignoreGroup = false;
									dummy->optionPickedCount[corecode] = 1;
									animationCount[corecode]++;
									
									if (BehaviorTemplate.optionlist[corecode].core)
									{
										newAnimation[corecode].emplace_back(make_unique<Furniture>(make_shared<unordered_map<string, AnimTemplate>>(), corecode,
											*c_functionlines, animationCount[corecode], "", *dummy));
									}
									else
									{
										// behavior file, template
										shared_ptr<unordered_map<string, AnimTemplate>> c_animTemplate = make_shared<unordered_map<string, AnimTemplate>>();

										for (auto& func : *c_functionlines)
										{
											if ((*c_animTemplate)[func.first].size == 0)
											{
												(*c_animTemplate)[func.first].hasDuration = false;
												(*c_animTemplate)[func.first].ExamineTemplate(templatecode, func.first, func.second, false, false,
													BehaviorTemplate.optionlist[corecode]);
											}
										}

										newAnimation[corecode].emplace_back(make_unique<Furniture>(c_animTemplate, corecode, unordered_map<string, vecstr>(),
											animationCount[corecode], "", *dummy));
									}

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

						for (auto& func : *functionlines)
						{
							if ((*animTemplate)[func.first].size == 0)
							{
								(*animTemplate)[func.first].ExamineTemplate(templatecode, func.first, func.second, false, false, BehaviorTemplate.optionlist[templatecode]);
							}
						}

						// read each line and generate lines categorized using template code (animation type)
						try
						{
							for (int j = 0; j < it->second; ++j)
							{
								int order = animationList[i]->isMulti[templatecode][j];
								int lastOrder = animationList[i]->last[templatecode][j];

								newAnimation[templatecode].emplace_back(make_unique<Furniture>(animTemplate, templatecode, *functionlines,
									animationCount[templatecode], modID + "\\", *animationList[i]->animInfo[templatecode][j]));
								newAnimation[templatecode].back()->addAnimData(*animdatalines);
								newAnimation[templatecode].back()->addAnimSetData(*asdlines);
								newAnimation[templatecode].back()->coreModID = coreModName;
								vecstr tempEventID = newAnimation[templatecode].back()->GetEventID();
								vecstr tempVariableID = newAnimation[templatecode].back()->GetVariableID();
								newAnimEvent[templatecode + coreModName].insert(tempEventID.begin(), tempEventID.end());
								newAnimVariable[templatecode + coreModName].insert(tempVariableID.begin(), tempVariableID.end());

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
													newAnimEvent[templatecode + coreModName].insert(animEvent[l]);
												}
											}
											else
											{
												newAnimEvent[templatecode + coreModName].insert(elementLine);
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
													newAnimVariable[templatecode + coreModName].insert(animVar[l]);
												}
											}
											else
											{
												newAnimVariable[templatecode + coreModName].insert(elementLine);
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
														newAnimEvent[templatecode + coreModName].insert(animEvent[l]);
													}
												}
												else
												{
													newAnimEvent[templatecode + coreModName].insert(elementLine);
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
														newAnimVariable[templatecode + coreModName].insert(animVar[l]);
													}
												}
												else
												{
													newAnimVariable[templatecode + coreModName].insert(elementLine);
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
												newAnimEvent[templatecode + coreModName].insert(animEvent[l]);
											}
										}
										else
										{
											newAnimEvent[templatecode + coreModName].insert(elementLine);
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
												newAnimVariable[templatecode + coreModName].insert(animVar[l]);
											}
										}
										else
										{
											newAnimVariable[templatecode + coreModName].insert(elementLine);
										}
									}
								}

								for (unsigned int k = 0; k < tempVariableID.size(); ++k)
								{
									string name = tempVariableID[k];
									AnimVar[name] = animationList[i]->AnimVar[name];
								}
							}
						}
						catch (nemesis::exception)
						{
							ErrorMessage(6000);
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
					DebugLogging("Process count: " + to_string(filenum));
					emit progressMax(filenum);
				}

				DebugLogging("New animations registration complete");
				emit progressUp();

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
					InstallScripts* ScriptWorker = new InstallScripts;		// install PCEA & AA script
					connect(ScriptWorker, SIGNAL(end()), behaviorProcess, SLOT(EndAttempt()));
					connect(ScriptWorker, SIGNAL(end()), ScriptWorker, SLOT(deleteLater()));
					QtConcurrent::run(ScriptWorker, &InstallScripts::Run);
				}

				{
					string filename(nemesisInfo->GetDataPath() + "scripts\\FNIS.pex");
					boost::filesystem::copy_file(boost::filesystem::path("alternate animation\\FNBE.pex"), filename, boost::filesystem::copy_option::overwrite_if_exists);
					FILE* f;
					fopen_s(&f, filename.c_str(), "r+b");

					if (f)
					{
						fseek(f, 20, SEEK_SET);
						unsigned char charcode = 73;
						fwrite(&charcode, sizeof(charcode), 1, f);

						fseek(f, 21, SEEK_SET);
						charcode = 83;
						fwrite(&charcode, sizeof(charcode), 1, f);

						fseek(f, 45, SEEK_SET);
						charcode = 73;
						fwrite(&charcode, sizeof(charcode), 1, f);

						fseek(f, 46, SEEK_SET);
						charcode = 83;
						fwrite(&charcode, sizeof(charcode), 1, f);

						fseek(f, 495, SEEK_SET);
						charcode = 49;
						fwrite(&charcode, sizeof(charcode), 1, f);

						fclose(f);
					}
					else
					{
						ErrorMessage(3002, filename);
					}
				}

				vecstr filelist;
				read_directory(directory, filelist);
				emit progressUp();

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

							if (error) throw nemesis::exception();

							BehaviorSub* worker = new BehaviorSub;
							worker->addInfo(directory, filelist, i, behaviorPriority, chosenBehavior, BehaviorTemplate, newAnimation, AnimVar, newAnimEvent, newAnimVariable,
								ignoreFunction, false, modID);

							if (!cmdline)
							{
								connect(worker, SIGNAL(progressAdd()), widget, SLOT(setProgressBarValue()));
							}

							connect(worker, SIGNAL(done()), behaviorProcess, SLOT(EndAttempt()));
							connect(worker, SIGNAL(done()), worker, SLOT(deleteLater()));

							if (boost::iequals(filelist[i], "animationdatasinglefile.txt"))
							{
								QtConcurrent::run(worker, &BehaviorSub::AnimDataCompilation);
							}
							else if (boost::iequals(filelist[i], "animationsetdatasinglefile.txt"))
							{
								QtConcurrent::run(worker, &BehaviorSub::ASDCompilation);
							}
							else if (temppath.find("characters") != 0)
							{
								QtConcurrent::run(worker, &BehaviorSub::BehaviorCompilation);
							}
							else
							{
								worker->isCharacter = true;
								connect(worker, SIGNAL(newAnim()), behaviorProcess, SLOT(increaseAnimCount()));
								QtConcurrent::run(worker, &BehaviorSub::BehaviorCompilation);
							}

							++repeatcount;
						}
					}
					else if (wordFind(filelist[i], "_1stperson") != NOT_FOUND)
					{
						vecstr fpfilelist;
						read_directory(directory + filelist[i], fpfilelist);

						for (auto& curfile : fpfilelist)
						{
							curfile = filelist[i] + "\\" + curfile;
						}

						for (unsigned int j = 0; j < fpfilelist.size(); ++j)
						{
							if (error) throw nemesis::exception();

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

									BehaviorSub* worker = new BehaviorSub;
									worker->addInfo(directory, fpfilelist, j, behaviorPriority, chosenBehavior, BehaviorTemplate, newAnimation, AnimVar, newAnimEvent,
										newAnimVariable, ignoreFunction, false, modID);

									if (!cmdline)
									{
										connect(worker, SIGNAL(progressAdd()), widget, SLOT(setProgressBarValue()));
									}

									connect(worker, SIGNAL(done()), behaviorProcess, SLOT(EndAttempt()));
									connect(worker, SIGNAL(done()), worker, SLOT(deleteLater()));

									if (boost::iequals(fpfilelist[j], "animationdatasinglefile.txt"))
									{
										QtConcurrent::run(worker, &BehaviorSub::AnimDataCompilation);
									}
									else if (boost::iequals(fpfilelist[j], "animationsetdatasinglefile.txt"))
									{
										QtConcurrent::run(worker, &BehaviorSub::ASDCompilation);
									}
									else if (temppath.find("characters") != 0)
									{
										QtConcurrent::run(worker, &BehaviorSub::BehaviorCompilation);
									}
									else
									{
										worker->isCharacter = true;
										connect(worker, SIGNAL(newAnim()), behaviorProcess, SLOT(increaseAnimCount()));
										QtConcurrent::run(worker, &BehaviorSub::BehaviorCompilation);
									}

									++repeatcount;
								}
							}
						}
					}
				}
			}
			catch (exception& ex)
			{
				ErrorMessage(6002, ex.what(), "None");
			}
		}
		catch (nemesis::exception) {}
	}
	catch (...)
	{
		try
		{
			ErrorMessage(6002, "Unknown", "None");
		}
		catch (nemesis::exception) {}
	}

	EndAttempt();
}

void BehaviorStart::milestoneStart()
{
	m_RunningThread = 1;
	PatchReset();
	start_time = boost::posix_time::microsec_clock::local_time();
	DebugLogging("Nemesis Behavior Version: v" + GetNemesisVersion());
	DebugLogging("Current Directory: " + QCoreApplication::applicationDirPath().toStdString());
	DebugLogging("Data Directory: " + nemesisInfo->GetDataPath());
	DebugLogging("Skyrim Special Edition: " + SSE ? "TRUE" : "FALSE");
	int counter = 0;

	for (auto& mod : behaviorPriority)
	{
		DebugLogging("Mod " + to_string(++counter) + ": " + mod);
	}

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
	}
	else
	{
		DebugLogging("Detecting processes...");
		read_directory(directory, filelist);

		if (isFileExist(directory + "\\xml")) --include;

		if (isFileExist(directory + "\\animationdatasinglefile.txt")) --add;

		if (isFileExist(directory + "\\animationsetdatasinglefile.txt")) --add;

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

void BehaviorStart::unregisterProcess(bool skip)
{
	if (!skip)
	{
		if (!error)
		{
			RunScript("scripts\\launcher\\end\\");

			if (error)
			{
				string msg = TextBoxMessage(1010);
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
					DebugLogging("Number of animations: " + to_string(animCount));
					int second = diff.total_milliseconds();

					if (second > 1000)
					{
						string milli = to_string(second % 1000);

						while (milli.length() < 3)
						{
							milli.insert(0, "0");
						}

						msg = TextBoxMessage(1009) + ": " + to_string(second / 1000) + "," + milli + " " + TextBoxMessage(1011);
					}
					else
					{
						msg = TextBoxMessage(1009) + ": " + to_string(second) + " " + TextBoxMessage(1011);
					}
				}
				else
				{
					DebugLogging("Number of animations: " + to_string(animCount));
					msg = TextBoxMessage(1009) + ": " + to_string(diff.total_seconds()) + " " + TextBoxMessage(1012);
				}

				interMsg(msg);
				DebugLogging(msg);
				emit enableCheck(false);
			}
		}
		else
		{
			string msg = TextBoxMessage(1010);
			interMsg(msg);
			DebugLogging(msg);
		}
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

	emit end();
}

void BehaviorStart::EndAttempt()
{
	while (atomic_lock.test_and_set(boost::memory_order_acquire));
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
		p_terminate->exitSignal();
	}

	atomic_lock.clear(boost::memory_order_release);
}

void BehaviorStart::increaseAnimCount()
{
	if (!error)
	{
		++animCount;
		emit totalAnim(animCount);
		boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
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
		++m_RunningThread;

		try
		{
			try
			{
				CompilingBehavior();
			}
			catch (exception& ex)
			{
				ErrorMessage(6002, ex.what(), filelist[curList]);
			}
		}
		catch (nemesis::exception) {}
	}
	catch (...)
	{
		try
		{
			ErrorMessage(6002, "Unknown", filelist[curList]);
		}
		catch (nemesis::exception) {}
	}

	emit done();
}

void BehaviorSub::CompilingBehavior()
{
	ImportContainer exportID;

	string filepath = directory + filelist[curList];
	string behaviorFile = filelist[curList].substr(0, filelist[curList].find_last_of("."));
	string lowerBehaviorFile = boost::algorithm::to_lower_copy(behaviorFile);

	bool isFirstPerson = false;

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
	if (error) throw nemesis::exception();

	DebugLogging("Processing behavior: " + filepath);
	DebugLogging("Behavior output path: " + outputdir);
	emit progressAdd();

	if (modID.length() > 0 && isFileExist(outputdir + ".hkx"))
	{
		int i = 0;

		while (i < 9)
		{
			emit progressAdd();
			++i;
		}

		return;
	}

	double duration;

	{
		bool hasAA = false;
		bool isOpen = true;
		bool special = false;
		bool newBone = false;

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

		if (lowerBehaviorFile.find("_1stperson") != NOT_FOUND)
		{
			isFirstPerson = true;
		}

		if (alternateAnim.size() != 0)
		{
			hasAA = true;
		}

		// read behavior file
		vecstr catalyst;
		vecstr modLine;

		{
			if (!boost::filesystem::is_directory(filepath))
			{
				size_t size = fileLineCount(filepath);
				catalyst.reserve(size);
				modLine.reserve(size);
				string line;
				char charline[2000];
				shared_ptr<FileReader> BehaviorFormat = make_shared<FileReader>(filepath);

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

								if (!chosenBehavior[mod]) isOpen = false;
								else newMod = mod;

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
									newMod.clear();
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

								modLine.push_back(newMod);
								catalyst.push_back(line);
								break;
							}
						}

						if (error) throw nemesis::exception();
					}
				}
				else
				{
					ErrorMessage(3002, filepath);
				}
			}
			else
			{
				ErrorMessage(3001, filepath);
			}

			if (catalyst.size() != 0 && catalyst.back().length() != 0 && catalyst.back().find("<!-- CONDITION END -->") == NOT_FOUND &&
				catalyst.back().find("<!-- CLOSE -->") == NOT_FOUND)
			{
				modLine.push_back(newMod);
				catalyst.push_back("");
			}
		}

		DebugLogging("Processing behavior: " + filepath + " (Check point 1, File extraction & mod selection complete)");

		if (isCharacter)
		{
			DebugLogging("Processing behavior: " + filepath + " (IsCharater: TRUE)");
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

							if (error) throw nemesis::exception();
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
					}
				}
				else
				{
					vecstr storeline;
					hkxcmdXmlInput(rigfile.substr(0, rigfile.find_last_of(".")), storeline);
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

				if (error) throw nemesis::exception();
			}

			DebugLogging("Processing behavior: " + filepath + " (Check point 1.5, Character bone identification complete)");
		}
		else
		{
			DebugLogging("Processing behavior: " + filepath + " (IsCharater: FALSE)");
		}

		if (error) throw nemesis::exception();

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

				if (!isFirstPerson)
				{
					auto replacer = animReplaced.find(animFile);

					if (replacer != animReplaced.end() && replacer->second.size())
					{
						pceaID.insert(make_pair(curID, &replacer->second));

						for (auto& data : replacer->second)
						{
							pceaMod.insert(data.modFile);
						}
					}
				}

				// Dynamic AA function without a template
				if (hasAA)
				{
					string tempAnimFile = isFirstPerson ? animFile + "_1p*" : animFile;
					auto aa = alternateAnim.find(tempAnimFile);

					if (aa != alternateAnim.end())
					{
						clipAA[curID][animFile] = aa->second;
						AAGroupList.insert(isFirstPerson ? AAGroup[animFile] + "_1p*" : AAGroup[animFile]);
						auto aaEvent = AAHasEvent.find(tempAnimFile);

						if (aaEvent != AAHasEvent.end())
						{
							for (string& AA_animName : aaEvent->second)
							{
								for (unsigned int k = 0; k < AAEvent[AA_animName].size(); ++k)
								{
									if (k % 2 == 0) AAEventName.insert(AAEvent[AA_animName][k]);
								}
							}
						}
					}
				}
			}

			if (error) throw nemesis::exception();
		}

		DebugLogging("Processing behavior: " + filepath + " (Check point 2, ID replacement & PCEA record complete)");

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
				}
			}
			else
			{
				firstID = stoi(boost::regex_replace(string(catalyst[1].substr(pos)), boost::regex("[^0-9]*([0-9]+).*"), string("\\1")));
			}
		}

		curID = firstID;
		bool doneEventName = false;
		bool doneEventInfo = false;
		bool doneAttributeName = false;
		bool doneAttributeDef = false;
		bool doneCharacProName = false;
		bool doneCharacProInfo = false;
		bool doneVarName = false;
		bool doneVarWord = false;
		bool doneVarInfo = false;
		bool doneAnimName = false;
		bool doneDeform = false;
		bool doneRigid = false;
		bool doneAnimFileName = false;
		bool isClipTrigger = false;
		bool replacedNum = false;
		
		bool eventOpen = false;
		bool varOpen = false;
		bool attriOpen = false;
		bool charOpen = false;
		bool animOpen = false;
		bool fp_animOpen = false;
		bool otherAnimOpen = false;
		bool norElement = false;
		bool characterAA = false;

		// add picked behavior and remove not picked behavior
		for (unsigned int l = 0; l < catalyst.size(); ++l)
		{
			bool elementCatch = false;
			string line = catalyst[l];
			boost::smatch check;

			if (line.length() == 0)
			{
				catalystMap[curID].push_back(line);
			}
			else
			{
				newMod = modLine[l];

				if (line.find("<!-- ", 0) != NOT_FOUND)
				{
					if (line.find("<!-- NEW ^", 0) != NOT_FOUND)
					{
						special = true;
					}
					else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
					{
						special = false;
					}
				}

				if (line.find("<hkobject name=\"", 0) != NOT_FOUND && line.find("signature=\"", 0) != NOT_FOUND)
				{
					isClipTrigger = false;
					nemesis::smatch match;
					
					if (nemesis::regex_search(line, match, boost::regex("<hkobject name=\"#(.*)\" class=\"([a-zA-Z]+)\" signature=\".*\">")))
					{
						curID = stoi(match[1]);
						
						if (match[2] == "hkbClipTriggerArray")
						{
							isClipTrigger = true;
						}
					}
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
						vecstr generator;
						StringSplit(line, generator);
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
				if ((!doneEventName && line.find("<hkparam name=\"eventNames\" numelements=", 0) != NOT_FOUND) ||
					(!doneEventInfo && line.find("<hkparam name=\"eventInfos\" numelements=", 0) != NOT_FOUND))
				{
					line.find("<hkparam name=\"eventNames\" numelements=", 0) != NOT_FOUND ? doneEventName = true : doneEventInfo = true;

					nemesis::smatch match;

					if (nemesis::regex_search(line, match, boost::regex("<hkparam name=\"(.+?)\" numelements=\"(.+?)\">")))
					{
						if (eventelements == -1)
						{
							if (line.find("</hkparam>") == NOT_FOUND)
							{
								counter = 0;
								eventOpen = true;
								elementCatch = true;
								string templine = line.substr(0, match.position(1));
								openRange = count(templine.begin(), templine.end(), '\t');
								curNum = match[1];
							}
						}
						else
						{
							string templine = line.substr(0, match.position(1));
							openRange = count(templine.begin(), templine.end(), '\t');
							curNum = match[1];
							replacedNum = true;
							eventOpen = true;
							line.replace(match.position(2), match[2].length(), to_string(eventelements));
						}
					}
				}
				else if ((!doneAttributeName && line.find("<hkparam name=\"attributeNames\" numelements=") != NOT_FOUND) ||
					(!doneAttributeDef && line.find("<hkparam name=\"attributeDefaults\" numelements=") != NOT_FOUND))
				{
					line.find("<hkparam name=\"attributeNames\" numelements=") != NOT_FOUND ? doneAttributeName = true : doneAttributeDef = true;

					nemesis::smatch match;

					if (nemesis::regex_search(line, match, boost::regex("<hkparam name=\"(.+?)\" numelements=\"(.+?)\">")))
					{
						if (attributeelements == -1)
						{
							if (line.find("</hkparam>") == NOT_FOUND)
							{
								counter = 0;
								attriOpen = true;
								elementCatch = true;
								string templine = line.substr(0, match.position(1));
								openRange = count(templine.begin(), templine.end(), '\t');
								curNum = match[1];
							}
						}
						else
						{
							line.replace(match.position(2), match[2].length(), to_string(attributeelements));
						}
					}
				}
				else if ((!doneCharacProName && line.find("<hkparam name=\"characterPropertyNames\" numelements=") != NOT_FOUND) ||
					(!doneCharacProInfo && line.find("<hkparam name=\"characterPropertyInfos\" numelements=") != NOT_FOUND))
				{
					line.find("<hkparam name=\"characterPropertyNames\" numelements=") != NOT_FOUND ? doneCharacProName = true : doneCharacProInfo = true;

					nemesis::smatch match;

					if (nemesis::regex_search(line, match, boost::regex("<hkparam name=\"(.+?)\" numelements=\"(.+?)\">")))
					{
						if (characterelements == -1)
						{
							if (line.find("</hkparam>") == NOT_FOUND)
							{
								counter = 0;
								charOpen = true;
								elementCatch = true;
								string templine = line.substr(0, match.position(1));
								openRange = count(templine.begin(), templine.end(), '\t');
								curNum = match[1];
							}
						}
						else
						{
							line.replace(match.position(2), match[2].length(), to_string(characterelements));
						}
					}
				}
				else if ((!doneVarName && line.find("<hkparam name=\"variableNames\" numelements=") != NOT_FOUND) ||
					(!doneVarWord && line.find("<hkparam name=\"wordVariableValues\" numelements=") != NOT_FOUND) ||
					(!doneVarInfo && line.find("<hkparam name=\"variableInfos\" numelements=") != NOT_FOUND))
				{
					line.find("<hkparam name=\"variableNames\" numelements=") != NOT_FOUND ? doneVarName = true :
						line.find("<hkparam name=\"wordVariableValues\" numelements=") != NOT_FOUND ? doneVarWord = true : doneVarInfo = true;

					nemesis::smatch match;
					
					if (nemesis::regex_search(line, match, boost::regex("<hkparam name=\"(.+?)\" numelements=\"(.+?)\">")))
					{
						if (variableelements == -1)
						{
							if (line.find("</hkparam>") == NOT_FOUND)
							{
								counter = 0;
								varOpen = true;
								elementCatch = true;
								string templine = line.substr(0, match.position(1));
								openRange = count(templine.begin(), templine.end(), '\t');
								curNum = match[1];
							}
						}
						else
						{
							replacedNum = true;
							varOpen = true;
							string templine = line.substr(0, match.position(1));
							openRange = count(templine.begin(), templine.end(), '\t');
							curNum = match[1];
							line.replace(match.position(2), match[2].length(), to_string(variableelements));
						}
					}
				}
				else if (!doneAnimName && line.find("<hkparam name=\"animationNames\" numelements=\"") != NOT_FOUND)
				{
					if (filelist[curList].find("_1stperson\\") != NOT_FOUND)
					{
						fp_animOpen = true;
					}

					doneAnimName = true;
					nemesis::smatch match;

					if (nemesis::regex_search(line, match, boost::regex("<hkparam name=\"(.+?)\" numelements=\"(.+?)\">")))
					{
						if (line.find("</hkparam>") == NOT_FOUND)
						{
							counter = 0;
							animOpen = true;
							elementCatch = true;
							string templine = line.substr(0, match.position(1));
							openRange = count(templine.begin(), templine.end(), '\t');
							string number = match[2];
							curNum = match[1];

							if (!isOnlyNumber(number))
							{
								ErrorMessage(1174, filepath, l + 1, line.substr(line.find("<hkparam name=\"animationNames\" numelements=\"")));
							}

							base = stoi(number);
						}
					}
				}
				else if ((!doneDeform && line.find("<hkparam name=\"deformableSkinNames\" numelements=\"") != NOT_FOUND) ||
					(!doneRigid && line.find("<hkparam name=\"rigidSkinNames\" numelements=\"") != NOT_FOUND) ||
					(!doneAnimFileName && line.find("<hkparam name=\"animationFilenames\" numelements=\"") != NOT_FOUND))
				{
					line.find("<hkparam name=\"deformableSkinNames\" numelements=\"") != NOT_FOUND ? doneDeform = true :
						line.find("<hkparam name=\"rigidSkinNames\" numelements=\"") != NOT_FOUND ? doneRigid = true : doneAnimFileName = true;

					nemesis::smatch match;

					if (nemesis::regex_search(line, match, boost::regex("<hkparam name=\"(.+?)\" numelements=\"(.+?)\">")))
					{
						if (line.find("</hkparam>") == NOT_FOUND)
						{
							counter = 0;
							otherAnimOpen = true;
							elementCatch = true;
							string templine = line.substr(0, match.position(1));
							openRange = count(templine.begin(), templine.end(), '\t');
							curNum = match[1];
						}
					}
				}
				else if (boost::regex_search(line, check, boost::regex("<hkparam name=\"(.+?)\" numelements=\"")) && line.find("</hkparam>", check.position(1)) == NOT_FOUND)
				{
					counter = 0;
					norElement = true;
					elementCatch = true;
					string templine = line.substr(0, check.position(1));
					openRange = count(templine.begin(), templine.end(), '\t');
				}
				// counting for numelement
				else if (eventOpen)
				{
					if (line.find("</hkparam>") != NOT_FOUND)
					{
						string templine = line.substr(0, line.find("</hkparam>"));
						size_t range = count(templine.begin(), templine.end(), '\t');

						if (openRange == range)
						{
							unordered_map<string, bool> isExist;

							for (auto& aaEvent : AAEventName)
							{
								AddEvents(curNum, catalystMap[curID], aaEvent, orievent, isExist, counter, ZeroEvent, eventid, eventName);
							}

							setstr codelist = BehaviorTemplate.grouplist[lowerBehaviorFile];

							for (auto& templatecode : codelist)
							{
								for (auto& newEvent : newAnimEvent[templatecode + modID])
								{
									AddEvents(curNum, catalystMap[curID], newEvent, orievent, isExist, counter, ZeroEvent, eventid, eventName);
								}
							}

							if (eventelements == -1)
							{
								eventelements = counter;
							}

							eventOpen = false;
							replacedNum ? replacedNum = false : elementUpdate(elementLine, counter, curID, catalystMap);
						}
					}
					else if (!replacedNum)
					{
						if (line.find("<hkcstring>") != NOT_FOUND)
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
						else if (line.find("<hkparam name=\"flags\">") != NOT_FOUND)
						{
							++counter;
						}
					}
				}
				else if (varOpen)
				{
					if (line.find("\t</hkparam>") != NOT_FOUND)
					{
						string templine = line.substr(0, line.find("</hkparam>"));
						size_t range = count(templine.begin(), templine.end(), '\t');

						if (openRange == range)
						{
							unordered_map<string, bool> isExist;

							for (auto& AAVariable : AAGroupList)
							{
								AddVariables(curNum, catalystMap[curID], "Nemesis_AA" + AAVariable, orivariable, isExist, counter, ZeroVariable, variableid, varName);
								AddVariables(curNum, catalystMap[curID], "Nemesis_AA_Priority" + AAVariable, orivariable, isExist, counter, ZeroVariable, variableid, varName);

								for (int k = 0; k < AAgroup_Counter[AAVariable]; ++k)
								{
									AddVariables(curNum, catalystMap[curID], "Nemesis_AA_Priority" + AAVariable + "_" + to_string(k), orivariable, isExist, counter,
										ZeroVariable, variableid, varName);
								}
							}

							setstr codelist = BehaviorTemplate.grouplist[lowerBehaviorFile];

							for (auto& templatecode : codelist)
							{
								for (auto& newVariable : newAnimVariable[templatecode + modID])
								{
									AddVariables(curNum, catalystMap[curID], newVariable, orivariable, isExist, counter, ZeroVariable, variableid, varName);
								}
							}

							for (auto& modname : pceaMod)
							{
								AddVariables(curNum, catalystMap[curID], modname, orivariable, isExist, counter, ZeroVariable, variableid, varName);
							}

							if (variableelements == -1)
							{
								variableelements = counter;
							}

							varOpen = false;
							replacedNum ? replacedNum = false : elementUpdate(elementLine, counter, curID, catalystMap);
						}
					}
					else if (!replacedNum)
					{
						if (curNum == "variableNames" && line.find("<hkcstring>") != NOT_FOUND)
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
						else if (curNum == "wordVariableValues" && line.find("<hkparam name=\"value\">") != NOT_FOUND)
						{
							++counter;
						}
						else if (curNum == "variableInfos" && line.find("<hkparam name=\"type\">") != NOT_FOUND)
						{
							++counter;
						}
					}
				}
				else if (attriOpen)
				{
					if (line.find("</hkparam>") != NOT_FOUND)
					{
						string templine = line.substr(0, line.find("</hkparam>"));
						size_t range = count(templine.begin(), templine.end(), '\t');

						if (openRange == range)
						{
							if (attributeelements == -1)
							{
								attributeelements = counter;
							}

							attriOpen = false;
							elementUpdate(elementLine, counter, curID, catalystMap);
						}
					}
					else if (line.find("<hkcstring>") != NOT_FOUND)
					{
						size_t nextpos = line.find("<hkcstring>") + 11;
						string name = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
						attriName[counter] = name;
						attriid[name] = counter;
						++counter;
					}
				}
				else if (charOpen)
				{
					if (line.find("</hkparam>") != NOT_FOUND)
					{
						string templine = line.substr(0, line.find("</hkparam>"));
						size_t range = count(templine.begin(), templine.end(), '\t');

						if (openRange == range)
						{
							if (characterelements == -1)
							{
								characterelements = counter;
							}

							charOpen = false;
							elementUpdate(elementLine, counter, curID, catalystMap);
						}
					}
					else if (line.find("<hkcstring>") != NOT_FOUND)
					{
						size_t nextpos = line.find("<hkcstring>") + 11;
						string name = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
						charName[counter] = name;
						charid[name] = counter;
						++counter;
					}
				}
				else if (animOpen)
				{
					if (line.find("</hkparam>") != NOT_FOUND)
					{
						string templine = line.substr(0, line.find("</hkparam>"));
						size_t range = count(templine.begin(), templine.end(), '\t');

						if (openRange == range)
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
														AddAnims(line, animPath, outputdir, behaviorFile, lowerBehaviorFile, newMod, catalystMap[curID], counter, isAdded, addAnim);

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
											AddAnims(line, animPath, outputdir, behaviorFile, lowerBehaviorFile, newMod, catalystMap[curID], counter, isAdded, addAnim);

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
										AddAnims(line, animPath.second, outputdir, behaviorFile, lowerBehaviorFile, newMod, catalystMap[curID], counter, isAdded, addAnim);

										if (addAnim)
										{
											addAnimation();
										}
									}
								}
							}

							animOpen = false;
							fp_animOpen = false;
							elementUpdate(elementLine, counter, curID, catalystMap);
						}
					}
					else if (line.find("<hkcstring>") != NOT_FOUND)
					{
						size_t nextpos = line.find("<hkcstring>") + 11;
						string animPath = line.substr(nextpos, line.find("</hkcstring>", nextpos) - nextpos);
						string animFile = GetFileName(animPath) + ".hkx";

						if (!fp_animOpen && !characterAA && alternateAnim.find(boost::to_lower_copy(animFile)) != alternateAnim.end())
						{
							if (!isCharacter)
							{
								ErrorMessage(1184, behaviorFile);
							}

							characterAA = true;
						}

						if (activatedBehavior["gender*"])
						{
							if (lowerBehaviorFile == "defaultfemale")
							{
								if (!boost::iequals(animPath, "Animations\\female\\" + animFile))
								{
									boost::filesystem::path animation(GetFileDirectory(outputdir));

									if (isFileExist(animation.parent_path().parent_path().string() + "\\Animations\\female\\" + animFile))
									{
										line.replace(nextpos, animPath.length(), "Animations\\female\\" + animFile);
									}
									else if (boost::iequals(animPath, "Animations\\male\\" + animFile))
									{
										if (isFileExist(animation.parent_path().parent_path().string() + "\\Animations\\" + animFile))
										{
											line.replace(nextpos, animPath.length(), "Animations\\" + animFile);
										}
									}
								}
							}
							else if (lowerBehaviorFile == "defaultmale" && !boost::iequals(animPath, "Animations\\male\\" + animFile))
							{
								boost::filesystem::path animation(GetFileDirectory(outputdir));

								if (isFileExist(animation.parent_path().parent_path().string() + "\\Animations\\male\\" + animFile))
								{
									line.replace(nextpos, animPath.length(), "Animations\\male\\" + animFile);
								}
								else if (boost::iequals(animPath, "Animations\\female\\" + animFile))
								{
									if (isFileExist(animation.parent_path().parent_path().string() + "\\Animations\\" + animFile))
									{
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

						vector<set<string>>* match_ptr = &animModMatch[lowerBehaviorFile][animFile];
						size_t matchSize = match_ptr->size();

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
					}
				}
				else if (otherAnimOpen)
				{
					if (line.find("</hkparam>") != NOT_FOUND)
					{
						string templine = line.substr(0, line.find("</hkparam>"));
						size_t range = count(templine.begin(), templine.end(), '\t');

						if (openRange == range)
						{
							otherAnimOpen = false;
							elementUpdate(elementLine, counter, curID, catalystMap);
						}
					}
					else if (line.find("<hkcstring>") != NOT_FOUND)
					{
						++counter;
					}
				}
				else if (norElement)
				{
					size_t pos = line.find("</hkparam>");

					if (pos != NOT_FOUND)
					{
						string templine = line.substr(0, pos);
						size_t range = count(templine.begin(), templine.end(), '\t');

						if (openRange == range)
						{
							norElement = false;
							elementUpdate(elementLine, counter, curID, catalystMap);
						}
					}
					else
					{
						pos = line.find("<hkobject>");

						if (pos != NOT_FOUND)
						{
							string templine = line.substr(0, pos);
							size_t range = count(templine.begin(), templine.end(), '\t');

							if (range == openRange + 1)
							{
								++counter;
							}
						}
						else if (line.find("\t\t\t#") != NOT_FOUND)
						{
							string templine = line.substr(0, line.find("#"));
							size_t reference = count(templine.begin(), templine.end(), '\t');

							if (reference == openRange + 1)
							{
								int number = int(count(line.begin(), line.end(), '#'));
								counter += number;
							}
						}
					}
				}

				if (error) throw nemesis::exception();
				
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

							if (error) throw nemesis::exception();
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

							if (error) throw nemesis::exception();
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
						}
					}
				}

				if (isClipTrigger && line.find("<hkparam name=\"localTime\">-") != NOT_FOUND)
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

					if (newBone && newline.find("%n") != NOT_FOUND)
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
			}

			if (error) throw nemesis::exception();
		}

		DebugLogging("Processing behavior: " + filepath + " (Check point 3, Behavior general processing complete)");

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

				DebugLogging("Processing behavior: " + filepath + " (Check point 3.4, No changes detected)");
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
				}
			}
		}
	}

	if (error) throw nemesis::exception();

	emit progressAdd();

	// add new animation
	vector<shared_ptr<vecstr>> allEditLines;
	unordered_map<string, bool> isCoreDone;
	unordered_map<int, int> functionState;
	int grouptimer = 0;
	int onetimer = 0;

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
				shared_ptr<master> groupFunctionIDs = make_shared<master>();
				vector<vector<shared_ptr<animationInfo>>> groupAnimInfo;

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
					group newSubFunctionIDs;
					vector<int> stateID;
					vector<int> stateMultiplier = { 0 };
					GetStateCount(stateMultiplier, BehaviorTemplate.behaviortemplate[templateCode][lowerBehaviorFile], templateCode, behaviorFile, hasGroup);

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

					if (error) throw nemesis::exception();

					if (stateMultiplier.size() != stateID.size() || stateMultiplier.size() == 0)
					{
						ErrorMessage(1192);
					}

					size_t newAnimCount = newAnimation[templateCode].size();

					if (newAnimCount > 0)
					{
						DebugLogging("Processing behavior: " + filepath + " (Check point 3.6, Mod code: " + templateCode + ", Animation count: " +
							to_string(newAnimCount) + ")");
						shared_ptr<Furniture> dummyAnimation;
						int IDMultiplier = newAnimation[templateCode][0]->getNextID(lowerBehaviorFile);
						newAnimLock animLock;
						size_t n_core = boost::thread::hardware_concurrency();
						boost::posix_time::ptime start_time = boost::posix_time::microsec_clock::local_time();

						if (newAnimCount > 500)
						{
							n_core = n_core * 3 / 4;
						}

						// individual animation
						if (hasGroup)
						{
							for (unsigned int k = 0; k < newAnimCount; ++k)
							{
								boost::thread_group multiThreads;

								try
								{
									for (k; k < newAnimCount; ++k)
									{
										if (newAnimation[templateCode][k]->coreModID != modID)
										{
											if (dummyAnimation != nullptr && !dummyAnimation->isLast())
											{
												ErrorMessage(1183);
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

											try
											{
												multiThreads.create_thread(boost::bind(animThreadStart, args));
											}
											catch (boost::thread_exception&)
											{
												animThreadStart(args);
											}
										}

										if (multiThreads.size() + m_RunningThread >= n_core + extraCore)
										{
											break;
										}

										if (error) throw nemesis::exception();
									}

									if (multiThreads.size() > 0)
									{
										multiThreads.join_all();
									}
								}
								catch (nemesis::exception) {}
							}
						}
						else
						{
							for (unsigned int k = 0; k < newAnimCount; ++k)
							{
								boost::thread_group multiThreads;

								try
								{
									for (k; k < newAnimCount; ++k)
									{
										if (newAnimation[templateCode][k]->coreModID != modID)
										{
											if (dummyAnimation != nullptr && !dummyAnimation->isLast())
											{
												ErrorMessage(1183);
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

											if (error) throw nemesis::exception();

											lastID += IDMultiplier;

											try
											{
												multiThreads.create_thread(boost::bind(animThreadStart, args));
											}
											catch (boost::thread_exception&)
											{
												animThreadStart(args);
											}
										}

										if (multiThreads.size() + m_RunningThread >= n_core + extraCore)
										{
											break;
										}
									}
								}
								catch (nemesis::exception) {}

								if (multiThreads.size() > 0)
								{
									multiThreads.join_all();
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
								}
							}
						}

						boost::posix_time::time_duration diff = boost::posix_time::microsec_clock::local_time() - start_time;
						onetimer += int(diff.total_milliseconds());
						start_time = boost::posix_time::microsec_clock::local_time();

						// group animation
						if (hasGroup && groupFunctionIDs->grouplist.size() > 0)
						{
							string filename = templateCode + "_group";
							shared_ptr<AnimTemplate> grouptemplate_pack = make_shared<AnimTemplate>();
							grouptemplate_pack->ExamineTemplate(templateCode, lowerBehaviorFile, BehaviorTemplate.behaviortemplate[filename][lowerBehaviorFile], true, false,
								BehaviorTemplate.optionlist[templateCode]);
							IDMultiplier = getTemplateNextID(BehaviorTemplate.behaviortemplate[filename][lowerBehaviorFile]);
							stateMultiplier = vector<int>();

							if (!GetStateCount(stateMultiplier, BehaviorTemplate.behaviortemplate[filename][lowerBehaviorFile], templateCode, behaviorFile, hasGroup)) return;

							hasMaster ? stateID = { 0 } : stateID = GetStateID(BehaviorTemplate.mainBehaviorJoint[templateCode][lowerBehaviorFile], catalystMap, functionState);
							size_t n_newAnimCount = groupFunctionIDs->grouplist.size();

							for (unsigned int k = 0; k < n_newAnimCount; ++k)
							{
								boost::thread_group multiThreads;

								try
								{
									for (k; k < n_newAnimCount; ++k)
									{
										allEditLines.emplace_back(make_shared<vecstr>());
										shared_ptr<groupTemplate> groupTemp = make_shared<groupTemplate>(BehaviorTemplate.behaviortemplate[filename][lowerBehaviorFile],
											grouptemplate_pack);
										groupTemp->setZeroEvent(ZeroEvent);
										groupTemp->setZeroVariable(ZeroVariable);
										newGroupArgs args(templateCode, lowerBehaviorFile, filename, lastID, k + 1, stateID, groupTemp, allEditLines.back(), exportID, eventid,
											variableid, animLock, ZeroEvent, ZeroVariable, groupFunctionIDs, groupAnimInfo);

										for (unsigned int statenum = 0; statenum < stateID.size(); ++statenum)
										{
											stateID[statenum] += stateMultiplier[statenum];
										}

										if (error) throw nemesis::exception();

										lastID += IDMultiplier;

										try
										{
											multiThreads.create_thread(boost::bind(groupThreadStart, args));
										}
										catch (boost::thread_exception&)
										{
											groupThreadStart(args);
										}

										if (multiThreads.size() + m_RunningThread >= n_core + extraCore)
										{
											break;
										}
									}
								}
								catch (nemesis::exception) {}

								if (multiThreads.size() > 0)
								{
									multiThreads.join_all();
								}
							}
						}

						// master animation
						if (hasMaster)
						{
							string filename = templateCode + "_master";
							shared_ptr<AnimTemplate> mastertemplate_pack = make_shared<AnimTemplate>();
							mastertemplate_pack->ExamineTemplate(templateCode, lowerBehaviorFile, BehaviorTemplate.behaviortemplate[filename][lowerBehaviorFile], true, true,
								BehaviorTemplate.optionlist[templateCode]);
							stateID = GetStateID(BehaviorTemplate.mainBehaviorJoint[templateCode][lowerBehaviorFile], catalystMap, functionState);
							shared_ptr<groupTemplate> masterTemp = make_shared<groupTemplate>(BehaviorTemplate.behaviortemplate[filename][lowerBehaviorFile],
								mastertemplate_pack);
							masterTemp->setZeroEvent(ZeroEvent);
							masterTemp->setZeroVariable(ZeroVariable);
							allEditLines.emplace_back(make_shared<vecstr>());
							masterTemp->getFunctionLines(allEditLines.back(), lowerBehaviorFile, filename, stateID, groupFunctionIDs,
								groupAnimInfo, lastID, exportID, eventid, variableid, templateCode, animLock, -1);

							lastID += getTemplateNextID(BehaviorTemplate.behaviortemplate[filename][lowerBehaviorFile]);

							if (error) throw nemesis::exception();
						}

						diff = boost::posix_time::microsec_clock::local_time() - start_time;
						grouptimer += int(diff.total_milliseconds());
						DebugLogging("Processing behavior: " + filepath + " (Check point 3.8, Mod code: " + templateCode + ", Animation count: " +
							to_string(newAnimCount) + " COMPLETE)");
					}

					break;
				}

				DebugLogging("Processing behavior: " + filepath + " (Check point 3.8, Mod code: " + templateCode + ", Existing ID count: " +
					to_string(BehaviorTemplate.existingFunctionID[templateCode][lowerBehaviorFile].size()) + ")");

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

					if (error) throw nemesis::exception();

					if (catalystMap[functionID].size() == 0)
					{
						ErrorMessage(1099);
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

					if (error) throw nemesis::exception();
				}

				DebugLogging("Processing behavior: " + filepath + " (Check point 3.8, Mod code: " + templateCode + ", Existing ID count: " +
					to_string(BehaviorTemplate.existingFunctionID[templateCode][lowerBehaviorFile].size()) + " COMPLETE)");
			}
			else
			{
				DebugLogging("Processing behavior: " + filepath + " (Check point 3.8, Mod code: " + templateCode + ", Existing ID count: " +
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
					
					if (catalystMap[functionID].size() == 0)
					{
						ErrorMessage(1099);
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

					if (error) throw nemesis::exception();
				}

				DebugLogging("Processing behavior: " + filepath + " (Check point 3.8, Mod code: " + templateCode + ", Existing ID count: " +
					to_string(BehaviorTemplate.existingFunctionID[templateCode][lowerBehaviorFile].size()) + " COMPLETE)");
			}

			vecstr closing;
			closing.push_back("<!-- ======================== NEMESIS " + templateCode + " TEMPLATE END ======================== -->");
			closing.push_back("");
			allEditLines.emplace_back(make_unique<vecstr>(closing));
		}
	}

	// check for error
	if (error) throw nemesis::exception();

	DebugLogging("Total single animation processing time for " + behaviorFile + ": " + to_string(onetimer));
	DebugLogging("Total group animation processing time for " + behaviorFile + ": " + to_string(grouptimer));
	DebugLogging("Processing behavior: " + filepath + " (Check point 4, New animation inclusion complete)");
	emit progressAdd();

	// AA animation installation
	vecstr AAlines;

	if (clipAA.size() != 0)
	{
		DebugLogging("Processing behavior: " + filepath + " (Check point 4.2, AA count: " + to_string(clipAA.size()) + ")");
		unordered_map<string, int> replacerCount;

		for (auto iter = clipAA.begin(); iter != clipAA.end(); ++iter)
		{
			bool isChange = false;
			vecstr msglines;

			if (error) throw nemesis::exception();

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
				importline = "variableID[Nemesis_AA" + (isFirstPerson ? groupName + "_1p*" : groupName) + "]";
				variableIDReplacer(importline, "AA", behaviorFile, variableid, ZeroVariable, 0);

				if (error) throw nemesis::exception();

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
					string animFile = boost::to_lower_copy(it->second[num].substr(it->second[num].find_last_of("\\") + 1));
					auto& aaEvent_itr = AAEvent.find(isFirstPerson ? animFile + "_1p*" : animFile);

					if (aaEvent_itr != AAEvent.end())
					{
						if (aaEvent_itr->second.size() > 0)
						{
							string tempID = to_string(lastID);

							while (tempID.length() < 4)
							{
								tempID = "0" + tempID;
							}

							AAlines.push_back("			<hkparam name=\"triggers\">#" + tempID + "</hkparam>");
							triggerID[tempID] = aaEvent_itr->second;
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

					if (error) throw nemesis::exception();

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

						if (error) throw nemesis::exception();

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

					if (error) throw nemesis::exception();

					AAlines.push_back("			</hkparam>");
					AAlines.push_back("		</hkobject>");
					AAlines.push_back("");
				}

				if (error) throw nemesis::exception();
			}

			if (isChange)
			{
				catalystMap[iter->first] = msglines;
			}
		}

		DebugLogging("Processing behavior: " + filepath + " (Check point 4.4, AA count: " + to_string(clipAA.size()) + " COMPLETE)");
	}

	vecstr PCEALines;

	if (pceaID.size() > 0)
	{
		DebugLogging("Processing behavior: " + filepath + " (Check point 4.6, PCEA count: " + to_string(pceaID.size()) + ")");
		unordered_map<string, int> replacerCount;

		for (auto& datalist : pceaID)
		{
			vector<vecstr> lineRe;
			string importline = to_string(datalist.first);

			if (error) throw nemesis::exception();

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

				if (error) throw nemesis::exception();

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

			if (error) throw nemesis::exception();

			catalystMap.erase(catalystMap.find(datalist.first));
		}

		DebugLogging("Processing behavior: " + filepath + " (Check point 4.8, PCEA count: " + to_string(pceaID.size()) + " COMPLETE)");
	}

	emit progressAdd();

	size_t reserveSize = 0;
	vecstr behaviorlines;

	// output import functions
	// Must be in vector
	vector<ImportContainer> groupExportID;
	groupExportID.push_back(exportID);
	vecstr additionallines = importOutput(groupExportID, 0, lastID, filelist[curList]);

	emit progressAdd();

	DebugLogging("Processing behavior: " + filepath + " (Check point 5, Prepare to output)");
	emit progressAdd();

	if (behaviorPath[lowerBehaviorFile].size() == 0)
	{
		ErrorMessage(1068, behaviorFile);
	}

	string filename = "temp_behaviors\\xml\\" + modID + lowerBehaviorFile;

	if (!FolderCreate(GetFileDirectory(filename)))
	{
		return;
	}
	else
	{
		if (!FolderCreate(GetFileDirectory(outputdir)))
		{
			return;
		}
	}

	ofstream output(filename + ".xml");

	if (!output.is_open())
	{
		ErrorMessage(1025, filename);
	}

	for (int i = firstID; i < firstID + 4; ++i)
	{
		for (auto& eachline : catalystMap[i])
		{
			output << eachline << "\n";
		}

		if (error) throw nemesis::exception();
	}

	output << "<!-- ======================== NEMESIS import TEMPLATE START ======================== -->\n";

	if (additionallines.size() > 0)
	{
		output << "\n";

		for (auto& eachline : additionallines)
		{
			output << eachline << "\n";
		}

		if (error) throw nemesis::exception();
	}

	output << "<!-- ======================== NEMESIS import TEMPLATE END ======================== -->\n\n";
	output << "<!-- ======================== NEMESIS PCEA TEMPLATE START ======================== -->\n";

	if (PCEALines.size() > 0)
	{
		output << "\n";

		for (auto& eachline : PCEALines)
		{
			output << eachline << "\n";
		}

		if (error) throw nemesis::exception();
	}

	output << "<!-- ======================== NEMESIS PCEA TEMPLATE END ======================== -->\n\n";
	output << "<!-- ======================== NEMESIS alternate animation TEMPLATE START ======================== -->\n";

	if (AAlines.size() > 0)
	{
		output << "\n";

		for (auto& eachline : AAlines)
		{
			output << eachline << "\n";
		}

		if (error) throw nemesis::exception();
	}

	output << "<!-- ======================== NEMESIS alternate animation TEMPLATE END ======================== -->\n\n";

	for (unsigned int j = 0; j < allEditLines.size(); ++j)
	{
		for (auto& eachline : (*allEditLines[j]))
		{
			output << eachline << "\n";
		}

		if (error) throw nemesis::exception();
	}

	firstID = firstID + 4;
	size_t size = catalystMap.rbegin()->first + 1;

	for (size_t it = firstID; it < size; ++it)
	{
		for (auto& eachline : catalystMap[it])
		{
			output << eachline << "\n";
		}

		if (error) throw nemesis::exception();
	}

	if (catalystMap.rbegin()->second.back().length() != 0)
	{
		output << "\n";
	}

	output << "	</hksection>" << "\n\n";
	output << "</hkpackfile>" << "\n";
	output.close();

	if (error) throw nemesis::exception();

	DebugLogging("Processing behavior: " + filepath + " (Check point 6, Behavior output complete)");
	emit progressAdd();
	++extraCore;

	if (hkxcmdProcess(filename, outputdir))
	{
		DebugLogging("Processing behavior: " + filepath + " (Check point 7, Behavior compile complete)");
		emit progressAdd();
	}
	
	--extraCore;
}

void BehaviorSub::AnimDataCompilation()
{
	try
	{
		++m_RunningThread;

		try
		{
			try
			{
				CompilingAnimData();
			}
			catch (exception& ex)
			{
				ErrorMessage(6002, ex.what(), filelist[curList]);
			}
		}
		catch (nemesis::exception) {}
	}
	catch (...)
	{
		try
		{
			ErrorMessage(6002, "New animation: Unknown", filelist[curList]);
		}
		catch (nemesis::exception) {}
	}

	emit done();
}

void BehaviorSub::CompilingAnimData()
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
				return;
			}

			DebugLogging("Processing behavior: " + filepath + " (Check point 1, File extraction complete)");
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

					if (error) throw nemesis::exception();
				}

				if (error) throw nemesis::exception();
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

		DebugLogging("Processing behavior: " + filepath + " (Check point 2, AnimData general processing complete)");
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
										}
									}

									if (catalystMap[projectplus][header].size() > 0)
									{
										ErrorMessage(5012, templateCode, project, header);
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
								}

								map<int, vecstr> extract;
								newAnimation[templateCode][k]->existingASDProcess(catalystMap[project][header], extract);

								if (error) throw nemesis::exception();

								editExtract[project][header].push_back(extract);
							}
						}

						if (error) throw nemesis::exception();
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
							}

							editExtract[project][header].push_back(*new map<int, vecstr>());

							if (error) throw nemesis::exception();
						}
					}
				}

				if (error) throw nemesis::exception();
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
					}

					for (int i = 1; i <= projectNameCount[project.first]; ++i)
					{
						combineExtraction(catalystMap[project.first + " " + to_string(i)][header.first], combined, project.first, header.first);
					}

					if (error) throw nemesis::exception();
				}

				if (error) throw nemesis::exception();
			}
		}
		else
		{
			emit progressAdd();
		}

		DebugLogging("Processing behavior: " + filepath + " (Check point 3, AnimData general new animations complete)");
		emit progressAdd();

		if (alternateAnim.size() > 0)
		{
			DebugLogging("Processing behavior: " + filepath + " (Check point 3.4, AnimData general new animations complete)");

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

					if (error) throw nemesis::exception();
				}

				if (error) throw nemesis::exception();
			}

			DebugLogging("Processing behavior: " + filepath + " (Check point 3.5, AnimData AA complete)");
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
				}

				if (error) throw nemesis::exception();

				try
				{
					AnimDataProject newProject(combined, projectList[i], filepath);
					ADProject.push_back(newProject);
				}
				catch (int)
				{
					return;
				}

				if (error) throw nemesis::exception();
			}
		}
		catch (int)
		{
			return;
		}
	}

	DebugLogging("Processing behavior: " + filepath + " (Check point 4, AnimData format check complete)");
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
			return;
		}
	}

	FileWriter output(filepath + ".txt");

	if (output.is_open())
	{
		output << to_string(projectList.size()) << "\n";

		for (unsigned int i = 0; i < projectList.size(); ++i)
		{
			output << projectList[i] << "\n";
		}

		for (unsigned int i = 0; i < projectList.size(); ++i)
		{
			output << to_string(ADProject[i].GetAnimTotalLine()) << "\n";
			output << ADProject[i].unknown1 << "\n";
			output << to_string(ADProject[i].behaviorlist.size()) << "\n";

			for (unsigned int j = 0; j < ADProject[i].behaviorlist.size(); ++j)
			{
				output << ADProject[i].behaviorlist[j] << "\n";
			}

			output << ADProject[i].unknown2 << "\n";

			if (ADProject[i].unknown2 != "0")
			{
				for (unsigned int j = 0; j < ADProject[i].animdatalist.size(); ++j)
				{
					output << ADProject[i].animdatalist[j].name << "\n";
					output << ADProject[i].animdatalist[j].uniquecode << "\n";
					output << ADProject[i].animdatalist[j].unknown1 << "\n";
					output << ADProject[i].animdatalist[j].unknown2 << "\n";
					output << ADProject[i].animdatalist[j].unknown3 << "\n";
					output << to_string(ADProject[i].animdatalist[j].eventname.size()) << "\n";

					for (unsigned int k = 0; k < ADProject[i].animdatalist[j].eventname.size(); ++k)
					{
						output << ADProject[i].animdatalist[j].eventname[k] << "\n";
					}

					output << "\n";

					if (error) throw nemesis::exception();
				}

				output << to_string(ADProject[i].GetInfoTotalLine()) << "\n";

				for (unsigned int j = 0; j < ADProject[i].infodatalist.size(); ++j)
				{
					output << ADProject[i].infodatalist[j].uniquecode << "\n";
					output << ADProject[i].infodatalist[j].duration << "\n";
					output << to_string(ADProject[i].infodatalist[j].motiondata.size()) << "\n";

					for (unsigned int k = 0; k < ADProject[i].infodatalist[j].motiondata.size(); ++k)
					{
						output << ADProject[i].infodatalist[j].motiondata[k] << "\n";
					}

					output << to_string(ADProject[i].infodatalist[j].rotationdata.size()) << "\n";

					for (unsigned int k = 0; k < ADProject[i].infodatalist[j].rotationdata.size(); ++k)
					{
						output << ADProject[i].infodatalist[j].rotationdata[k] << "\n";
					}

					output << "\n";

					if (error) throw nemesis::exception();
				}
			}

			if (error) throw nemesis::exception();
		}
	}
	else
	{
		ErrorMessage(1025, filepath);
	}

	DebugLogging("Processing behavior: " + filepath + " (Check point 5, AnimData output complete)");
	emit progressAdd();
}

void BehaviorSub::ASDCompilation()
{
	try
	{
		++m_RunningThread;

		try
		{
			try
			{
				CompilingASD();
			}
			catch (exception& ex)
			{
				ErrorMessage(6002, ex.what(), filelist[curList]);
			}
		}
		catch (nemesis::exception) {}
	}
	catch (...)
	{
		try
		{
			ErrorMessage(6002, "New animation: Unknown", filelist[curList]);
		}
		catch (nemesis::exception) {}
	}

	emit done();
}

void BehaviorSub::CompilingASD()
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
			return;
		}

		while (catalyst.back().length() == 0)
		{
			catalyst.pop_back();
		}

		DebugLogging("Processing behavior: " + filepath + " (Check point 1, File extraction complete)");
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

			if (error) throw nemesis::exception();
		}

		for (int i = 1; i < catalyst.size(); ++i)
		{
			if (isOnlyNumber(catalyst[i]))
			{
				break;
			}

			projectList.push_back(catalyst[i]);
			newline.push_back(catalyst[i]);

			if (error) throw nemesis::exception();
		}

		DebugLogging("Processing behavior: " + filepath + " (Check point 2, AnimSetData mod selection complete)");
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

			if (error) throw nemesis::exception();

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

			if (error) throw nemesis::exception();

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

		DebugLogging("Processing behavior: " + filepath + " (Check point 3, AnimSetData general processing complete)");
		emit progressAdd();

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
								}

								if (error) throw nemesis::exception();

								ASDPack[interproject][interheader] = iter->second;
							}
						}

						DebugLogging("Processing behavior: " + filepath + " (Check point 3.5, Mod code: " + templateCode + ", Existing AnimData" +
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
								}

								map<int, vecstr> extract;
								newAnimation[templateCode][k]->existingASDProcess(ASDPack[interproject][interheader], extract, vector<int>(1));
								editExtract[interproject][interheader].push_back(extract);

								if (error) throw nemesis::exception();
							}
						}

						DebugLogging("Processing behavior: " + filepath + " (Check point 3.5, Mod code: " + templateCode + ", Existing AnimData" +
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
							}

							editExtract[interproject][interheader].push_back(*new map<int, vecstr>());

							if (error) throw nemesis::exception();
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
					}

					combineExtraction(ASDPack[project.first][header.first], combined, project.first, header.first);

					if (error) throw nemesis::exception();
				}
			}
		}
		else
		{
			emit progressAdd();
		}

		DebugLogging("Processing behavior: " + filepath + " (Check point 4, AnimSetData new animations complete)");
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

			if (error) throw nemesis::exception();

			size_t pos = projectList[i].find_last_of("\\") + 1;
			string projectFile = projectList[i].substr(pos, projectList[i].find_last_of(".") - pos);
			boost::to_lower(projectFile);
			AnimationDataProject newProject(startline, projectline, filepath, projectFile);
			ASDData[projectList[i]] = newProject;

			if (error) throw nemesis::exception();
		}

		DebugLogging("Processing behavior: " + filepath + " (Check point 5, AnimSetData format check complete)");
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
				return;
			}
		}

		FileWriter output(filepath + ".txt");

		if (output.is_open())
		{
			output << to_string(projectList.size()) << "\n";

			for (unsigned int i = 0; i < projectList.size(); ++i)
			{
				output << projectList[i] << "\n";
			}

			for (unsigned int i = 0; i < projectList.size(); ++i)
			{
				string curProject = projectList[i];
				output << to_string(ASDData[curProject].datalist.size()) << "\n";

				for (auto& data : ASDData[curProject].datalist)
				{
					output << data.first << "\n";
				}

				for (auto& data : ASDData[curProject].datalist)
				{
					output << "V3" << "\n";
					output << to_string(data.second.equiplist.size()) << "\n";

					for (auto& equip : data.second.equiplist)
					{
						output << equip.name << "\n";
					}

					output << to_string(data.second.typelist.size()) << "\n";

					for (auto& type : data.second.typelist)
					{
						output << type.name << "\n";
						output << type.equiptype1 << "\n";
						output << type.equiptype2 << "\n";
					}

					output << to_string(data.second.animlist.size()) << "\n";

					for (auto& anim : data.second.animlist)
					{
						output << anim.name << "\n";
						output << anim.unknown << "\n";
						output << to_string(anim.attack.size()) << "\n";

						for (auto& atk : anim.attack)
						{
							output << atk.data << "\n";
						}
					}

					output << to_string(data.second.crc32list.size()) << "\n";

					for (auto& crc32 : data.second.crc32list)
					{
						output << crc32.filepath << "\n";
						output << crc32.filename << "\n";
						output << crc32.fileformat << "\n";
					}

					if (error) throw nemesis::exception();
				}

				if (error) throw nemesis::exception();
			}
		}
		else
		{
			ErrorMessage(1025, filepath);
		}

		DebugLogging("Processing behavior: " + filepath + " (Check point 6, AnimSetData output complete)");
		emit progressAdd();
	}
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

		if (error) throw nemesis::exception();
	}

	if (!skip)
	{
		duplicatedStateList[filename][ID][sID].insert(modcode);
		duplicatedStateList[filename][ID][sID].insert("Vanilla");
	}
}

bool isRunning(Terminator*& curEvent)
{
	if (m_RunningThread == 0) return false;

	curEvent = p_terminate;
	return true;
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
			try
			{
				args.groupTemp->setZeroEvent(args.ZeroEvent);
				args.groupTemp->setZeroVariable(args.ZeroVariable);
				args.groupTemp->getFunctionLines(args.allEditLines, args.lowerBehaviorFile, args.filename, args.stateID, args.groupFunctionIDs, args.groupAnimInfo,
					args.lastID, args.exportID, args.eventid, args.variableid, args.templateCode, args.atomicLock, args.groupCount);
			}
			catch (const exception& ex)
			{
				ErrorMessage(6002, ex.what(), args.filename);
			}
		}
		catch (nemesis::exception) {}
	}
	catch (...)
	{
		try
		{
			ErrorMessage(6002, "New animation: Unknown", args.filename);
		}
		catch (nemesis::exception) {}
	}
}

void animThreadStart(newAnimArgs args)
{
	try
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
				ErrorMessage(6002, ex.what(), args.dummyAnimation->GetFormatName());
			}
		}
		catch (nemesis::exception) {}
	}
	catch (...)
	{
		try
		{
			ErrorMessage(6002, "New animation: Unknown", args.dummyAnimation->GetFormatName());
		}
		catch (nemesis::exception) {}
	}
}

void DummyLog::message(std::string input)
{
	emit incomingMessage(QString::fromStdString(input));
}

void InstallScripts::Run()
{
	try
	{
		++m_RunningThread;

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
				ErrorMessage(6002, ex.what(), "Alternate Animation");
			}
		}
		catch (nemesis::exception)
		{
			emit end();
			return;
		}
	}
	catch (...)
	{
		try
		{
			ErrorMessage(6002, "AA animation: Unknown", "Alternate Animation");
		}
		catch (nemesis::exception) {}
	}
	
	try
	{
		try
		{
			if (error) throw nemesis::exception();

			try
			{
				if (pcealist.size() > 0)
				{
					PCEAInstallation();
				}
			}
			catch (exception& ex)
			{
				ErrorMessage(6002, ex.what(), "PCEA Animation");
			}
		}
		catch (nemesis::exception) {}
	}
	catch (...)
	{
		try
		{
			ErrorMessage(6002, "PCEA animation: Unknown", "PCEA Animation");
		}
		catch (nemesis::exception) {}
	}

	emit end();
}

void Terminator::exitSignal()
{
	emit end();
}

vecstr getHiddenMods()
{
	return hiddenMods;
}

void elementUpdate(size_t& elementLine, int& counter, int& curID, map<int, vecstr>& catalystMap)
{
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
}
