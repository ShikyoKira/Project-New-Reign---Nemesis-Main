#include "Global.h"

#include "generate/animationdata.h"
#include "generate/animationdatatracker.h"

#pragma warning(disable : 4503)

using namespace std;

struct InfoDataTracker
{
    string data;
    uint index;

    InfoDataTracker(string d, uint i)
        : data(d)
        , index(i)
    {}
};

bool IDExistProcess(string change,
                    vector<InfoDataPack>& storeline,
                    map<string, vector<InfoDataTracker>> original,
                    map<string, bool>& isExist,
                    map<string, string>& exchange,
                    vector<AnimDataPack>& animDataPack,
                    map<string, vector<int>>& codeTracker,
                    map<string, bool>& loopCheck);
void BehaviorListProcess(
    AnimDataProject& storeline, int& startline, VecStr& animdatafile, string project, string modcode);
void AnimDataProcess(vector<AnimDataPack>& storeline,
                     int& startline,
                     VecStr& animdatafile,
                     string project,
                     string modcode,
                     map<string, string>& exchange,
                     map<string, vector<shared_ptr<AnimationDataTracker>>>& animDataTracker,
                     map<string, vector<int>>& codeTracker);
void InfoDataProcess(vector<InfoDataPack>& storeline,
                     int& startline,
                     VecStr& animdatafile,
                     string project,
                     string modcode,
                     map<string, string>& exchange,
                     vector<AnimDataPack>& animDataPack,
                     map<string, vector<int>>& codeTracker);

bool IDExistProcess(string change,
                    vector<InfoDataPack>& storeline,
                    map<string, vector<InfoDataTracker>> original,
                    map<string, bool>& isExist,
                    map<string, string>& exchange,
                    vector<AnimDataPack>& animDataPack,
                    map<string, vector<int>>& codeTracker,
                    map<string, bool>& loopCheck)
{
    const auto& ori = original.find(change);

    if (ori == original.end()) return false;

    if (loopCheck[change]) return true;

    loopCheck[change] = true;

    for (auto& each : ori->second)
    {
        auto curTrack = codeTracker.find(each.data + "-" + change);

        if (curTrack == codeTracker.end() || curTrack->second.size() == 0) throw false;

        for (auto track : curTrack->second)
        {
            animDataPack[track].uniquecode = each.data;
        }

        exchange.erase(each.data);

        if (isExist[each.data]
            && !IDExistProcess(
                each.data, storeline, original, isExist, exchange, animDataPack, codeTracker, loopCheck))
        { return false; }

        storeline[each.index].uniquecode = each.data;
        isExist[each.data]               = true;
    }

    return true;
}

AnimDataProject::AnimDataProject(VecStr animdatafile, string project, string filepath, string modcode)
{
    int startline = 0;
    BehaviorListProcess(*this, startline, animdatafile, project, modcode);

    if (error) throw nemesis::exception();

    if (startline >= int(animdatafile.size())) return;

    map<string, string> exchange;
    map<string, vector<int>> codeTracker;
    string characterFile = nemesis::to_lower_copy(
        std::filesystem::path(behaviorlist[behaviorlist.size() - 2]).stem().string());
    AnimDataProcess(animdatalist,
                    startline,
                    animdatafile,
                    project,
                    modcode,
                    exchange,
                    clipPtrAnimData[characterFile],
                    codeTracker);

    if (error) throw nemesis::exception();

    if (startline >= int(animdatafile.size())) return;

    InfoDataProcess(
        infodatalist, startline, animdatafile, project, modcode, exchange, animdatalist, codeTracker);
}

void BehaviorListProcess(
    AnimDataProject& storeline, int& startline, VecStr& animdatafile, string project, string modcode)
{
    if (!isOnlyNumber(animdatafile[startline])) ErrorMessage(3005, project, "Header");

    int i = startline + 1;

    if (i + 3 > int(animdatafile.size())) ErrorMessage(3021, project);

    storeline.projectActive = animdatafile[i++];
    ++i;

    while (!isOnlyNumber(animdatafile[i]))
    {
        if (hasAlpha(animdatafile[i]))
            storeline.behaviorlist.push_back(animdatafile[i++]);
        else
            ErrorMessage(3005, project, "Header");
    }

    storeline.childActive = animdatafile[i++];
    startline             = i;

    if (i < int(animdatafile.size()) && !hasAlpha(animdatafile[i])) ErrorMessage(3005, project, "Header");

    if (error) throw nemesis::exception();
}

