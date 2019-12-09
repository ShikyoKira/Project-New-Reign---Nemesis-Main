#include <unordered_set>

#include <boost\regex.hpp>
#include <boost\algorithm\string.hpp>
#include <QtCore\QStandardPaths.h>

#include "playerexclusive.h"
#include "alternateanimation.h"
#include "functions\writetextfile.h"
#include "Nemesis Main GUI\src\utilities\filerelease.h"
#include "Nemesis Main GUI\src\utilities\wstrconvert.h"

using namespace std;
namespace bf = boost::filesystem;

#define min(a,b)            (((a) < (b)) ? (a) : (b))

vector<PCEA> pcealist;
unordered_map<string, vector<PCEAData>> animReplaced;

bool Delete(bf::path file)
{
	if (boost::filesystem::is_directory(file))
	{
		string tempbehavior = file.string();
		vecstr filelist;
		read_directory(tempbehavior, filelist);
		tempbehavior.append("\\");

		for (auto& curfile : filelist)
		{
			Delete(tempbehavior + curfile);
		}
	}

	if (ReleaseLockedFile(file.string()) && !boost::filesystem::remove(file))
	{
		ErrorMessage(1082, "PCEA_animations", file.string());
	}

	return true;
}

void forcedCopy(bf::path animFile, bf::path newAnimFile, int count = 0)
{
	try
	{
		bf::copy_file(animFile, newAnimFile, bf::copy_option::overwrite_if_exists);
	}
	catch (const exception& ex)
	{
		if (count > 200) throw ex;

		forcedCopy(animFile, newAnimFile, count + 1);
	}
}

void PCEASubFolder(string path, unsigned short number, string pceafolder, string subpath, PCEA& mod)
{
	vecstr animlist;
	read_directory(path, animlist);

	for (auto& anim : animlist)
	{
		bf::path animFile(path + "\\" + anim);

		if (!bf::is_directory(animFile))
		{
			if (!boost::iequals(animFile.extension().string(), ".hkx")) continue;

			int numb = 0;
			string newFileName = "fp2" + to_string(numb++) + "_" + anim;

			while (isFileExist(pceafolder + "\\" + newFileName)) newFileName = "fp2" + to_string(numb++) + "_" + anim;

			bf::path newAnimFile(pceafolder + "\\" + newFileName);
			string lowerAnim = boost::to_lower_copy(anim);
			mod.animPathList[lowerAnim] = "Animations\\Nemesis_PCEA\\" + subpath + "\\" + newFileName;
			
			if (!isFileExist(pceafolder)) bf::create_directories(pceafolder);

			forcedCopy(animFile, newAnimFile);

			if (!bf::exists(animFile)) ErrorMessage(1185, newAnimFile.string());
		}
		else
		{
			PCEASubFolder(animFile.string(), number, pceafolder + "\\" + anim, subpath + "\\" + anim, mod);
		}
	}
}

void ReadPCEA()
{
	DebugLogging("Reading PCEA files...");
	pcealist = vector<PCEA>();
	animReplaced = unordered_map<string, vector<PCEAData>>();

	vecstr folderlist;
	string datapath = nemesisInfo->GetDataPath() + "meshes\\actors\\character\\Animations\\Nemesis_PCEA";
	bf::path pceafolder(datapath + "\\PCEA_animations");

	if (isFileExist(pceafolder.string())) Delete(pceafolder);

	if (!FolderCreate(datapath + "\\PCEA_animations\\")) return;

	read_directory(datapath, folderlist);
	map<unsigned short, PCEA> modlist;
	map<unsigned short, string> pceaFolderMapList;

	for (auto& folder : folderlist)
	{
		string path = datapath + "\\" + folder;

		if (isdigit(folder[0]) && bf::is_directory(path))
		{
			unsigned short number = static_cast<unsigned short>(stoi(boost::regex_replace(string(folder), boost::regex("([0-9]+)[^\n]+"), string("\\1"))));
			pceaFolderMapList[number] = path + "|" + folder;
		}

		if (error) throw nemesis::exception();
	}

	for (auto& pceaFolderMap : pceaFolderMapList)
	{
		string folder = pceaFolderMap.second;
		unsigned short number = pceaFolderMap.first;

		if (modlist.find(number) == modlist.end())
		{
			PCEA mod;
			mod.modFile = folder.substr(folder.find("|") + 1);
			PCEASubFolder(folder.substr(0, folder.find("|")), number, pceafolder.string(), "PCEA_animations", mod);

			if (mod.animPathList.size() > 0)
			{
				modlist[number] = mod;
			}
		}
	}

	// limitation lifts
	// if (modlist.size() > 10) ErrorMessage(7000);

	if (modlist.size() > 0)
	{
		interMsg(TextBoxMessage(1006) + ": Nemesis PCEA\n");
		DebugLogging(EngTextBoxMessage(1006) + ": Nemesis PCEA");
	}

	for (auto& mod : modlist)
	{
		pcealist.push_back(mod.second);
		string msg = "PCEA Mod: " + mod.second.modFile;
		interMsg(msg);
		DebugLogging(msg);
		
		for (auto& anim : pcealist.back().animPathList)
		{
			PCEAData newData;
			newData.modFile = pcealist.back().modFile;
			newData.animPath = anim.second;
			animReplaced[anim.first].push_back(newData);
		}

		if (error) throw nemesis::exception();
	}
}

