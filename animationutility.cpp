#include "animationutility.h"

using namespace std;

animationutility::animationutility(id neweventid, id newvariableid, vector<int> newFixedStateID, std::vector<int> newStateCountMultiplier, bool newHasGroup, int newOptionMulti, int newAnimMulti, string newMultiOption)
{
	eventid = neweventid;
	variableid = newvariableid;
	fixedStateID = newFixedStateID;
	stateCountMultiplier = newStateCountMultiplier;
	hasGroup = newHasGroup;
	optionMulti = newOptionMulti;
	animMulti = newAnimMulti;
	multiOption = newMultiOption;
}

newStateID::newStateID()
{
	lastState = { make_shared<int>(0) };
}

void newStateID::push_back(std::shared_ptr<int> num)
{
	lastState.push_back(num);
}

void newStateID::reset()
{
	lastState = { make_shared<int>(0) };
}

std::shared_ptr<int>& newStateID::operator[](unsigned int number)
{
	return lastState[number];
}

unsigned int newStateID::size()
{
	return lastState.size();
}

bool newStateID::stateUpdate(int ID, string format, string behaviorFile, int linecount, string state, bool hasGroup)
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
			return false;
		}
	}

	if (stoi(state) >= *lastState[ID])
	{
		*lastState[ID] = stoi(state) + 1;
	}

	return true;
}
