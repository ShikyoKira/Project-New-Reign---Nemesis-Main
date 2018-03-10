#ifndef KILLMOVE_H_
#define KILLMOVE_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <boost\regex.hpp>

typedef std::vector<std::string> vecstr;
typedef std::unordered_map<std::string, std::string> AOConvert;
typedef std::unordered_map<std::string, int> id;

class KillMove
{
private:
	vecstr killmovelines;

	vecstr eventID;
	vecstr variableID;
	std::string nextState;
	std::unordered_map<int, std::string> AObject;
	AOConvert NewAnimObject;
	int nextFunctionID;
	int blend = 0;
	double duration;
	bool AC = false;
	bool AC0 = false;
	bool AC1 = false;
	bool Loop = true;
	bool AO = false;
	bool PAO = false;
	bool HT = false;
	bool Known = false;
	bool MD = false;
	bool nextTrans = false;
	AOConvert TriggerTime;
	AOConvert TriggerTime1;

	bool error = false;

public:
	KillMove(std::string line, vecstr killmoveformat, int killmovecount, AOConvert import, std::string filepath, int stateID);
	vecstr GetKillMoveLine(int ID, AOConvert AnimObjectNames, id eventid, id variableid);
	AOConvert GetAnimObjectName();
	vecstr GetEventID();
	vecstr GetVariableID();
	std::string mainAnimEvent;

};

#endif