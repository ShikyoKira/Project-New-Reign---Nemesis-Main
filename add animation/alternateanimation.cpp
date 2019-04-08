#include <boost\algorithm\string.hpp>
#include <boost\process.hpp>
#include <boost\process\windows.hpp>
#include <boost\iostreams\device\array.hpp>
#include <boost\iostreams\stream_buffer.hpp>
#include <boost\crc.hpp>
#include <QtCore\QStandardPaths.h>
#include "functions\readtextfile.h"
#include "functions\writetextfile.h"
#include "generator_utility.h"
#include "alternateanimation.h"
#include "Nemesis Main GUI\src\utilities\filerelease.h"

#pragma warning(disable:4503)

using namespace std;

unordered_map<string, int> AAgroup_Counter;

bool AACoreCompile(string filename, string import, string destination, string filepath, string appdata_path, vecstr& newFunctions, unsigned int& maxGroup,
	unsigned int& uniquekey);
bool AAnimAPICompile(string filename, string import, string destination, string filepath, string appdata_path, vecstr& newFunctions, unsigned int maxGroup,
	unsigned int& uniquekey);
void fixedKeyInitialize();
unsigned int getUniqueKey(unsigned char bytearray[], int byte1, int byte2);
bool PapyrusCompileProcess(string pscfile, string import, string destination, string filepath, string appdata_path, boost::filesystem::path compiler, bool tryagain = false);
void ByteCopyToData(string target, string destination);

struct ModIDByGroup
{
	string groupBase;
	string modID;
};

void AAInitialize(string AAList)
{
	vecstr groupList;
	char charline[2000];
	unordered_map<string, string> existAAAnim;			// animation name, animation group name; has the animation been registered for AA?
	DebugLogging("Caching alternate animation group...");
	read_directory(AAList, groupList);

	for(string& groupName : groupList)
	{
		if (!boost::iequals(groupName, "alternate animation.script") && boost::filesystem::path(AAList + "\\" + groupName).extension().string() == ".txt")
		{
			shared_ptr<FileReader> doc = make_shared<FileReader>(AAList + "\\" + groupName);

			if (doc->GetFile())
			{
				string AAGroupName = groupName.substr(0, groupName.find_last_of("."));

				while (fgets(charline, 2000, doc->GetFile()))
				{
					string animFile = charline;

					if (animFile.length() != 0)
					{
						if (animFile[animFile.size() - 1] == '\n')
						{
							animFile.pop_back();
						}

						string lowerAnimFile = boost::algorithm::to_lower_copy(animFile);

						if (existAAAnim[lowerAnimFile].length() == 0)
						{
							string lowerGroupName = boost::algorithm::to_lower_copy(AAGroupName);
							groupAA[lowerGroupName].push_back(lowerAnimFile);
							AAGroup[lowerAnimFile] = lowerGroupName;
							existAAAnim[lowerAnimFile] = lowerGroupName;
							groupNameList.insert(lowerGroupName);
							groupNameList.insert(lowerGroupName + "_1p*");
						}
						else
						{
							ErrorMessage(4001, AAGroupName, existAAAnim[lowerAnimFile]);
						}
					}
				}
			}
			else
			{
				ErrorMessage(4000, AAList);
			}
		}
	}

	DebugLogging("Caching alternate animation complete");
}

