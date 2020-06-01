#include "Global.h"

#include "debuglog.h"
#include "nemesisinfo.h"

#include "generate/papyruscompile.h"
#include "generate/generator_utility.h"
#include "generate/alternateanimation.h"

#include "utilities/crc32.h"
#include "utilities/algorithm.h"
#include "utilities/lastupdate.h"
#include "utilities/readtextfile.h"
#include "utilities/writetextfile.h"

#pragma warning(disable : 4503)

using namespace std;
namespace sf = filesystem;

unordered_map<string, int> AAgroup_Counter;

bool AACoreCompile(sf::path pscfile,
                   sf::path import,
                   sf::path destination,
                   sf::path filepath,
                   sf::path tempcompiling,
                   VecStr& newFunctions,
                   uint& maxGroup,
                   uint& uniquekey,
                   sf::path target);
bool AAnimAPICompile(sf::path pscfile,
                     sf::path import,
                     sf::path destination,
                     sf::path filepath,
                     sf::path tempcompiling,
                     VecStr& newFunctions,
                     uint maxGroup,
                     uint& uniquekey,
                     sf::path compilerpath);
void fixedKeyInitialize();
uint getUniqueKey(unsigned char bytearray[], int byte1, int byte2);

struct ModIDByGroup
{
    string groupBase;
    string modID;
};

