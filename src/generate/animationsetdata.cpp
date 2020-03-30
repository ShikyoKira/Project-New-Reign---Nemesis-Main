#include "Global.h"

#include "utilities/algorithm.h"

#include "generate/animationdatatracker.h"
#include "generate/animationsetdata.h"
#include "generate/playerexclusive.h"

using namespace std;

unordered_map<string, string> crc32Cache;

void DataPackProcess(map<string, datapack, alphanum_less>& storeline, int& startline, VecStr& animdatafile);
void EquipPackProcess(
    vector<equip>& storeline, int& startline, VecStr& animdatafile, string projectname, string header);
void TypePackProcess(
    vector<typepack>& storeline, int& startline, VecStr& animdatafile, string projectname, string header);
void AnimPackProcess(
    vector<animpack>& storeline, int& startline, VecStr& animdatafile, string projectname, string header);
void CRC32Process(vector<crc32>& storeline,
                  int& startline,
                  VecStr& animdatafile,
                  string projectname,
                  string header,
                  unordered_map<string, shared_ptr<VecStr>>& AAList,
                  string projectPath);

AnimationDataProject::AnimationDataProject(int& startline,
                                           VecStr& animdatafile,
                                           string filename,
                                           string projectname)
{
    try
    {
        // data list
        DataPackProcess(datalist, startline, animdatafile);
        ++startline;
        string projectPath;
        string projectPath_fp;
        unordered_map<string, shared_ptr<VecStr>> AAList;
        string projectFileName = nemesis::to_lower_copy(std::filesystem::path(projectname).stem().string());

        // assume current project has new alternate animation installed
        if (behaviorProjectPath[projectFileName].length() > 0)
        {
            projectPath
                = nemesis::to_lower_copy(string(behaviorProjectPath[projectFileName]) + "\\animations");
            projectPath_fp  = projectPath + "\\_1stperson";
            VecStr pathList = {projectPath,
                               projectPath + "\\male",
                               projectPath + "\\female",
                               projectPath + "\\horse_rider",
                               projectPath + "\\dlc01",
                               projectPath + "\\dlc02"};
            VecStr pathCRC32;
            VecStr pathCRC32_fp;
            VecStr pathList_fp = {projectPath_fp,
                                  projectPath_fp + "\\male",
                                  projectPath_fp + "\\female",
                                  projectPath_fp + "\\horse_rider",
                                  projectPath_fp + "\\dlc01",
                                  projectPath_fp + "\\dlc02"};
            pathCRC32.reserve(pathList.size());
            pathCRC32_fp.reserve(pathList.size());

            for (auto& path : pathList)
            {
                pathCRC32.push_back(to_string(CRC32Convert(path)));
            }

            for (auto& path : pathList_fp)
            {
                pathCRC32_fp.push_back(to_string(CRC32Convert(path)));
            }

            VecStr* pathListPoint;
            VecStr* pathCRC32Point;

            // cache all alternate animations
            for (auto& anim : alternateAnim)
            {
                size_t pos       = anim.first.rfind("_1p*");
                bool fp          = pos == anim.first.length() - 4;
                string animFile  = fp ? anim.first.substr(0, pos) : anim.first;
                string animCRC32 = to_string(CRC32Convert(nemesis::to_lower_copy(GetFileName(animFile))));

                if (fp)
                {
                    pathListPoint  = &pathList_fp;
                    pathCRC32Point = &pathCRC32_fp;
                }
                else
                {
                    pathListPoint  = &pathList;
                    pathCRC32Point = &pathCRC32;
                }

                for (uint i = 0; i < pathCRC32Point->size(); ++i)
                {
                    AAList[pathCRC32Point->at(i) + "," + animCRC32 + ",7891816"]
                        = make_shared<VecStr>(anim.second);
                }
            }

            if (isFileExist(nemesisInfo->GetDataPath() + projectPath + "\\nemesis_pcea\\pcea_animations")
                && pcealist.size() > 0)
            {
                // cache all pcea animations
                for (auto& pcea : pcealist)
                {
                    for (auto& animPath : pcea.animPathList)
                    {
                        string animCRC32
                            = to_string(CRC32Convert(nemesis::to_lower_copy(GetFileName(animPath.first))));
                        string pathline = animPath.second.substr(wordFind(animPath.second, "Nemesis_PCEA"));

                        for (uint i = 0; i < pathCRC32.size(); ++i)
                        {
                            string crc32line = pathCRC32[i] + "," + animCRC32 + ",7891816";

                            if (!AAList[crc32line]) AAList[crc32line] = make_shared<VecStr>();

                            AAList[crc32line]->push_back(pathline);
                        }
                    }
                }
            }
        }

        for (auto it = datalist.begin(); it != datalist.end(); ++it)
        {
            ++startline;

            if (startline >= int(animdatafile.size())) ErrorMessage(5019, projectname);

            // equip list
            EquipPackProcess(it->second.equiplist, startline, animdatafile, projectname, it->first);

            // type list
            TypePackProcess(it->second.typelist, startline, animdatafile, projectname, it->first);

            // anim list
            AnimPackProcess(it->second.animlist, startline, animdatafile, projectname, it->first);

            // crc3 list
            CRC32Process(
                it->second.crc32list, startline, animdatafile, projectname, it->first, AAList, projectPath);
        }
    }
    catch (int)
    {
        // empty
    }
}

