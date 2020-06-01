#include "Global.h"

#include "generate/behaviorcheck.h"
#include "generate/behaviorprocess.h"
#include "generate/generator_utility.h"

#pragma warning(disable : 4503)

using namespace std;

typedef unordered_set<wstring> USetWstr;

VecWstr warningMsges;

void beginConnectionCheck(const string& current,
                          const string& original,
                          USetStr& noRepeat,
                          const unordered_map<wstring, USetWstr>& postBhvrRefBy,
                          const NemesisInfo& nemesisInfo);

bool connectionCheckLoop(const string& current,
                         const string& original,
                         USetStr& noRepeat,
                         const unordered_map<wstring, USetWstr>& postBhvrRefBy,
                         const VecStr& characterList,
                         const NemesisInfo& nemesisInfo);

bool isConnectedToCharacter(const string& current,
                            const string& original,
                            USetStr& noRepeat,
                            const unordered_map<wstring, USetWstr>& postBhvrRefBy,
                            const VecStr& characterList,
                            const NemesisInfo& nemesisInfo);

void beginConnectionCheck(const string& current,
                          const string& original,
                          USetStr& noRepeat,
                          const unordered_map<wstring, USetWstr>& postBhvrRefBy,
                          const NemesisInfo& nemesisInfo)
{
    string file = GetFileName(original);
    auto bhvitr = behaviorJoints.find(file);

    if (bhvitr == behaviorJoints.end() || bhvitr->second.empty()) return;

    connectionCheckLoop(current, original, noRepeat, postBhvrRefBy, bhvitr->second, nemesisInfo);
}

bool connectionCheckLoop(const string& current,
                         const string& original,
                         USetStr& noRepeat,
                         const unordered_map<wstring, USetWstr>& postBhvrRefBy,
                         const VecStr& characterList,
                         const NemesisInfo& nemesisInfo)
{
    if (noRepeat.find(current) != noRepeat.end()) return true;

    if (!isConnectedToCharacter(current, original, noRepeat, postBhvrRefBy, characterList, nemesisInfo)) return false;

    if (!isFileExist(current))
    {
        // Staging
        if (nemesisInfo.GetStagePath() != nemesisInfo.GetDataPath())
        {
            wstring stage = nemesisInfo.GetStagePath();
            wstring data = nemesisInfo.GetDataPath();
            wstring wcurrent = nemesis::transform_to<wstring>(current);

            if (wordFind(wcurrent, stage) != 0) ErrorMessage(1210, current);

            wcurrent = data + wcurrent.substr(stage.length());

            if (!isFileExist(wcurrent)) ErrorMessage(1210, wcurrent);
        }
        else
        {
            ErrorMessage(1210, current);
        }
    }

    noRepeat.insert(current);
    return true;
}

bool isConnectedToCharacter(const string& current,
                            const string& original,
                            USetStr& noRepeat,
                            const unordered_map<wstring, USetWstr>& postBhvrRefBy,
                            const VecStr& characterList,
                            const NemesisInfo& nemesisInfo)
{
    auto itr = postBhvrRefBy.find(nemesis::transform_to<wstring>(current));

    if (itr != postBhvrRefBy.end())
    {
        bool rst = false;

        for (auto& innerLoop : itr->second)
        {
            if (connectionCheckLoop(nemesis::transform_to<string>(innerLoop),
                                    original,
                                    noRepeat,
                                    postBhvrRefBy,
                                    characterList, nemesisInfo))
            {
                rst = true;
            }
        }

        return rst;
    }

    string chac = GetFileName(current);

    for (auto& each : characterList)
    {
        if (chac == each) return true;
    }

    return false;
}

void behaviorCheck(BehaviorStart* process)
{
    // test if anim file used has been registered
    for (auto& it : usedAnim)
    {
        VecStr forwardPort = behaviorJoints[it.first];

        for (auto& behavior : forwardPort)
        {
            auto& registBhvr = registeredAnim[nemesis::to_lower_copy(behavior)];

            for (auto& path : it.second)
            {
                string lowerfilename = nemesis::to_lower_copy(filesystem::path(path).filename().string());

                if (registBhvr.find(lowerfilename) == registBhvr.end())
                {
                    WarningMessage(1013, behavior + ".hkx", path);
                }
            }
        }
    }

    // test duplicate anim file
    for (auto& animation : animModMatch) // first string
    {
        for (auto duplicatelist : animation.second) // second string
        {
            if (duplicatelist.second.size() == 2)
            {
                if (duplicatelist.second[0].size() > 1)
                {
                    string warning;

                    for (auto& duplicate : duplicatelist.second[1])
                    {
                        warning = warning + duplicate + ", ";
                    }

                    warning.pop_back();
                    warning.pop_back();
                    WarningMessage(1014, animation.first, duplicatelist.first, warning);
                }
            }
            else
            {
                ErrorMessage(1066);
            }
        }
    }

    USetStr noRepeat;

    for (auto& behaviorfiles : process->postBhvrRefBy)
    {
        beginConnectionCheck(nemesis::transform_to<string>(behaviorfiles.first),
                             nemesis::transform_to<string>(behaviorfiles.first),
                             noRepeat,
                             process->postBhvrRefBy, *process->nemesisInfo);
    }
}

void warningCheck()
{
    int warningcount = 0;

    for (auto& warn : warningMsges)
    {
        interMsg(warn);
        ++warningcount;
    }

    interMsg("Warning Count: " + to_string(warningcount));
    interMsg("");
    interMsg(TextBoxMessage(1015));
}
