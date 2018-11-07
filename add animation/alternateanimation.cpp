#include "alternateanimation.h"
#include "readtextfile.h"
#include <boost\algorithm\string.hpp>
#include <boost\process.hpp>
#include <boost\process\windows.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost\crc.hpp>
#include "generator_utility.h"

#pragma warning(disable:4503)

using namespace std;

unordered_map<string, int> AAgroupCount;

bool AACoreCompile(string filename, string import, string destination, string filepath, vecstr& newFunctions, unsigned int& uniquekey);
bool AAnimAPICompile(string filename, string import, string destination, string filepath, vecstr& newFunctions, unsigned int& uniquekey);
void fixedKeyInitialize();
unsigned int getUniqueKey(unsigned char bytearray[], int byte1, int byte2);
bool PapyrusCompileProcess(string pscfile, string import, string destination, string filepath, boost::filesystem::path compiler);

struct ModIDByGroup
{
	string groupBase;
	string modID;
};

void AAInitialize(string AAList)
{
	vecstr groupName;
	char charline[2000];
	unordered_map<string, string> existAAAnim;			// animation name, animation group name; has the animation been registered for AA?
	DebugLogging("Caching alternate animation group...");
	read_directory(AAList, groupName);

	for (unsigned int i = 0; i < groupName.size(); ++i)
	{
		if (!boost::iequals(groupName[i], "alternate animation.script") && boost::filesystem::path(AAList + "\\" + groupName[i]).extension().string() == ".txt")
		{
			shared_ptr<TextFile> doc = make_shared<TextFile>(AAList + "\\" + groupName[i]);

			if (doc->GetFile())
			{
				string AAGroupName = groupName[i].substr(0, groupName[i].find_last_of("."));

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
						}
						else
						{
							ErrorMessage(4001, AAGroupName, existAAAnim[lowerAnimFile]);
							return;
						}
					}
				}
			}
			else
			{
				ErrorMessage(4000, AAList);
				return;
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

	string import;
	string destination = skyrimDataPath->GetDataPath() + "scripts";

	if (SSE)
	{
		import = skyrimDataPath->GetDataPath() + "source\\scripts";
	}
	else
	{
		import = skyrimDataPath->GetDataPath() + "scripts\\source";
	}

	string pscfile = import + "\\Nemesis_AA_Core.psc";
	string filepath = destination + "\\Nemesis_AA_Core.pex";
	DebugLogging(pscfile);
	DebugLogging(filepath);

	if (!FolderCreate(import))
	{
		return false;
	}

	fixedKeyInitialize();
	boost::filesystem::path source("alternate animation\\alternate animation.script");
	boost::filesystem::path target(pscfile);
	boost::filesystem::copy_file(source, target, boost::filesystem::copy_option::overwrite_if_exists);	
	vecstr newFunctions;
	
	if (!AACoreCompile(pscfile, import, destination, filepath, newFunctions, uniquekey))
	{
		return false;
	}

	if (error)
	{
		return false;
	}

	pscfile = import + "\\FNIS_aa.psc";
	filepath = destination + "\\FNIS_aa.pex";
	DebugLogging(pscfile);
	DebugLogging(filepath);

	source = boost::filesystem::path("alternate animation\\alternate animation 2.script");
	target = boost::filesystem::path(pscfile);
	boost::filesystem::copy_file(source, target, boost::filesystem::copy_option::overwrite_if_exists);

	if (!AAnimAPICompile(pscfile, import, destination, filepath, newFunctions, uniquekey))
	{
		return false;
	}

	return true;
}