void DataPackProcess(map<string, datapack, alphanum_less>& storeline, int& startline, VecStr& animdatafile)
{
    if (startline >= int(animdatafile.size()))
        ErrorMessage(5018, "Header", "Header");
    else if (!isOnlyNumber(animdatafile[startline]))
        ErrorMessage(5001, "Header", "Header");

    for (int i = startline + 1; i < int(animdatafile.size()); ++i)
    {
        datapack newDataPack;
        storeline[animdatafile[i]] = newDataPack;

        if (i + 4 >= int(animdatafile.size())) ErrorMessage(5018, "Header", "Header");

        if (nemesis::iequals(animdatafile[i + 1], "V3"))
        {
            startline = i;
            return;
        }
    }
}

void EquipPackProcess(
    std::vector<equip>& storeline, int& startline, VecStr& animdatafile, string projectname, string header)
{
    if (startline >= int(animdatafile.size()))
        ErrorMessage(5018, projectname, header);
    else if (!isOnlyNumber(animdatafile[startline]))
        ErrorMessage(5001, projectname, "Header");

    for (int i = startline + 1; i < int(animdatafile.size()); ++i)
    {
        if (isOnlyNumber(animdatafile[i]))
        {
            startline = i;
            return;
        }

        equip tempEquip;
        tempEquip.name = animdatafile[i];
        storeline.push_back(tempEquip);

        if (i + 1 >= int(animdatafile.size())) ErrorMessage(5018, projectname, header);
    }
}

void TypePackProcess(
    vector<typepack>& storeline, int& startline, VecStr& animdatafile, string projectname, string header)
{
    if (startline >= int(animdatafile.size()))
        ErrorMessage(5018, projectname, header);
    else if (!isOnlyNumber(animdatafile[startline]))
        ErrorMessage(5001, projectname, header);

    int counter = 0;

    for (int i = startline + 1; i < int(animdatafile.size()); ++i)
    {
        if (isOnlyNumber(animdatafile[i]))
        {
            if (i + 1 < int(animdatafile.size()) && hasAlpha(animdatafile[i + 1])
                || (i + 4 < int(animdatafile.size()) && animdatafile[i + 4] == "7891816")
                || (i + 2 < int(animdatafile.size()) && nemesis::iequals(animdatafile[i + 2], "V3"))
                || (i + 2 == animdatafile.size()))
            {
                startline = i;
                return;
            }
        }

        typepack tempTP;

        if (isOnlyNumber(animdatafile[i]) || !hasAlpha(animdatafile[i]))
            ErrorMessage(5001, projectname, header);

        tempTP.name = animdatafile[i];

        if (++i >= int(animdatafile.size()))
            ErrorMessage(5018, projectname, header);
        else if (!isOnlyNumber(animdatafile[i]))
            ErrorMessage(5001, projectname, header);

        tempTP.equiptype1 = animdatafile[i];

        if (++i >= int(animdatafile.size()))
            ErrorMessage(5018, projectname, header);
        else if (!isOnlyNumber(animdatafile[i]))
            ErrorMessage(5001, projectname, header);

        tempTP.equiptype2 = animdatafile[i];
        storeline.push_back(tempTP);

        if (i + 1 >= int(animdatafile.size())) ErrorMessage(5018, projectname, header);
    }
}

