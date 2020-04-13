#include "debugmsg.h"

#include "generate/AnimationUtility.h"

using namespace std;

AnimationUtility::AnimationUtility(string condition, id neweventid, id newvariableid, vector<int> newFixedStateID, vector<int> newStateCountMultiplier, bool newHasGroup,
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
	lastState = { make_shared<int>(0) };
}

void NewStateID::push_back(std::shared_ptr<int> num)
{
	lastState.push_back(num);
}

void NewStateID::reset()
{
	lastState = { make_shared<int>(0) };
}

shared_ptr<int>& NewStateID::operator[](unsigned int number)
{
	return lastState[number];
}

unsigned int NewStateID::size()
{
	return lastState.size();
}

bool NewStateID::stateUpdate(int ID, string format, string behaviorFile, int linecount, string state, bool hasGroup)
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