bool AACoreCompile(string filename, string import, string destination, string filepath, vecstr& newFunctions, unsigned int& uniquekey)
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

	if (!GetFunctionLines(filename, storeline))
	{
		return false;
	}

	int AACounter = 0;
	int i = 0;

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
		string templine = "	if(curAAPrefix == \"" + *nextprefix + "\")";
		string rr = "		return 0";
		prefixID[*nextprefix] = i;
		prefixlines.push_back(templine);
		prefixlines.push_back(rr);
		++nextprefix;
		++i;

		for (auto& prefix = nextprefix; prefix != prefixList.end(); ++prefix)
		{
			templine = "	elseif(curAAPrefix == \"" + *prefix + "\")";
			rr = "		return " + to_string(i);
			prefixID[*prefix] = i;
			prefixlines.push_back(templine);
			prefixlines.push_back(rr);
			++i;
		}
	}

	i = 0;
	DebugLogging("AA prefix script complete");

	if (groupNameList.size() > 0)		// Assign base value
	{
		vecstr groupID;
		groupIDFunction.push_back("int Function GetGroupID(string groupName) global");

		for (auto& groupName : groupNameList)
		{
			for (auto& prefix : prefixID)
			{
				for (unsigned int j = 0; j < groupAAPrefix[groupName].size(); ++j)
				{
					if (groupAAPrefix[groupName][j] == prefix.first)
					{
						string templine = "	AASet[num] = ";
						templine.replace(templine.find("num"), 3, to_string(AACounter));
						string number = to_string(prefix.second / 10) + to_string(prefix.second % 10) + to_string(i / 10) + to_string(i % 10);
						string AAgroupID = to_string(i / 10) + to_string(i % 10);

						if (groupIDCounter[groupName] == 0)
						{
							groupIDCounter[groupName] = 1;
						}

						string counter = to_string(groupIDCounter[groupName]);
						string base = counter.substr(0, 3);
						ModIDByGroup mod;
						mod.groupBase = base;
						mod.modID = to_string(prefix.second / 10) + to_string(prefix.second % 10);
						GetModByGroupValue[AAgroupID].push_back(mod);

						if (AAGroupCount[groupAAPrefix[groupName][j]][groupName] == 0)
						{
							ErrorMessage(3013, groupAAPrefix[groupName][j], groupName);
							return false;
						}

						groupIDCounter[groupName] += AAGroupCount[groupAAPrefix[groupName][j]][groupName];

						while (counter.length() < 3)
						{
							counter = "0" + counter;
						}

						baseOrder["AAgroupID == " + AAgroupID] = "		return " + to_string(++AAgroupCount[groupName]);
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
			newFunctions.push_back("int Function " + groupName + "() global");
			groupID.push_back(groupName);
			groupID.push_back(to_string(i));
			newFunctions.push_back("	return " + to_string(i));
			newFunctions.push_back("endFunction");
			newFunctions.push_back("");
			++i;
		}

		if (groupID.size() > 0)
		{
			groupIDFunction.push_back("	if groupName == \"" + groupID[0] + "\"");
			groupIDFunction.push_back("		return " + groupID[1]);

			for (unsigned int k = 2; k < groupID.size(); ++k)
			{
				if (k % 2 == 0)
				{
					groupIDFunction.push_back("	elseif groupName == \"" + groupID[k] + "\"");
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
			for (auto& AALine : groupAAlines)
			{
				newline.push_back(AALine);
			}

			skip = true;
		}
		else if (line.find("(curAAPrefix == ") != NOT_FOUND)
		{
			if (prefixlines.size() > 0)
			{
				for (auto& prefix : prefixlines)
				{
					newline.push_back(prefix);
				}

				newline.push_back("	endif");
			}

			skip = true;
		}
		else if (line.find("(DataCode == num)") != NOT_FOUND)
		{
			if (baseMatch.size() > 0)
			{
				newline.push_back("	if(" + baseMatch[0] + ")");
				newline.push_back(baseMatch[1]);				

				for (unsigned int j = 2; j < baseMatch.size(); ++j)
				{
					if (j % 2 == 0)
					{
						newline.push_back("	elseif(" + baseMatch[j] + ")");
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
				newline.push_back("	if(" + firstOrder->first + ")");
				newline.push_back(firstOrder->second);
				++firstOrder;

				for (auto& order = firstOrder; order != baseOrder.end(); ++order)
				{
					newline.push_back("	elseif(" + order->first + ")");
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
				newline.push_back(space + "if(AAgroupID == " + firstGroup->first + ")");
				space += "	";

				if (firstGroup->second.size() > 1)
				{
					for (unsigned int j = 0; j < firstGroup->second.size(); ++j)
					{
						if (j + 1 == firstGroup->second.size() - 1)
						{
							newline.push_back(space + "if(groupValue < " + firstGroup->second[j].groupBase + ")");
							newline.push_back(space + "	return " + firstGroup->second[j].modID);
							newline.push_back(space + "else");
							newline.push_back(space + "	return " + firstGroup->second[j + 1].modID);
							break;
						}
						else
						{
							newline.push_back(space + "if(groupValue < " + firstGroup->second[j + 1].groupBase + ")");
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
					newline.push_back(space + "elseif(AAgroupID == " + group->first + ")");
					space += "	";

					if (group->second.size() > 1)
					{
						for (unsigned int j = 0; j < group->second.size(); ++j)
						{
							if (j + 1 == group->second.size() - 1)
							{
								newline.push_back(space + "if(groupValue < " + group->second[j + 1].groupBase + ")");
								newline.push_back(space + "	return " + group->second[j].modID);
								newline.push_back(space + "else");
								newline.push_back(space + "	return " + group->second[j + 1].modID);
								break;
							}
							else
							{
								newline.push_back(space + "if(groupValue < " + group->second[j + 1].groupBase + ")");
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

	ofstream output(filename);

	if (output.is_open())
	{
		for (auto& line : newline)
		{
			output << line << "\n";
		}

		output.close();
	}
	else
	{
		ErrorMessage(3002, filename);
		return false;
	}

	if (isFileExist(import))
	{
		if (!PapyrusCompile(filename, import, destination, filepath))
		{
			return false;
		}
	}
	else
	{
		ErrorMessage(2010, import);
		return false;
	}

	DebugLogging("AA core script complete");
	return true;
}

bool AAnimAPICompile(string filename, string import, string destination, string filepath, vecstr& newFunctions, unsigned int& uniquekey)
{
	vecstr storeline;
	vecstr newline;
	newline.reserve(storeline.size());

	if (!GetFunctionLines(filename, storeline))
	{
		return false;
	}

	for (unsigned int k = 0; k < storeline.size(); ++k)
	{
		string line = storeline[k];

		if (line.find("$InstallationKey$") != NOT_FOUND)
		{
			line.replace(line.find("$InstallationKey$"), 17, to_string(uniquekey));
		}

		newline.push_back(line);
	}

	if (newline.size() > 0 && newline.back().length() > 0)
	{
		newline.push_back("");
	}

	newline.insert(newline.end(), newFunctions.begin(), newFunctions.end());
	ofstream output(filename);

	if (output.is_open())
	{
		for (auto& line : newline)
		{
			output << line << "\n";
		}

		output.close();
	}
	else
	{
		ErrorMessage(3002, filename);
		return false;
	}

	if (isFileExist(import))
	{
		if (!PapyrusCompile(filename, import, destination, filepath))
		{
			return false;
		}
	}
	else
	{
		ErrorMessage(2010, import);
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
	HANDLE file;
	FILETIME lastmodified;
	SYSTEMTIME sysUTC;
	file = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE)
	{
		ErrorMessage(2020);
		return "";
	}

	if (!GetFileTime(file, NULL, NULL, &lastmodified))
	{
		ErrorMessage(2022);
		return "";
	}

	FileTimeToSystemTime(&lastmodified, &sysUTC);
	return to_string(sysUTC.wDay) + "/" + to_string(sysUTC.wMonth) + "/" + to_string(sysUTC.wYear) + " " + to_string(sysUTC.wHour) + ":" + to_string(sysUTC.wMinute);
}

bool FolderCreate(string curBehaviorPath)
{
	try
	{
		boost::filesystem::create_directories(curBehaviorPath);
	}
	catch (const std::exception& ex)
	{
		ErrorMessage(6002, ex.what());
		return false;
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

bool PapyrusCompile(string pscfile, string import, string destination, string filepath)
{
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
		if (remove(filepath.c_str()) != 0)
		{
			timeline = GetLastModified(filepath);
		}
	}
		
	if (!isFileExist(target.string()) || !PapyrusCompileProcess(pscfile, import, destination, filepath, target))
	{
		string compiler = "Papyrus Compiler\\PapyrusCompiler.exe";

		if (isFileExist(compiler))
		{
			if (!PapyrusCompileProcess(pscfile, import, destination, filepath, compiler))
			{
				return false;
			}
		}
		else
		{
			ErrorMessage(6007);
			return false;
		}
	}

	if (timeline.length() > 0)
	{
		if (timeline == GetLastModified(filepath))
		{
			ErrorMessage(1185, filepath);
			return false;
		}
	}

	return true;
}

bool PapyrusCompileProcess(string pscfile, string scriptsource, string destination, string filepath, boost::filesystem::path compiler)
{
	vecstr args{ GetFileName(pscfile) + ".psc", "-f=TESV_Papyrus_Flags.flg" , "-i=" + scriptsource + ";Papyrus Compiler\\backup scripts" , "-o=" + destination};
	future<vector<char>> p_reader, p_error;
	
	if (isFileExist(filepath) && !boost::filesystem::remove(filepath))
	{
		ErrorMessage(1082, filepath);
		return false;
	}

	try
	{
		try
		{
			if (boost::process::system(compiler, args, boost::process::std_out > p_reader, boost::process::std_err > p_error, boost::process::windows::hide) != 0)
			{
				// Compilation fail
			}
		}
		catch (const std::exception& ex)
		{
			interMsg(ex.what());
			return false;
		}
	}
	catch (...)
	{
		interMsg("Non conventional exception captured");
		DebugLogging("Non conventional exception captured");
		return false;
	}

	if (!isFileExist(filepath))
	{
		string line;
		vecstr linelist;

		{
			auto raw = p_reader.get();
			vector<string> data;
			string templine;
			boost::iostreams::stream_buffer<boost::iostreams::array_source> sb(raw.data(), raw.size());
			istream is(&sb);

			while (getline(is, templine) && !templine.empty())
			{
				linelist.push_back(templine + "\n");
			}

			for (unsigned int i = 0; i < linelist.size() - 1; ++i)
			{
				line.append(linelist[i]);
			}
		}

		auto raw = p_error.get();
		vector<string> data;
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

		ErrorMessage(1185, filepath);
		interMsg("Output: \n" + line);
		DebugLogging("\nOutput: \n" + line, false);
		return false;
	}

	return true;
}