void AnimPackProcess(
    vector<animpack>& storeline, int& startline, VecStr& animdatafile, string projectname, string header)
{
    if (startline >= int(animdatafile.size()))
        ErrorMessage(5018, projectname, header);
    else if (!isOnlyNumber(animdatafile[startline]))
        ErrorMessage(5001, projectname, header);

    for (int i = startline + 1; i < int(animdatafile.size()); ++i)
    {
        if (animdatafile[i] == "0")
        {
            if ((i + 1 < int(animdatafile.size()) && animdatafile[i + 1] == "V3")
                || (i + 2 < int(animdatafile.size()) && animdatafile[i + 2].find(".txt") != NOT_FOUND))
            {
                startline = i;
                break;
            }
        }

        if (animdatafile[i] == "1")
        {
            if ((i + 4 < int(animdatafile.size()) && animdatafile[i + 4] == "V3")
                || (i + 5 < int(animdatafile.size()) && animdatafile[i + 5].find(".txt") != NOT_FOUND))
            {
                startline = i;
                break;
            }
        }

        if (isOnlyNumber(animdatafile[i]))
        {
            if ((i + 3 < int(animdatafile.size()) && animdatafile[i + 3] == "7891816")
                || i + 1 == animdatafile.size())
            {
                startline = i;
                break;
            }
        }

        animpack tempAP;
        tempAP.eventname = animdatafile[i++];
        tempAP.unknown   = animdatafile[i++];
        unordered_map<string, bool> clipExist;

        if (i >= int(animdatafile.size()))
            ErrorMessage(5018, projectname, header);
        else if (!isOnlyNumber(animdatafile[i]))
            ErrorMessage(5001, projectname, header);

        while (i < int(animdatafile.size()))
        {
            ++i;

            if (animdatafile[i] == "0")
            {
                if ((i + 1 < int(animdatafile.size()) && animdatafile[i + 1] == "V3")
                    || (i + 2 < int(animdatafile.size()) && animdatafile[i + 2].find(".txt") != NOT_FOUND))
                {
                    --i;
                    break;
                }
            }

            if (animdatafile[i] == "1")
            {
                if ((i + 4 < int(animdatafile.size()) && animdatafile[i + 4] == "V3")
                    || (i + 5 < int(animdatafile.size()) && animdatafile[i + 5].find(".txt") != NOT_FOUND))
                {
                    --i;
                    break;
                }
            }

            if (isOnlyNumber(animdatafile[i]))
            {
                if (i + 3 < int(animdatafile.size()) && animdatafile[i + 3] == "7891816")
                {
                    --i;
                    break;
                }
                else if (i + 1 == animdatafile.size())
                {
                    startline = i;
                    break;
                }
            }

            if (hasAlpha(animdatafile[i]))
            {
                bool out = false;

                for (int k = i + 1; k < i + 3; ++k)
                {
                    if (!isOnlyNumber(animdatafile[k]))
                    {
                        out = false;
                        break;
                    }
                    else
                    {
                        out = true;
                    }
                }

                if (out)
                {
                    if (i + 3 < int(animdatafile.size()) && hasAlpha(animdatafile[i + 3])
                        && animdatafile[i + 3] != "V3")
                    {
                        --i;
                        break;
                    }
                }
            }

            tempAP.attackClip.push_back(animdatafile[i]);
            clipExist[tempAP.attackClip.back().data] = true;
        }

        for (auto& atkData : tempAP.attackClip)
        {
            const auto& ptr = clipPtrAnimData.find(projectname);

            if (ptr != clipPtrAnimData.end())
            {
                const auto& ptr2 = ptr->second.find(atkData.data);

                if (ptr2 != ptr->second.end())
                {
                    if (ptr2->second.size() > 0)
                    {
                        size_t size = ptr2->second[0]->cliplist.size();

                        for (auto& clipname : ptr2->second[0]->cliplist)
                        {
                            if (!clipExist[atkData.data])
                            {
                                tempAP.attackClip.push_back(clipname);
                                clipExist[clipname] = true;
                            }
                            else
                            {
                                --size;
                            }
                        }

                        if (size == 0) break;
                    }
                }
            }
        }

        storeline.push_back(tempAP);

        if (i + 1 >= int(animdatafile.size())) ErrorMessage(5018, projectname, header);
    }
}

void CRC32Process(vector<crc32>& storeline,
                  int& startline,
                  VecStr& animdatafile,
                  string projectname,
                  string header,
                  unordered_map<string, shared_ptr<VecStr>>& AAList,
                  string projectPath)
{
    if (startline >= int(animdatafile.size()))
        ErrorMessage(5018, projectname, header);
    else if (!isOnlyNumber(animdatafile[startline]))
        ErrorMessage(5001, projectname, header);

    vector<crc32> newCRC;
    unordered_set<string> isExisted;

    for (int i = startline + 1; i < int(animdatafile.size()); ++i)
    {
        if (animdatafile[i] == "V3")
        {
            startline = i;
            break;
        }
        else if (i + 1 < int(animdatafile.size()) && animdatafile[i + 1].find(".txt") != NOT_FOUND)
        {
            startline = i;
            break;
        }

        storeline.push_back(crc32());

        if (!isOnlyNumber(animdatafile[i])) ErrorMessage(5001, projectname, header);

        storeline.back().filepath = animdatafile[i];

        if (++i >= int(animdatafile.size()))
            ErrorMessage(5018, projectname, header);
        else if (!isOnlyNumber(animdatafile[i]))
            ErrorMessage(5001, projectname, header);

        storeline.back().filename = animdatafile[i];

        if (++i >= int(animdatafile.size()))
            ErrorMessage(5018, projectname, header);
        else if (!isOnlyNumber(animdatafile[i]))
            ErrorMessage(5001, projectname, header);

        storeline.back().fileformat = animdatafile[i];

        if (i + 1 >= int(animdatafile.size())) startline = i;

        // if existing crc32 line match the assumed line, new alternate animations will be converted into crc32 number and added in
        auto it = AAList.find(storeline.back().filepath + "," + storeline.back().filename + ","
                              + storeline.back().fileformat);

        if (it != AAList.end())
        {
            for (auto& anim : *it->second)
            {
                if (anim != "x")
                {
                    crc32 newCRC32;
                    string line;
                    string combined;
                    string modID = GetFileDirectory(anim);
                    modID.pop_back();
                    modID   = nemesis::to_lower_copy(projectPath + "\\" + modID);
                    auto it = crc32Cache.find(modID);

                    if (it != crc32Cache.end())
                        line = it->second;
                    else
                    {
                        line              = to_string(CRC32Convert(modID));
                        crc32Cache[modID] = line;
                    }

                    combined.append(line + ",");
                    newCRC32.filepath = line;

                    modID = nemesis::to_lower_copy(GetFileName(anim));
                    it    = crc32Cache.find(modID);

                    if (it != crc32Cache.end())
                        line = it->second;
                    else
                    {
                        line              = to_string(CRC32Convert(modID));
                        crc32Cache[modID] = line;
                    }

                    combined.append(line + ",7891816");
                    newCRC32.filename   = line;
                    newCRC32.fileformat = "7891816";

                    if (isExisted.find(combined) == isExisted.end())
                    {
                        newCRC.push_back(newCRC32);
                        isExisted.insert(combined);
                    }
                }
            }
        }
    }

    storeline.insert(storeline.end(), newCRC.begin(), newCRC.end());
}

