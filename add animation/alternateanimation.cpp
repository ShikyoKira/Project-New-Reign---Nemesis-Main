#include "alternateanimation.h"
#include <boost\algorithm\string.hpp>
#include <boost\process.hpp>
#include <boost\process\windows.hpp>
#include <boost\crc.hpp>

#pragma warning(disable:4503)

using namespace std;

void fixedKeyInitialize();
unsigned int getUniqueKey(unsigned char bytearray[], int byte1, int byte2);
bool PapyrusCompileProcess(string pscfile, string import, string destination, string filepath, boost::filesystem::path compiler);

void AAInitialize(string AAList)
{
	vecstr groupName;
	char charline[2000];
	unordered_map<string, string> existAAAnim;			// animation name, animation group name; has the animation been registered for AA?
	read_directory(AAList, groupName);

	for (unsigned int i = 0; i < groupName.size(); ++i)
	{
		if (!boost::iequals(groupName[i], "alternate animation.script") && boost::filesystem::path(AAList + "\\" + groupName[i]).extension().string() == ".txt")
		{
			FILE* input;
			fopen_s(&input, (AAList + "\\" + groupName[i]).c_str(), "r");

			if (input)
			{
				string AAGroupName = groupName[i].substr(0, groupName[i].find_last_of("."));

				while (fgets(charline, 2000, input))
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
							fclose(input);
							return;
						}
					}
				}

				fclose(input);
			}
			else
			{
				ErrorMessage(4000, AAList);
				return;
			}
		}
	}
}

