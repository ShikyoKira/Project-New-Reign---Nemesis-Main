#include <mutex>
#include <QtCore\QCoreApplication.h>

#include "version.h"
#include "lastupdate.h"
#include "filechecker.h"
#include "updateprocess.h"
#include "dataunification.h"

#include "functions\renew.h"

#include "utilities\Terminator.h"
#include "utilities\externalscript.h"


using namespace std;

typedef unordered_map<string, unique_ptr<SSMap>> SSSMap;
typedef unordered_map<string, unique_ptr<map<string, unordered_map<string, bool>>>> MapChildState;

extern bool processdone;
extern mutex processlock;
extern condition_variable cv;
extern Terminator* p_terminate;
extern atomic<int> m_RunningThread;
extern boost::atomic_flag atomic_lock;
extern boost::atomic_flag newAnimAdditionLock;

void writeSave(FileWriter& writer, const char* line, string& store);
void writeSave(FileWriter& writer, string& line, string& store);
void stateCheck(SSMap& parent, string parentID, string lowerbehaviorfile, string sID, unique_ptr<SSMap>& stateID, unique_ptr<SSMap>& n_stateID,
	vecstr children, string filename, string ID, string modcode, StateIDList& duplicatedStateList, boost::atomic_flag& lockless);

class UpdateLock
{
	unordered_map<string, NodeU>& modUpdate;

public:
	UpdateLock(unordered_map<string, NodeU>& n_modUpdate) : modUpdate(n_modUpdate) {}

	NodeU& operator[](string key)
	{
		Lockless lock(atomic_lock);
		return modUpdate[key];
	}
};

struct arguPack
{
	arguPack(map<string, unique_ptr<map<string, vecstr, alphanum_less>>>& n_newFile, SSSMap& n_parent, MasterAnimData& n_animData, MasterAnimSetData& n_animSetData,
		shared_ptr<UpdateLock> n_modUpdate, boost::atomic_flag& n_parentLock)
		: newFile(n_newFile), parent(n_parent), animData(n_animData), animSetData(n_animSetData), parentLock(n_parentLock)
	{
		modUpdate = n_modUpdate;
	}

	shared_ptr<UpdateLock> modUpdate;
	map<string, unique_ptr<map<string, vecstr, alphanum_less>>>& newFile;

	unordered_map<string, unique_ptr<SSMap>> n_stateID;
	boost::atomic_flag stateLock = BOOST_ATOMIC_FLAG_INIT;

	unordered_map<string, unique_ptr<unordered_map<string, vecstr>>> statelist;

	SSSMap& parent;
	boost::atomic_flag& parentLock;

	MasterAnimData& animData;
	MasterAnimSetData& animSetData;
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
			ClearGlobal();

			string directory = "mod\\";
			string newAnimDirectory = "behavior templates\\";
			milestoneStart(directory);

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

				emit progressUp();	// 2