ASDFormat::position
ASDPosition(VecStr animData, string project, string header, string modcode, int linecount, bool muteError)
{
    // has function
    bool isOpen       = false;
    int functionstart = -1;
    int conditionOpen = 0;
    unordered_map<int, bool> isCondition;
    unordered_map<int, bool> isConditionOri;
    unordered_map<int, ASDFunct> marker;

    if (linecount >= int(animData.size())) { ErrorMessage(5010, modcode, project, header); }

    if (animData[linecount].find("<!-- ") != NOT_FOUND)
    {
        if (!muteError) { ErrorMessage(3007, modcode, "animationsetdatasinglefile.txt", linecount, header); }
    }

    bool mod = false;

    for (uint i = 0; i < animData.size(); ++i)
    {
        if (animData[i].find("<!-- ") != NOT_FOUND)
        {
            if (functionstart == -1) { functionstart = i; }

            marker[i].skip = true;

            if (animData[i].find("<!-- CLOSE -->") != NOT_FOUND)
            {
                if (mod)
                {
                    isCondition[conditionOpen]    = false;
                    isConditionOri[conditionOpen] = false;
                    --conditionOpen;
                    mod = false;
                }
                else
                {
                    if (!isOpen)
                    {
                        ErrorMessage(
                            1171, modcode, project + "~" + header.substr(0, header.find_last_of(".")), i + 1);
                    }

                    isOpen = false;
                }
            }
            else if (animData[i].find("<!-- CONDITION END -->") != NOT_FOUND)
            {
                isCondition[conditionOpen]    = false;
                isConditionOri[conditionOpen] = false;
                --conditionOpen;
            }
        }

        if (isOpen) { marker[i].isNew = true; }

        if (isCondition[conditionOpen])
        {
            marker[i].isCondition   = true;
            marker[i].conditionOpen = conditionOpen;
        }
        else if (isConditionOri[conditionOpen])
        {
            marker[i].isConditionOri = true;
            marker[i].conditionOpen  = conditionOpen;
        }

        if (animData[i].find("<!-- ") != NOT_FOUND)
        {
            if (animData[i].find("<!-- NEW") != NOT_FOUND)
            {
                if (isOpen)
                {
                    ErrorMessage(
                        1115, modcode, project + "~" + header.substr(0, header.find_last_of(".")), i + 1);
                }

                isOpen = true;
            }
            else if (animData[i].find("<!-- CONDITION START ^") != NOT_FOUND)
            {
                ++conditionOpen;
                isCondition[conditionOpen] = true;
            }
            else if (animData[i].find("<!-- CONDITION -->") != NOT_FOUND)
            {
                isCondition[conditionOpen]    = false;
                isConditionOri[conditionOpen] = true;
            }
            else if (animData[i].find("<!-- CONDITION ^") != NOT_FOUND)
            {
                marker[i].nextCondition = true;
            }
            else if (animData[i].find("<!-- MOD_CODE ~") != NOT_FOUND)
            {
                ++conditionOpen;
                isCondition[conditionOpen] = true;
                mod                        = true;
            }
            else if (animData[i].find("<!-- ORIGINAL -->") != NOT_FOUND && mod)
            {
                isCondition[conditionOpen]    = false;
                isConditionOri[conditionOpen] = true;
            }
        }
    }

    using namespace ASDFormat;

    if (linecount < 2 && functionstart < 1)
    {
        return ASDConvert(linecount + 1, muteError);
        // 1. V3
        // 2. equipcount
    }
    else
    {
        int id = 0;

        try
        {
            int curline = 0;
            int result  = PositionLineCondition(
                curline, 0, linecount, animData, marker, modcode, header, true, muteError);

            if (result == -2)
            {
                if (!muteError) { ErrorMessage(5008, modcode, project, header); }
            }
        }
        catch (double curID)
        {
            id = static_cast<int>(curID);
        }

        return ASDConvert(id, muteError);
    }

    if (!muteError) { ErrorMessage(5009, project, header); }

    return xerror;
}

ASDFormat::position ASDConvert(int position, bool muteError)
{
    using namespace ASDFormat;

    if (position == 1) { return V3; }
    else if (position == 2)
    {
        return equipcount;
    }
    else if (position == 3)
    {
        return equiplist;
    }
    else if (position == 4)
    {
        return typecount;
    }
    else if (position == 5)
    {
        return typelist;
    }
    else if (position == 6)
    {
        return animpackcount;
    }
    else if (position == 7)
    {
        return animpackname;
    }
    else if (position == 8)
    {
        return unknown3;
    }
    else if (position == 9)
    {
        return attackcount;
    }
    else if (position == 10)
    {
        return attacklist;
    }
    else if (position == 11)
    {
        return crc32count;
    }
    else if (position == 12)
    {
        return crc32list;
    }
    else if (position == 13)
    {
        return unknown1;
    }
    else if (position == 14)
    {
        return unknown2;
    }

    return xerror;
}