void AAInitialize(string AAList)
{
    VecStr groupList;
    unordered_map<string, string>
        existAAAnim; // animation name, animation group name; has the animation been registered for AA?
    DebugLogging("Caching alternate animation group...");
    read_directory(AAList, groupList);

    for (string& groupName : groupList)
    {
        if (!nemesis::iequals(groupName, "alternate animation.script")
            && nemesis::iequals(sf::path(AAList + "\\" + groupName).extension().string(), ".txt"))
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
                            AAGroup[lowerAnimFile]     = lowerGroupName;
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

bool AAInstallation(const NemesisInfo* nemesisInfo)
{
    if (AAGroup.size() == 0) return true;

    uint uniquekey;
    wstring cachedir = papyrusTempCompile();

    try
    {
        sf::create_directories(cachedir);
    }
    catch (const exception& ex)
    {
        ErrorMessage(6002, cachedir, ex.what());
    }

    if (error) throw nemesis::exception();

    sf::path import(nemesisInfo->GetDataPath() + L"scripts\\source");
    sf::path destination(nemesisInfo->GetStagePath() + L"scripts");
    sf::path source("alternate animation\\alternate animation.script");
    sf::path pscfile(cachedir + L"\\Nemesis_AA_Core.psc");
    sf::path filepath(destination.wstring() + L"\\Nemesis_AA_Core.pex");
    sf::copy_file(source, pscfile, sf::copy_options::overwrite_existing);
    DebugLogging(pscfile.wstring());
    DebugLogging(filepath.wstring());

    if (!FolderCreate(import)) ErrorMessage(2010, import);

    if (!isFileExist(import)) ErrorMessage(2010, import);

    uint maxGroup;
    fixedKeyInitialize();
    VecStr newFunctions;

    if (!AACoreCompile(pscfile,
                       import,
                       destination,
                       filepath,
                       cachedir,
                       newFunctions,
                       maxGroup,
                       uniquekey,
                       nemesisInfo->GetDataPath()))
    {
        return false;
    }

    if (error) throw nemesis::exception();

    source            = sf::path("alternate animation\\alternate animation 2.script");
    sf::path pscfile2 = sf::path(cachedir + L"\\FNIS_aa.psc");
    filepath          = sf::path(destination.wstring() + L"\\FNIS_aa.pex");
    sf::copy_file(source, pscfile2, sf::copy_options::overwrite_existing);
    DebugLogging(pscfile2.string());
    DebugLogging(filepath);

    if (!AAnimAPICompile(pscfile2,
                         import,
                         destination,
                         filepath,
                         cachedir,
                         newFunctions,
                         maxGroup,
                         uniquekey,
                         nemesisInfo->GetDataPath()))
    {
        return false;
    }

    try
    {
        if (!sf::remove(pscfile)) ErrorMessage(1082, pscfile);
    }
    catch (const exception& ex)
    {
        ErrorMessage(6002, pscfile, ex.what());
    }

    try
    {
        if (!sf::remove(pscfile2)) ErrorMessage(1082, pscfile2);
    }
    catch (const exception& ex)
    {
        ErrorMessage(6002, pscfile2, ex.what());
    }

    if (error) throw nemesis::exception();

    return true;
}

bool AACoreCompile(sf::path pscfile,
                   sf::path import,
                   sf::path destination,
                   sf::path filepath,
                   sf::path tempcompiling,
                   VecStr& newFunctions,
                   uint& maxGroup,
                   uint& uniquekey,
                   sf::path compilerpath)
{
    bool prefixDone = false;
    VecStr prefixList;
    unordered_map<string, bool> prefixCheck;
    unordered_map<string, int> prefixID;
    unordered_map<string, int> groupIDCounter;
    unordered_map<string, string> baseOrder;
    unordered_map<string, vector<ModIDByGroup>> GetModByGroupValue;
    VecStr baseMatch;

    VecStr groupIDFunction;
    VecStr prefixlines;
    VecStr groupAAlines;
    VecStr storeline;
    VecStr newline;
    newline.reserve(storeline.size());
    GetFunctionLines(pscfile.c_str(), storeline);

    int AACounter = 0;
    maxGroup      = 0;

    for (auto& groupName : groupNameList)
    {
        for (
            auto& AAprefixGroup : groupAAPrefix
                [groupName]) // list of group aa prefix categorized by animation group name while eliminating duplicates using set container
        {
            if (!prefixCheck[AAprefixGroup])
            {
                prefixList.push_back(AAprefixGroup);
                prefixCheck[AAprefixGroup] = true;
            }
        }
    }

    if (prefixList.size() > 0) // Assign mod ID
    {
        auto nextprefix       = prefixList.begin();
        string templine       = "	if (curAAPrefix == \"" + *nextprefix + "\")";
        string rr             = "		return 0";
        prefixID[*nextprefix] = maxGroup;
        prefixlines.push_back(templine);
        prefixlines.push_back(rr);
        ++nextprefix;
        ++maxGroup;

        for (auto& prefix = nextprefix; prefix != prefixList.end(); ++prefix)
        {
            templine          = "	elseif (curAAPrefix == \"" + *prefix + "\")";
            rr                = "		return " + to_string(maxGroup);
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
        VecStr groupID;
        groupIDFunction.push_back("int Function GetGroupID(string groupName) global");

        for (auto& groupName : groupNameList)
        {
            string adjGN = groupName;

            if (groupName.length() > 4 && groupName.rfind("_1p*") == groupName.length() - 4) adjGN.pop_back();

            for (auto& prefix : groupAAPrefix[groupName])
            {
                int maxG        = prefixID[prefix];
                string templine = "	AASet[num] = ";
                templine.replace(templine.find("num"), 3, to_string(AACounter));
                string number = to_string(maxG / 10) + to_string(maxG % 10) + to_string(maxGroup / 100)
                                + to_string(maxGroup % 100 / 10) + to_string(maxGroup % 10);
                string AAgroupID = to_string(maxGroup / 10) + to_string(maxGroup % 10);

                if (groupIDCounter[adjGN] == 0) groupIDCounter[adjGN] = 1;

                string counter = to_string(groupIDCounter[adjGN]);
                string base    = counter.substr(0, 3);
                ModIDByGroup mod;
                mod.groupBase = base;
                mod.modID     = to_string(maxG / 10) + to_string(maxG % 10);
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

        if (groupID.size() > 0)
        {
            groupIDFunction.push_back("	if (groupName == \"" + groupID[0] + "\")");
            groupIDFunction.push_back("		return " + groupID[1]);

            for (uint k = 2; k < groupID.size(); ++k)
            {
                groupIDFunction.push_back(k % 2 == 0 ? "	elseif (groupName == \"" + groupID[k] + "\")"
                                                     : "		return " + groupID[k]);
            }

            groupIDFunction.push_back("	endif");
            groupIDFunction.push_back(
                "	Debug.Trace(\"ERROR: Unknown Nemesis AA group name (Group Name: \" + groupName + \")\")");
            groupIDFunction.push_back("	return -1");
            groupIDFunction.push_back("endFunction");
            groupIDFunction.push_back("");
        }
    }

    DebugLogging("Group base value complete");

    for (uint k = 0; k < storeline.size(); ++k)
    {
        bool skip   = false;
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

                for (uint j = 2; j < baseMatch.size(); ++j)
                {
                    newline.push_back(j % 2 == 0 ? "	elseif (" + baseMatch[j] + ")" : baseMatch[j]);
                }

                newline.push_back("	endif");
            }

            skip = true;
        }
        else if (line.find("(AAgroupID == num)") != NOT_FOUND)
        {
            if (baseOrder.size() > 0)
            {
                int counter     = 0;
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
        }
        else if (line.find("(AAgroupID == value)") != NOT_FOUND)
        {
            if (GetModByGroupValue.size() > 0)
            {
                auto firstGroup = GetModByGroupValue.begin();
                string space    = "	";
                newline.push_back(space + "if (AAgroupID == " + firstGroup->first + ")");
                space += "	";

                if (firstGroup->second.size() > 1)
                {
                    for (uint j = 0; j < firstGroup->second.size(); ++j)
                    {
                        if (j + 1 == firstGroup->second.size() - 1)
                        {
                            newline.push_back(space + "if (groupValue < " + firstGroup->second[j].groupBase
                                              + ")");
                            newline.push_back(space + "	return " + firstGroup->second[j].modID);
                            newline.push_back(space + "else");
                            newline.push_back(space + "	return " + firstGroup->second[j + 1].modID);
                            break;
                        }
                        else
                        {
                            newline.push_back(space + "if (groupValue < "
                                              + firstGroup->second[j + 1].groupBase + ")");
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
                        for (uint j = 0; j < group->second.size(); ++j)
                        {
                            if (j + 1 == group->second.size() - 1)
                            {
                                newline.push_back(space + "if (groupValue < " + group->second[j + 1].groupBase
                                                  + ")");
                                newline.push_back(space + "	return " + group->second[j].modID);
                                newline.push_back(space + "else");
                                newline.push_back(space + "	return " + group->second[j + 1].modID);
                                break;
                            }
                            else
                            {
                                newline.push_back(space + "if (groupValue < " + group->second[j + 1].groupBase
                                                  + ")");
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

        if (!skip) newline.push_back(line);
    }

    if (groupIDFunction.size() > 0)
        newline.insert(newline.end(), groupIDFunction.begin(), groupIDFunction.end());

    for (auto& curline : newline)
    {
        uniquekey = uniquekey + CRC32Convert(curline);
        uniquekey = uniquekey % 987123;
    }

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
            ErrorMessage(3002, pscfile.wstring());
        }
    }

    if (!PapyrusCompile(pscfile, import, destination, filepath, tempcompiling, compilerpath)) return false;

    DebugLogging("AA core script complete");
    return true;
}

bool AAnimAPICompile(sf::path pscfile,
                     sf::path import,
                     sf::path destination,
                     sf::path filepath,
                     sf::path tempcompiling,
                     VecStr& newFunctions,
                     uint maxGroup,
                     uint& uniquekey,
                     sf::path compilerpath)
{
    VecStr storeline;
    VecStr newline;
    newline.reserve(storeline.size());

    if (!GetFunctionLines(pscfile.c_str(), storeline)) return false;

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
            ErrorMessage(3002, pscfile);
        }
    }

    return PapyrusCompile(pscfile, import, destination, filepath, tempcompiling, compilerpath);
}

void fixedKeyInitialize()
{
    int counter = 0;

    while (counter <= 256)
    {
        uint key1         = static_cast<uint>(counter);
        int key2          = 0;

        while (key2 <= 7)
        {
            if ((key1 & 1) > 0)
                key1 = key1 >> 1 ^ 3988292384; // is odd number
            else
                key1 >>= 1;

            ++key2;
        }

        fixedkey[counter] = key1;
        ++counter;
    }
}

unsigned int CRC32Convert(string line)
{
    static nemesis::CRC32 crc32;
    return crc32.FullCRC(line);
}

unsigned int CRC32Convert(wstring line)
{
    static nemesis::CRC32 crc32;
    return crc32.FullCRC(nemesis::transform_to<string>(line));
}

uint getUniqueKey(unsigned char bytearray[], int byte1, int byte2)
{
    uint uniqueKey         = 0;
    int key1               = byte1;
    int key2               = byte2;
    int counter            = byte1;

    while (counter <= key2)
    {
        unsigned char curByte = bytearray[counter];
        uint key3             = uniqueKey ^ static_cast<uint>(curByte);
        uniqueKey             = uniqueKey >> 8 ^ fixedkey[key3 & 255];
        counter++;
    }

    return uniqueKey;
}