void AnimDataProcess(vector<AnimDataPack>& storeline,
                     int& startline,
                     VecStr& animdatafile,
                     string project,
                     string modcode,
                     map<string, string>& exchange,
                     map<string, vector<shared_ptr<AnimationDataTracker>>>& animDataTracker,
                     map<string, vector<int>>& codeTracker)
{
    unordered_map<string, unsigned long> tracker;
    unordered_map<string, bool> isExist;

    for (int i = startline; i < int(animdatafile.size()); ++i)
    {
        bool skip = false;

        if (!hasAlpha(animdatafile[i])) ErrorMessage(3005, project, "Unknown");

        string name = animdatafile[i++];

        if (!isOnlyNumber(animdatafile[i])) ErrorMessage(3005, project, name);

        auto animDataInfo = animDataTracker.find(name);
        string uniquecode = animdatafile[i++];

        if (animDataInfo != animDataTracker.end())
        {
            int first = -1;

            for (uint i = 0; i < animDataInfo->second.size(); ++i)
            {
                if (!(tracker[name] & (1 << i)))
                {
                    if (to_string(animDataInfo->second[i]->GetOrder()) == uniquecode)
                    {
                        first = i;
                        break;
                    }
                    else if (first == -1)
                    {
                        first = i;
                    }
                }
            }

            if (first != -1)
            {
                string change = to_string(animDataInfo->second[first]->GetOrder());
                tracker[name] |= 1 << first;

                if (change == "-1") { skip = true; }
                else
                {
                    exchange[uniquecode] = change;
                    codeTracker[uniquecode + "-" + change].push_back(storeline.size());
                    uniquecode = change;
                }
            }
        }

        if (!isExist[name])
            isExist[name] = true;
        else
            WarningMessage(1028, project, name);

        AnimDataPack curAP;
        curAP.name       = name;
        curAP.uniquecode = uniquecode;

        if (!isOnlyNumber(animdatafile[i])) ErrorMessage(3005, project, name);

        curAP.unknown1 = animdatafile[i++];

        if (!isOnlyNumber(animdatafile[i])) ErrorMessage(3005, project, name);

        curAP.unknown2 = animdatafile[i++];

        if (!isOnlyNumber(animdatafile[i])) ErrorMessage(3005, project, name);

        curAP.unknown3 = animdatafile[i++];

        if (!isOnlyNumber(animdatafile[i++])) ErrorMessage(3005, project, name);

        if (animdatafile[i].length() != 0 && hasAlpha(animdatafile[i]))
        {
            while (animdatafile[i].length() != 0)
            {
                curAP.eventname.push_back(animdatafile[i++]);
            }
        }

        if (animdatafile[i++].length() != 0) ErrorMessage(3005, project, name);

        if (!skip) storeline.push_back(curAP);

        if (error) throw nemesis::exception();

        if (isOnlyNumber(animdatafile[i]))
        {
            startline = i;
            break;
        }
        else
        {
            --i;
        }
    }
}