bool AAInstallation()
{
	if (AAGroup.size() == 0)
	{
		return true;
	}

	unsigned int uniquekey;
	wstring cachedir = QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0).toStdWString() + L"/Nemesis";
	replace(cachedir.begin(), cachedir.end(), '/', '\\');

	try
	{
		boost::filesystem::create_directories(cachedir);
	}
	catch (const exception& ex)
	{
		ErrorMessage(6002, ex.what());
	}

	if (error) throw nemesis::exception();

	namespace bf = boost::filesystem;
	string import = skyrimDataPath->GetDataPath() + (SSE ? "source\\scripts" : "scripts\\source");
	string destination = skyrimDataPath->GetDataPath() + "scripts";
	bf::path source("alternate animation\\alternate animation.script");
	bf::path pscfile(cachedir + L"\\Nemesis_AA_Core.psc");
	string filepath = destination + "\\Nemesis_AA_Core.pex";
	bf::copy_file(source, pscfile, bf::copy_option::overwrite_if_exists);
	DebugLogging(pscfile.string());
	DebugLogging(filepath);

	if (!FolderCreate(import))
	{
		ErrorMessage(2010, import);
	}

	if (!isFileExist(import))
	{
		ErrorMessage(2010, import);
	}

	unsigned int maxGroup;
	string sCacheDir = bf::path(cachedir).string();
	fixedKeyInitialize();
	vecstr newFunctions;
	
	if (!AACoreCompile(pscfile.string(), import, destination, filepath, sCacheDir, newFunctions, maxGroup, uniquekey))
	{
		return false;
	}

	if (error) throw nemesis::exception();

	source = bf::path("alternate animation\\alternate animation 2.script");
	pscfile = bf::path(cachedir + L"\\FNIS_aa.psc");
	filepath = destination + "\\FNIS_aa.pex";
	bf::copy_file(source, pscfile, bf::copy_option::overwrite_if_exists);
	DebugLogging(pscfile.string());
	DebugLogging(filepath);

	if (!AAnimAPICompile(pscfile.string(), import, destination, filepath, sCacheDir, newFunctions, maxGroup, uniquekey))
	{
		return false;
	}

	if (error) throw nemesis::exception();

	return true;
}

