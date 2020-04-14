#include "debugmsg.h"

#include "generate/AnimationUtility.h"

using namespace std;

AnimationUtility::AnimationUtility(string condition, ID neweventid, ID newvariableid, vector<int> newFixedStateID, vector<int> newStateCountMultiplier, bool newHasGroup,
	int newOptionMulti, int newAnimMulti, string newMultiOption)
{
	originalCondition = condition;
	eventid = neweventid;
	variableid = newvariableid;
	fixedStateID = newFixedStateID;
	stateCountMultiplier = newStateCountMultiplier;
	hasGroup = newHasGroup;
	optionMulti = newOptionMulti;
	animMulti = newAnimMulti;
	multiOption = newMultiOption;
}

NewStateID::NewStateID()
{
    lastState.push_back(make_shared<int>(0));
}

void NewStateID::push_back(std::shared_ptr<int> num)
{
	lastState.push_back(num);
}

void NewStateID::reset()
{
	lastState = { make_shared<int>(0) };
}

shared_ptr<int>& NewStateID::operator[](uint number)
{
	return lastState[number];
}

unsigned int NewStateID::size()
{
	return lastState.size();
}

bool NewStateID::stateUpdate(int ID, const string& format, const string& behaviorFile, int linecount, const string& state, bool hasGroup)
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
			ErrorMessage(1168, format, behaviorFile, linecount, state);
		}
	}

	if (stoi(state) >= *lastState[ID])
	{
		*lastState[ID] = stoi(state) + 1;
	}

	return true;
}
