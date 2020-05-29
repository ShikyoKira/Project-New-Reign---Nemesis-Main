#include <array>

#include "Global.h"
#include "debuglog.h"
#include "nemesisinfo.h"

#include "utilities/filechecker.h"

using namespace std;

void processFileError(pair<string_view, int> err);

void processFileError(pair<string_view, int> err)
{
    if (!isFileExist(err.first)) ErrorMessage(err.second, err.first);
}

bool FileCheck(bool isUpdate)
{
    DebugLogging("Initializing file check...");
    using fileError = pair<string_view, int>;

    constexpr std::array<fileError, 7> filesToCheck
        = {fileError{"alternate animation", 1001},
           {"alternate animation\\alternate animation.script", 1092},
           {"alternate animation\\alternate animation 2.script", 1092},
           {"behavior templates", 1001},
           {"hkxcmd.exe", 1092},
           {"languages", 1001},
           {"languages\\english.txt", 1092}};

    constexpr std::array<fileError, 1> filesToNotCheckInUpdate
        = {fileError{"cache\\animationdata_list", 1092}};

    for (const auto& pair : filesToCheck)
    {
        processFileError(pair);
    }

    CreateFolder("mod");

    if (!isUpdate)
    {
        for (const auto& pair : filesToNotCheckInUpdate)
        {
            processFileError(pair);
        }
    }

    DebugLogging("File Check complete");
    return true;
}

bool PCEACheck(const NemesisInfo* nemesisInfo)
{
    DebugLogging("Initializing PCEA Check...");
    wstring file = nemesisInfo->GetDataPath() + L"Nemesis PCEA.esp";

    if (isFileExist(file))
    {
        file = L"alternate animation\\nemesis pcea.script";

        if (!isFileExist(file)) ErrorMessage(1092, file);

        file = nemesisInfo->GetDataPath() + L"scripts\\Nemesis_PCEA_MCM.pex";

        if (!isFileExist(file)) ErrorMessage(1092, file);
    }
    else
    {
        return false;
    }

    DebugLogging("PCEA Check complete");
    return true;
}

void behaviorActivateMod(VecStr behaviorPriority)
{
    unordered_map<string, VecStr>
        behaviorActivator; // modcode, behavior; existence of the behavior in any of these
    string directory = "mod\\";
    VecStr modlist;
    uint i = 1;

    for (auto itr = behaviorPriority.rbegin(); itr != behaviorPriority.rend(); ++itr)
    {
        string modcode = *itr;
        string newpath = directory + modcode;
        DebugLogging("Mod Checked " + to_string(i) + ": " + modcode);
        interMsg(TextBoxMessage(1013) + L" " + to_wstring(i++) + L": " + nemesis::transform_to<wstring>(modcode));

        if (!isFileExist(newpath) || !std::filesystem::is_directory(newpath)) continue;

        if (modcode == "gender")
        {
            activatedBehavior["gender*"] = true;
            continue;
        }

        VecStr behaviorlist;
        read_directory(newpath, behaviorlist);

        for (auto& behavior : behaviorlist)
        {
            if (!nemesis::iequals(behavior, "info.ini") && !nemesis::iequals(behavior, "_1stperson"))
            { 
                activatedBehavior[nemesis::to_lower_copy(behavior)] = true;
            }
            else if (nemesis::iequals(behavior, "_1stperson"))
            {
                VecStr fpbehaviorlist;
                read_directory(newpath + "\\" + behavior, fpbehaviorlist);

                for (auto& fpbehavior : fpbehaviorlist)
                {
                    activatedBehavior["_1stperson\\" + nemesis::to_lower_copy(fpbehavior)] = true;
                }
            }
        }
    }

    if (!behaviorPriority.empty()) interMsg("");
}