bool AACoreCompile(string filename, string import, string destination, string filepath, string appdata_path, vecstr& newFunctions, unsigned int& maxGroup,
	unsigned int& uniquekey)
{
	bool prefixDone = false;
	set<string> prefixList;
	unordered_map<string, int> prefixID;
	unordered_map<string, int> groupIDCounter;
	unordered_map<string, string> baseOrder;
	unordered_map<string, vector<ModIDByGroup>> GetModByGroupValue;
	vecstr baseMatch;

	vecstr groupIDFunction;
	vecstr prefixlines;
	vecstr groupAAlines;
	vecstr storeline;
	vecstr newline;
	newline.reserve(storeline.size());
	GetFunctionLines(filename, storeline);

	int AACounter = 0;
	maxGroup = 0;

	for (auto& groupName : groupNameList)
	{
		for (auto& AAprefixGroup : groupAAPrefix[groupName])	// list of group aa prefix categorized by animation group name while eliminating duplicates using set container
		{
			prefixList.insert(AAprefixGroup);
		}
	}

	if (prefixList.size() > 0)			// Assign mod ID
	{
		auto& nextprefix = prefixList.begin();
		string templine = "	if (curAAPrefix == \"" + *nextprefix + "\")";
		string rr = "		return 0";
		prefixID[*nextprefix] = maxGroup;
		prefixlines.push_back(templine);
		prefixlines.push_back(rr);
		++nextprefix;
		++maxGroup;

		for (auto& prefix = nextprefix; prefix != prefixList.end(); ++prefix)
		{
			templine = "	elseif (curAAPrefix == \"" + *prefix + "\")";
			rr = "		return " + to_string(maxGroup);
			prefixID[*prefix] = maxGroup;
			prefixlines.push_back(templine);
			prefixlines.push_back(rr);
			++maxGroup;
		}
	}

	maxGroup = 0;
	DebugLogging("AA prefix script complete");

	if (groupNameList.size() > 0)		// Assign base value
	{
		vecstr groupID;
		groupIDFunction.push_back("int Function GetGroupID(string groupName) global");

		for (auto& groupName : groupNameList)
		{
			string adjGN = groupName;

			if (groupName.rfind("_1p*") == groupName.length() - 4) adjGN.pop_back();

			for (auto& prefix : prefixID)
			{
				for (unsigned int j = 0; j < groupAAPrefix[groupName].size(); ++j)
				{
					if (groupAAPrefix[groupName][j] == prefix.first)
					{
						string templine = "	AASet[num] = ";
						templine.replace(templine.find("num"), 3, to_string(AACounter));
						string number = to_string(prefix.second / 10) + to_string(prefix.second % 10) + to_string(maxGroup / 100) + to_string(maxGroup % 100 / 10) +
							to_string(maxGroup % 10);
						string AAgroupID = to_string(maxGroup / 10) + to_string(maxGroup % 10);

						if (groupIDCounter[adjGN] == 0)
						{
							groupIDCounter[adjGN] = 1;
						}

						string counter = to_string(groupIDCounter[adjGN]);
						string base = counter.substr(0, 3);
						ModIDByGroup mod;
						mod.groupBase = base;
						mod.modID = to_string(prefix.second / 10) + to_string(prefix.second % 10);
						GetModByGroupValue[AAgroupID].push_back(mod);

						if (AAGroupCount[groupAAPrefix[groupName][j]][groupName] == 0)
						{
							ErrorMessage(3013, groupAAPrefix[groupName][j], groupName);
						}

						groupIDCounter[adjGN] += AAGroupCount[groupAAPrefix[groupName][j]][groupName];

						while (counter.length() < 3)
						{
							counter = "0" + counter;
						}

						baseOrder["AAgroupID == " + AAgroupID] = "		return " + to_string(++AAgroup_Counter[adjGN]);
						baseMatch.push_back("DataCode == " + number + "000");
						baseMatch.push_back("		return " + base);
						number = number + counter.substr(0, 3);
						templine.append(number);
						groupAAlines.push_back(templine);
						++AACounter;
					}
				}
			}

			// Assign animation group ID
			newFunctions.push_back("int Function " + adjGN + "() global");
			groupID.push_back(adjGN);
			groupID.push_back(to_string(maxGroup));
			newFunctions.push_back("	return " + to_string(maxGroup));
			newFunctions.push_back("endFunction");
			newFunctions.push_back("");
			++maxGroup;
		}

		if (groupID.size() > 0)
		{
			groupIDFunction.push_back("	if (groupName == \"" + groupID[0] + "\")");
			groupIDFunction.push_back("		return " + groupID[1]);

			for (unsigned int k = 2; k < groupID.size(); ++k)
			{
				if (k % 2 == 0)
				{
					groupIDFunction.push_back("	elseif (groupName == \"" + groupID[k] + "\")");
				}
				else
				{
					groupIDFunction.push_back("		return " + groupID[k]);
				}
			}
			
			groupIDFunction.push_back("	endif");
			groupIDFunction.push_back("	Debug.Trace(\"ERROR: Unknown Nemesis AA group name (Group Name: \" + groupName + \")\")");
			groupIDFunction.push_back("	return -1");
			groupIDFunction.push_back("endFunction");
			groupIDFunction.push_back("");
		}
	}

	DebugLogging("Group base value complete");

	for (unsigned int k = 0; k < storeline.size(); ++k)
	{
		bool skip = false;
		string line = storeline[k];

		if (line.find("AASet[num] = ") != NOT_FOUND)
		{
			newline.insert(newline.end(), groupAAlines.begin(), groupAAlines.end());
			skip = true;
		}
		else if (line.find("(curAAPrefix == ") != NOT_FOUND)
		{
			if (prefixlines.size() > 0)
			{
				newline.insert(newline.end(), prefixlines.begin(), prefixlines.end());
				newline.push_back("	endif");
			}

			skip = true;
		}
		else if (line.find("(DataCode == num)") != NOT_FOUND)
		{
			if (baseMatch.size() > 0)
			{
				newline.push_back("	if (" + baseMatch[0] + ")");
				newline.push_back(baseMatch[1]);				

				for (unsigned int j = 2; j < baseMatch.size(); ++j)
				{
					if (j % 2 == 0)
					{
						newline.push_back("	elseif (" + baseMatch[j] + ")");
					}
					else
					{
						newline.push_back(baseMatch[j]);
					}
				}

				newline.push_back("	endif");
			}

			skip = true;
		}
		else if (line.find("(AAgroupID == num)") != NOT_FOUND)
		{
			if (baseOrder.size() > 0)
			{
				int counter = 0;
				auto& firstOrder = baseOrder.begin();
				newline.push_back("	if (" + firstOrder->first + ")");
				newline.push_back(firstOrder->second);
				++firstOrder;

				for (auto& order = firstOrder; order != baseOrder.end(); ++order)
				{
					newline.push_back("	elseif (" + order->first + ")");
					newline.push_back(order->second);

					++counter;
				}

				newline.push_back("	endif");
			}

			skip = true;
		}
		else if (line.find("(AAgroupID == value)") != NOT_FOUND)
		{
			if (GetModByGroupValue.size() > 0)
			{
				auto& firstGroup = GetModByGroupValue.begin();
				string space = "	";
				newline.push_back(space + "if (AAgroupID == " + firstGroup->first + ")");
				space += "	";

				if (firstGroup->second.size() > 1)
				{
					for (unsigned int j = 0; j < firstGroup->second.size(); ++j)
					{
						if (j + 1 == firstGroup->second.size() - 1)
						{
							newline.push_back(space + "if (groupValue < " + firstGroup->second[j].groupBase + ")");
							newline.push_back(space + "	return " + firstGroup->second[j].modID);
							newline.push_back(space + "else");
							newline.push_back(space + "	return " + firstGroup->second[j + 1].modID);
							break;
						}
						else
						{
							newline.push_back(space + "if (groupValue < " + firstGroup->second[j + 1].groupBase + ")");
							space += "	";
							newline.push_back(space + "return " + firstGroup->second[j].modID);
						}
					}

					while (space.length() > 1)
					{
						newline.push_back(space + "endif");
						space.pop_back();
					}
				}
				else
				{
					newline.push_back(space + "return " + firstGroup->second[0].modID);
					space.pop_back();
				}

				++firstGroup;

				for (auto& group = firstGroup; group != GetModByGroupValue.end(); ++group)
				{
					newline.push_back(space + "elseif (AAgroupID == " + group->first + ")");
					space += "	";

					if (group->second.size() > 1)
					{
						for (unsigned int j = 0; j < group->second.size(); ++j)
						{
							if (j + 1 == group->second.size() - 1)
							{
								newline.push_back(space + "if (groupValue < " + group->second[j + 1].groupBase + ")");
								newline.push_back(space + "	return " + group->second[j].modID);
								newline.push_back(space + "else");
								newline.push_back(space + "	return " + group->second[j + 1].modID);
								break;
							}
							else
							{
								newline.push_back(space + "if (groupValue < " + group->second[j + 1].groupBase + ")");
								space += "	";
								newline.push_back(space + "return " + group->second[j].modID);
							}
						}

						while (space.length() > 1)
						{
							newline.push_back(space + "endif");
							space.pop_back();
						}
					}
					else
					{
						newline.push_back(space + "return " + group->second[0].modID);
						space.pop_back();
					}
				}

				newline.push_back(space + "endif");
			}

			skip = true;
		}
		else if (line.find("$SetCount$") != NOT_FOUND)
		{
			line.replace(line.find("$SetCount$"), 10, to_string(AACounter));
		}
		else if (line.find("$GroupCount$") != NOT_FOUND)
		{
			line.replace(line.find("$GroupCount$"), 12, to_string(groupNameList.size()));
		}

		if (!skip)
		{
			newline.push_back(line);
		}
	}

	if (groupIDFunction.size() > 0)
	{
		newline.insert(newline.end(), groupIDFunction.begin(), groupIDFunction.end());
	}

	for (auto& curline : newline)
	{
		uniquekey = uniquekey + CRC32Convert(curline);
		uniquekey = uniquekey % 987123;
	}

	{
		FileWriter output(filename);

		if (output.is_open())
		{
			for (auto& line : newline)
			{
				output << line << "\n";
			}
		}
		else
		{
			ErrorMessage(3002, filename);
		}
	}

	if (!PapyrusCompile(filename, import, destination, filepath, appdata_path))
	{
		return false;
	}

	DebugLogging("AA core script complete");
	return true;
}

