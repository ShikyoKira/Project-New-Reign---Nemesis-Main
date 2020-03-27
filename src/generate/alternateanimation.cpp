#include "Global.h"

#include <filesystem>

#include <boost/crc.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream_buffer.hpp>

#include <QtCore/QProcess>
#include <QtCore/QStandardPaths.h>

#include "debuglog.h"

#include "generate/generator_utility.h"
#include "generate/alternateanimation.h"

#include "utilities/lastupdate.h"
#include "utilities/readtextfile.h"
#include "utilities/writetextfile.h"

#pragma warning(disable:4503)

using namespace std;
namespace sf = filesystem;

unordered_map<string, int> AAgroup_Counter;

bool AACoreCompile(sf::path filename, wstring import, string destination, string filepath, sf::path appdata_path, vecstr& newFunctions,
	unsigned int& maxGroup, unsigned int& uniquekey);
bool AAnimAPICompile(sf::path filename, wstring import, string destination, string filepath, sf::path appdata_path, vecstr& newFunctions,
	unsigned int maxGroup, unsigned int& uniquekey);
void fixedKeyInitialize();
unsigned int getUniqueKey(unsigned char bytearray[], int byte1, int byte2);
bool PapyrusCompileProcess(sf::path pscfile, wstring import, string destination, string filepath, sf::path appdata_path,
	sf::path compiler, bool tryagain = false);
void ByteCopyToData(string target, string destination);
void ByteCopyToData(wstring target, wstring destination);
void forcedRemove(wstring target, int counter = 0);
void forcedRemove(string target, int counter = 0);

struct ModIDByGroup
{
	string groupBase;
	string modID;
};

void forcedRemove(wstring target, int counter)
{
	try
	{
		if (!sf::remove(target)) ErrorMessage(1082, WStringToString(target));
	}
	catch (exception)
	{
		if (counter > 200)
		{
			// ErrorMessage(6002, WStringToString(target), ex.what());
			return;
		}

		forcedRemove(target, counter + 1);
	}
}

void forcedRemove(string target, int counter)
{
	try
	{
		if (!sf::remove(target)) ErrorMessage(1082, target);
	}
	catch (exception)
	{
		if (counter > 200)
		{
			// ErrorMessage(6002, target, ex.what());
			return;
		}

		forcedRemove(target, counter + 1);
	}
}

