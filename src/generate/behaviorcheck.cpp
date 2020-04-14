#include "Global.h"

#include "generate/behaviorcheck.h"
#include "generate/generator_utility.h"

#pragma warning(disable : 4503)

using namespace std;

VecStr warningMsges;

void behaviorCheck()
{
    // test if anim file used has been registered
    for (auto& it : usedAnim)
    {
        VecStr forwardPort = behaviorJoints[it.first];

        for (auto& behavior : forwardPort)
        {
            for (auto& path : it.second)
            {
                string filename = GetFileName(path);

                if (!registeredAnim[behavior][filename]) WarningMessage(1013, behavior + ".hkx", path);
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