bool AAnimAPICompile(string filename, string import, string destination, string filepath, string appdata_path, vecstr& newFunctions, unsigned int maxGroup,
	unsigned int& uniquekey)
{
	vecstr storeline;
	vecstr newline;
	newline.reserve(storeline.size());

	if (!GetFunctionLines(filename, storeline))
	{
		return false;
	}

	for (string& line : storeline)
	{
		size_t pos = line.find("$InstallationKey$");

		if (pos != NOT_FOUND)
		{
			line.replace(pos, 17, to_string(uniquekey));
		}
		else
		{
			pos = line.find("$MaxGroup$");

			if (pos != NOT_FOUND)
			{
				line.replace(pos, 10, to_string(maxGroup - 1));
			}
		}

		newline.push_back(line);
	}

	if (newline.size() > 0 && newline.back().length() > 0)
	{
		newline.push_back("");
	}

	newline.insert(newline.end(), newFunctions.begin(), newFunctions.end());

	{
		FileWriter output(filename);

		if (output.is_open())
		{
			for (auto& line : newline)
			{
				output << line << "\n";
			}
		}
		else
		{
			ErrorMessage(3002, filename);
		}
	}

	if (!PapyrusCompile(filename, import, destination, filepath, appdata_path))
	{
		return false;
	}

	return true;
}