void AAInitialize(string AAList)
{
	vecstr groupList;
	unordered_map<string, string> existAAAnim;			// animation name, animation group name; has the animation been registered for AA?
	DebugLogging("Caching alternate animation group...");
	read_directory(AAList, groupList);

	for(string& groupName : groupList)
	{
		if (!nemesis::iequals(groupName, "alternate animation.script") && nemesis::iequals(sf::path(AAList + "\\" + groupName).extension().string(), ".txt"))
		{
			FileReader doc(AAList + "\\" + groupName);

			if (doc.GetFile())
			{
				string AAGroupName = groupName.substr(0, groupName.find_last_of("."));
				string animFile;

				while (doc.GetLines(animFile))
				{
					if (animFile.length() != 0)
					{
						string lowerAnimFile = nemesis::to_lower_copy(animFile);

						if (existAAAnim[lowerAnimFile].length() == 0)
						{
							string lowerGroupName = nemesis::to_lower_copy(AAGroupName);
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
	if (AAGroup.size() == 0) return true;

	unsigned int uniquekey;
	wstring cachedir = sf::path(QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0).toStdWString()).parent_path().wstring() +
		L"/Nemesis";
	replace(cachedir.begin(), cachedir.end(), '/', '\\');

	try
	{
		sf::create_directories(cachedir);
	}
	catch (const exception& ex)
	{
		ErrorMessage(6002, WStringToString(cachedir), ex.what());
	}

	if (error) throw nemesis::exception();

	string import = nemesisInfo->GetDataPath() + string(SSE ? "source\\scripts" : "scripts\\source");
	string destination = nemesisInfo->GetDataPath() + "scripts";
	sf::path source("alternate animation\\alternate animation.script");
	sf::path pscfile(cachedir + L"\\Nemesis_AA_Core.psc");
	string filepath = destination + "\\Nemesis_AA_Core.pex";
	sf::copy_file(source, pscfile, sf::copy_options::overwrite_existing);
	DebugLogging(pscfile.string());
	DebugLogging(filepath);

	if (!FolderCreate(import)) ErrorMessage(2010, import);

	if (!isFileExist(import)) ErrorMessage(2010, import);

	unsigned int maxGroup;
	fixedKeyInitialize();
	vecstr newFunctions;
	
	if (!AACoreCompile(pscfile, StringToWString(import), destination, filepath, cachedir, newFunctions, maxGroup, uniquekey)) return false;

	if (error) throw nemesis::exception();

	source = sf::path("alternate animation\\alternate animation 2.script");
	sf::path pscfile2 = sf::path(cachedir + L"\\FNIS_aa.psc");
	filepath = destination + "\\FNIS_aa.pex";
	sf::copy_file(source, pscfile2, sf::copy_options::overwrite_existing);
	DebugLogging(pscfile2.string());
	DebugLogging(filepath);

	if (!AAnimAPICompile(pscfile2, StringToWString(import), destination, filepath, cachedir, newFunctions, maxGroup, uniquekey)) return false;

	try
	{
		if (!sf::remove(pscfile)) ErrorMessage(1082, pscfile.string());
	}
	catch (const exception& ex)
	{
		ErrorMessage(6002, pscfile.string(), ex.what());
	}

	try
	{
		if (!sf::remove(pscfile2)) ErrorMessage(1082, pscfile2.string());
	}
	catch (const exception& ex)
	{
		ErrorMessage(6002, pscfile2.string(), ex.what());
	}

	if (error) throw nemesis::exception();

	return true;
}

bool AACoreCompile(sf::path filename, wstring import, string destination, string filepath, sf::path appdata_path, vecstr& newFunctions,
	unsigned int& maxGroup, unsigned int& uniquekey)
{
	bool prefixDone = false;
	vecstr prefixList;
	unordered_map<string, bool> prefixCheck;
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
	GetFunctionLines(filename.c_str(), storeline);

	int AACounter = 0;
	maxGroup = 0;

	for (auto& groupName : groupNameList)
	{
        for (auto &AAprefixGroup :
             groupAAPrefix[groupName]) // list of group aa prefix categorized by animation group name while eliminating duplicates using set container
        {
            if (!prefixCheck[AAprefixGroup]) {
                prefixList.push_back(AAprefixGroup);
                prefixCheck[AAprefixGroup] = true;
            }
        }
    }

    if (prefixList.size() > 0) // Assign mod ID
    {
        auto nextprefix = prefixList.begin();
        string templine = "	if (curAAPrefix == \"" + *nextprefix + "\")";
        string rr = "		return 0";
        prefixID[*nextprefix] = maxGroup;
        prefixlines.push_back(templine);
        prefixlines.push_back(rr);
        ++nextprefix;
        ++maxGroup;

        for (auto &prefix = nextprefix; prefix != prefixList.end(); ++prefix) {
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

    if (groupNameList.size() > 0) // Assign base value
    {
        vecstr groupID;
        groupIDFunction.push_back("int Function GetGroupID(string groupName) global");

        for (auto &groupName : groupNameList) {
            string adjGN = groupName;

            if (groupName.length() > 4 && groupName.rfind("_1p*") == groupName.length() - 4) adjGN.pop_back();

			for (auto& prefix : groupAAPrefix[groupName])
			{
				int maxG = prefixID[prefix];
				string templine = "	AASet[num] = ";
				templine.replace(templine.find("num"), 3, to_string(AACounter));
				string number = to_string(maxG / 10) + to_string(maxG % 10) + to_string(maxGroup / 100) + to_string(maxGroup % 100 / 10) +
					to_string(maxGroup % 10);
				string AAgroupID = to_string(maxGroup / 10) + to_string(maxGroup % 10);

				if (groupIDCounter[adjGN] == 0) groupIDCounter[adjGN] = 1;

				string counter = to_string(groupIDCounter[adjGN]);
				string base = counter.substr(0, 3);
				ModIDByGroup mod;
				mod.groupBase = base;
				mod.modID = to_string(maxG / 10) + to_string(maxG % 10);
				GetModByGroupValue[AAgroupID].push_back(mod);

				if (AAGroupCount[prefix][groupName] == 0) ErrorMessage(3013, prefix, groupName);

				groupIDCounter[adjGN] += AAGroupCount[prefix][groupName];

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

			// Assign animation group ID
			newFunctions.push_back("int Function " + adjGN + "() global");
			groupID.push_back(adjGN);
			groupID.push_back(to_string(maxGroup));
			newFunctions.push_back("	return " + to_string(maxGroup));
			newFunctions.push_back("endFunction");
			newFunctions.push_back("");
            ++maxGroup;
        }

        if (groupID.size() > 0) {
            groupIDFunction.push_back("	if (groupName == \"" + groupID[0] + "\")");
			groupIDFunction.push_back("		return " + groupID[1]);

			for (unsigned int k = 2; k < groupID.size(); ++k)
			{
				groupIDFunction.push_back(k % 2 == 0 ? "	elseif (groupName == \"" + groupID[k] + "\")" : "		return " + groupID[k]);
			}
			
			groupIDFunction.push_back("	endif");
			groupIDFunction.push_back("	Debug.Trace(\"ERROR: Unknown Nemesis AA group name (Group Name: \" + groupName + \")\")");
            groupIDFunction.push_back("	return -1");
            groupIDFunction.push_back("endFunction");
            groupIDFunction.push_back("");
        }
    }

    DebugLogging("Group base value complete");

    for (unsigned int k = 0; k < storeline.size(); ++k) {
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
					newline.push_back(j % 2 == 0 ? "	elseif (" + baseMatch[j] + ")" : baseMatch[j]);
				}

				newline.push_back("	endif");
			}

			skip = true;
        } else if (line.find("(AAgroupID == num)") != NOT_FOUND) {
            if (baseOrder.size() > 0) {
                int counter = 0;
                auto firstOrder = baseOrder.begin();
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
        } else if (line.find("(AAgroupID == value)") != NOT_FOUND) {
            if (GetModByGroupValue.size() > 0) {
                auto firstGroup = GetModByGroupValue.begin();
                string space = "	";
                newline.push_back(space + "if (AAgroupID == " + firstGroup->first + ")");
                space += "	";

                if (firstGroup->second.size() > 1) {
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
                } else {
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
        } else if (line.find("$SetCount$") != NOT_FOUND) {
            line.replace(line.find("$SetCount$"), 10, to_string(AACounter));
        } else if (line.find("$GroupCount$") != NOT_FOUND) {
            line.replace(line.find("$GroupCount$"), 12, to_string(groupNameList.size()));
        }

        if (!skip)
            newline.push_back(line);
    }

    if (groupIDFunction.size() > 0)
        newline.insert(newline.end(), groupIDFunction.begin(), groupIDFunction.end());

    for (auto &curline : newline) {
        uniquekey = uniquekey + CRC32Convert(curline);
		uniquekey = uniquekey % 987123;
    }

    {
		FileWriter output(filename.string());

		if (output.is_open())
		{
			for (auto& line : newline)
			{
				output << line << "\n";
			}
		}
		else
		{
			ErrorMessage(3002, filename.string());
		}
	}

	if (!PapyrusCompile(filename, import, destination, filepath, appdata_path)) return false;

	DebugLogging("AA core script complete");
	return true;
}

bool AAnimAPICompile(sf::path filename, wstring import, string destination, string filepath, sf::path appdata_path, vecstr& newFunctions,
	unsigned int maxGroup, unsigned int& uniquekey)
{
	vecstr storeline;
	vecstr newline;
	newline.reserve(storeline.size());

	if (!GetFunctionLines(filename.c_str(), storeline)) return false;

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
			else
			{
				pos = line.find("FNBE_aa Hidden");

				if (pos != NOT_FOUND) line.replace(pos + 2, 2, "IS");
			}
		}

		newline.push_back(line);
	}

	if (newline.size() > 0 && newline.back().length() > 0) newline.push_back("");

	newline.insert(newline.end(), newFunctions.begin(), newFunctions.end());

	{
		FileWriter output(filename.string());

		if (output.is_open())
		{
			for (auto& line : newline)
			{
				output << line << "\n";
			}
		}
		else
		{
			ErrorMessage(3002, filename.string());
		}
	}

	return PapyrusCompile(filename, import, destination, filepath, appdata_path);
}

void fixedKeyInitialize()
{
    int counter = 0;

    while (counter <= 256) {
        unsigned int key1 = static_cast<unsigned int>(counter);
        int key2 = 0;

        while (key2 <= 7) {
            if ((key1 & 1) > 0) key1 = key1 >> 1 ^ 3988292384;		// is odd number
			else key1 >>= 1;

			++key2;
        }

        fixedkey[counter] = key1;
        ++counter;
    }
}

bool FolderCreate(string curBehaviorPath)
{
	try
	{
		sf::create_directories(curBehaviorPath);
	}
	catch (const exception& ex)
	{
		ErrorMessage(6002, curBehaviorPath, ex.what());
	}

	return true;
}

bool FolderCreate(wstring curBehaviorPath)
{
	try
	{
		sf::create_directories(curBehaviorPath);
	}
	catch (const exception& ex)
	{
		ErrorMessage(6002, WStringToString(curBehaviorPath), ex.what());
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

    while (counter <= key2) {
        unsigned char curByte = bytearray[counter];
        unsigned int key3 = uniqueKey ^ static_cast<unsigned int>(curByte);
        uniqueKey = uniqueKey >> 8 ^ fixedkey[key3 & 255];
        counter++;
    }

    return uniqueKey;
}

bool PapyrusCompile(sf::path pscfile, wstring import, string destination, string filepath, sf::path appdata_path)
{
	if (!sf::exists(pscfile)) ErrorMessage(1092, pscfile.string());
	if (!sf::exists(destination)) ErrorMessage(1001, destination);

	string timeline;
	sf::path target = sf::path(nemesisInfo->GetDataPath());

	while (!nemesis::iequals(target.stem().string(), "data"))
	{
		target = target.parent_path();
	}

	target = target.parent_path();
	target = sf::path(target.string() + "\\Papyrus Compiler\\PapyrusCompiler.exe");

	if (isFileExist(filepath) && !sf::remove(filepath)) timeline = GetLastModified(filepath);

	sf::path desPsc = import + L"\\" + pscfile.stem().wstring() + L".psc";

	if (sf::exists(desPsc) && !sf::remove(desPsc)) ErrorMessage(1082, pscfile.string() + ".psc", desPsc.string());
		
	if (!sf::exists(target) || !PapyrusCompileProcess(pscfile, import, destination, filepath, appdata_path, target))
	{
		string compiler = "Papyrus Compiler\\PapyrusCompiler.exe";

		if (sf::exists(compiler))
		{
			if (!PapyrusCompileProcess(pscfile, import, destination, filepath, appdata_path, compiler, true)) throw nemesis::exception();
		}
		else
		{
			ErrorMessage(6007);
		}
	}

	if (timeline.length() > 0)
	{
		if (timeline == GetLastModified(filepath)) ErrorMessage(1185, filepath);
	}

	return true;
}

bool PapyrusCompileProcess(sf::path pscfile, wstring import, string destination, string filepath, sf::path appdata_path,
	sf::path compiler, bool tryagain)
{
	namespace sf = sf;
	pscfile = pscfile.stem().wstring() + L".psc";
	wstring importedSource = import + L"\\" + pscfile.filename().wstring();
	wstring dep = L"Papyrus Compiler\\scripts";
	wstring backUpDep = L"Papyrus Compiler\\backup scripts";

	if ((sf::exists(dep) || FolderCreate(dep)) && sf::exists(backUpDep))
	{
		vector<wstring> backUpDepList;
		read_directory(backUpDep, backUpDepList);

		for (wstring bkUp : backUpDepList)
		{
			sf::copy_file(backUpDep + L"\\" + bkUp, dep + L"\\" + bkUp, sf::copy_options::overwrite_existing);
		}
	}

	if (isFileExist(filepath) && !sf::is_directory(filepath) && !sf::remove(filepath)) ErrorMessage(1082, filepath);

	if (sf::exists(importedSource) && !sf::is_directory(importedSource) && !sf::remove(importedSource))
	{
		ErrorMessage(1082, WStringToString(importedSource));
	}

	QProcess process;
	QString exe = QString::fromStdWString(compiler.wstring());
	QStringList args{ QString::fromStdWString(pscfile.wstring()), "-f=TESV_Papyrus_Flags.flg", "-i=" + QString::fromStdWString(appdata_path.wstring()) + ";" +
		QString::fromStdWString(dep), "-o=" + QString::fromStdWString(appdata_path.wstring()) };

	string cmd = exe.toStdString();

	for (auto& arg : args)
	{
		cmd.append(" " + arg.toStdString());
	}

	DebugLogging(cmd);
	process.setReadChannel(QProcess::StandardOutput);
	process.setProcessChannelMode(QProcess::MergedChannels);
	process.setCurrentReadChannel(QProcess::StandardOutput);
	process.start(exe, args);
	process.waitForFinished();
	
	string tempfile = GetFileName(filepath) + ".pex";
	wstring tempfilepath = appdata_path.wstring() + L"\\" + StringToWString(tempfile);

	if (!sf::exists(tempfilepath))
	{
		while (!process.waitForReadyRead());

        string line = process.readAllStandardOutput().toStdString();

        if (line.find("Compilation succeeded") != NOT_FOUND && line.find("Assembly succeeded") != NOT_FOUND && line.find("0 error") != NOT_FOUND) return true;

		if (!tryagain) return false;

		try
		{
			interMsg("Output: \n" + line);
			DebugLogging("\nOutput: \n" + line, false);
			ErrorMessage(1185, filepath);
			wstring temp = pscfile.wstring();

			for (QString arg : args)
			{
				temp += L" " + arg.toStdWString();
			}

			interMsg("Command: " + WStringToString(temp));
		}
		catch (nemesis::exception)
		{
			return false;
		}
	}

	ByteCopyToData(tempfilepath, StringToWString(destination + "\\" + tempfile));
	return true;
}

void ByteCopyToData(string target, string destination)
{
	ifstream file(target, ios::binary);
	ofstream paste(destination, ios::binary);
	copy(istreambuf_iterator<char>(file), istreambuf_iterator<char>(), ostreambuf_iterator<char>(paste));

	while (file.is_open())
	{
		file.close();
	}

	forcedRemove(target);
}

void ByteCopyToData(wstring target, wstring destination)
{
	wifstream file(target, ios::binary);
	wofstream paste(destination, ios::binary);
	copy(istreambuf_iterator<wchar_t>(file), istreambuf_iterator<wchar_t>(), ostreambuf_iterator<wchar_t>(paste));

	while (file.is_open())
	{
		file.close();
	}

	forcedRemove(target);
}