				// copy latest vanilla into memory
				if (!error && VanillaUpdate())
				{
					if (!error)
					{
						DebugLogging("Data record complete");
						emit progressUp(); // 4

						// check template for association with vanilla nodes from behavior template file
						if (newAnimUpdate(newAnimDirectory))
						{
							DebugLogging("New Animations record complete");
							emit progressUp(); // 5

							// comparing if different from mod file
							JoiningEdits(directory);

							if (!error)
							{
								DebugLogging("Modification successfully extracted");
								emit progressUp();	// 27

								// compiling all behaviors in "data/meshes" to "temp_behaviors" folder
								CombiningFiles();

								emit progressUp();	// 32
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

bool UpdateFilesStart::QueueThreadEnd()
{
	Lockless lock(stackLock);
	return queuing == multiThreads.size();
}

bool UpdateFilesStart::PathThreadCheck()
{
	return ending == 0 || getPathLoopList.size() > 0;
}

bool UpdateFilesStart::VanillaUpdate()
{
	if (error) throw nemesis::exception();

	while (multiThreads.size() <= boost::thread::hardware_concurrency() * 2)
	{
		multiThreads.push_back(make_unique<std::thread>(&UpdateFilesStart::RegisterBehavior, this));
	}

	for (auto& th : multiThreads)
	{
		th->join();
	}

	multiThreads.clear();
	emit progressUp();		// 3

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
					(wordFind(curFileName, "Nemesis_") == 0 && wordFind(curFileName, "_List") != curFileName.length() - 5 &&
						wordFind(curFileName, "_Project") != curFileName.length() - 8) || (wordFind(curFileName, "Nemesis_") == 0 &&
							wordFind(curFileName, "_Project") + 8 == curFileName.length()))
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

void UpdateFilesStart::RegisterBehavior()
{
	shared_ptr<RegisterQueue> curBehavior = registeredFiles.getNext();

	if (curBehavior == nullptr) return;

	string curFileName = curBehavior->file.stem().string();
	string fileFullName = curBehavior->file.filename().string();
	string newPath = curBehavior->file.string();

	if (boost::iequals(curFileName, "nemesis_animationdatasinglefile"))
	{
		curFileName = curFileName.substr(8);
		DebugLogging("AnimData Disassemble start (File: " + newPath + ")");

		{
			Lockless lock(behaviorPathLock);
			behaviorPath[boost::to_lower_copy(curFileName)] = boost::to_lower_copy(curBehavior->file.parent_path().string() + "\\" + curFileName);
		}

		Lockless lock(animDataLock);

		if (!AnimDataDisassemble(newPath, animData)) return;

		lock.Unlock();

		if (!saveLastUpdate(boost::to_lower_copy(newPath), lastUpdate)) return;

		DebugLogging("AnimData Disassemble complete (File: " + newPath + ")");
		emit progressUp();
	}
	else if (boost::iequals(curFileName, "nemesis_animationsetdatasinglefile"))
	{
		curFileName = curFileName.substr(8);
		DebugLogging("AnimSetData Disassemble start (File: " + newPath + ")");

		{
			Lockless lock(behaviorPathLock);
			behaviorPath[boost::to_lower_copy(curFileName)] = boost::to_lower_copy(curBehavior->file.parent_path().string() + "\\" + curFileName);
		}

		Lockless locker(animSetDataLock);

		if (!AnimSetDataDisassemble(newPath, animSetData)) return;

		locker.Unlock();

		if (!saveLastUpdate(boost::to_lower_copy(newPath), lastUpdate)) return;

		DebugLogging("AnimSetData Disassemble complete (File: " + newPath + ")");

		emit progressUp();
	}
	else if (wordFind(curFileName, "Nemesis_") == 0 && wordFind(curFileName, "_List") != curFileName.length() - 5 &&
		wordFind(curFileName, "_Project") != curFileName.length() - 8)
	{
		string firstperson = "";

		if (curBehavior->isFirstPerson) firstperson = "_1stperson\\";

		curFileName = firstperson + curFileName.substr(8);
		boost::to_lower(curFileName);
		DebugLogging("Behavior Disassemble start (File: " + newPath + ")");

		{
			Lockless lock(behaviorPathLock);
			behaviorPath[curFileName] = boost::to_lower_copy(curBehavior->file.parent_path().string() + "\\" + curBehavior->file.stem().string().substr(8));
		}

		unique_ptr<map<string, vecstr, alphanum_less>> _curNewFile = make_unique<map<string, vecstr, alphanum_less>>();
		unique_ptr<map<string, unordered_map<string, bool>>> _childrenState = make_unique<map<string, unordered_map<string, bool>>>();
		unique_ptr<SSMap> _stateID = make_unique<SSMap>();
		unique_ptr<SSMap> _parent = make_unique<SSMap>();

		VanillaDisassemble(newPath, _curNewFile, _childrenState, _stateID, _parent);

		{
			Lockless lock(newFileLock);
			newFile[curFileName] = move(_curNewFile);
			childrenState[curFileName] = move(_childrenState);
			stateID[curFileName] = move(_stateID);
			parent[curFileName] = move(_parent);
		}

		if (!saveLastUpdate(boost::to_lower_copy(newPath), lastUpdate)) return;

		DebugLogging("Behavior Disassemble complete (File: " + newPath + ")");
		emit progressUp();

		if (boost::algorithm::to_lower_copy(curBehavior->file.parent_path().filename().string()).find("characters") == 0)
		{
			unordered_map<string, bool> empty;
			registeredAnim[boost::algorithm::to_lower_copy(curFileName)] = empty;
		}
	}
	else if (wordFind(curFileName, "Nemesis_") == 0 && wordFind(curFileName, "_Project") + 8 == curFileName.length())
	{
		string firstperson = "";

		if (curBehavior->isFirstPerson) firstperson = "_1stperson\\";

		string curPath = boost::to_lower_copy(curBehavior->file.parent_path().string());
		curPath = curPath.substr(curPath.find("\\meshes\\") + 1);
		curFileName = boost::to_lower_copy(firstperson + curFileName.substr(8, curFileName.length() - 16));

		{
			Lockless lock(behaviorProjectPathLock);
			behaviorProjectPath[curFileName] = curPath;
		}

		vecstr storeline;
		bool record = false;
		DebugLogging("Nemesis Project Record start (File: " + newPath + ")");

		if (!GetFunctionLines(newPath, storeline)) return;

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

				Lockless lock(behaviorProjectLock);
				behaviorProject[characterfile].push_back(curFileName);
			}

			if (line.find("<hkparam name=\"characterFilenames\" numelements=\"") != NOT_FOUND && line.find("</hkparam>") == NOT_FOUND) record = true;
		}

		if (!saveLastUpdate(boost::to_lower_copy(newPath), lastUpdate)) return;

		emit progressUp();
		DebugLogging("Nemesis Project Record complete (File: " + newPath + ")");
	}

	RegisterBehavior();
}

void UpdateFilesStart::GetPathLoop(string path, bool isFirstPerson, bool newThread)
{
	try
	{
		try
		{
			try
			{
				if (newThread) ++ending;

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
								Lockless locker(queueLock);
								registeredFiles.files.push_back(make_shared<RegisterQueue>(curfile, false));
							}
							else if ((wordFind(curFileName, "Nemesis_") == 0 && wordFind(curFileName, "_List") != curFileName.length() - 5 &&
								wordFind(curFileName, "_Project") != curFileName.length() - 8) || (wordFind(curFileName, "Nemesis_") == 0 &&
									wordFind(curFileName, "_Project") + 8 == curFileName.length()))
							{
								Lockless locker(queueLock);
								registeredFiles.files.push_back(make_shared<RegisterQueue>(curfile, isFirstPerson));
							}
						}
					}
					else
					{
						if (boost::iequals(file, "_1stperson"))
						{
							{
								lock_guard<mutex> lock(m_cv3);

								if (waiting > 0)
								{
									Lockless locker(pathLoopLock);
									getPathLoopList.push_back(make_unique<GetPathLoopLink>(newPath + "\\", true));
									cv3.notify_one();
									continue;
								}
							}

							{
								Lockless lock(stackLock);
								bool result = multiThreads.size() <= boost::thread::hardware_concurrency() * 2;

								// look deeper into the folder for behavior file
								if (result)
								{
									multiThreads.emplace_back(make_unique<std::thread>(&UpdateFilesStart::GetPathLoop, this, newPath + "\\", true, true));

									if (error) throw nemesis::exception();

									continue;
								}
							}

							GetPathLoop(newPath + "\\", true, false);
						}
						else
						{
							{
								lock_guard<mutex> lock(m_cv3);

								if (waiting > 0)
								{
									Lockless locker(pathLoopLock);
									getPathLoopList.push_back(make_unique<GetPathLoopLink>(newPath + "\\", isFirstPerson));
									cv3.notify_one();
									continue;
								}
							}


							{
								Lockless lock(stackLock);
								bool result = multiThreads.size() <= boost::thread::hardware_concurrency() * 2;

								// look deeper into the folder for behavior file
								if (result)
								{
									multiThreads.emplace_back(make_unique<std::thread>(&UpdateFilesStart::GetPathLoop, this, newPath + "\\", isFirstPerson, true));

									if (error) throw nemesis::exception();

									continue;
								}
							}

							GetPathLoop(newPath + "\\", isFirstPerson, false);
						}
					}

					if (error) throw nemesis::exception();
				}

				if (newThread)
				{
					--ending;

					if (ending == 0)
					{
						cv3.notify_all();
					}
					else
					{
						while (ending != 0)
						{
							unique_ptr<GetPathLoopLink> loop;

							{
								unique_lock<mutex> lock(m_cv3);
								++waiting;
								cv3.wait(lock, bind(&UpdateFilesStart::PathThreadCheck, this));
								--waiting;

								if (ending == 0) break;

								Lockless locker(pathLoopLock);
								loop = move(getPathLoopList.back());
								getPathLoopList.pop_back();
							}

							GetPathLoop(loop->path, loop->first, false);
						}
					}

					{
						lock_guard<mutex> lock2(m_cv2);
						queuing++;
						cv2.notify_one();
					}

					{
						unique_lock<mutex> lock(m_cv1);

						if (queuing != 0) cv1.wait(lock);
					}

					RegisterBehavior();
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
}

bool UpdateFilesStart::VanillaDisassemble(string path, unique_ptr<map<string, vecstr, alphanum_less>>& curNewFile,
	unique_ptr<map<string, unordered_map<string, bool>>>& childrenState, unique_ptr<SSMap>& stateID, unique_ptr<SSMap>& parent)
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

						if (stateIDStr != curline)
						{
							(*stateID)[curID] = stateIDStr;
						}
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
									(*parent)[element] = curID;
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
								(*curNewFile)[curID] = storeline;
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
			(*curNewFile)[curID] = storeline;
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
			string sid = (*stateID)[ID];

			if (sid.empty()) ErrorMessage(1187);

			(*childrenState)[state.first][sid] = true;
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

void UpdateFilesStart::ModThread(string directory, string node, string behavior, unordered_map<string, shared_ptr<arguPack>>& pack)
{
	vecstr* modlist = &modQueue[behavior][node];

	for (string& modcode : *modlist)
	{
		if (error) throw nemesis::exception();

		boost::filesystem::path curPath(directory + modcode + "\\" + behavior + "\\" + node);

		if (boost::filesystem::is_directory(curPath))
		{
			if (boost::iequals(behavior, "animationdatasinglefile"))
			{
				MasterAnimData& animData(pack[modcode]->animData);

				if (animData.newAnimData.size() == 0) ErrorMessage(3017, "nemesis_animationdatasinglefile.txt");

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
				string filepath = curPath.string();
				read_directory(filepath, uniquecodelist);

				for (string& uniquecode : uniquecodelist)
				{
					if (!boost::filesystem::is_directory(filepath + "\\" + uniquecode))
					{
						AnimDataUpdate(modcode, behavior, projectname, filepath + "\\" + uniquecode, animData, newChar, lastUpdate, openAnim, openInfo);

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
			else if (boost::iequals(behavior, "animationsetdatasinglefile"))
			{
				MasterAnimSetData& animSetData(pack[modcode]->animSetData);

				if (animSetData.newAnimSetData.size() == 0) ErrorMessage(3017, "nemesis_animationsetdatasinglefile.txt");

				if (boost::filesystem::is_directory(curPath) && node.find("~") != NOT_FOUND)
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
					read_directory(curPath.string(), uniquecodelist);

					for (string& uniquecode : uniquecodelist)
					{
						AnimSetDataUpdate(modcode, behavior, node, lowerproject, curPath.string() + "\\" + uniquecode, animSetData, newProject, lastUpdate);

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
		else
		{
			unique_ptr<map<string, vecstr, alphanum_less>>& newFile(pack[modcode]->newFile[behavior]);
			shared_ptr<UpdateLock> modUpdate(pack[modcode]->modUpdate);

			(!(*modUpdate)[behavior + node.substr(0, node.find_last_of("."))].FunctionUpdate(modcode, behavior, node, newFile, pack[modcode]->n_stateID[behavior],
				pack[modcode]->parent[behavior], pack[modcode]->statelist[behavior], lastUpdate, newFileLock, pack[modcode]->stateLock, pack[modcode]->parentLock));

			if (error) throw nemesis::exception();
		}

		Lockless lock(fileCountLock);
		size_t counter = --modFileCounter[modcode][behavior];
		lock.Unlock();

		if (counter <= 0 && behavior != "animationdatasinglefile" && behavior != "animationsetdatasinglefile")
		{
			Lockless plock(pack[modcode]->parentLock);
			SSMap parent = *pack[modcode]->parent[behavior];
			plock.Unlock();
			string spath = directory + modcode + "\\" + behavior + "\\";

			for (auto& curNode : modFileList[modcode][behavior])
			{
				if (error) throw nemesis::exception();

				if (boost::filesystem::is_directory(spath + curNode)) continue;

				unordered_map<string, bool> skipped;
				unique_ptr<SSMap>& _stateID(pack[modcode]->n_stateID[behavior]);
				auto& statelist = pack[modcode]->statelist[behavior];

				for (auto& state : *statelist)
				{
					for (string& ID : state.second)		// state machine info node ID
					{
						if (error) throw nemesis::exception();

						string filename = spath + ID + ".txt";

						if (_stateID->find(ID) != _stateID->end())
						{
							string sID = (*_stateID)[ID];
							skipped[ID] = true;

							if (sID.empty()) ErrorMessage(1188, modcode, filename);

							if ((*childrenState[behavior])[state.first][sID])
							{
								stateCheck(parent, state.first, behavior, sID, stateID[behavior], _stateID, state.second, filename, ID,
									modcode, duplicatedStateList, duplicatedLock);
							}
							else
							{
								Lockless locker(stateListLock);
								set<string> list = modStateList[behavior][state.first][sID];
								locker.Unlock();

								if (list.size() > 0)
								{
									Lockless curlock(duplicatedLock);

									for (auto& modname : list)
									{
										duplicatedStateList[filename][ID][sID].insert(modname);
									}

									duplicatedStateList[filename][ID][sID].insert(modcode);
									curlock.Unlock();
								}

								list.insert(modcode);
								Lockless lockee(stateListLock);
								modStateList[behavior][ID][sID] = list;
							}
						}
						else if (ID.find("$") != NOT_FOUND)
						{
							ErrorMessage(1190, modcode, state.first, ID);
						}
					}
				}

				for (auto& ID : (*_stateID))
				{
					if (error) throw nemesis::exception();

					if (skipped[ID.first]) continue;

					bool skip = false;
					string parentID = parent[ID.first];
					string filename = spath + ID.first + ".txt";

					if (parentID.empty())
					{
						if (ID.first.find("$") != NOT_FOUND) skip = true;
						else ErrorMessage(1133, modcode, ID.first);
					}

					if (skip) continue;

					if ((*stateID[behavior])[ID.first] != ID.second)
					{
						if ((*childrenState[behavior])[parentID][ID.second])
						{
							stateCheck(parent, parentID, behavior, ID.second, stateID[behavior], _stateID,
								(*pack[modcode]->statelist[behavior])[parentID], filename, ID.first, modcode, duplicatedStateList, duplicatedLock);
						}
						else
						{
							Lockless locker(stateListLock);
							set<string> list = modStateList[behavior][parentID][ID.second];
							locker.Unlock();

							if (list.size() > 0)
							{
								Lockless curlock(duplicatedLock);

								for (auto& modname : list)
								{
									duplicatedStateList[filename][ID.first][ID.second].insert(modname);
								}

								duplicatedStateList[filename][ID.first][ID.second].insert(modcode);
								curlock.Unlock();
							}

							list.insert(modcode);
							Lockless lockee(stateListLock);
							modStateList[behavior][parentID][ID.second] = list;
						}
					}
				}
			}
		}
	}

	if (error) throw nemesis::exception();
}

void UpdateFilesStart::SeparateMod(string directory, unordered_map<string, shared_ptr<arguPack>>& pack)
{
	try
	{
		string node;
		string behavior;

		try
		{
			while (true)
			{
				int curQueue;
				int nextQueue;
				Lockless locker(stackLock);
				size_t thisQueue = queuing;
				queuing++;
				locker.Unlock();

				if (thisQueue >= processQueue.size()) break;

				node = processQueue[thisQueue].node;
				behavior = processQueue[thisQueue].file;
				curQueue = thisQueue * 20 / processQueue.size();
				thisQueue++;
				nextQueue = thisQueue * 20 / processQueue.size();

				ModThread(directory, node, behavior, pack);
				
				while (curQueue < nextQueue && curQueue < 20)
				{
					emit progressUp();
					++curQueue;
				}
			}
		}
		catch (exception& ex)
		{
			if (node.length() > 0 && behavior.length() > 0) DebugLogging("Failed to process file (Node: " + node + ", Behavior: " + behavior + ")");

			ErrorMessage(6001, ex.what());
		}
	}
	catch (nemesis::exception&) {}
}

void UpdateFilesStart::JoiningEdits(string directory)
{
	try
	{
		try
		{
			if (isFileExist(directory))
			{
				vecstr filelist;
				read_directory(directory, filelist);

				unordered_map<string, NodeU> modUpdate;
				unordered_map<string, vecstr> filelist2;
				shared_ptr<UpdateLock> modUpPtr = make_shared<UpdateLock>(modUpdate);

				boost::atomic_flag parentLock = BOOST_ATOMIC_FLAG_INIT;
				unordered_map<string, shared_ptr<arguPack>> pack;
				vector<boost::filesystem::path> pathlist;

				for (auto& modcode : filelist)
				{
					if (!boost::filesystem::is_directory(directory + modcode)) continue;

					boost::to_lower(modcode);
					read_directory(directory + modcode + "\\", filelist2[modcode]);
					pack.insert(make_pair(modcode, make_shared<arguPack>(newFile, parent, animData, animSetData, modUpPtr, parentLock)));

					for (auto& behavior : filelist2[modcode])
					{
						boost::to_lower(behavior);
						string path = directory + modcode + "\\" + behavior;

						if (!boost::filesystem::is_directory(path)) continue;

						path.append("\\");

						if (behavior == "_1stperson")
						{
							vecstr fbehaviorlist;
							read_directory(path, fbehaviorlist);

							for (auto& fbehavior : fbehaviorlist)
							{
								boost::to_lower(fbehavior);
								string fpath = path + fbehavior + "\\";
								string recName = behavior + "\\" + fbehavior;
								read_directory(fpath, modFileList[modcode][recName]);
								vecstr* curList = &modFileList[modcode][recName];

								for (auto& node : *curList)
								{
									if (boost::filesystem::is_directory(fpath + node)) continue;

									boost::to_lower(node);
									modQueue[recName][node].push_back(modcode);
								}

								pack[modcode]->statelist.insert(make_pair(recName, make_unique<unordered_map<string, vecstr>>()));
								pack[modcode]->n_stateID.insert(make_pair(recName, make_unique<SSMap>()));
							}
						}
						else
						{
							read_directory(path, modFileList[modcode][behavior]);
							vecstr* curList = &modFileList[modcode][behavior];

							if (behavior != "animationdatasinglefile" && behavior != "animationsetdatasinglefile")
							{
								for (auto& node : *curList)
								{
									if (boost::filesystem::is_directory(path + node)) continue;

									boost::to_lower(node);
									modQueue[behavior][node].push_back(modcode);
								}

								pack[modcode]->statelist.insert(make_pair(behavior, make_unique<unordered_map<string, vecstr>>()));
								pack[modcode]->n_stateID.insert(make_pair(behavior, make_unique<SSMap>()));
							}
							else
							{
								for (auto& node : *curList)
								{
									modQueue[behavior][node].push_back(modcode);
								}
							}
						}
					}
				}

				for (auto& behavior : modQueue)
				{
					for (auto& node : behavior.second)
					{
						processQueue.push_back(TargetQueue(behavior.first, node.first));
					}
				}

				for (auto& modcode : modFileList)
				{
					for (auto& behavior : modcode.second)
					{
						modFileCounter[modcode.first][behavior.first] = behavior.second.size();
					}
				}

				queuing = 0;

				if (processQueue.size() > 0)
				{
					boost::thread_group groupThread;

					while (groupThread.size() <= boost::thread::hardware_concurrency() * 2)
					{
						groupThread.create_thread(boost::bind(&UpdateFilesStart::SeparateMod, this, directory, pack));
					}

					groupThread.join_all();
				}
				else
				{
					while (queuing < 20)
					{
						emit progressUp();
						++queuing;
					}
				}

				if (error) throw nemesis::exception();

				emit progressUp();	// 26

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

void UpdateFilesStart::CombiningFiles()
{
	vecstr fileline;
	string compilingfolder = "temp_behaviors\\";
	unsigned long long bigNum = CRC32Convert(GetNemesisVersion());
	unsigned long long bigNum2 = bigNum;

	for (auto& newAnim : newAnimAddition)
	{
		string total = newAnim.first;

		for (auto& line : newAnim.second)
		{
			total.append(line);
		}

		bigNum2 += CRC32Convert(total);
	}

	for (auto& behavior : newFile) // behavior file name
	{
		string rootID;
		bool isOpen = false;
		string OpeningMod;

		for (auto& node : (*behavior.second)) // behavior node ID
		{
			if (node.first.find("$") != NOT_FOUND)
			{
				string modID = node.first.substr(1, node.first.find("$") - 1);

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

			vecstr storeline = node.second;
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
				ErrorMessage(2008, behavior.first + " (" + node.first + ")");
			}
		}

		if (isOpen)
		{
			fileline.push_back("<!-- CLOSE -->");
			isOpen = false;
		}

		if (CreateFolder(compilingfolder))
		{
			string lowerBehaviorFile = behavior.first;
			string filename = compilingfolder + lowerBehaviorFile + ".txt";
			bool firstPerson = lowerBehaviorFile.find("_1stperson\\") == 0;

			if (firstPerson) boost::filesystem::create_directory(boost::filesystem::path(compilingfolder + "_1stperson\\"));

			FileWriter output(filename);

			if (output.is_open())
			{
				bool behaviorRef = false;
				string total = filename + "\n";

				writeSave(output, "<?xml version=\"1.0\" encoding=\"ascii\"?>\n", total);
				writeSave(output, "<hkpackfile classversion=\"8\" contentsversion=\"hk_2010.2.0 - r1\" toplevelobject=\"" + rootID + "\">\n\n", total);
				writeSave(output, "	<hksection name=\"__data__\">\n\n", total);

				for (auto& line : fileline)
				{
					writeSave(output, line + "\n", total);
					size_t pos = line.find("<hkobject name=\"");

					if (pos != NOT_FOUND && line.find("signature=\"", pos) != NOT_FOUND)
					{
						behaviorRef = line.find("class=\"hkbBehaviorReferenceGenerator\" signature=\"", pos) != NOT_FOUND;
					}

					if (behaviorRef && line.find("<hkparam name=\"behaviorName\">") != NOT_FOUND)
					{
						size_t nextpos = line.find("behaviorName\">") + 14;
						string behaviorName = GetFileName(line.substr(nextpos, line.find("</hkparam>", nextpos) - nextpos));
						boost::algorithm::to_lower(behaviorName);

						if (lowerBehaviorFile.find("_1stperson") != NOT_FOUND) behaviorName = "_1stperson\\" + behaviorName;

						behaviorJoints[behaviorName].push_back(lowerBehaviorFile);
						behaviorRef = false;
					}
					else if (line.find("<hkparam name=\"behaviorFilename\">") != NOT_FOUND)
					{
						size_t nextpos = line.find("behaviorFilename\">") + 18;
						string behaviorName = line.substr(nextpos, line.find("</hkparam>", nextpos) - nextpos);
						behaviorName = GetFileName(behaviorName);
						boost::algorithm::to_lower(behaviorName);

						if (lowerBehaviorFile.find("_1stperson") != NOT_FOUND) behaviorName = "_1stperson\\" + behaviorName;

						behaviorJoints[behaviorName].push_back(lowerBehaviorFile);
					}

				}

				writeSave(output, "	</hksection>\n\n", total);
				writeSave(output, "</hkpackfile>\n", total);
				fileline.clear();
				(firstPerson ? bigNum2 : bigNum) += CRC32Convert(total);
			}
			else
			{
				ErrorMessage(2009, filename);
			}
		}
	}

	emit progressUp();	// 28
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
				string total = filename + "\n";
				writeSave(output, to_string(animData.animDataChar.size() - 1) + "\n", total);

				for (unsigned int i = 0; i < animData.newAnimData["$header$"]["$header$"].size(); ++i)
				{
					writeSave(output, animData.newAnimData["$header$"]["$header$"][i] + "\n", total);
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
						writeSave(output, to_string(animsize) + "\n", total);

						for (unsigned int j = 0; j < animData.animDataHeader[project].size(); ++j)
						{
							string header = animData.animDataHeader[project][j];
							outputlist << header + "\n";

							for (unsigned int k = 0; k < animData.newAnimData[project][header].size(); ++k)
							{
								writeSave(output, animData.newAnimData[project][header][k] + "\n", total);
							}
						}
					}

					for (auto& header : animData.animDataInfo[project])
					{
						infosize += animData.newAnimData[project][header].size();
					}

					if (infosize > 0)
					{
						writeSave(output, to_string(infosize) + "\n", total);

						for (unsigned int j = 0; j < animData.animDataInfo[project].size(); ++j)
						{
							string header = animData.animDataInfo[project][j];
							outputlist << header + "\n";

							for (unsigned int k = 0; k < animData.newAnimData[project][header].size(); ++k)
							{
								writeSave(output, animData.newAnimData[project][header][k] + "\n", total);
							}
						}

						outputlist << "\n";
					}
				}

				bigNum2 += CRC32Convert(total);
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

	emit progressUp();	// 29

	if (CreateFolder(compilingfolder))
	{
		string filename = compilingfolder + "animationsetdatasinglefile.txt";
		FileWriter output(filename);
		FileWriter outputlist("cache\\animationsetdata_list");

		if (output.is_open())
		{
			if (outputlist.is_open())
			{
				string total = filename + "\n";
				writeSave(output, to_string(animSetData.projectList.size() - 1) + "\n", total);

				for (string& header : animSetData.newAnimSetData["$header$"]["$header$"])
				{
					writeSave(output, header + "\n", total);
				}

				for (string& project : animSetData.projectList)
				{
					outputlist << project + "\n";		// character
					outputlist << "$header$\n";

					for (string& line : animSetData.newAnimSetData[project]["$header$"])
					{
						writeSave(output, line + "\n", total);
					}

					for (auto it = animSetData.newAnimSetData[project].begin(); it != animSetData.newAnimSetData[project].end(); ++it)
					{
						string header = it->first;

						if (header != "$header$")
						{
							outputlist << header + "\n";

							for (unsigned int k = 0; k < it->second.size(); ++k)
							{
								writeSave(output, it->second[k] + "\n", total);
							}
						}
					}

					outputlist << "\n";
				}

				bigNum2 += CRC32Convert(total);
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

	emit progressUp();	// 30

	FileWriter lastmod("cache\\engine_update");

	if (lastmod.is_open())
	{
		lastmod << GetNemesisVersion() << "\n";
		engineVersion = to_string(bigNum % 10000) + "-" + to_string(bigNum2 % 10000);
		lastmod << engineVersion << "\n";

		for (auto& it : lastUpdate)
		{
			lastmod << it.first + ">>" + it.second + "\n";
		}
	}
	else
	{
		ErrorMessage(2009, "cache\\engine_update");
	}

	emit progressUp();	// 31
}

bool UpdateFilesStart::newAnimUpdate(string sourcefolder)
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
										newAnimAddition, lastUpdate)) return false;
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
										animSetData, newAnimAddition, lastUpdate)) return false;
								}
							}

							DebugLogging("New Animations extraction complete (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");
						}
						else
						{
							if (boost::iequals(behaviorlist[j], "_1stperson")) ErrorMessage(6004, folderpath + "\\" + behaviorlist[j]);

							DebugLogging("New Animations extraction start (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");

							if (!newAnimUpdateExt(folderpath, codelist[i], boost::to_lower_copy(behaviorlist[j]), *newFile[boost::to_lower_copy(behaviorlist[j])],
								newAnimAddition, lastUpdate))
								return false;

							DebugLogging("New Animations extraction complete (Folder: " + folderpath + "\\" + behaviorlist[j] + ")");
						}
					}
				}
			}
			else if (codefile.extension().string() == ".txt")
			{
				vecstr storeline;

				if (!saveLastUpdate(boost::to_lower_copy(folderpath), lastUpdate)) return false;

				if (!GetFunctionLines(folderpath, storeline)) return false;

				Lockless lock(newAnimAdditionLock);
				newAnimAddition[boost::to_lower_copy(codefile.string())] = storeline;
			}
			else
			{
				ErrorMessage(3024, folderpath);
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
	filenum = 32;

#ifdef DEBUG
	string path = "data\\";
#else
	string path = nemesisInfo->GetDataPath() + "meshes";
#endif

	DebugLogging("Detecting processes...");

	if (!isFileExist(path)) boost::filesystem::create_directory(path);

	queuing = 0;
	waiting = 0;
	ending = 0;

	GetPathLoop(nemesisInfo->GetDataPath() + "meshes\\", false, false);

	{
		unique_lock<mutex> ulock2(m_cv2);
		cv2.wait(ulock2, bind(&UpdateFilesStart::QueueThreadEnd, this));
	}

	filenum += registeredFiles.files.size();

	{
		lock_guard<mutex> ulock(m_cv1);
		queuing = 0;
		cv1.notify_all();
	}

	DebugLogging("Process count: " + to_string(filenum));
	emit progressMax(filenum);
	connectProcess(this);

	if (error) throw nemesis::exception();

	emit progressUp();		// 1
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
			msg = TextBoxMessage(1017) + ": " + engineVersion;
			interMsg(msg);
			DebugLogging(msg);
			emit disableLaunch(false);
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
		emit disable(false);
		emit hide(true);
	}

	if (error) emit disableLaunch(true);

	m_RunningThread = 0;
	emit end();
}

void writeSave(FileWriter& writer, const char* line, string& store)
{
	writer << line;
	store.append(line);
}

void writeSave(FileWriter& writer, string& line, string& store)
{
	writer << line;
	store.append(line);
}

void stateCheck(SSMap& parent, string parentID, string lowerbehaviorfile, string sID, unique_ptr<SSMap>& stateID, unique_ptr<SSMap>& n_stateID,
	vecstr children, string filename, string ID, string modcode, StateIDList& duplicatedStateList, boost::atomic_flag& lockless)
{
	bool skip = false;

	for (auto& child : parent)
	{
		if (parentID == child.second)
		{
			if ((*stateID)[child.first] == sID)
			{
				for (string& match : children)
				{
					if (match == child.first)
					{
						if (n_stateID->find(match) != n_stateID->end())
						{
							if (ID != match || ((*n_stateID)[match] == sID && ID == match))
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
		Lockless lock(lockless);
		duplicatedStateList[filename][ID][sID].insert(modcode);
		duplicatedStateList[filename][ID][sID].insert("Vanilla");
	}
}