void InfoDataProcess(vector<InfoDataPack>& storeline,
                     int& startline,
                     VecStr& animdatafile,
                     string project,
                     string modcode,
                     map<string, string>& exchange,
                     vector<AnimDataPack>& animDataPack,
                     map<string, vector<int>>& codeTracker)
{
    map<string, bool> isExist;
    map<string, size_t> locate;
    map<string, vector<InfoDataTracker>> original;

    for (int i = startline; i < int(animdatafile.size()); ++i)
    {
        if (!isOnlyNumber(animdatafile[i])) ErrorMessage(3020, project, animdatafile[i]);

        InfoDataPack curIP;
        string uniquecode  = animdatafile[i++];
        locate[uniquecode] = i - 1;
        const auto& exch   = exchange.find(uniquecode);

        if (exch != exchange.end())
        {
            uniquecode = exch->second;
            original[uniquecode].push_back(InfoDataTracker(exch->first, storeline.size()));
        }

        if (isExist[uniquecode])
        {
            string change = animdatafile[i - 1];

            try
            {
                if (isExist[change])
                {
                    if (change != uniquecode) throw false;

                    map<string, bool> loopCheck;

                    if (!IDExistProcess(change,
                                        storeline,
                                        original,
                                        isExist,
                                        exchange,
                                        animDataPack,
                                        codeTracker,
                                        loopCheck))
                        throw false;

                    uniquecode          = change;
                    isExist[uniquecode] = false;
                }
                else
                {
                    auto curTrack = codeTracker.find(change + "-" + uniquecode);

                    if (curTrack == codeTracker.end() || curTrack->second.size() == 0) throw false;

                    for (auto track : curTrack->second)
                    {
                        animDataPack[track].uniquecode = change;
                    }

                    exchange.erase(uniquecode);

                    for (uint i = 0; i < original[uniquecode].size(); ++i)
                    {
                        if (original[uniquecode][i].data == change)
                        {
                            original[uniquecode].erase(original[uniquecode].begin() + i);
                            --i;
                        }
                    }

                    uniquecode = change;
                }
            }
            catch (bool)
            {
                ErrorMessage(3012, project, uniquecode);
            }
        }

        if (isExist[uniquecode]) ErrorMessage(3012, project, uniquecode);

        isExist[uniquecode] = true;
        curIP.uniquecode    = uniquecode;

        if (!isOnlyNumber(animdatafile[i])) ErrorMessage(3020, project, uniquecode);

        curIP.duration = animdatafile[i];

        if (!isOnlyNumber(animdatafile[++i])) ErrorMessage(3020, project, uniquecode);

        ++i;

        while (count(animdatafile[i].begin(), animdatafile[i].end(), ' ') != 0)
        {
            curIP.motiondata.push_back(animdatafile[i++]);
        }

        if (!isOnlyNumber(animdatafile[i++])) ErrorMessage(3020, project, uniquecode);

        while (count(animdatafile[i].begin(), animdatafile[i].end(), ' ') != 0)
        {
            curIP.rotationdata.push_back(animdatafile[i++]);
        }

        if (animdatafile[i].length() != 0) ErrorMessage(3020, project, uniquecode);

        if (error) throw nemesis::exception();

        storeline.push_back(curIP);
    }
}

int AnimDataProject::GetAnimTotalLine()
{
    int counter = 3 + int(behaviorlist.size());

    for (auto it : animdatalist)
    {
        counter += 7 + it.eventname.size();
    }

    return counter;
}

int AnimDataProject::GetInfoTotalLine()
{
    int counter = 0;

    for (auto it : infodatalist)
    {
        counter += 5 + int(it.motiondata.size()) + int(it.rotationdata.size());
    }

    return counter;
}

