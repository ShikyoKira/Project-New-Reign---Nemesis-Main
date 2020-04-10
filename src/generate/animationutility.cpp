#include "Global.h"
#include <utility>

#include "debugmsg.h"

#include "generate/AnimationUtility.h"

using namespace std;

AnimationUtility::AnimationUtility(string condition,
                                   ID neweventid,
                                   ID newvariableid,
                                   vector<int> newFixedStateID,
                                   vector<int> newStateCountMultiplier,
                                   bool newHasGroup,
                                   int newOptionMulti,
                                   int newAnimMulti,
                                   string newMultiOption)
{
    originalCondition    = std::move(condition);
    eventid              = std::move(neweventid);
    variableid           = std::move(newvariableid);
    fixedStateID         = std::move(newFixedStateID);
    stateCountMultiplier = std::move(newStateCountMultiplier);
    hasGroup             = newHasGroup;
    optionMulti          = newOptionMulti;
    animMulti            = newAnimMulti;
    multiOption          = std::move(newMultiOption);
}

newStateID::newStateID()
{
    lastState = {make_shared<int>(0)};
}

void newStateID::push_back(std::shared_ptr<int> num)
{
    lastState.push_back(num);
}

void newStateID::reset()
{
    lastState = {make_shared<int>(0)};
}

shared_ptr<int>& newStateID::operator[](uint number)
{
    return lastState[number];
}

uint newStateID::size()
{
    return lastState.size();
}

bool newStateID::stateUpdate(
    int ID, string format, string behaviorFile, int linecount, const string& state, bool hasGroup)
{
    if (ID >= int(lastState.size()))
    {
        if (hasGroup)
        {
            while (ID >= int(lastState.size()))
            {
                lastState.push_back(make_shared<int>(0));
            }
        }
        else
        {
            ErrorMessage(1168, std::move(format), std::move(behaviorFile), linecount, state);
        }
    }

    if (stoi(state) >= *lastState[ID]) { *lastState[ID] = stoi(state) + 1; }

    return true;
}
