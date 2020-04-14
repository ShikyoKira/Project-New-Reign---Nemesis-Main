#include "Global.h"

#include <map>
#include <regex>
#include <unordered_set>

#include <QtCore/QStandardPaths.h>

#include "debuglog.h"
#include "nemesisinfo.h"

#include "utilities/regex.h"
#include "utilities/writetextfile.h"

#include "generate/alternateanimation.h"
#include "generate/playerexclusive.h"

using namespace std;
namespace sf = filesystem;

#define min(a, b) (((a) < (b)) ? (a) : (b))

vector<PCEA> pcealist;
unordered_map<string, vector<PCEAData>> animReplaced;

void tryDelete(string file, int repeated = 0)
{
    if (repeated > 100) ErrorMessage(1082, "PCEA_animations", file);

    if (!sf::remove(file)) tryDelete(file, repeated + 1);
}

bool Delete(sf::path file)
{
    if (sf::is_directory(file))
    {
        string tempbehavior = file.string();
        VecStr filelist;
        read_directory(tempbehavior, filelist);
        tempbehavior.append("\\");

        for (auto& curfile : filelist)
        {
            Delete(tempbehavior + curfile);
        }
    }

    tryDelete(file.string());
    return true;
}

void forcedCopy(sf::path animFile, sf::path newAnimFile, int count = 0)
{
    try
    {
        sf::copy_file(animFile, newAnimFile, sf::copy_options::overwrite_existing);
    }
    catch (const exception& ex)
    {
        if (count > 200) throw ex;

        forcedCopy(animFile, newAnimFile, count + 1);
    }
}

void PCEASubFolder(string path, unsigned short number, string pceafolder, string subpath, PCEA& mod)
{
    VecStr animlist;
    read_directory(path, animlist);

    for (auto& anim : animlist)
    {
        sf::path animFile(path + "\\" + anim);

        if (!sf::is_directory(animFile))
        {
            if (!nemesis::iequals(animFile.extension().string(), ".hkx")) continue;

            int numb           = 0;
            string newFileName = "fp2" + to_string(numb++) + "_" + anim;

            while (isFileExist(pceafolder + "\\" + newFileName))
                newFileName = "fp2" + to_string(numb++) + "_" + anim;

            sf::path newAnimFile(pceafolder + "\\" + newFileName);
            string lowerAnim            = nemesis::to_lower_copy(anim);
            mod.animPathList[lowerAnim] = "Animations\\Nemesis_PCEA\\" + subpath + "\\" + newFileName;

            if (!isFileExist(pceafolder)) sf::create_directories(pceafolder);

            forcedCopy(animFile, newAnimFile);

            if (!sf::exists(animFile)) ErrorMessage(1185, newAnimFile.string());
        }
        else
        {
            PCEASubFolder(animFile.string(), number, pceafolder + "\\" + anim, subpath + "\\" + anim, mod);
        }
    }
}

void ReadPCEA(const NemesisInfo* nemesisInfo)
{
    DebugLogging("Reading PCEA files...");
    pcealist     = vector<PCEA>();
    animReplaced = unordered_map<string, vector<PCEAData>>();

    VecStr folderlist;
    string datapath = nemesisInfo->GetDataPath() + "meshes\\actors\\character\\Animations\\Nemesis_PCEA";
    sf::path pceafolder(datapath + "\\PCEA_animations");

    if (isFileExist(pceafolder.string())) Delete(pceafolder);

    if (!FolderCreate(datapath + "\\PCEA_animations\\")) return;

    read_directory(datapath, folderlist);
    map<unsigned short, PCEA> modlist;
    map<unsigned short, string> pceaFolderMapList;

    for (auto& folder : folderlist)
    {
        string path = datapath + "\\" + folder;

        if (isdigit(folder[0]) && sf::is_directory(path))
        {
            unsigned short number = static_cast<unsigned short>(
                stoi(nemesis::regex_replace(string(folder), nemesis::regex("([0-9]+)[^\n]+"), string("\\1"))));
            pceaFolderMapList[number] = path + "|" + folder;
        }

        if (error) throw nemesis::exception();
    }

    for (auto& pceaFolderMap : pceaFolderMapList)
    {
        string folder         = pceaFolderMap.second;
        unsigned short number = pceaFolderMap.first;

        if (modlist.find(number) == modlist.end())
        {
            PCEA mod;
            mod.modFile = folder.substr(folder.find("|") + 1);
            PCEASubFolder(
                folder.substr(0, folder.find("|")), number, pceafolder.string(), "PCEA_animations", mod);

            if (mod.animPathList.size() > 0) modlist[number] = mod;
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
            newData.modFile  = pcealist.back().modFile;
            newData.animPath = anim.second;
            animReplaced[anim.first].push_back(newData);
        }

        if (error) throw nemesis::exception();
    }
}

bool PCEAInstallation(const NemesisInfo* nemesisInfo)
{
#ifdef DEBUG
    string import = "data\\scripts\\source";
#else
    string import = nemesisInfo->GetDataPath() + "scripts\\source";
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

            for (uint j = 0; j < pcealist.size(); ++j)
            {
                string number        = to_string(j + 1);
                uint size    = min(pcealist[j].modFile.length(), 113 - number.length());
                uint counter = startnum + (117 * j);

                for (uint i = 0; i < number.length(); ++i)
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

                for (uint i = 0; i < size; ++i)
                {
                    fseek(f, counter + i, SEEK_SET);
                    charcode = static_cast<unsigned char>(pcealist[j].modFile[i]);
                    fwrite(&charcode, sizeof(charcode), 1, f);
                }

                if (error) throw nemesis::exception();
            }

            for (uint j = pcealist.size(); j < 10; ++j)
            {
                for (uint i = 0; i < 115; ++i)
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
    wstring cachedir
        = sf::path(QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0).toStdWString())
              .parent_path()
              .wstring()
          + L"/Nemesis";
    replace(cachedir.begin(), cachedir.end(), '/', '\\');

    try
    {
        sf::create_directories(cachedir);
    }
    catch (const exception& ex)
    {
        ErrorMessage(6002, nemesis::transform_to<string>(cachedir), ex.what());
    }

    sf::path source("alternate animation\\nemesis pcea.script");
    sf::path pscfile(cachedir + L"\\Nemesis_PCEA_Core.psc");
    sf::copy_file(source, pscfile, sf::copy_options::overwrite_existing);
    DebugLogging(source.string());
    DebugLogging(pscfile.string());

    VecStr storeline;
    VecStr newline;

    if (!GetFunctionLines(pscfile.string(), storeline)) return false;

    for (auto& line : storeline)
    {
        bool skip = false;

        if (line.find("\tPCEA[num] =") != NOT_FOUND)
        {
            for (uint j = 0; j < pcealist.size(); ++j)
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
            for (uint j = 0; j < pcealist.size(); ++j)
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

        if (!skip) newline.push_back(line);

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

        if (!FolderCreate(import)) ErrorMessage(2010, import);
    }

    if (!isFileExist(import)) ErrorMessage(2010, import);

    if (error) throw nemesis::exception();

    string destination = nemesisInfo->GetDataPath() + "scripts";
    string filepath    = destination + "\\Nemesis_PCEA_Core.pex";

    if (!PapyrusCompile(
            pscfile, nemesis::transform_to<wstring>(import), destination, filepath, cachedir, nemesisInfo->GetDataPath()))
        return false;

    DebugLogging("PCEA core script complete");
    return true;
}
