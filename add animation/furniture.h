#ifndef FURNITURE_H_
#define FURNITURE_H_

#include <iostream>
#include <string>
#include <vector>
#include <locale>
#include <sstream>
#include <unordered_map>
#include <boost\regex.hpp>
#include <boost\algorithm\string.hpp>
// #include "tinyexpress.h"
#include "optionlist.h"
#include "registeranimation.h"
#include "animationinfo.h"

typedef std::vector<std::string> vecstr;
typedef std::unordered_map<std::string, int> id;
typedef std::unordered_map<std::string, std::string> SSMap;
typedef std::unordered_map<std::string, SSMap> ImportContainer;

class Furniture
{
private:
	std::string filepath;
	std::string filename;
	std::string format;
	std::string strID;
	vecstr eventID;
	vecstr variableID;
	vecstr furniturelines;
	SSMap IDExist;
	SSMap subFunctionIDs;
	SSMap mixOptRegis;
	ImportContainer* newImport;
	ImportContainer addition;
	bool isLastOrder = false;
	bool hasDuration = false;
	bool isEnd = false;
	int furnitureCount = 0;
	int* nextFunctionID;
	int* lastState;
	int order = 0;
	int lastOrder = 0;
	double duration = 0;
	std::unordered_map<int, std::string> AnimObject;
	std::unordered_map<std::string, bool> optionPicked;
	std::unordered_map<std::string, int> optionPickedCount;
	std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> groupAddition;
	std::unordered_map<std::string, vecstr> addOn;
	std::unordered_map<std::string, vecstr> linkedOption;
	std::unordered_map<std::string, vecstr> mixOptRever;
	std::vector<std::shared_ptr<animationInfo>> groupAnimInfo;

	void multiChoice(std::string& line, std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, int numline);
	bool newCondition(std::string condition, std::vector<std::unordered_map<std::string, bool>> groupOptionPicked, int numline);
	void stateReplacer(std::string& line, int stateID, int linecount, std::string otherAnimOrder = "", bool otherAnim = false);
	void processing(std::string& newline, std::string masterFormat, int linecount, id eventid, id variableid, int stateID, int stateCountMultiplier, int optionMulti = -1, int animMulti = -1, std::string multiOption = "");

public:
	Furniture(vecstr furnitureformat, std::string formatname, int furniturecount, std::string filepath, animationInfo& behaviorOption);
	vecstr GetFurnitureLine(int& ID, ImportContainer& import, id eventid, id variableid, int& stateID, int stateCountMultiplier);
	vecstr GetEventID();
	vecstr GetVariableID();
	SSMap GetNewIDs();
	SSMap GetMixOpt();
	ImportContainer GetAddition();
	std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> GetGroupAddition();
	std::vector<std::shared_ptr<animationInfo>> GetGroupAnimInfo();
	inline void newID();
	void storeAnimObject(vecstr animobjects, std::string listFilename, int lineCount);
	void addGroupAnimInfo(std::vector<std::shared_ptr<animationInfo>> animInfo);
	void setZeroEvent(std::string eventname);
	void setLastOrder(int order);
	void setOrder(int curOrder);
	int getOrder();
	bool isLast();
	std::string addOnConverter(std::string curline, vecstr elements);

	std::string mainAnimEvent;
};

template <class T>
inline int numDigits(T number);
inline int sameWordCount(std::string line, std::string word);
inline bool isPassed(int condition, std::unordered_map<int, bool> IsConditionOpened);
std::string getOption(std::string curline, bool multi);
extern void addOnReplacer(std::string& line, std::unordered_map<std::string, vecstr> addOn, ImportContainer addition, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> groupAddition, SSMap mixOpt, unsigned int count, std::string format, int numline);
extern void animObjectReplacer(std::string& line, std::unordered_map<int, std::string> AnimObject, int id, std::string format, int linecount, bool otherAnim = false);
extern void eventIDReplacer(std::string& line, std::string format, id eventid, std::string firstEvent, int linecount);
extern void variableIDReplacer(std::string& line, std::string format, id eventid, int linecount);
extern vecstr GetOptionInfo(std::string line, std::string templatename, int numline, size_t lastOrder, std::vector<std::shared_ptr<animationInfo>> groupAnimInfo, bool allowNoFixAnim, bool isCondition, bool isGroup = true, int animMulti = -1, int optionMulti = -1);

#endif