void fixedKeyInitialize()
{
	int counter = 0;

	while (counter <= 256)
	{
		unsigned int key1 = unsigned int(counter);
		int key2 = 0;

		while (key2 <= 7)
		{
			if ((key1 & 1) > 0)		// is odd number
			{
				key1 = key1 >> 1 ^ 3988292384;
			}
			else
			{
				key1 >>= 1;
			}

			++key2;
		}

		fixedkey[counter] = key1;
		++counter;
	}
}

string GetLastModified(string filename)
{
	try
	{
		std::time_t lastmodified = boost::filesystem::last_write_time(filename);
		char time1[26];
		struct tm buf;
		localtime_s(&buf, &lastmodified);
		asctime_s(time1, sizeof time1, &buf);
		string time = time1;
		time.pop_back();

		return time;
	}
	catch (...)
	{
		ErrorMessage(2022);
	}
}

bool FolderCreate(string curBehaviorPath)
{
	try
	{
		boost::filesystem::create_directories(curBehaviorPath);
	}
	catch (const exception& ex)
	{
		ErrorMessage(6002, ex.what());
	}

	return true;
}

unsigned int CRC32Convert(string line)
{
	boost::crc_optimal<32, 0x4C11DB7, 0, 0, true, true> result;
	result.process_bytes(line.data(), line.length());
	return result.checksum();
}

unsigned int getUniqueKey(unsigned char bytearray[], int byte1, int byte2)
{
	unsigned int uniqueKey = 0;
	int key1 = byte1;
	int key2 = byte2;
	int counter = byte1;

	while (counter <= key2)
	{
		unsigned char curByte = bytearray[counter];
		unsigned int key3 = uniqueKey ^ unsigned int(curByte);
		uniqueKey = uniqueKey >> 8 ^ fixedkey[key3 & 255];
		counter++;
	}

	return uniqueKey;
}

bool PapyrusCompile(string pscfile, string import, string destination, string filepath, string appdata_path)
{
	if (!isFileExist(pscfile)) ErrorMessage(1092, pscfile);
	if (!isFileExist(destination)) ErrorMessage(1001, destination);

	string timeline;
	namespace bf = boost::filesystem;
	bf::path target = bf::path(skyrimDataPath->GetDataPath());

	while (!boost::iequals(target.stem().string(), "data"))
	{
		target = target.parent_path();
	}

	target = target.parent_path();
	target = bf::path(target.string() + "\\Papyrus Compiler\\PapyrusCompiler.exe");

	if (isFileExist(filepath))
	{
		if (ReleaseLockedFile(filepath) && !boost::filesystem::remove(filepath))
		{
			timeline = GetLastModified(filepath);
		}
	}
		
	if (!isFileExist(target.string()) || !PapyrusCompileProcess(pscfile, import, destination, filepath, appdata_path, target))
	{
		string compiler = "Papyrus Compiler\\PapyrusCompiler.exe";

		if (isFileExist(compiler))
		{
			if (!PapyrusCompileProcess(pscfile, import, destination, filepath, appdata_path, compiler, true))
			{
				throw nemesis::exception();
			}
		}
		else
		{
			ErrorMessage(6007);
		}
	}

	if (timeline.length() > 0)
	{
		if (timeline == GetLastModified(filepath))
		{
			ErrorMessage(1185, filepath);
		}
	}

	return true;
}