bool PCEAInstallation()
{
#ifdef DEBUG
	string import = SSE ? "data\\source\\scripts" : "data\\scripts\\source";
#else
	string import = nemesisInfo->GetDataPath() + string(SSE ? "source\\scripts" : "scripts\\source");
#endif

	string filename = nemesisInfo->GetDataPath() + "Nemesis PCEA.esp";
	DebugLogging(filename);

	{
		FILE* f;
		fopen_s(&f, filename.c_str(), "r+b");

		if (f)
		{
			int c;
			string line;

			while ((c = fgetc(f)) != EOF)
			{
				line.push_back(c);
			}

			size_t startnum = line.find("Which animation pack do you wish to activate?");

			if (startnum == NOT_FOUND)
			{
				ErrorMessage(6009, "PCEA.esp", "PCEA mod");
			}
			else
			{
				startnum += 49;
			}

			for (unsigned int j = 0; j < pcealist.size(); ++j)
			{
				string number = to_string(j + 1);
				unsigned int size = min(pcealist[j].modFile.length(), 113 - number.length());
				unsigned int counter = startnum + (117 * j);

				for (unsigned int i = 0; i < number.length(); ++i)
				{
					fseek(f, counter, SEEK_SET);
					unsigned char charcode = static_cast<unsigned char>(number[i]);
					fwrite(&charcode, sizeof(charcode), 1, f);
					++counter;
				}

				fseek(f, counter, SEEK_SET);
				unsigned char charcode = 46;
				fwrite(&charcode, sizeof(charcode), 1, f);
				++counter;
				fseek(f, counter, SEEK_SET);
				charcode = 32;
				fwrite(&charcode, sizeof(charcode), 1, f);
				++counter;

				for (unsigned int i = 0; i < size; ++i)
				{
					fseek(f, counter + i, SEEK_SET);
					charcode = static_cast<unsigned char>(pcealist[j].modFile[i]);
					fwrite(&charcode, sizeof(charcode), 1, f);
				}

				if (error) throw nemesis::exception();
			}

			for (unsigned int j = pcealist.size(); j < 10; ++j)
			{
				for (unsigned int i = 0; i < 115; ++i)
				{
					fseek(f, startnum + (117 * j) + i, SEEK_SET);
					unsigned char charcode = 32;
					fwrite(&charcode, sizeof(charcode), 1, f);
				}

				if (error) throw nemesis::exception();
			}

			fclose(f);
		}
		else
		{
			ErrorMessage(3002, filename);
		}
	}

	DebugLogging("PCEA esp modification complete");

	if (error) throw nemesis::exception();

	DebugLogging("PCEA begin script input");
	wstring cachedir = boost::filesystem::path(QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0).toStdWString()).parent_path().wstring() +
		L"/Nemesis";
	replace(cachedir.begin(), cachedir.end(), '/', '\\');

	try
	{
		boost::filesystem::create_directories(cachedir);
	}
	catch (const exception& ex)
	{
		ErrorMessage(6002, wstrConv.to_bytes(cachedir), ex.what());
	}

	bf::path source("alternate animation\\nemesis pcea.script");
	bf::path pscfile(cachedir + L"\\Nemesis_PCEA_Core.psc");
	bf::copy_file(source, pscfile, bf::copy_option::overwrite_if_exists);
	DebugLogging(source.string());
	DebugLogging(pscfile.string());

	vecstr storeline;
	vecstr newline;

	if (!GetFunctionLines(pscfile.string(), storeline))
	{
		return false;
	}

	for (auto& line : storeline)
	{
		bool skip = false;

		if (line.find("\tPCEA[num] =") != NOT_FOUND)
		{
			for (unsigned int j = 0; j < pcealist.size(); ++j)
			{
				string templine = line;
				templine.replace(templine.find("num"), 3, to_string(j));
				templine.append("\"" + pcealist[j].modFile + "\"");
				newline.push_back(templine);
			}

			skip = true;
		}
		else if (line.find("\tactivation[num]") != NOT_FOUND)
		{
			for (unsigned int j = 0; j < pcealist.size(); ++j)
			{
				string templine = line;
				templine.replace(templine.find("num"), 3, to_string(j));
				templine.append("0");
				newline.push_back(templine);
			}

			skip = true;
		}
		else if (line.find("return crc32ID") != NOT_FOUND)
		{
			int uniquekey = 0;

			for (auto& curline : newline)
			{
				uniquekey = uniquekey + CRC32Convert(curline);
				uniquekey = uniquekey % 987123;
			}

			line.replace(line.find("crc32ID"), 7, to_string(uniquekey));
		}
		else if (line.find("number") != NOT_FOUND)
		{
			size_t pos = line.find("number");
			line.replace(pos, 6, to_string(pcealist.size()));
		}

		if (!skip)
		{
			newline.push_back(line);
		}

		if (error) throw nemesis::exception();
	}

	DebugLogging("PCEA script input complete");

	{
		FileWriter output(pscfile.string());

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

		if (!FolderCreate(import))
		{
			ErrorMessage(2010, import);
		}
	}

	if (!isFileExist(import))
	{
		ErrorMessage(2010, import);
	}

	if (error) throw nemesis::exception();

	string destination = nemesisInfo->GetDataPath() + "scripts";
	string filepath = destination + "\\Nemesis_PCEA_Core.pex";

	if (!PapyrusCompile(pscfile, StringToWString(import), destination, filepath, cachedir))
	{
		return false;
	}

	DebugLogging("PCEA core script complete");
	return true;
}