AnimDataFormat::position AnimDataPosition(VecStr animData,
                                          string character,
                                          string header,
                                          string modcode,
                                          string filepath,
                                          int linecount,
                                          int type,
                                          bool muteError)
{
    // has function
    bool isOpen       = false;
    int functionstart = -1;
    int conditionOpen = 0;
    unordered_map<int, bool> isCondition;
    unordered_map<int, bool> isConditionOri;
    unordered_map<int, AnimDataFunct> marker;

    for (uint i = 0; i < animData.size(); ++i)
    {
        if (animData[i].find("<!-- ") != NOT_FOUND)
        {
            if (functionstart == -1) functionstart = i;

            if (animData[i].find("<!-- original -->") == NOT_FOUND) marker[i].skip = true;

            if (animData[i].find("<!-- CLOSE -->") != NOT_FOUND) { isOpen = false; }
            else if (animData[i].find("<!-- CONDITION END -->") != NOT_FOUND)
            {
                isCondition[conditionOpen]    = false;
                isConditionOri[conditionOpen] = false;
                --conditionOpen;
            }
        }

        if (isOpen) marker[i].isNew = true;

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
            if (animData[i].find("<!-- NEW") != NOT_FOUND) { isOpen = true; }
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
        }
    }

    using namespace AnimDataFormat;

    if (linecount < int(animData.size()))
    {
        if (type == 0)
        {
            if (linecount < 3 && functionstart == 0)
            {
                return AnimDataConvert(type, linecount + 1, muteError);
                // 1. total line count
                // 2. unknown number
                // 3. behavior file count
            }
            else
            {
                if (animData[linecount].find("<!-- ") != NOT_FOUND)
                {
                    if (!muteError) ErrorMessage(3007, modcode, filepath, linecount, header);
                }

                int id = 0;
                unordered_map<int, int> subid;

                for (int i = 0; i < linecount + 1; ++i)
                {
                    if (!marker[i].skip)
                    {
                        if (marker[i].isCondition) { ++subid[marker[i].conditionOpen]; }
                        else
                        {
                            if (marker[i].conditionOpen > 1)
                            {
                                subid[marker[i].conditionOpen] = 0;
                                ++subid[marker[i].conditionOpen - 1];
                            }
                            else
                            {
                                subid[1] = 0;
                                ++id;

                                if (id > 3) break;
                            }
                        }

                        if (marker[i].isNew)
                        {
                            int tempid = id;

                            for (auto it = subid.begin(); it != subid.end(); ++it)
                            {
                                tempid = tempid + it->second;
                            }

                            if (tempid != 4)
                            {
                                if (!muteError) ErrorMessage(3007, modcode, filepath, linecount, header);
                            }
                        }
                    }
                    else if (marker[i].nextCondition)
                    {
                        subid[marker[i].conditionOpen] = 0;
                    }
                }

                for (auto it = subid.begin(); it != subid.end(); ++it)
                {
                    id = id + it->second;
                }

                if (id < 4) { return AnimDataConvert(type, id, muteError); }
            }

            if (isOnlyNumber(animData[linecount]))
            {
                return unknown5;
                // 5. unknown number
            }
            else
            {
                return behaviorfilelist;
                // 4. behavior file list
            }
        }
        else if (type == 1)
        {
            if (linecount < 6 && functionstart > 5)
            {
                return AnimDataConvert(type, linecount + 1, muteError);
                // 1. animation name (clip)
                // 2. unique code
                // 3. unknown number (1)
                // 4. unknown number (0)
                // 5. unknown number (0)
                // 6. event name count
            }
            else
            {
                if (animData[linecount].find("<!-- ") != NOT_FOUND)
                {
                    if (!muteError) ErrorMessage(3007, modcode, filepath, linecount, header);
                }

                int id = 0;
                unordered_map<int, int> subid;

                for (int i = 0; i < linecount + 1; ++i)
                {
                    if (!marker[i].skip)
                    {
                        if (marker[i].isCondition) { ++subid[marker[i].conditionOpen]; }
                        else
                        {
                            if (marker[i].conditionOpen > 1)
                            {
                                subid[marker[i].conditionOpen] = 0;
                                ++subid[marker[i].conditionOpen - 1];
                            }
                            else
                            {
                                subid[1] = 0;
                                ++id;

                                if (id > 6) { break; }
                            }
                        }

                        if (marker[i].isNew)
                        {
                            int tempid = id;

                            for (auto it = subid.begin(); it != subid.end(); ++it)
                            {
                                tempid = tempid + it->second;
                            }

                            if (tempid != 7) ErrorMessage(3007, modcode, filepath, linecount, header);
                        }
                    }
                    else if (marker[i].nextCondition)
                    {
                        subid[marker[i].conditionOpen] = 0;
                    }
                }

                for (auto it = subid.begin(); it != subid.end(); ++it)
                {
                    id = id + it->second;
                }

                if (id < 7) { return AnimDataConvert(type, id, muteError); }
            }

            if (linecount == int(animData.size()) - 1)
            {
                return space;
                // 8. <space>
            }
            else if (hasAlpha(animData[linecount]))
            {
                return eventnamelist;
                // 7. event name list
            }
        }
        else if (type == 2)
        {
            int nextline = 3;

            if (linecount < 3 && functionstart > 2)
            {
                return AnimDataConvert(type, linecount + 1, muteError);
                // 1. unique code
                // 2. total animation (clip) length
                // 3. motion data count
            }
            else
            {
                if (animData[linecount].find("<!-- ") != NOT_FOUND)
                {
                    if (!muteError) ErrorMessage(3007, modcode, filepath, linecount, header);
                }

                int id = 0;
                unordered_map<int, int> subid;

                for (int i = 0; i < linecount + 1; ++i)
                {
                    if (!marker[i].skip)
                    {
                        if (marker[i].isCondition) { ++subid[marker[i].conditionOpen]; }
                        else
                        {
                            if (marker[i].conditionOpen > 1)
                            {
                                subid[marker[i].conditionOpen] = 0;
                                ++subid[marker[i].conditionOpen - 1];
                            }
                            else
                            {
                                subid[1] = 0;
                                ++id;

                                if (id > 3)
                                {
                                    nextline = i;
                                    break;
                                }
                            }
                        }

                        if (marker[i].isNew)
                        {
                            int tempid = id;

                            for (auto it = subid.begin(); it != subid.end(); ++it)
                            {
                                tempid = tempid + it->second;
                            }

                            if (tempid != 4 && tempid != 6)
                            {
                                if (!muteError) ErrorMessage(3007, modcode, filepath, linecount, header);
                            }
                        }
                    }
                    else if (marker[i].nextCondition)
                    {
                        subid[marker[i].conditionOpen] = 0;
                    }
                }

                for (auto it = subid.begin(); it != subid.end(); ++it)
                {
                    id = id + it->second;
                }

                if (id < 4) { return AnimDataConvert(type, id, muteError); }
            }

            if (linecount == int(animData.size()) - 1)
            {
                return space;
                // 7. <space>
            }

            int id        = 4;
            bool nextplus = false;
            unordered_map<int, int> subid;

            for (int i = nextline; i < linecount + 1; ++i)
            {
                if (!marker[i].skip)
                {
                    if (marker[i].isCondition)
                    {
                        if (isOnlyNumber(animData[i])
                            && count(animData[i].begin(), animData[i].end(), ' ') == 0)
                        {
                            ++subid[marker[i].conditionOpen];

                            if (marker.find(i + 1) != marker.end())
                            {
                                if (marker[i + 1].conditionOpen == marker[i].conditionOpen) nextplus = true;
                            }
                        }
                        else if (nextplus)
                        {
                            ++subid[marker[i].conditionOpen];
                            nextplus = false;
                        }
                    }
                    else
                    {
                        if (marker[i].conditionOpen > 1)
                        {
                            if (isOnlyNumber(animData[i])
                                && count(animData[i].begin(), animData[i].end(), ' ') == 0)
                            {
                                subid[marker[i].conditionOpen] = 0;
                                ++subid[marker[i].conditionOpen - 1];

                                if (marker.find(i + 1) != marker.end())
                                {
                                    if (marker[i + 1].conditionOpen == marker[i].conditionOpen)
                                        nextplus = true;
                                }
                            }
                            else if (nextplus)
                            {
                                subid[marker[i].conditionOpen] = 0;
                                ++subid[marker[i].conditionOpen - 1];
                                nextplus = false;
                            }
                        }
                        else
                        {
                            if (isOnlyNumber(animData[i])
                                && count(animData[i].begin(), animData[i].end(), ' ') == 0)
                            {
                                subid[1] = 0;
                                ++id;

                                if (marker.find(i + 1) == marker.end()) nextplus = true;
                            }
                            else if (nextplus)
                            {
                                subid[1] = 0;
                                ++id;
                                nextplus = false;
                            }
                        }
                    }

                    if (marker[i].isNew)
                    {
                        int tempid = id;

                        for (auto it = subid.begin(); it != subid.end(); ++it)
                        {
                            tempid = tempid + it->second;
                        }

                        if (tempid != 4 && tempid != 6)
                            ErrorMessage(3007, modcode, filepath, linecount, header);
                    }
                }
                else if (marker[i].nextCondition)
                {
                    subid[marker[i].conditionOpen] = 0;
                }
            }

            for (auto it = subid.begin(); it != subid.end(); ++it)
            {
                id = id + it->second;
            }

            if (id < 7 && id > 3)
            {
                return AnimDataConvert(type, id, muteError);
                // 4. motion data list
                // 5. rotation data count
                // 6. rotation data list
            }
        }
    }

    if (!muteError) ErrorMessage(3005, character, header);

    return xerror;
}