int PositionLineCondition(int& i,
                          double curID,
                          int linecount,
                          VecStr animDataSet,
                          unordered_map<int, ASDFunct>& marker,
                          string modcode,
                          string header,
                          bool last,
                          bool muteError)
{
    using namespace ASDFormat;
    double id         = curID;
    int conditionOpen = marker[i].conditionOpen;
    bool jump         = false;

    if (id < 3)
    {
        while (i < linecount + 1)
        {
            if (!marker[i].skip)
            {
                if (id == 2)
                {
                    ++id;
                    break;
                }

                if (marker[i].isCondition) // new condition
                {
                    if (conditionOpen < marker[i].conditionOpen)
                    {
                        int exit = PositionLineCondition(
                            i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

                        if (exit == -1 && !last) { return -1; }
                    }
                    else if (conditionOpen > marker[i].conditionOpen)
                    {
                        --i;
                        return 0;
                    }
                    else
                    {
                        ++id;
                    }
                }
                else
                {
                    if (marker[i].isConditionOri
                        && marker[i].conditionOpen != conditionOpen + 1) // original condition
                    {
                        --i;
                        return 0;
                    }
                    else
                    {
                        if (!last)
                        {
                            --i;
                            return -1;
                        }
                        else
                        {
                            ++id;
                        }
                    }
                }

                if (marker[i].isNew)
                {
                    if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                    return -1;
                }
            }
            else if (marker[i].nextCondition) // next condition or close condition
            {
                id = curID;
            }

            ++i;
        }
    }

    if (id == 3)
    {
        while (i < linecount + 1)
        {
            if (!marker[i].skip)
            {
                if (isOnlyNumber(animDataSet[i]))
                {
                    ++id;
                    break;
                }

                if (marker[i].isCondition) // new condition
                {
                    if (conditionOpen < marker[i].conditionOpen)
                    {
                        int exit = PositionLineCondition(
                            i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

                        if (exit == -1 && !last) { return -1; }
                    }
                    else if (conditionOpen > marker[i].conditionOpen)
                    {
                        --i;
                        return 0;
                    }
                }
                else
                {
                    if (marker[i].isConditionOri
                        && marker[i].conditionOpen != conditionOpen + 1) // original condition
                    {
                        --i;
                        return 0;
                    }
                    else
                    {
                        if (!last)
                        {
                            --i;
                            return -1;
                        }
                    }
                }

                if (marker[i].isNew)
                {
                    if (id != 3)
                    {
                        if (!muteError) { ErrorMessage(5007, modcode, header, linecount); }

                        return -1;
                    }
                }
            }
            else if (marker[i].nextCondition) // next condition or close condition
            {
                id = curID;
            }

            ++i;
        }
    }

    if (id == 4)
    {
        if (isOnlyNumber(animDataSet[i]) && animDataSet[i] == "0")
        {
            if (i == linecount) { throw id; }

            ++id;
            ++id;
            ++i;
        }
        else
        {
            while (i < linecount + 1)
            {
                if (!marker[i].skip)
                {
                    if (hasAlpha(animDataSet[i]))
                    {
                        jump = true;
                        ++id;
                        break;
                    }

                    if (marker[i].isCondition) // new condition
                    {
                        if (conditionOpen < marker[i].conditionOpen)
                        {
                            int exit = PositionLineCondition(
                                i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

                            if (exit == -1 && !last) { return -1; }
                        }
                        else if (conditionOpen > marker[i].conditionOpen)
                        {
                            --i;
                            return 0;
                        }
                    }
                    else
                    {
                        if (marker[i].isConditionOri
                            && marker[i].conditionOpen != conditionOpen + 1) // original condition
                        {
                            --i;
                            return 0;
                        }
                        else
                        {
                            if (!last)
                            {
                                --i;
                                return -1;
                            }
                        }
                    }

                    if (marker[i].isNew)
                    {
                        if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                        return -1;
                    }
                }
                else if (marker[i].nextCondition) // next condition or close condition
                {
                    id = curID;
                }

                ++i;
            }
        }
    }

    if (id == 5 || id == 13 || id == 14) // change this
    {
        if (id == 5 && jump)
        {
            jump = false;
            id   = 14;
        }

        while (i < linecount + 1)
        {
            if (!marker[i].skip)
            {
                bool invert = false;

                if (i == linecount && marker[i].isCondition) { invert = true; }

                if (isOnlyNumber(animDataSet[i]) && id == 14)
                {
                    if (animDataSet[i] == "0")
                    {
                        if (id == 14)
                        {
                            id = 6;
                            break;
                        }
                    }
                    else
                    {
                        int next = 1;

                        if (animDataSet[i + next].find("<!--") != NOT_FOUND)
                        {
                            ++next;

                            while (true)
                            {
                                bool isCondition;

                                if (invert) { isCondition = marker[i + next].isConditionOri; }
                                else
                                {
                                    isCondition = marker[i + next].isCondition;
                                }

                                if (!isCondition)
                                {
                                    if (animDataSet[i + next].find("<!-- CLOSE -->") != NOT_FOUND) { ++next; }

                                    break;
                                }

                                ++next;
                            }
                        }

                        if (hasAlpha(animDataSet[i + next]))
                        {
                            ++next;

                            if (animDataSet[i + next].find("<!--") != NOT_FOUND)
                            {
                                ++next;

                                while (true)
                                {
                                    bool isCondition;

                                    if (invert) { isCondition = marker[i + next].isConditionOri; }
                                    else
                                    {
                                        isCondition = marker[i + next].isCondition;
                                    }

                                    if (!isCondition)
                                    {
                                        if (animDataSet[i + next].find("<!-- CLOSE -->") != NOT_FOUND)
                                        { ++next; }

                                        break;
                                    }

                                    ++next;
                                }
                            }

                            if (isOnlyNumber(animDataSet[i + next]))
                            {
                                ++next;

                                if (animDataSet[i + next].find("<!--") != NOT_FOUND)
                                {
                                    ++next;

                                    while (true)
                                    {
                                        bool isCondition;

                                        if (invert) { isCondition = marker[i + next].isConditionOri; }
                                        else
                                        {
                                            isCondition = marker[i + next].isCondition;
                                        }

                                        if (!isCondition)
                                        {
                                            if (animDataSet[i + next].find("<!-- CLOSE -->") != NOT_FOUND)
                                            { ++next; }

                                            break;
                                        }

                                        ++next;
                                    }
                                }

                                if (isOnlyNumber(animDataSet[i + next]))
                                {
                                    ++next;

                                    if (animDataSet[i + next].find("<!--") != NOT_FOUND)
                                    {
                                        ++next;

                                        while (true)
                                        {
                                            bool isCondition;

                                            if (invert) { isCondition = marker[i + next].isConditionOri; }
                                            else
                                            {
                                                isCondition = marker[i + next].isCondition;
                                            }

                                            if (!isCondition)
                                            {
                                                if (animDataSet[i + next].find("<!-- CLOSE -->") != NOT_FOUND)
                                                { ++next; }

                                                break;
                                            }

                                            ++next;
                                        }
                                    }

                                    if (hasAlpha(animDataSet[i + next]))
                                    {
                                        id = 6;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }

                if (marker[i].isCondition) // new condition
                {
                    if (conditionOpen < marker[i].conditionOpen)
                    {
                        int exit = PositionLineCondition(
                            i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

                        if (exit == -1 && !last) { return -1; }
                    }
                    else if (conditionOpen > marker[i].conditionOpen)
                    {
                        --i;
                        return 0;
                    }
                    else
                    {
                        if (id == 14)
                        {
                            if (hasAlpha(animDataSet[i])) { id = 5; }
                            else
                            {
                                if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                return -1;
                            }
                        }
                        else if (id == 5)
                        {
                            if (isOnlyNumber(animDataSet[i])) { id = 13; }
                            else
                            {
                                if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                return -1;
                            }
                        }
                        else if (id == 13)
                        {
                            if (isOnlyNumber(animDataSet[i])) { id = 14; }
                            else
                            {
                                if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                return -1;
                            }
                        }
                        else
                        {
                            if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                            return -1;
                        }
                    }
                }
                else
                {
                    if (marker[i].isConditionOri
                        && marker[i].conditionOpen != conditionOpen + 1) // original condition
                    {
                        --i;
                        return 0;
                    }
                    else
                    {
                        if (!last)
                        {
                            --i;
                            return -1;
                        }
                        else
                        {
                            if (id == 14)
                            {
                                if (hasAlpha(animDataSet[i])) { id = 5; }
                                else
                                {
                                    if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                    return -1;
                                }
                            }
                            else if (id == 5)
                            {
                                if (isOnlyNumber(animDataSet[i])) { id = 13; }
                                else
                                {
                                    if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                    return -1;
                                }
                            }
                            else if (id == 13)
                            {
                                if (isOnlyNumber(animDataSet[i])) { id = 14; }
                                else
                                {
                                    if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                    return -1;
                                }
                            }
                            else
                            {
                                if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                return -1;
                            }
                        }
                    }
                }

                if (marker[i].isNew)
                {
                    if (id != 5 && id != 13 && id != 14)
                    {
                        if (!muteError) { ErrorMessage(5007, modcode, header, linecount); }

                        return -1;
                    }
                }
            }
            else if (marker[i].nextCondition) // next condition or close condition
            {
                id = curID;
            }

            ++i;
        }
    }

    if (id == 6)
    {
        if (isOnlyNumber(animDataSet[i]) && animDataSet[i] == "0")
        {
            if (i == linecount) { throw id; }

            id = 11;
            ++i;
        }
        else
        {
            while (i < linecount + 1)
            {
                if (!marker[i].skip)
                {
                    if (hasAlpha(animDataSet[i]))
                    {
                        jump = true;
                        ++id;
                        break;
                    }

                    if (marker[i].isCondition) // new condition
                    {
                        if (conditionOpen < marker[i].conditionOpen)
                        {
                            int exit = PositionLineCondition(
                                i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

                            if (exit == -1 && !last) { return -1; }
                        }
                        else if (conditionOpen > marker[i].conditionOpen)
                        {
                            --i;
                            return 0;
                        }
                        else
                        {
                            if (id != 6)
                            {
                                if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                return -1;
                            }
                        }
                    }
                    else
                    {
                        if (marker[i].isConditionOri
                            && marker[i].conditionOpen != conditionOpen + 1) // original condition
                        {
                            --i;
                            return 0;
                        }
                        else
                        {
                            if (!last)
                            {
                                --i;
                                return -1;
                            }
                            else
                            {
                                if (id != 6)
                                {
                                    if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                    return -1;
                                }
                            }
                        }
                    }

                    if (marker[i].isNew)
                    {
                        if (!muteError) { ErrorMessage(5007, modcode, header, linecount); }

                        return -1;
                    }
                }
                else if (marker[i].nextCondition) // next condition or close condition
                {
                    id = curID;
                }

                ++i;
            }
        }
    }

    if (id > 6 && id < 11)
    {
        if (id == 7 && jump)
        {
            jump = false;
            id   = 10;
        }

        while (i < linecount + 1)
        {
            if (!marker[i].skip)
            {
                bool invert = false;

                if (i == linecount && marker[i].isCondition) { invert = true; }

                if (isOnlyNumber(animDataSet[i]) && (id == 10 || id == 7))
                {
                    if (animDataSet[i] == "0"
                        && (i == int(animDataSet.size()) - 1 || nemesis::iequals(animDataSet[i + 1], "V3")))
                    {
                        ++id;
                        break;
                    }
                    else
                    {
                        int next = 1;

                        if (animDataSet[i + next].find("<!--") != NOT_FOUND)
                        {
                            ++next;

                            while (true)
                            {
                                bool isCondition;

                                if (invert) { isCondition = marker[i + next].isConditionOri; }
                                else
                                {
                                    isCondition = marker[i + next].isCondition;
                                }

                                if (!isCondition)
                                {
                                    if (animDataSet[i + next].find("<!-- CLOSE -->") != NOT_FOUND) { ++next; }

                                    break;
                                }

                                ++next;
                            }
                        }

                        if (isOnlyNumber(animDataSet[i + next]))
                        {
                            ++next;

                            if (animDataSet[i + next].find("<!--") != NOT_FOUND)
                            {
                                ++next;

                                while (true)
                                {
                                    bool isCondition;

                                    if (invert) { isCondition = marker[i + next].isConditionOri; }
                                    else
                                    {
                                        isCondition = marker[i + next].isCondition;
                                    }

                                    if (!isCondition)
                                    {
                                        if (animDataSet[i + next].find("<!-- CLOSE -->") != NOT_FOUND)
                                        { ++next; }

                                        break;
                                    }

                                    ++next;
                                }
                            }

                            if (isOnlyNumber(animDataSet[i + next]))
                            {
                                if (invert)
                                {
                                    if (animDataSet[i + next] == "7891816")
                                    {
                                        id = 11;
                                        break;
                                    }
                                }

                                ++next;

                                if (animDataSet[i + next].find("<!--") != NOT_FOUND)
                                {
                                    ++next;

                                    while (true)
                                    {
                                        bool isCondition;

                                        if (invert) { isCondition = marker[i + next].isConditionOri; }
                                        else
                                        {
                                            isCondition = marker[i + next].isCondition;
                                        }

                                        if (!isCondition)
                                        {
                                            if (animDataSet[i + next].find("<!-- CLOSE -->") != NOT_FOUND)
                                            { ++next; }

                                            break;
                                        }

                                        ++next;
                                    }
                                }

                                if (isOnlyNumber(animDataSet[i + next]) && animDataSet[i + next] == "7891816")
                                {
                                    id = 11;
                                    break;
                                }
                            }
                        }
                    }
                }

                if (marker[i].isCondition) // new condition
                {
                    if (conditionOpen < marker[i].conditionOpen)
                    {
                        int exit = PositionLineCondition(
                            i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

                        if (exit == -1 && !last) { return -1; }
                    }
                    else if (conditionOpen > marker[i].conditionOpen)
                    {
                        --i;
                        return 0;
                    }
                    else
                    {
                        if (id == 10)
                        {
                            if (hasAlpha(animDataSet[i]))
                            {
                                int next = i + 1;

                                while (true)
                                {
                                    if (marker[next].skip) { ++next; }
                                    else
                                    {
                                        break;
                                    }
                                }

                                if (isOnlyNumber(animDataSet[next])) { id = 7; }
                            }
                            else
                            {
                                if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                return -1;
                            }
                        }
                        else if (id == 7)
                        {
                            if (isOnlyNumber(animDataSet[i])) { id = 8; }
                            else
                            {
                                if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                return -1;
                            }
                        }
                        else if (id == 8)
                        {
                            if (isOnlyNumber(animDataSet[i])) { id = 9; }
                            else
                            {
                                if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                return -1;
                            }
                        }
                        else if (id == 9)
                        {
                            if (hasAlpha(animDataSet[i])) { id = 10; }
                            else
                            {
                                if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                return -1;
                            }
                        }
                        else
                        {
                            if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                            return -1;
                        }
                    }
                }
                else
                {
                    if (marker[i].isConditionOri
                        && marker[i].conditionOpen != conditionOpen + 1) // original condition
                    {
                        --i;
                        return 0;
                    }
                    else
                    {
                        if (!last)
                        {
                            --i;
                            return -1;
                        }
                        else
                        {
                            if (id == 10)
                            {
                                if (hasAlpha(animDataSet[i]))
                                {
                                    int next = i + 1;

                                    while (true)
                                    {
                                        if (marker[next].skip) { ++next; }
                                        else
                                        {
                                            break;
                                        }
                                    }

                                    if (isOnlyNumber(animDataSet[next])) { id = 7; }
                                }
                                else
                                {
                                    if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                    return -1;
                                }
                            }
                            else if (id == 7)
                            {
                                if (isOnlyNumber(animDataSet[i])) { id = 8; }
                                else
                                {
                                    if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                    return -1;
                                }
                            }
                            else if (id == 8)
                            {
                                if (isOnlyNumber(animDataSet[i])) { id = 9; }
                                else
                                {
                                    if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                    return -1;
                                }
                            }
                            else if (id == 9)
                            {
                                if (hasAlpha(animDataSet[i])) { id = 10; }
                                else
                                {
                                    if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                    return -1;
                                }
                            }
                            else
                            {
                                if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                return -1;
                            }
                        }
                    }
                }

                if (marker[i].isNew)
                {
                    if (id != 7 && id != 8 && id != 9 && id != 10)
                    {
                        if (!muteError) { ErrorMessage(5007, modcode, header, linecount); }

                        return -1;
                    }
                }
            }
            else if (marker[i].nextCondition) // next condition or close condition
            {
                id = curID;
            }

            ++i;
        }
    }

    if (id == 11)
    {
        if (isOnlyNumber(animDataSet[i]) && animDataSet[i] == "0")
        {
            if (i == linecount) { throw id; }

            ++id;
        }
        else
        {
            while (i < linecount + 1)
            {
                bool invert = false;

                if (i == linecount && marker[i].isCondition) { invert = true; }

                if (i + 3 < int(animDataSet.size()) && isOnlyNumber(animDataSet[i]))
                {
                    int next = 1;

                    if (animDataSet[i + next].find("<!--") != NOT_FOUND)
                    {
                        ++next;

                        while (true)
                        {
                            bool isCondition;

                            if (invert) { isCondition = marker[i + next].isConditionOri; }
                            else
                            {
                                isCondition = marker[i + next].isCondition;
                            }

                            if (!isCondition)
                            {
                                if (animDataSet[i + next].find("<!-- CLOSE -->") != NOT_FOUND) { ++next; }

                                break;
                            }

                            ++next;
                        }
                    }

                    if (isOnlyNumber(animDataSet[i + next]))
                    {
                        if (invert)
                        {
                            if (animDataSet[i + next] == "7891816")
                            {
                                ++id;
                                break;
                            }
                        }

                        ++next;

                        if (animDataSet[i + next].find("<!--") != NOT_FOUND)
                        {
                            ++next;

                            while (true)
                            {
                                bool isCondition;

                                if (invert) { isCondition = marker[i + next].isConditionOri; }
                                else
                                {
                                    isCondition = marker[i + next].isCondition;
                                }

                                if (!isCondition)
                                {
                                    if (animDataSet[i + next].find("<!-- CLOSE -->") != NOT_FOUND) { ++next; }

                                    break;
                                }

                                ++next;
                            }
                        }

                        if (animDataSet[i + next] == "7891816")
                        {
                            jump = true;
                            ++id;
                            break;
                        }
                    }
                }

                if (!marker[i].skip)
                {
                    if (marker[i].isCondition) // new condition
                    {
                        if (conditionOpen < marker[i].conditionOpen)
                        {
                            int exit = PositionLineCondition(
                                i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

                            if (exit == -1 && !last) { return -1; }
                        }
                        else if (conditionOpen > marker[i].conditionOpen)
                        {
                            --i;
                            return 0;
                        }
                        else
                        {
                            if (id != 11)
                            {
                                if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                return -1;
                            }
                        }
                    }
                    else
                    {
                        if (marker[i].isConditionOri
                            && marker[i].conditionOpen != conditionOpen + 1) // original condition
                        {
                            --i;
                            return 0;
                        }
                        else
                        {
                            if (!last)
                            {
                                --i;
                                return -1;
                            }
                            else
                            {
                                if (id != 11)
                                {
                                    if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

                                    return -1;
                                }
                            }
                        }
                    }

                    if (marker[i].isNew)
                    {
                        if (!muteError) { ErrorMessage(5007, modcode, header, linecount); }

                        return -1;
                    }
                }
                else if (marker[i].nextCondition) // next condition or close condition
                {
                    id = curID;
                }

                ++i;
            }
        }
    }

    if (id == 12)
    {
        if (!isOnlyNumber(animDataSet[i]))
        {
            if (!muteError) { ErrorMessage(5007, modcode, header, i + 1); }

            return -1;
        }
    }

    if (id < 15) { throw id; }

    return -2;
}

void combineExtraction(VecStr& storeline, map<int, VecStr> extract, string project, string header)
{
    VecStr newline;
    bool newOpen  = false;
    int condition = 0;

    for (uint i = 0; i < storeline.size(); ++i)
    {
        string line = storeline[i];

        if (line.find("<!-- CONDITION START ^", 0) != NOT_FOUND) { condition++; }
        else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ -->", 0) != NOT_FOUND
                 || line.find("<!-- FOREACH ^", 0) != NOT_FOUND && line.find("^ -->", 0) != NOT_FOUND
                 || line.find("<!-- NEW ORDER ", 0) != NOT_FOUND && line.find(" -->", 0) != NOT_FOUND)
        {
            newOpen = true;
        }

        if (condition == 0 && !newOpen) { newline.push_back(storeline[i]); }

        if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND) { newOpen = false; }
        else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
        {
            condition--;
        }

        if (extract[i].size() > 0)
        {
            if (condition == 0 && !newOpen) { ErrorMessage(5013, project, header); }

            for (uint k = 0; k < extract[i].size(); ++k)
            {
                newline.push_back(extract[i][k]);
            }
        }
    }

    storeline = newline;
}