bool AAInstallation()
{
	if (AAGroup.size() == 0)
	{
		return true;
	}

	string filename = "alternate animation\\alternate animation.script";
	boost::filesystem::path source(filename);

#ifdef DEBUG
	string import = "data\\scripts\\source";
#else
	string import = skyrimDataPath->GetDataPath() + "scripts\\source";
#endif

	FolderCreate(import);
	string destination = skyrimDataPath->GetDataPath() + "scripts";
	string pscfile = import + "\\Nemesis_AA_Core.psc";
	string filepath = destination + "\\Nemesis_AA_Core.pex";

	bool AASetDone = false;
	bool prefixDone = false;
	set<string> prefixList;
	unordered_map<string, int> prefixID;
	unordered_map<string, int> groupCounter;

	fixedKeyInitialize();

	boost::filesystem::path target(pscfile);
	boost::filesystem::copy_file(source, target, boost::filesystem::copy_option::overwrite_if_exists);
	vecstr storeline;
	vecstr newline;
	vecstr newFunctions;
	GetFunctionLines(target.string(), storeline);
	newline.reserve(storeline.size());

	if (error)
	{
		return false;
	}

	int AACounter = 0;

	for (auto& groupName : groupNameList)
	{
		for (unsigned int j = 0; j < groupAAPrefix[groupName].size(); ++j)	// list of group aa prefix categorized by animation group name
		{
			prefixList.insert(groupAAPrefix[groupName][j]);
		}
	}

	for (unsigned int k = 0; k < storeline.size(); ++k)
	{
		string line = storeline[k];

		if (line.find("AASet[" + to_string(AACounter) + "] = ") != NOT_FOUND && !AASetDone)
		{
			int i = 0;

			if (groupNameList.size() > 0)
			{
				for (auto& groupName : groupNameList)
				{
					for (auto& prefix : prefixID)
					{
						for (unsigned int j = 0; j < groupAAPrefix[groupName].size(); ++j)
						{
							if (groupAAPrefix[groupName][j] == prefix.first)
							{
								while (k < storeline.size())
								{
									line = storeline[k];
									size_t pos = line.find("AASet[" + to_string(AACounter) + "] = ");

									if (pos != NOT_FOUND)
									{
										string number = to_string(prefix.second / 10) + to_string(prefix.second % 10) + to_string(i / 10) + to_string(i % 10);

										if (groupCounter[groupName] == 0)
										{
											groupCounter[groupName] = 1;
										}

										string counter = to_string(groupCounter[groupName]);

										if (AAGroupCount[groupAAPrefix[groupName][j]][groupName] == 0)
										{
											ErrorMessage(3013, groupAAPrefix[groupName][j], groupName);
											return false;
										}

										groupCounter[groupName] += AAGroupCount[groupAAPrefix[groupName][j]][groupName];

										while (counter.length() < 3)
										{
											counter = "0" + counter;
										}

										number = number + counter.substr(0, 3);
										pos = line.find("=", pos) + 2;
										line = line.substr(0, pos);
										line = line + number;
										newline.push_back(line);
										++AACounter;
										++k;
										break;
									}
									else
									{
										newline.push_back(line);
										++k;
									}
								}
							}
						}
					}

					newFunctions.push_back("int Function " + groupName + "() global");
					newFunctions.push_back("	return " + to_string(i));
					newFunctions.push_back("endFunction");
					newFunctions.push_back("");
					++i;
				}

				newline.pop_back();
				--k;
			}

			AASetDone = true;
		}
		else if (line.find("(curAAPrefix == ") != NOT_FOUND && !prefixDone)
		{
			size_t pos;
			int i = 0;

			if (prefixList.size() > 0)
			{
				for (auto& prefix : prefixList)
				{
					while (k < storeline.size())
					{
						line = storeline[k];
						pos = line.find("(curAAPrefix == ");

						if (pos != NOT_FOUND)
						{
							line = line.substr(0, pos + 16);
							line = line + "\"" + prefix + "\")";
							prefixID[prefix] = i;
							newline.push_back(line);
							++k;
							++i;
							break;
						}
						else
						{
							newline.push_back(line);
							++k;
						}
					}
				}

				newline.pop_back();
				--k;
			}

			prefixDone = true;
		}
		else if (line.find("$SetNumber$") != NOT_FOUND)
		{
			line.replace(line.find("$SetNumber$"), 12, to_string(AACounter));
		}
		else if (line.find("$InstallationKey$") != NOT_FOUND)
		{
			unsigned int uniqueKey = 0;

			for (auto& curline : newline)
			{
				uniqueKey = uniqueKey + CRC32Convert(curline);
				uniqueKey = uniqueKey % 500000;
			}

			line.replace(line.find("$InstallationKey$"), 17, to_string(uniqueKey));
		}

		newline.push_back(line);
	}

	newline.insert(newline.end(), newFunctions.begin(), newFunctions.end());

	namespace bf = boost::filesystem;

	ofstream output(pscfile);

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
		ErrorMessage(3002, pscfile);
		return false;
	}

	if (isFileExist(import))
	{
		if (!PapyrusCompile(pscfile, import, destination, filepath))
		{
			return false;
		}
	}

	filename = "alternate animation\\alternate animation 2.script";
	source = boost::filesystem::path("alternate animation\\alternate animation 2.script");
	target = boost::filesystem::path(destination + "\\FNIS_aa.pex");
	boost::filesystem::copy_file(source, target, boost::filesystem::copy_option::overwrite_if_exists);

	if (!isFileExist(target.string()))
	{
		ErrorMessage(1185, target.string());
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

void FolderCreate(string curBehaviorPath)
{
	size_t pos = curBehaviorPath.find("\\") + 1;
	string curFolder = curBehaviorPath.substr(0, pos);
	__int64 counter = sameWordCount(curBehaviorPath, "\\");

	for (int i = 0; i < counter; ++i)
	{
		if (CreateFolder(curFolder))
		{
			pos = curBehaviorPath.find("\\", pos) + 1;

			if (pos != 0)
			{
				curFolder = curBehaviorPath.substr(0, pos);
			}
		}
	}
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
	bf::path target = bf::path(skyrimDataPath->GetDataPath()).parent_path().parent_path();
	target = bf::path(target.string() + "\\Papyrus Compiler\\PapyrusCompiler.exe");

	if (isFileExist(filepath))
	{
		if (remove(filepath.c_str()) != 0)
		{
			timeline = GetLastModified(filepath);
		}
	}

	if (isFileExist(target.string()))
	{
		PapyrusCompileProcess(pscfile, import, destination, filepath, target);
	}
	else
	{
		string compiler = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Papyrus Compiler\\PapyrusCompiler.exe";
		string additional = ";C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Data\\scripts\\Source";

		if (isFileExist(compiler))
		{
			PapyrusCompileProcess(pscfile, import + additional, destination, filepath, compiler);
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
	vector<string> args{ pscfile, "-f=TESV_Papyrus_Flags.flg" , "-i=" + scriptsource , "-o=" + destination };

	try
	{
		try
		{
			boost::process::system(compiler, args, boost::process::windows::hide);
		}
		catch (const std::exception& ex)
		{
			interMsg(ex.what());
		}
	}
	catch (...)
	{
		interMsg("Non conventional exception captured");
	}

	if (!isFileExist(filepath))
	{
		ErrorMessage(1185, filepath);
		return false;
	}

	return true;
}

namespace keepsake
{
	void AAInstallation()
	{
		string filename = "alternate animation\\alternate animation.script";
		boost::filesystem::path source(filename);

#ifdef DEBUG
		filename = "data\\";
#else
		filename = skyrimDataPath->GetDataPath();
#endif

		if (CreateFolder(filename))
		{
			filename = filename + "scripts";

			if (CreateFolder(filename))
			{
				filename = filename + "\\Nemesis_AA_Core.pex";
			}
		}

		vecstr prefixList;
		unordered_map<string, bool> prefixDone;
		unordered_map<string, int> groupCounter;
		int prefixCounter = 0;

		fixedKeyInitialize();

		boost::filesystem::path target(filename);
		boost::filesystem::copy_file(source, target, boost::filesystem::copy_option::overwrite_if_exists);
		FILE* f;
		fopen_s(&f, filename.c_str(), "r+b");

		if (f)
		{
			int i = 0;

			for (auto& groupName : groupNameList)							// animation group name; ie. _mtidle
			{
				for (unsigned int j = 0; j < groupAAPrefix[groupName].size(); ++j)	// list of group aa prefix categorized by animation group name
				{
					unsigned char charcode;

					if (!prefixDone[groupAAPrefix[groupName][j]])
					{
						prefixList.push_back(groupAAPrefix[groupName][j]);
						prefixDone[groupAAPrefix[groupName][j]] = true;
					}

					fseek(f, 1059 + (9 * (prefixCounter)), SEEK_SET);
					charcode = 48 + (j / 10);
					fwrite(&charcode, sizeof(charcode), 1, f);

					fseek(f, 1060 + (9 * (prefixCounter)), SEEK_SET);
					charcode = 48 + (j % 10);
					fwrite(&charcode, sizeof(charcode), 1, f);

					fseek(f, 1061 + (9 * (prefixCounter)), SEEK_SET);
					charcode = 48 + (i / 10);
					fwrite(&charcode, sizeof(charcode), 1, f);

					fseek(f, 1062 + (9 * (prefixCounter)), SEEK_SET);
					charcode = 48 + (i % 10);
					fwrite(&charcode, sizeof(charcode), 1, f);

					string counter = to_string(AAGroupCount[groupAAPrefix[groupName][j]][groupName]);

					if (counter == "0")
					{
						ErrorMessage(3013, groupAAPrefix[groupName][j], groupName);
						fclose(f);
						return;
					}

					groupCounter[groupName] = groupCounter[groupName] + stoi(counter);
					counter = to_string(groupCounter[groupName]);

					while (counter.length() < 3)
					{
						counter = "0" + counter;
					}

					for (unsigned int k = 0; k < 3; ++k)
					{
						fseek(f, 1063 + k + (9 * (prefixCounter)), SEEK_SET);
						charcode = unsigned char(counter[k]);
						fwrite(&charcode, sizeof(charcode), 1, f);
					}

					++prefixCounter;
				}

				++i;
			}

			for (unsigned int i = 0; i < prefixList.size(); ++i)
			{
				int num = 0;

				while (num < 3)
				{
					fseek(f, 2431 + num + (5 * i), SEEK_SET);
					unsigned char charcode = unsigned char(prefixList[i][num]);
					fwrite(&charcode, sizeof(charcode), 1, f);
					++num;
				}
			}

			unsigned char filebyte[15000];
			fseek(f, 0, SEEK_END);
			int fileLength = ftell(f);

			if (fileLength == 0)
			{
				ErrorMessage(4009);
				fclose(f);
				return;
			}

			rewind(f);
			fread(filebyte, fileLength, 1, f);

			int installationKey = ((getUniqueKey(filebyte, 1059, 1059 + prefixCounter * 9 - 3) % 500000) + (getUniqueKey(filebyte, 2431, 2431 + prefixList.size() * 5 - 3) % 500000));

			for (unsigned int i = 0; i < 6; ++i)
			{
				fseek(f, 2220 + i, SEEK_SET);
				unsigned char charcode = unsigned char(48 + (installationKey % 10));
				fwrite(&charcode, sizeof(charcode), 1, f);
				installationKey /= 10;
			}

			fclose(f);
		}
		else
		{
			ErrorMessage(3002, filename);
			return;
		}

		namespace bf = boost::filesystem;

		string scriptfile = skyrimDataPath->GetDataPath() + "scripts\\source";
		string destination = skyrimDataPath->GetDataPath() + "scripts";
		string filepath = destination + "\\Nemesis_AA_Core.pex";
		string timeline;

		if (isFileExist(filepath))
		{
			if (remove(filepath.c_str()) != 0)
			{
				timeline = GetLastModified(filepath);
			}
		}

		if (isFileExist(scriptfile))
		{
			target = bf::path(skyrimDataPath->GetDataPath()).parent_path().parent_path();
			target = bf::path(target.string() + "\\Papyrus Compiler");

			if (isFileExist(target.string()))
			{
				string input = "cd " + target.string() + " & PapyrusCompiler \"" + scriptfile + "\\Nemesis_AA_Core.psc\" -f=\"TESV_Papyrus_Flags.flg\" -i=\"" + scriptfile + "\" -o=\"" + destination + "\"";
				boost::process::system(input, boost::process::windows::hide);

				if (!isFileExist(filepath))
				{
					ErrorMessage(1185, filepath);
					return;
				}
			}
			else
			{
				string compilerDir = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Papyrus Compiler";
				string additional = ";C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Data\\scripts\\Source";

				if (isFileExist(compilerDir))
				{
					string input = "cd " + compilerDir + " & PapyrusCompiler \"" + scriptfile + "\\Nemesis_AA_Core.psc\" -f=\"TESV_Papyrus_Flags.flg\" -i=\"" + scriptfile + additional + "\" -o=\"" + destination + "\"";
					boost::process::system(input, boost::process::windows::hide);

					if (!isFileExist(filepath))
					{
						ErrorMessage(1185, filepath);
						return;
					}
				}
			}

			if (timeline.length() > 0)
			{
				if (timeline == GetLastModified(filepath))
				{
					ErrorMessage(1185, filepath);
					return;
				}
			}
		}
	}
}