AnimDataFormat::position AnimDataConvert(int type, int position, bool muteError)
{
    using namespace AnimDataFormat;

    if (type == 0)
    {
        if (position == 1) { return totallinecount; }
        else if (position == 2)
        {
            return unknown2;
        }
        else if (position == 3)
        {
            return behaviorfilecount;
        }
        else if (position == 4)
        {
            return behaviorfilelist;
        }
        else if (position == 5)
        {
            return unknown5;
        }
    }
    else if (type == 1)
    {
        if (position == 1) { return animationname; }
        else if (position == 2)
        {
            return uniquecode;
        }
        else if (position == 3)
        {
            return unknown3;
        }
        else if (position == 4)
        {
            return unknown4;
        }
        else if (position == 5)
        {
            return unknown5;
        }
        else if (position == 6)
        {
            return eventnamecount;
        }
        else if (position == 7)
        {
            return eventnamelist;
        }
        else if (position == 8)
        {
            return space;
        }
    }
    else
    {
        if (position == 1) { return uniquecode; }
        else if (position == 2)
        {
            return totalcliplength;
        }
        else if (position == 3)
        {
            return motiondatacount;
        }
        else if (position == 4)
        {
            return motiondatalist;
        }
        else if (position == 5)
        {
            return rotationdatacount;
        }
        else if (position == 6)
        {
            return rotationdatalist;
        }
        else if (position == 7)
        {
            return space;
        }
    }

    return xerror;
}
