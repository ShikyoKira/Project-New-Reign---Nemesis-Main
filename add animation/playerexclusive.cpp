#include "playerexclusive.h"
#include "alternateanimation.h"
#include <boost\regex.hpp>
#include <boost\algorithm\string.hpp>

using namespace std;
namespace bf = boost::filesystem;

#define min(a,b)            (((a) < (b)) ? (a) : (b))

vector<PCEA> pcealist;
unordered_map<string, vector<PCEAData>> animReplaced;

void ReadPCEA()
{
	vecstr folderlist;
	string datapath = skyrimDataPath->GetDataPath() + "meshes\\actors\\character\\Animations\\Nemesis_PCEA";
	bf::path pceafolder(datapath + "\\PCEA_animations");

	if (isFileExist(datapath + "\\PCEA_animations"))
	{
		if (!bf::remove_all(pceafolder))
		{
			ErrorMessage(1082, "PCEA_animations", pceafolder);
			return;
		}
	}

	if (!FolderCreate(datapath + "\\PCEA_animations\\"))
	{
		return;
	}

	read_directory(datapath, folderlist);
	map<int, PCEA> modlist;
	map<int, bool> taken;

	for (auto& folder : folderlist)
	{
		string path = datapath + "\\" + folder;

		if (isdigit(folder[0]) && bf::is_directory(path))
		{
			int number = stoi(boost::regex_replace(string(folder), boost::regex("([0-9])+[^\n]+"), string("\\1")));

			if (number < 10 && !taken[number])
			{
				PCEA mod;
				mod.modFile = folder;
				vecstr animlist;
				read_directory(path, animlist);

				for (auto& anim : animlist)
				{
					string newFileName = to_string(number) + "_" + anim;
					bf::path animFile(path + "\\" + anim);
					bf::path newAnimFile(pceafolder.string() + "\\" + newFileName);

					if (!bf::is_directory(animFile) && boost::iequals(animFile.extension().string(), ".hkx"))
					{
						string lowerAnim = boost::to_lower_copy(anim);
						mod.animPathList[lowerAnim] = "Animations\\Nemesis_PCEA\\PCEA_animations\\" + newFileName;
						bf::copy_file(animFile, newAnimFile, bf::copy_option::overwrite_if_exists);

						if (!bf::exists(animFile))
						{
							ErrorMessage(1185, newAnimFile.string());
							return;
						}
					}
				}

				if (mod.animPathList.size() > 0)
				{
					modlist[number] = mod;
					taken[number] = true;
				}
			}
		}
	}

	if (modlist.size() > 10)
	{
		ErrorMessage(7000);
		return;
	}

	for (auto& mod : modlist)
	{
		pcealist.push_back(mod.second);
		PatchDebug("PCEA Mod: " + mod.second.modFile);
		
		for (auto& anim : pcealist.back().animPathList)
		{
			PCEAData newData;
			newData.modFile = pcealist.back().modFile;
			newData.animPath = anim.second;
			animReplaced[anim.first].push_back(newData);
		}
	}

	if (pcealist.size() > 0)
	{
		interMsg(TextBoxMessage(1006) + "Nemesis PCEA");
		PatchDebug(TextBoxMessage(1006) + "Nemesis PCEA");
	}
}

bool PCEAInstallation()
{
	string import;

#ifdef DEBUG
	if (SSE)
	{
		import = "data\\source\\scripts";
	}
	else
	{
		import = "data\\scripts\\source";
	}
#else
	if (SSE)
	{
		import = skyrimDataPath->GetDataPath() + "source";
	}
	else
	{
		import = skyrimDataPath->GetDataPath() + "scripts\\source";
	}
#endif

	string filename = skyrimDataPath->GetDataPath() + "Nemesis PCEA.esp";
	PatchDebug(filename);

	{
		FILE* f;
		fopen_s(&f, filename.c_str(), "r+b");

		if (f)
		{
			for (unsigned int j = 0; j < pcealist.size(); ++j)
			{
				unsigned int size = min(pcealist[j].modFile.length(), 112);

				for (unsigned int i = 0; i < size; ++i)
				{
					fseek(f, 2304 + (117 * j) + i, SEEK_SET);
					unsigned char charcode = static_cast<unsigned char>(pcealist[j].modFile[i]);
					fwrite(&charcode, sizeof(charcode), 1, f);
				}
			}

			for (unsigned int j = pcealist.size(); j < 10; ++j)
			{
				for (unsigned int i = 0; i < 112; ++i)
				{
					fseek(f, 2304 + (117 * j) + i, SEEK_SET);
					unsigned char charcode = 32;
					fwrite(&charcode, sizeof(charcode), 1, f);
				}
			}

			fclose(f);
		}
		else
		{
			ErrorMessage(3002, filename);
			return false;
		}
	}

	PatchDebug("PCEA esp modification complete");

	{
		bf::path source("alternate animation\\nemesis pcea.script");
		string pscfile = import + "\\Nemesis_PCEA_Core.psc";
		bf::path target(pscfile);
		bf::copy_file(source, target, bf::copy_option::overwrite_if_exists);
		PatchDebug(source.string());
		PatchDebug(pscfile);

		vecstr storeline;
		vecstr newline;

		if (!GetFunctionLines(pscfile, storeline))
		{
			return false;
		}

		for (auto& line : storeline)
		{
			bool skip = false;

			if (line.find("\tPCEA[num] =") != NOT_FOUND)
			{
				unsigned int counter = 0;

				while (counter < pcealist.size())
				{
					string templine = line;
					templine.replace(templine.find("num"), 3, to_string(counter));
					templine.append("\"" + pcealist[counter].modFile + "\"");
					newline.push_back(templine);
					++counter;
				}

				skip = true;
			}
			else if (line.find("\tactivation[num]") != NOT_FOUND)
			{
				unsigned int counter = 0;

				while (counter < pcealist.size())
				{
					string templine = line;
					templine.replace(templine.find("num"), 3, to_string(counter));
					templine.append("0");
					newline.push_back(templine);
					++counter;
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
		}

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
			ErrorMessage(3002, filename);
			return false;
		}

		if (!FolderCreate(import))
		{
			return false;
		}

		string destination = skyrimDataPath->GetDataPath() + "scripts";
		string filepath = destination + "\\Nemesis_PCEA_Core.pex";

		if (!PapyrusCompile(pscfile, import, destination, filepath))
		{
			return false;
		}
	}

	PatchDebug("PCEA core script complete");
	return true;
}
