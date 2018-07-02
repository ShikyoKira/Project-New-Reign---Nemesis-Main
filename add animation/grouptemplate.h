#ifndef GROUPTEMPLATE_H_
#define GROUPTEMPLATE_H_

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <boost\regex.hpp>
#include <boost\algorithm\string.hpp>
#include "compute.h"
#include "furniture.h"

typedef std::vector<std::string> vecstr;
typedef std::unordered_map<std::string, int> id;
typedef std::unordered_map<std::string, std::string> SSMap;
typedef std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ImportContainer;

struct groupTemplate
{
private:
	id eventid;
	id variableid;
	std::string filename;
	vecstr templatelines;
	ImportContainer* newImport;
	std::string format;
	std::string strID;
	int* nextFunctionID;
	std::vector<std::shared_ptr<int>> nextStateID;
	SSMap IDExist;
	master* subFunctionIDs;
	std::vector<std::vector<std::shared_ptr<animationInfo>>> groupAnimInfo;

public:
	groupTemplate(vecstr groupfunctionformat);
	vecstr getFunctionLines(std::string behaviorFile, std::string formatname, std::vector<std::shared_ptr<int>>& stateID, master& newSubFunctionIDs, std::vector<std::vector<std::shared_ptr<animationInfo>>> newGroupAnimInfo, int& nFunctionID, ImportContainer& import, id eventid, id variableID, std::string masterFormat, int groupCount = 0);
	void stateReplacer(std::string& line, std::string filename, std::string statenum, int stateID, int linecount, int groupMulti);
	void processing(std::string& line, std::string filename, std::string masterFormat, int linecount, id eventid, id variableid, int groupMulti = -1, int optionMulti = -1, int animMulti = -1, std::string multiOption = "");
	inline void newID();
	void setZeroEvent(std::string eventname);

};

struct ExistingFunction
{
private:
	int* nextFunctionID;
	ImportContainer* newImport;
	std::string strID;
	std::string format;
	SSMap IDExist;
	master* subFunctionIDs;
	std::vector<std::vector<std::shared_ptr<animationInfo>>> groupAnimInfo;
	id eventid;
	id variableid;

public:
	vecstr groupExistingFunctionProcess(int curFunctionID, vecstr existingFunctionLines, master& newSubFunctionIDs, std::vector<std::vector<std::shared_ptr<animationInfo>>> newGroupAnimInfo, std::string format, ImportContainer& import, id newEventID, id newVariableID, int& nFunctionID, bool hasMaster, bool hasGroup, vecstr templateGroup);
	void processing(std::string& line, std::string filename, int curFunctionID, int linecount, id eventid, id variableid, int groupMulti, int optionMulti = -1, int animMulti = -1, std::string multiOption = "");
	inline void newID();
	void setZeroEvent(std::string eventname);

};

int formatGroupReplace(std::string& curline, std::string oriline, int point, std::string filename, std::string format, master subFunctionIDs, std::vector<std::vector<std::shared_ptr<animationInfo>>> groupAnimInfo, int linecount, int groupMulti, int optionMulti, int animMulti, std::string multiOption);
void multiChoice(std::string& line, std::string filename, std::vector<std::vector<std::unordered_map<std::string, bool>>> groupOptionPicked, std::vector<std::vector<std::shared_ptr<animationInfo>>> groupAnimInfo, int numline, std::string templatename, std::string masterformat, animationutility utility);
bool newCondition(std::string condition, std::string filename, std::vector<std::vector<std::unordered_map<std::string, bool>>> optionPicked, std::vector<std::vector<std::shared_ptr<animationInfo>>> groupAnimInfo, int numline, std::string templatename, std::string masterformat, animationutility utility);
bool conditionProcess(std::string condition, std::string masterformat, std::string filename, int numline, std::vector<std::vector<std::unordered_map<std::string, bool>>> curOptionPicked, std::vector<std::vector<std::shared_ptr<animationInfo>>> groupAnimInfo, bool isNot, animationutility utility);
inline bool isPassed(int condition, std::unordered_map<int, bool> IsConditionOpened);
vecstr GetOptionInfo(std::string line, std::string format, std::string filename, int numline, std::vector<std::vector<std::shared_ptr<animationInfo>>> groupAnimInfo, bool allowNoFixAnim, bool isCondition, int groupMulti = -1, int animMulti = -1, int optionMulti = -1, std::string multiOption = "");
void nonGroupOptionInfo(vecstr& optionInfo, std::string line, std::string format, std::string filename, int numline, std::vector<std::vector<std::shared_ptr<animationInfo>>> groupAnimInfo, bool allowNoFixAnim, bool isCondition, int groupMulti, int animMulti, int optionMulti, std::string multiOption);
void optionLimiter(vecstr optionInfo, std::string line, std::string format, std::string filename, int numline, std::vector<std::vector<std::shared_ptr<animationInfo>>> groupAnimInfo, int open, bool isCondition, int curGroup, int curOrder, int limiter, int optionMulti);
std::string optionOrderProcess(std::string line, std::string format, std::string filename, int numline, int groupMulti, int lastOrder, bool isCondition, bool allowNoFixAnim);

#endif