bool PapyrusCompileProcess(string pscfile, string import, string destination, string filepath, string appdata_path, boost::filesystem::path compiler, bool tryagain)
{
	pscfile = GetFileName(pscfile) + ".psc";
	string importedSource = import + "\\" + pscfile;
	vecstr args{ pscfile, "-f=TESV_Papyrus_Flags.flg" , "-i=" + appdata_path + ";" + import + ";Papyrus Compiler\\backup scripts" ,
		"-o=" + appdata_path };
	future<vector<char>> p_reader, p_error;
	
	if (isFileExist(filepath) && !boost::filesystem::is_directory(filepath) && ReleaseLockedFile(filepath) && !boost::filesystem::remove(filepath))
	{
		ErrorMessage(1082, filepath);
	}

	if (isFileExist(importedSource) && !boost::filesystem::is_directory(importedSource) && ReleaseLockedFile(importedSource) && !boost::filesystem::remove(importedSource))
	{
		ErrorMessage(1082, importedSource);
	}


	if (boost::process::system(compiler, args, boost::process::std_out > p_reader, boost::process::std_err > p_error, boost::process::windows::hide) != 0)
	{
		// Compilation fail
	}

	string tempfile = GetFileName(filepath) + ".pex";
	string tempfilepath = appdata_path + "\\" + tempfile;

	if (!isFileExist(tempfilepath))
	{
		string line;
		vecstr linelist;

		{
			auto raw = p_reader.get();
			vecstr data;
			string templine;
			boost::iostreams::stream_buffer<boost::iostreams::array_source> sb(raw.data(), raw.size());
			istream is(&sb);

			while (getline(is, templine) && !templine.empty())
			{
				linelist.push_back(templine + "\n");
			}

			if (linelist.size() > 1)
			{
				for (unsigned int i = 0; i < linelist.size() - 1; ++i)
				{
					line.append(linelist[i]);
				}
			}
		}

		auto raw = p_error.get();
		vecstr data;
		string templine;
		boost::iostreams::stream_buffer<boost::iostreams::array_source> sb(raw.data(), raw.size());
		istream is(&sb);

		while (getline(is, templine) && !templine.empty())
		{
			line.append(templine + "\n");
		}

		if (linelist.size() > 0)
		{
			line.append(linelist.back());
		}

		if (line.find("Compilation succeeded") != NOT_FOUND && line.find("Assembly succeeded") != NOT_FOUND && line.find("0 error") != NOT_FOUND)
		{
			return true;
		}
		
		if (!tryagain) return false;

		try
		{
			interMsg("Output: \n" + line);
			DebugLogging("\nOutput: \n" + line, false);
			ErrorMessage(1185, filepath);
		}
		catch (nemesis::exception)
		{
			return false;
		}
	}

	ByteCopyToData(tempfilepath, destination + "\\" + tempfile);
	ByteCopyToData(appdata_path + "\\" + pscfile, importedSource);
	return true;
}

void ByteCopyToData(string target, string destination)
{
	ifstream file(target, ios::binary);
	ofstream paste(destination, ios::binary);
	copy(istreambuf_iterator<char>(file), istreambuf_iterator<char>(), ostreambuf_iterator<char>(paste));
	file.close();

	try
	{
		if (!boost::filesystem::remove(target))
		{
			ErrorMessage(1082, target);
		}
	}
	catch (std::exception& ex)
	{
		ErrorMessage(6002, ex.what());
	}
}
