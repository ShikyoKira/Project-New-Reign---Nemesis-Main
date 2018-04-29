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
// #include "tinyexpress.h"
#include "furniture.h"

typedef std::vector<std::string> vecstr;
typedef std::unordered_map<std::string, int> id;
typedef std::unordered_map<std::string, std::string> SSMap;
typedef std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ImportContainer;

struct groupTemplate
{
private:
	vecstr templatelines;
	ImportContainer* newImport;
	SSMap functionIDs;
	std::string format;
	std::string strID;
	int* nextFunctionID;
	std::vector<int>* nextStateID;
	SSMap IDExist;

public:
	groupTemplate(vecstr groupfunctionformat);
	vecstr getFunctionLines(std::string behaviorFile, std::string formatname, std::vector<int>& stateID, std::vector<SSMap> subFunctionIDs, std::vector<std::shared_ptr<animationInfo>> groupAnimInfo, int& nFunctionID, ImportContainer& import, id eventid, id variableID, std::string masterFormat, int groupCount = 0);
	SSMap getFunctionIDs();
	void stateReplacer(std::string& line, std::string statenum, int stateID, int linecount, int groupCount);
	void processing(std::string& line, std::string masterFormat, int linecount, std::vector<SSMap> subFunctionIDs, std::vector<std::shared_ptr<animationInfo>> groupAnimInfo, id eventid, id variableid, int optionMulti = -1, int animMulti = -1, std::string multiOption = "");
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

public:
	vecstr groupExistingFunctionProcess(int curFunctionID, vecstr existingFunctionLines, std::vector<SSMap> subFunctionIDs, std::vector<std::shared_ptr<animationInfo>> groupAnimInfo, std::string format, ImportContainer import, id eventid, id variableid, int& nFunctionID, bool hasMaster, bool hasGroup, vecstr templateGroup);
	void processing(std::string& line, int curFunctionID, int linecount, std::vector<SSMap> subFunctionIDs, std::vector<std::shared_ptr<animationInfo>> groupAnimInfo, id eventid, id variableid, bool hasGroup, int optionMulti = -1, int animMulti = -1, std::string multiOption = "");
	inline void newID();
	void setZeroEvent(std::string eventname);

};

void formatReplace(std::string& curline, std::string format, SSMap subFunctionIDs, std::vector<std::shared_ptr<animationInfo>> groupAnimInfo, int linecount, size_t lastOrder, int optionMulti, int animMulti);
void multiChoice(std::string& line, std::vector<std::unordered_map<std::string, bool>> optionMap, std::vector<std::shared_ptr<animationInfo>> groupAnimInfo, int numline, std::string templatename, std::string masterformat);
bool newCondition(std::string condition, std::vector<std::unordered_map<std::string, bool>> optionPicked, std::vector<std::shared_ptr<animationInfo>> groupAnimInfo, int numline, std::string templatename, std::string masterformat);
inline bool isPassed(int condition, std::unordered_map<int, bool> IsConditionOpened);

#endif