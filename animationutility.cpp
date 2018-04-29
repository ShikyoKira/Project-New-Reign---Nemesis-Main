#include "animationutility.h"

using namespace std;

animationutility::animationutility(id neweventid, id newvariableid, vector<int> newFixedStateID, int newStateCountMultiplier, bool newHasGroup, int newOptionMulti, int newAnimMulti, string newMultiOption)
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